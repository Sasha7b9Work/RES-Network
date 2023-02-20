// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/HC12/HC12.h"
#include "stm32f1xx_hal.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include <cstring>


#define PORT_SET GPIOA
#define PIN_SET  GPIO_PIN_6


namespace HC12
{
    struct RecvBuffer
    {
        static const int SIZE = 128;

        RecvBuffer() : pointer(0) { }

        void Push(char symbol)
        {
            if (pointer < SIZE)
            {
                data[pointer++] = symbol;
            }
        }

        char *Data()        { return &data[0]; }
        int NumSymbols()    { return pointer;  }
        void Clear()        { pointer = 0;     }

    private:
        char data[SIZE];
        int pointer;
    } recv_buffer;

    char recv_byte = 0;

    static UART_HandleTypeDef handleUART;

    void *handle = (void *)&handleUART;
}


void HC12::Init()
{
    GPIO_InitTypeDef  is;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    is.Pin       = GPIO_PIN_9;
    is.Mode      = GPIO_MODE_AF_PP;
    is.Speed     = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOA, &is);
    
    is.Pin  = GPIO_PIN_10;              // TX
    is.Pull = GPIO_NOPULL;
    is.Mode = GPIO_MODE_INPUT;
    
    HAL_GPIO_Init(GPIOA, &is);

    is.Pin = PIN_SET;                // SET Для АТ команд
    is.Mode = GPIO_MODE_OUTPUT_PP;

    HAL_GPIO_Init(PORT_SET, &is);

    HAL_GPIO_WritePin(PORT_SET, PIN_SET, GPIO_PIN_SET);

    handleUART.Instance          = USART1;
    handleUART.Init.BaudRate     = 9600;
    handleUART.Init.WordLength   = UART_WORDLENGTH_8B;
    handleUART.Init.StopBits     = UART_STOPBITS_1;
    handleUART.Init.Parity       = UART_PARITY_NONE;
    handleUART.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    handleUART.Init.Mode         = UART_MODE_TX_RX;
    handleUART.Init.OverSampling = UART_OVERSAMPLING_16;

    HAL_UART_Init(&handleUART);

    HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

    HAL_UART_Receive_IT(&handleUART, (uint8 *)&recv_byte, 1);

    Command("AT+DEFAULT");
}


void HC12::Transmit(const void *buffer, int size)
{
    if (!buffer)
    {
        return;
    }

    HAL_UART_Transmit(&handleUART, (uint8_t *)buffer, (uint16_t)size, 0xFFFF);
}


void HC12::Command(pchar command)
{
    HAL_GPIO_WritePin(PORT_SET, PIN_SET, GPIO_PIN_RESET);

    TimeMeterMS().WaitMS(40);

    recv_buffer.Clear();

    Transmit(command, (int)std::strlen(command));
    Transmit("\r", 1);

    HAL_GPIO_WritePin(PORT_SET, PIN_SET, GPIO_PIN_SET);

    TimeMeterMS().WaitMS(80);

    char *answer = recv_buffer.Data();
    answer = answer;
    
    int num = recv_buffer.NumSymbols();
    num = num;
}


void HC12::ReceiveCallback()
{
    recv_buffer.Push(recv_byte);

    HAL_UART_Receive_IT(&handleUART, (uint8 *)&recv_byte, 1);
}

// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/HC12/HC12.h"
#include "stm32f3xx_hal.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include <cstring>


namespace HAL_USART_HC12
{
    extern char recv_byte;
}


namespace HC12
{
#define PORT_SET GPIOA
#define PIN_SET  GPIO_PIN_6

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
}


void HC12::Init()
{
    return;
    
    pinCS_HC12.Init();
    pinCS_HC12.ToHi();

//    while (true)
    {
        Command("AT+DEFAULT");
        Command("AT");

//        Timer::Delay(100);
    }
}


void HC12::Transmit(const void *buffer, int size)
{
    return;
    
    HAL_USART_HC12::Transmit(buffer, size);
}


void HC12::Command(pchar command)
{
    return;
    pinCS_HC12.ToLow();

    TimeMeterMS().PauseOnMS(40);

    recv_buffer.Clear();

    Transmit(command, (int)std::strlen(command));
    Transmit("\r", 1);

    pinCS_HC12.ToHi();
}


void HC12::_ReceiveCallback()
{
    recv_buffer.Push(HAL_USART_HC12::recv_byte);

    HAL_UART_Receive_IT((UART_HandleTypeDef *)HAL_USART_HC12::handle, (uint8 *)&HAL_USART_HC12::recv_byte, 1);
}

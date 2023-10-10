// 2023/06/15 08:56:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include <stm32f3xx_hal.h>


namespace HAL_ADC
{
    static ADC_HandleTypeDef handleADC;
    void *handle = (void *)&handleADC;
    static bool flag_ready = false;

    static float ReadChannel(uint channel);
}


void HAL_ADC::Init()
{
#ifndef WIN32
    __HAL_RCC_ADC1_CLK_ENABLE();
#endif

    pinADC.Init();
    pinHumidity.Init();

    ADC_ChannelConfTypeDef sConfig = { 0 };

    handleADC.Instance = ADC1;
    handleADC.Init.ScanConvMode = ADC_SCAN_DISABLE;
    handleADC.Init.ContinuousConvMode = DISABLE;
    handleADC.Init.DiscontinuousConvMode = DISABLE;
    handleADC.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    handleADC.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    handleADC.Init.NbrOfConversion = 1;

    HAL_ADC_Init(&handleADC);

    sConfig.Channel = ADC_CHANNEL_4;
    sConfig.Rank = ADC_REGULAR_RANK_1;
//    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;

    HAL_NVIC_SetPriority(ADC1_IRQn, 1, 1);

    HAL_NVIC_EnableIRQ(ADC1_IRQn);

    HAL_ADC_Start_IT(&handleADC);
}


float HAL_ADC::ReadChannel(uint channel)
{
    ADC_ChannelConfTypeDef config = { 0 };

    config.Channel = channel;
    config.Rank = ADC_REGULAR_RANK_1;
//    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;

    HAL_ADC_ConfigChannel(&handleADC, &config);

    HAL_NVIC_EnableIRQ(ADC1_IRQn);

    flag_ready = false;

    HAL_ADC_Start_IT(&handleADC);

    while (!flag_ready)
    {
    }

    float value = (float)HAL_ADC_GetValue(&handleADC) / (1 << 12) * 3.3f * 1.25f;

    HAL_NVIC_DisableIRQ(ADC1_IRQn);

    return value;
}


float HAL_ADC::GetVoltage()
{
    return ReadChannel(ADC_CHANNEL_4);
}


float HAL_ADC::GetHumidity()
{
    return ReadChannel(ADC_CHANNEL_1);
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *)
{
    HAL_ADC::flag_ready = true;
}

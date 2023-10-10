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

    handleADC.Instance = ADC1;
    handleADC.Init.ScanConvMode = ADC_SCAN_DISABLE;
    handleADC.Init.ContinuousConvMode = DISABLE;
    handleADC.Init.DiscontinuousConvMode = DISABLE;
    handleADC.Init.NbrOfDiscConversion = 0;
    handleADC.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    handleADC.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    handleADC.Init.NbrOfConversion = 1;

    HAL_ADC_Init(&handleADC);
}


float HAL_ADC::ReadChannel(uint channel)
{
    ADC_ChannelConfTypeDef config = { 0 };

    config.Channel = channel;
    config.Rank = ADC_REGULAR_RANK_1;
    config.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;

    HAL_ADC_ConfigChannel(&handleADC, &config);

    HAL_ADC_Start(&handleADC);

    HAL_ADC_PollForConversion(&handleADC, 10);

    float value = (float)HAL_ADC_GetValue(&handleADC) / (1 << 12) * 3.3f * 1.25f;

    HAL_NVIC_DisableIRQ(ADC1_IRQn);

    return value;
}


float HAL_ADC::GetVoltage()
{
    static TimeMeterMS meter;

    static float voltage = 0.0f;

    if (meter.IsFinished())
    {
        voltage = ReadChannel(ADC_CHANNEL_4);
    }

    meter.FinishAfter(1000);

    return voltage;
}


float HAL_ADC::GetHumidity()
{
    static TimeMeterMS meter;

    static float humidity = 0.0f;

    if (meter.IsFinished())
    {
        humidity = ReadChannel(ADC_CHANNEL_1);
    }

    meter.FinishAfter(1000);

    return humidity;
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *)
{
    HAL_ADC::flag_ready = true;
}

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

    static uint ReadChannel(uint channel);
}


void HAL_ADC::Init()
{
    pinADC.Init();
    pinHumidity.Init();

    handleADC.Instance = ADC1;
    handleADC.Init.Resolution = ADC_RESOLUTION_12B;
    handleADC.Init.ScanConvMode = ADC_SCAN_DISABLE;
    handleADC.Init.ContinuousConvMode = DISABLE;
    handleADC.Init.DiscontinuousConvMode = DISABLE;
    handleADC.Init.NbrOfDiscConversion = 0;
    handleADC.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    handleADC.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    handleADC.Init.NbrOfConversion = 1;

    HAL_ADC_Init(&handleADC);

    HAL_NVIC_SetPriority(ADC1_IRQn, 1, 1);
}


uint HAL_ADC::ReadChannel(uint channel)
{
    ADC_ChannelConfTypeDef config = { 0 };

    config.Channel = channel;
    config.Rank = ADC_REGULAR_RANK_1;
    config.SamplingTime = ADC_SAMPLETIME_601CYCLES_5;

    uint value = 0;

    if (HAL_ADC_ConfigChannel(&handleADC, &config) == HAL_OK)
    {
        HAL_NVIC_EnableIRQ(ADC1_IRQn);

        flag_ready = false;

        if (HAL_ADC_Start_IT(&handleADC) == HAL_OK)
        {
            while (!flag_ready)
            {
            }

            value = HAL_ADC_GetValue(&handleADC);

            if (value != 0 && value != 0xFFF && value != 0xF9D)
            {
                value = value;
            }
        }
    }

    return value;
}


float HAL_ADC::GetVoltage()
{
    return 0.0f;

    static TimeMeterMS meter;

    static float voltage = 0.0f;

    if (meter.IsFinished())
    {
        voltage = (float)ReadChannel(ADC_CHANNEL_4) * 3.3f * 1.25f / (float)(1 << 12);

        meter.FinishAfter(1000);
    }

    return voltage;
}


float HAL_ADC::GetHumidity()
{
    static TimeMeterMS meter;

    static float humidity = 0.0f;

    if (meter.IsFinished())
    {
        humidity = (float)ReadChannel(ADC_CHANNEL_3) * 3.3f / (float)(1 << 12);

        meter.FinishAfter(1000);
    }

    return humidity;
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *)
{
    HAL_ADC::flag_ready = true;
}

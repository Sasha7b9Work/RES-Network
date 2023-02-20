// 2023/01/30 15:35:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f1xx_hal.h>


namespace HAL_RTC
{
#define WAKEUP_TIMER_ENABLE 0x32F2

    static RTC_HandleTypeDef handleRTC =
    {
        RTC,
        {
            RTC_AUTO_1_SECOND       // AsynchPrediv
        }
    };
}


void HAL_RTC::Init()
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

    /*##-1- Configue LSI as RTC clock soucre ###################################*/
    HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        HAL::Delay(100);
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        HAL::Delay(100);
    }

    /*##-2- Enable RTC peripheral Clocks #######################################*/
    /* Enable RTC Clock */
    __HAL_RCC_RTC_ENABLE();

    if (HAL_RTC_Init(&handleRTC) != HAL_OK)
    {
        /* Initialization Error */
        HAL::Delay(100);
    }

    /*##-2- Check if data stored in BackUp register1: Wakeup timer enable #######*/
    /* Read the Back Up Register 1 Data */
    if (HAL_RTCEx_BKUPRead(&handleRTC, RTC_BKP_DR1) == WAKEUP_TIMER_ENABLE)
    {
        /* if the wakeup timer is enabled then desable it to disable the wakeup timer interrupt */
//        if (HAL_RTCEx_DeactivateSecond(&handleRTC) != HAL_OK)
        {
            /* Initialization Error */
            HAL::Delay(100);
        }
    }

    /*##-3- Configure the RTC Wakeup peripheral #################################*/
//    HAL_RTCEx_SetSecond_IT(&handleRTC);

    /*##-4- Write 'wakeup timer enabled' tag in RTC Backup data Register 1 #######*/
    HAL_RTCEx_BKUPWrite(&handleRTC, RTC_BKP_DR1, WAKEUP_TIMER_ENABLE);

    RTC_TimeTypeDef time;
    time.Hours = 8;
    time.Minutes = 15;
    time.Seconds = 0;

    RTC_DateTypeDef date;
    date.Date = 1;
    date.Month = 2;
    date.Year = 23;

    HAL_RTC_SetTime(&handleRTC, &time, RTC_FORMAT_BIN);

    HAL_RTC_SetDate(&handleRTC, &date, RTC_FORMAT_BIN);
}


PackedTime HAL_RTC::GetPackedTime()
{
    RTC_TimeTypeDef time;

    HAL_RTC_GetTime(&handleRTC, &time, RTC_FORMAT_BIN);

    RTC_DateTypeDef date;

    HAL_RTC_GetDate(&handleRTC, &date, RTC_FORMAT_BIN);

    PackedTime result(time.Hours, time.Minutes, time.Seconds, date.Date, date.Month, (uint)(2000 + date.Year));

    return result;
}


String<> PackedTime::ToString() const
{
    return String<>("%02d:%02d:%02d:%03d", hours, minutes, seconds, ms);
}

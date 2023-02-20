// 2022/05/06 11:30:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Keyboard.h"
#include "Hardware/Timer.h"
#include "Menu/Menu.h"
#include <stm32f1xx_hal.h>


namespace Keyboard
{
    static const int TIME_LONG_PRESS = 500;

    TimeMeterMS meter;

    bool pressed = false;               // Если true, клавиша нажата
    bool taboo_long = false;            // Если true, запрещено длинное срабатывание

#define KEY_PRESSED (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET)
}


void Keyboard::Init()
{
    GPIO_InitTypeDef is =
    {
        GPIO_PIN_1,
        GPIO_MODE_INPUT,
        GPIO_PULLUP,
        GPIO_SPEED_FREQ_MEDIUM
    };

    HAL_GPIO_Init(GPIOA, &is);
}


void Keyboard::Update()
{
    if (meter.ElapsedTime() < 100)
    {
        return;
    }

    if (pressed)
    {
        if (meter.ElapsedTime() > TIME_LONG_PRESS && !taboo_long)
        {
            Menu::LongPress();
            taboo_long = true;
        }
        else
        {
            if (!KEY_PRESSED)
            {
                pressed = false;
                meter.Reset();
                if (!taboo_long)
                {
                    Menu::ShortPress();
                }
                taboo_long = false;
            }
        }
    }
    else
    {
        if (KEY_PRESSED)
        {
            pressed = true;
            meter.Reset();
        }
    }
}

// 2022/05/05 15:23:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"


static const Settings default =
{
    // Display
    {
        {1, 1, 1, 0},
        { TypeDisplayedInformation::AllMeasures }
    },
    // System
    {
        0                                           // serial_number
    },
    // Measures
    {
        {-100, 0, 0, -100},
        {100, 1000, 100, 100}
    }
};


Settings gset = default;


void Settings::Load()
{
    Settings settings;

    if (HAL_ROM::LoadSettings(settings))
    {
        gset = settings;
    }
    else
    {
        gset = default;
    }
}


void Settings::Update()
{
    static TimeMeterMS meter;

    if (meter.ElapsedTime() > 10000)
    {
        meter.Reset();

        Settings settings;

        if (HAL_ROM::LoadSettings(settings))
        {
            if (gset != settings)
            {
                HAL_ROM::SaveSettings(gset);
            }
        }
        else
        {
            HAL_ROM::SaveSettings(gset);
        }
    }
}


bool Settings::operator!=(const Settings &rhs)
{
    return std::memcmp(this, &rhs, sizeof(Settings)) != 0;
}

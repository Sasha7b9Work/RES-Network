// 2022/05/05 15:23:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"


Settings gset =
{
    // Display
    {
        {1, 1, 1, 1
#ifdef TYPE_1
        , 1
#endif
        },
        { TypeDisplayedInformation::AllMeasures }
    },
    // System
    {
        0                                           // serial_number
    }
};


#ifdef WIN32
uint Settings::GetID()
{
    return 112;
}
#else
uint Settings::GetID()
{
    uint address = 0x8000000 + 0x10000 - 4;

    uint *pointer = (uint *)address;

    return *pointer;
}
#endif

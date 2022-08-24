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


uint Settings::GetID()
{
    return 111;
}

// 2023/10/10 16:03:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/HIH4000/HIH4000.h"
#include "Hardware/HAL/HAL.h"


float HIH4000::GetHumidity()
{
    return HAL_ADC::GetHumidity();
}

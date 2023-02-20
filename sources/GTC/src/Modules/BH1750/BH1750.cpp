// 2022/02/21 15:41:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/BH1750/BH1750.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f1xx_hal.h>
#include <cstdlib>


namespace BH1750
{
    static const uint8 CMD_POWER_ON   = 0x01;
    static const uint8 CMD_RESET      = 0x03;
    static const uint8 CMD_H_RES_MODE = 0x10;

    uint timeNext = 1;

    bool WriteUINT8(uint8);

    bool ReadUINT16(uint8 *);
}


void BH1750::Init()
{
    WriteUINT8(CMD_POWER_ON);
    WriteUINT8(CMD_RESET);
    WriteUINT8(CMD_H_RES_MODE);
}


bool BH1750::GetMeasure(float *illumination)
{
    if (HAL_GetTick() < timeNext)
    {
        return false;
    }

    timeNext += TIME_MEASURE + (std::rand() % 100);

#ifdef IN_MODE_TEST

    static float value = 1.5f;

    value *= 1.98f;

    *illumination = value / 100.0f;

    if (value > 1e4f)
    {
        value = 1.3f;
    }

    return true;

#else

    BitSet32 result;

    if (ReadUINT16(&result.byte[0]))
    {
        *illumination = (float)(result.byte[1] | (result.byte[0] << 8)) / 1.2f;

        return true;
    }

    return false;

#endif
}


bool BH1750::WriteUINT8(uint8 byte)
{
    return HAL_I2C1::Write8(0x23, byte) == 0;
}


bool BH1750::ReadUINT16(uint8 *buffer)
{
    return HAL_I2C1::Read16(0x23, buffer) == 0;
}

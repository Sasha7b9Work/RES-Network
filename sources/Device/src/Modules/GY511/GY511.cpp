// 2023/01/30 16:45:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/GY511/GY511.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f1xx_hal.h>


#define REG_MAG_OUT_X_H     0x03U
#define REG_MAG_OUT_X_L     0x04U
#define REG_MAG_OUT_Y_H     0x07U
#define REG_MAG_OUT_Y_L     0x80U
#define REG_MAG_OUT_Z_H     0x05U
#define REG_MAG_OUT_Z_L     0x06U
#define REG_MAG_MR          0x02U
#define REG_MAG_SR          0x09U

#define GY511_CTRL_REG1     0x20U
#define GY511_CTRL_REG3     0x22U
#define GY511_CTRL_REG4     0x23U
#define GY511_STATUS_REG    0x27U

#define GY511_OUT_X_L       0x28U
#define GY511_OUT_X_H       0x29U
#define GY511_OUT_Y_L       0x2AU
#define GY511_OUT_Y_H       0x2BU
#define GY511_OUT_Z_L       0x2CU
#define GY511_OUT_Z_H       0x2DU


namespace GY511
{
    static StructDataRaw raw_acce_x;
    static StructDataRaw raw_acce_y;
    static StructDataRaw raw_acce_z;

    static StructDataRaw magnetic_x;
    static StructDataRaw magnetic_y;
    static StructDataRaw magnetic_z;

    static void WriteA(uint8 reg, uint8 data)
    {
        HAL_I2C1::Write(0x19, reg, &data, 1);
    }

    static uint8 ReadA(uint8 reg)
    {
        uint8 result = 0;
        HAL_I2C1::Read(0x19, reg, &result, 1);
        return result;
    }

    static void WriteM(uint8 reg, uint8 data)
    {
        HAL_I2C1::Write(0x1e, reg, &data, 1);
    }

    static uint8 ReadM(uint8 reg)
    {
        uint8 result = 0;
        HAL_I2C1::Read(0x1e, reg, &result, 1);
        return result;
    }
}


void GY511::Init()
{
    uint8 data = 0;
    _SET_BIT(data, 4);                              // I1_ZYXDA = 1 - разрешаем прерывания INT1 по полученным измерениям
    WriteA(GY511_CTRL_REG3, data);

    // Enable Block Data Update.
    data = ReadA(GY511_CTRL_REG4);
    data |= (1 << 7);                               // BDU = 1
    WriteA(GY511_CTRL_REG4, data);

    // Set Output Data Rate to 1Hz.
    HAL_I2C1::Read(0x19, GY511_CTRL_REG1, &data, 1);
    data |= (1 << 4);                               // ODR = 0b0001, 1Hz
    HAL_I2C1::Write(0x19, GY511_CTRL_REG1, &data, 1);

    // Set device in continuous mode with 12 bit resol.
    HAL_I2C1::Read(0x19, GY511_CTRL_REG4, &data, 1);
    data |= (1 << 3);                                           // HR = 1, (LPen = 0 - High resolution mode)
    WriteA(GY511_CTRL_REG4, data);

    WriteM(REG_MAG_MR, 0);
}


void GY511::Update()
{
    if (ReadA(GY511_STATUS_REG) & (1 << 3))
    {
        raw_acce_x.byte[0] = ReadA(GY511_OUT_X_L);
        raw_acce_x.byte[1] = ReadA(GY511_OUT_X_H);

        raw_acce_y.byte[0] = ReadA(GY511_OUT_Y_L);
        raw_acce_y.byte[1] = ReadA(GY511_OUT_Y_H);

        raw_acce_z.byte[0] = ReadA(GY511_OUT_Z_L);
        raw_acce_z.byte[1] = ReadA(GY511_OUT_Z_H);
    }

    if (ReadM(REG_MAG_SR) & 1)
    {
        magnetic_x.byte[0] = ReadM(REG_MAG_OUT_X_L);
        magnetic_x.byte[1] = ReadM(REG_MAG_OUT_X_H);

        magnetic_y.byte[0] = ReadM(REG_MAG_OUT_Y_L);
        magnetic_y.byte[1] = ReadM(REG_MAG_OUT_Y_H);

        magnetic_z.byte[0] = ReadM(REG_MAG_OUT_Z_L);
        magnetic_z.byte[1] = ReadM(REG_MAG_OUT_Z_H);

        WriteM(REG_MAG_SR, 2);
    }
}


StructDataRaw GY511::GetAccelerationX()
{
    return raw_acce_x;
}


StructDataRaw GY511::GetAccelerationY()
{
    return raw_acce_y;
}


StructDataRaw GY511::GetAccelerationZ()
{
    return raw_acce_z;
}


StructDataRaw GY511::GetMagneticX()
{
    return magnetic_x;
}


StructDataRaw GY511::GetMagneticY()
{
    return magnetic_y;
}


StructDataRaw GY511::GetMagneticZ()
{
    return magnetic_z;
}

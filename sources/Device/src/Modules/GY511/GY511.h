// 2023/01/30 16:44:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Структура содержит нативные данные, считанные из акселерометра
union StructDataRaw
{
    union
    {
        uint8 byte[2];
        int16 raw;
    };

    float ToTemperatrue()
    {
        return 25.0f + (float)raw / 256.0f;
    }

    float ToAccelearation()
    {
        return (float)raw / 16.0f / 1000.0f;
    }

    float ToMagneticXY()
    {
        return (float)raw / 1100.0f;
    }

    float ToMagneticZ()
    {
        return (float)raw / 980.0f;
    }
};



namespace GY511
{
    void Init();

    void Update();

    StructDataRaw GetAccelerationX();

    StructDataRaw GetAccelerationY();

    StructDataRaw GetAccelerationZ();

    StructDataRaw GetMagneticX();

    StructDataRaw GetMagneticY();

    StructDataRaw GetMagneticZ();
}

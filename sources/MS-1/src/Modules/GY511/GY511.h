// 2023/01/30 16:44:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// ��������� �������� �������� ������, ��������� �� �������������
union StructDataRaw
{
    union
    {
        uint8 byte[2];
        int16 raw;
    };

    float ToMagnetic()
    {
        return (float)raw / 16.0f / 1000.0f;
    }
};



namespace GY511
{
    void Init();

    void Update();

    StructDataRaw GetAccelerationX();

    StructDataRaw GetAccelerationY();

    StructDataRaw GetAccelerationZ();
}

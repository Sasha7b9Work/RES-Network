﻿// 2023/09/08 11:46:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/HAL/HAL.h"


struct DataStruct
{
    float temperature;
    float pressure;
    float humidity;
    float dev_point;
    PackedTime time;
};


namespace Storage
{
    static const int SIZE = 1024 * 1024;        // 8 MBit = 1 MByte

    void AppendData(const DataStruct &);

    bool IsEmpty();

    // Возвращает указатель на следующие данные, если таковые имеются. Не удаляет данные из хранилищца.
    // Чтобы удалить, нужно вызывать EraseData() с указателем на эти данные.
    DataStruct *GetNextData();

    void EraseData(DataStruct *);
}


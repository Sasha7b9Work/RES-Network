﻿// 2023/04/04 08:29:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Storage/Measure.h"


namespace NEO_M8N
{
    void CallbackOnReceive();

    char *GetData();

    // Извлекает слово после num_field запятой. Запятая после GNGGA имеет номер 1
    char *ExtractField(int num_filed, char buffer[32]);

    bool IsReady();

    //                         широта             долгота             высота
    bool GetMeasures(Measure *latitude, Measure *longitude, Measure *altitude);
}

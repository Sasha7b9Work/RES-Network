// 2022/05/05 15:22:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Display.h"


struct TypeDisplayedInformation
{
    enum E
    {
        MeasurePressure,
        MeasureIllumination,
        MeasureTemperature,
        MeasureHumidity,
#ifdef TYPE_1
        MeasureVelocity,
#endif
        AllMeasures,
        Menu,
        Count
    };

    E value;

    TypeDisplayedInformation &operator++()
    {
        value = (E)(value + 1);

        return *this;
    }

    bool IsAllMeasures() const { return value == AllMeasures; }
};



struct SettingsDisplay
{
    uint8                       show_measure[TypeMeasure::Count];
    TypeDisplayedInformation    typeDisplaydInfo;
};


struct SettingsSystem
{
    int serial_number;
};


struct Settings
{
    SettingsDisplay display;
    SettingsSystem  system;

    static uint GetID();

    static void Load();
    static void Save();
};


extern Settings gset;

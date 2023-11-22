// 2022/05/05 15:22:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Display.h"


struct TypeDisplayedInformation
{
    enum E
    {
        MeasureTemperature,
        MeasureHumidity,
        MeasurePressure,
        MeasureDewPoint,
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
    uint8                       show_measure[Measure::Name::Count];
    TypeDisplayedInformation    typeDisplaydInfo;
};


struct SettingsMeasures
{
    int   limit_min[Measure::Name::Count];
    int   limit_max[Measure::Name::Count];
    float value_min[Measure::Name::Count];
    float value_max[Measure::Name::Count];
};


struct SettingsSystem
{
    int serial_number;
};


struct Settings
{
    uint crc;
    uint number;
    SettingsDisplay  display;
    SettingsSystem   system;
    SettingsMeasures measures;

    bool operator!=(const Settings &);
    bool operator==(const Settings &);

    static void Load();

    static void Save();

    static void Reset();

    // ���� value ������ ��� ������ ��� ������������ ���������, �� �������������� ������
    static void SaveMeasure(const Measure &);

    // ���������� ��� � ���� �������� ���������
    static void ResetMeasure(Measure::Name::E);
};


extern Settings gset;

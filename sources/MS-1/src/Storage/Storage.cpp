// 2023/09/08 11:47:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Storage/Storage.h"
#include "Hardware/Timer.h"
#include "Storage/MemoryStorage.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Math.h"


/*
*   В первых двух секторах хранится служебная информация
*/


namespace Storage
{
    struct DataInfo
    {
        uint address_first;     // адрес первой записи
        uint address_last;      // адрес последней записи
    };

    static Measure measures[NUM_MEASURES_TO_CONTROL];

    // Послать некоторые измререния в USB
    static void SendMeasures();
}


void Storage::Init()
{
    for (int i = 0; i < NUM_MEASURES_TO_CONTROL; i++)
    {
        measures[i].Set((Measure::E)i, 0.0f);
        measures[i].correct = false;
    }
}


void Storage::AppendMeasure(const Measure &measure)
{
    if (measure.GetName() < NUM_MEASURES_TO_CONTROL)
    {
        if (measure.correct)
        {
            measures[measure.GetName()] = measure;
        }
    }
}


void Storage::Update()
{
    static TimeMeterMS meter;
    
    if(!meter.IsFinished())
    {
        return;
    }

    meter.FinishAfter(5000);

    Measurements measurements = GetLastMeasurements();

    MemoryStorage::Append(measurements);

    SendMeasures();
}


void Storage::SendMeasures()
{

}


bool Storage::GetMeasure(Measure::E name, Measure &measure)
{
    if (((int)name) >= 0 && name < NUM_MEASURES_TO_CONTROL)
    {
        measure = measures[name];

        return measure.correct;
    }

    return false;
}


Measurements Storage::GetLastMeasurements()
{
    Measurements measurements
    (
        (float)measures[Measure::Temperature].GetDouble(),
        (float)measures[Measure::Pressure].GetDouble(),
        (float)measures[Measure::Humidity].GetDouble(),
        (float)measures[Measure::DewPoint].GetDouble(),
        (float)measures[Measure::Velocity].GetDouble(),
        HAL_RTC::GetTime()
        );

    return measurements;
}

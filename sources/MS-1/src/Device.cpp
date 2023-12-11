// 2022/04/27 11:48:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Measures.h"
#include "Hardware/HAL/HAL.h"
#include "Modules/HC12/HC12.h"
#include "Modules/BME280/BME280.h"
#include "Modules/CG-Anem/CG-Anem.h"
#include "Hardware/CDC/CDC.h"
#include "Modules/ST7735/ST7735.h"
#include "Modules/W25Q80DV/W25Q80DV.h"
#include "Hardware/Timer.h"
#include "Hardware/InterCom.h"
#include "Display/Display.h"
#include "Hardware/Keyboard.h"
#include "Hardware/Beeper.h"
#include "Display/StartScreen.h"
#include "Hardware/EnergySwitch.h"
#include "Modules/GY511/GY511.h"
#include "Modules/NEO-M8N/NEO-M8N.h"
#include <cmath>


void Device::Init()
{
    HAL::Init();

    ST7735::Init();         // Дисплей

    Timer::Delay(1000);

    EnergySwitch::Init();

    gset.Load();

    gset.Reset();

//    BME280::Init();         // Температура, давление, влажность, точка росы

//    GY511::Init();          // Компас

    HC12::Init();           // Радиомодуль

    Keyboard::Init();

    W25Q80DV::Test::Run();  // Микросхема памяти

    Beeper::Init();

//    Beeper::Start(4000);

    InterCom::SetDirection((Direction::E)(Direction::CDC | Direction::HC12 | Direction::Display));

    HAL_USART2::Init();

//    HAL_IWDG::Init();

    pinSCL.Init();
    pinSDA.Init();

    while (true)
    {
        pinSCL.ToHi();
        pinSDA.ToHi();
        pinSCL.ToLow();
        pinSDA.ToLow();
    }
}


void Device::Update()
{
//    HAL_IWDG::Update();

    if (Beeper::Running() && TIME_MS > 2000)
    {
        Beeper::Stop();
    }

    Measure temp;
    Measure pressure;
    Measure humidity;
    Measure dew_point;
    Measure velocity;
    Measure azimuth;
    Measure latitude;
    Measure longitude;
    Measure altitude;

    uint time = TIME_MS;

    if (BME280::GetMeasures(&temp, &pressure, &humidity, &dew_point))
    {
        InterCom::Send(temp, time);
        InterCom::Send(pressure, time);
        InterCom::Send(humidity, time);
        InterCom::Send(dew_point, time);

        bool in_range = Measures::InRange(temp) &&
            Measures::InRange(pressure) &&
            Measures::InRange(humidity) &&
            Measures::InRange(dew_point);

        if (in_range)
        {
            Beeper::Stop();
        }
        else
        {
            Beeper::Start(100);
        }
    }

    if (CG_Anem::GetMeasure(&velocity))
    {
        InterCom::Send(velocity, time);
    }

    if (GY511::GetMagnetic(&azimuth))
    {
        InterCom::Send(azimuth, time);
    }

    if (NEO_M8N::GetMeasures(&latitude, &longitude, &altitude))
    {
        InterCom::Send(latitude, time);
        InterCom::Send(longitude, time);
        InterCom::Send(altitude, time);
    }

    GY511::Update();

    Keyboard::Update();

    Display::Update(TIME_MS);

    HAL_ADC::GetVoltage();

    EnergySwitch::Update();
}

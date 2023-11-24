// 2022/04/27 11:48:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Measures.h"
#include "Hardware/HAL/HAL.h"
#include "Modules/HC12/HC12.h"
#include "Modules/BME280/BME280.h"
#include "Modules/BH1750/BH1750.h"
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
#include <cmath>


namespace Device
{
    static float CalculateDewPoint(float temperature, float humidity);
    static float CalculateF(float temperature, float humidity);
}


void Device::Init()
{
    HAL::Init();

    EnergySwitch::Init();

    gset.Load();

    gset.Reset();

    ST7735::Init();

    BME280::Init();

    BH1750::Init();

    HC12::Init();

    Keyboard::Init();

    InterCom::SetDirection((Direction::E)(Direction::CDC | Direction::HC12 | Direction::Display));

    Beeper::Init();

    W25Q80DV::Test::Run();

//    Beeper::Start(4000);
}


void Device::Update()
{
    static int counter = 0;
    counter++;
    
   
    if (Beeper::Running() && TIME_MS > 2000)
    {
//        Beeper::Stop();
    }

    float temp = 0.0f;
    float pressure = 0.0f;
    float humidity = 0.0;
    float illumination = 0.0f;

    if (BME280::GetMeasures(&temp, &pressure, &humidity))
    {
            if(counter == 1)
    {
        counter = counter;
    }
        InterCom::Send(TypeMeasure::Temperature, temp);
        if(counter == 1)
    {
        counter = counter;
    }
        InterCom::Send(TypeMeasure::Pressure, pressure);
        InterCom::Send(TypeMeasure::Humidity, humidity);

        float dew_point = CalculateDewPoint(temp, humidity);

        InterCom::Send(TypeMeasure::DewPoint, dew_point);

        bool in_range = Measures::InRange(TypeMeasure::Temperature, temp) &&
            Measures::InRange(TypeMeasure::Pressure, pressure) &&
            Measures::InRange(TypeMeasure::Humidity, humidity) &&
            Measures::InRange(TypeMeasure::DewPoint, dew_point);

        if (in_range)
        {
//            Beeper::Stop();
        }
        else
        {
//            Beeper::Start(100);
        }
    }

    if (BH1750::GetMeasure(&illumination))
    {
        InterCom::Send(TypeMeasure::Illumination, illumination);
    }

    Keyboard::Update();

    Display::Update();

    HAL_ADC::GetVoltage();

    EnergySwitch::Update();
}


float Device::CalculateDewPoint(float temperature, float humidity)
{
    float f = CalculateF(temperature, humidity);

    return (237.7f * f) / (17.27f - f);
}


float Device::CalculateF(float temperature, float humidity)
{
    return (17.27f * temperature) / (237.7f + temperature) + std::log(humidity / 100.0f);
}

// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Modules/HC12/HC12.h"
#include "Hardware/HAL/HAL.h"
#include "Modules/BME280/BME280.h"
#include "Modules/CG-Anem/CG-Anem.h"
#include "Modules/BH1750/BH1750.h"
#include "Hardware/CDC/CDC.h"
#include "Modules/ST7735/ST7735.h"
#include "Hardware/Timer.h"
#include "Hardware/InterCom.h"
#include "Display/Display.h"
#include "Message.h"
#include <cstdlib>


static void Test();


int main(void)
{
    HAL::Init();

    ST7735::Init();

    BME280::Init();

    HC12::Init();

    CG_Anem::Init();

    BH1750::Init();

    InterCom::SetDirection((Direction::E)(Direction::CDC | Direction::HC12 | Direction::Display));

    while (1)
    {
        float temp = 0.0f;
        float pressure = 0.0f;
        float humidity = 0.0;

        if (BME280::GetMeasures(1000, &temp, &pressure, &humidity))
        {
            InterCom::Send(TypeMeasure::Temperature, temp);
            InterCom::Send(TypeMeasure::Pressure, pressure);
            InterCom::Send(TypeMeasure::Humidity, humidity);
        }

        float velocity = 0.0f;

        if (CG_Anem::GetMeasure(1000, &velocity))
        {
            InterCom::Send(TypeMeasure::Velocity, velocity);
        }

        float illumination = 0.0f;

        if (BH1750::GetMeasure(1000, &illumination))
        {
            InterCom::Send(TypeMeasure::Illumination, illumination);
        }

        Display::Update();

        Test();
    }
}


void Test()
{
    static uint8 buf[160];
    
    TimeMeterMS meter;

    for (int i = 0; i < 240; i++)
    {
        for (int y = 0; y < 1; y++)
        {
            uint num_segments = 0;
            uint color = 0;
            uint num_points = 0;

            uint8* line = buf;                  // ”казатель на очередную передаваемую линию
            uint8* end = line + 160;

            DynamicMessage<1024> message(Command::Paint_DirectLine);

            message.PushByte(0);                            // —юда потом положим размер

            uint8 points = *line;

            uint8* buffer = message.Data();

            {
                color = (uint8)(points & 0x0f);
                *buffer++ = (uint8)color;
                num_points = 1;
                num_segments = 1;

                if ((uint)(points >> 4) == color)
                {
                    num_points++;
                }
                else
                {
                    *buffer++ = (uint8)num_points;
                    color = (uint8)(points >> 4);
                    *buffer++ = (uint8)color;
                    num_points = 1;
                    num_segments++;
                }
            }

            line++;

            while (line != end)
            {
                points = *line;

                uint new_color = (uint)(points & 0x0f);
                
                if (new_color == color)
                {
                    num_points++;
                }
                else
                {
                    *buffer++ = (uint8)num_points;
                    color = (uint8)(new_color);
                    num_points = 1;
                    num_segments++;
                }

                new_color = (uint)(points >> 4);
                
                if (new_color == color)
                {
                    num_points++;
                }
                else
                {
                    *buffer++ = (uint8)num_points;
                    color = (uint8)(new_color);
                    *buffer++ = (uint8)color;
                    num_points = 1;
                    num_segments++;
                }

                line++;
            }

            message.PushByte((uint8)num_points);

            message.PushByte(1, (uint8)num_segments);
        }
    }

    volatile uint time = meter.ElapsedTime();
    Display::SetTime(time);
}

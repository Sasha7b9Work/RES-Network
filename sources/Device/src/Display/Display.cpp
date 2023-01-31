// 2022/04/20 08:53:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "Hardware/Timer.h"
#include "Display/Colors.h"
#include "Modules/ST7735/ST7735.h"
#include "Display/Font/Font.h"
#include "Utils/Text/String.h"
#include "Utils/Text/Text.h"
#include "Display/Zones.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Hardware/HAL/HAL.h"
#include <cstdlib>


namespace Display
{
    bool need_redraw = true;

    uint time_prev_redraw = 0;

    static ZoneFPS zoneFPS;

    static Zone *zones[] =
    {
        &zoneFPS,
        nullptr
    };

    void DrawZones();

    struct Measure
    {
        String<> old;
        String<> current;

        TypeMeasure::E type;
        float value;                // Последнее установленное значение
        int position;               // Текущая отрисовываемая позиция
        uint time;                  // Время последнего изменения текущей отрисовываемой позиции

        Measure(TypeMeasure::E t) : type(t), value(0.0f), position(0), time(0) {}

        void Draw(const int x, const int y, int size = 1);

        String<> Name();

        String<> Units();
    };

    static Measure measures[TypeMeasure::Count] =
    {
        Measure(TypeMeasure::Pressure),
        Measure(TypeMeasure::Illumination),
        Measure(TypeMeasure::Temperature),
        Measure(TypeMeasure::Humidity)
#ifdef TYPE_1
        ,Measure(TypeMeasure::Velocity),
#endif
        Measure(TypeMeasure::DewPoint),
        Measure(TypeMeasure::AccelerateX),
        Measure(TypeMeasure::AccelerateY),
        Measure(TypeMeasure::AccelerateZ)
    };

    static void DrawMeasures();

    // Вывести одно измерение на весь экран
    static void DrawBigMeasure();

    static void DrawTime();

    static void DrawAcceleration();

    namespace Buffer
    {
        static uint8 buffer[WIDTH * HEIGHT / 2];       // Четырёхбитный цвет

        uint8 GetPixels(int x, int y)
        {
            return buffer[(y * WIDTH + x) / 2];
        }

        uint8* GetLine(int x, int y)
        {
            return &buffer[(y * WIDTH + x) / 2];
        }

        static void SetPoint(int x, int y)
        {
            if (x < 0) { return; }
            if (y < 0) { return; }
            if (x >= WIDTH) { return; }
            if (y >= HEIGHT) { return; }

            uint8* pixels = &buffer[(y * WIDTH + x) / 2];

            uint8 value = *pixels;

            if (x & 0x1)
            {
                value &= 0x0F;
                value |= (Color::GetCurrent() << 4);
            }
            else
            {
                value &= 0xF0;
                value |= Color::GetCurrent();
            }

            *pixels = value;
        }

        static void Fill(Color::E color)
        {
            uint8 value = (uint8)((int)(color) | (int)(color << 4));

            std::memset(buffer, value, WIDTH * HEIGHT / 2);
        }
    }
}


void HLine::Draw(int x0, int y, Color::E color)
{
    Color::SetCurrent(color);

    for (int x = x0; x < x0 + width + 1; x++)
    {
        Display::Buffer::SetPoint(x, y);
    }
}


void VLine::Draw(int x, int y0, Color::E color)
{
    Color::SetCurrent(color);

    for (int y = y0; y < y0 + height; y++)
    {
        Display::Buffer::SetPoint(x, y);
    }
}


void Point::Set(int x, int y, Color::E color)
{
    Color::SetCurrent(color);

    Display::Buffer::SetPoint(x, y);
}


void Rectangle::Fill(int x0, int y0, Color::E color)
{
    Color::SetCurrent(color);

    for (int x = x0; x < x0 + width + 1; x++)
    {
        VLine(height + 1).Draw(x, y0);
    }
}


void Rectangle::DrawFilled(int x, int y, Color::E fill, Color::E draw)
{
    Rectangle(width, height).Draw(x, y, draw);

    Rectangle(width - 2, height - 2).Fill(x + 1, y + 1, fill);
}


void Rectangle::Draw(int x, int y, Color::E color)
{
    HLine(width).Draw(x, y, color);
    HLine(width).Draw(x, y + height);
    VLine(height).Draw(x, y);
    VLine(height).Draw(x + width, y);
}


void Display::SetMeasure(TypeMeasure::E type, float value)
{
    Measure &measure = measures[type];

    if (value == measure.value) //-V550
    {
        return;
    };

    measure.old.SetFormat(measure.current.c_str());

    measure.position = 0;
    measure.time = TIME_MS;
    measure.value = value;

    measure.current.SetFormat("%f", value);
    measure.current[6] = '\0';
}


void Display::Measure::Draw(const int x0, const int y0, int size)
{
    int width_zone = 38;
    int height_zone = 12;
    int y_zone = y0;

    Rectangle(width_zone, height_zone).Fill(x0, y_zone, Color::BLACK);

    /*
    if (position >= current.Size())
    {
    */

    Font::Text::DrawBig(x0, y0, size, current.c_str(), Color::WHITE);

//    current.Draw(x0, y0, Color::GREEN);

    /*
    }
    else
    {
        int x = x0;

        for (int i = 0; i < position; i++)
        {
            x = Char(current[i]).Draw(x, y0, size, Color::GREEN) + size;
        }

        int x_rect = x;

        for (int i = position; i < old.Size(); i++)
        {
            x = Char(old[i]).Draw(x, y0, size, Color::GREEN) + size;
        }

        if (TIME_MS > time + 25)
        {
            position++;
            time = TIME_MS;
        }

        Rectangle(5 * size, 7 * size).Fill(x_rect, y0 + 1, Color::WHITE);
    }
    */

    ST7735::WriteBuffer(x0, y_zone, width_zone, height_zone);
}


void Display::DrawZones()
{
    for (int i = 0; true; i++)
    {
        Zone *zone = zones[i];

        if (zone == nullptr)
        {
            break;
        }

        zone->Draw();
    }
}


void Display::BeginScene(Color::E color)
{
    Buffer::Fill(color);
}


void Display::EndScene()
{
    ST7735::WriteBuffer(0, 0, WIDTH, HEIGHT);
}


void Display::Update()
{
    TimeMeterMS meter_fps;

    if (Menu::Opened())
    {
        Menu::Draw();

        need_redraw = true;
    }
    else
    {
        if (gset.display.typeDisplaydInfo.IsAllMeasures())
        {
            if (need_redraw)
            {
                BeginScene(Color::BLACK);
            }

            DrawMeasures();

            if (need_redraw)
            {
                EndScene();

                need_redraw = false;
            }

//            DrawZones();
        }
        else
        {
            DrawBigMeasure();
        }

        zoneFPS.string.SetFormat("%02d ms", meter_fps.ElapsedTime());
    }
}


void Display::DrawMeasures()
{
    Font::Set(TypeFont::_12_10);

    const int x0 = 3;

    // Пустое место между строками
    const int d_lines = (Display::HEIGHT - 5 * Font::Height()) / (5 + 1);

    const int y0 = d_lines;
    const int dY = d_lines + Font::Height();

    static const int NUM_MEASURES = 3;

    static const TypeMeasure::E types[NUM_MEASURES] =
    {
        TypeMeasure::Temperature,
        TypeMeasure::Humidity,
        TypeMeasure::DewPoint
    };

    for (int i = 0; i < NUM_MEASURES; i++)
    {
//        if (gset.display.show_measure[types[i]])
        {
            int y = y0 + i * dY;

            if (need_redraw)
            {
                String<>("%s", measures[types[i]].Name().c_str()).Draw(x0, y, Color::WHITE);
                measures[types[i]].Units().Draw(x0 + 134, y);
            }

            measures[types[i]].Draw(93, y);
        }
    }

    DrawTime();

    DrawAcceleration();
}


void Display::DrawTime()
{
    int width = 160;
    int height = 32;
    int y = 85;

    Rectangle(width, height).Fill(10, y - 1, Color::BLACK);

    PackedTime time = HAL_RTC::GetPackedTime();

    String<>("%02d:%02d:%04d", time.day, time.month, time.year).Draw(5, 85, Color::WHITE);

    String<>("%02d:%02d:%02d", time.hours, time.minutes, time.seconds).Draw(5, 105);
}


void Display::DrawAcceleration()
{
    int x = 65;
    int dX = 33;

    String<>("%3.2f", measures[TypeMeasure::AccelerateX].value).Draw(x, 105);
    x += dX;

    String<>("%3.2f", measures[TypeMeasure::AccelerateY].value).Draw(x, 105);
    x += dX;

    String<>("%3.2f", measures[TypeMeasure::AccelerateZ].value).Draw(x, 105);

    ST7735::WriteBuffer(0, 85, 160, 32);
}


void Display::DrawBigMeasure()
{
    Font::Set(TypeFont::_8);

    BeginScene(Color::BLACK);

    static const int x[TypeMeasure::Count] =
    {
        30,
        10,
        12,
        28
#ifdef TYPE_1
        , 35
#endif
    };

    Measure &measure = measures[gset.display.typeDisplaydInfo.value];

    Font::Text::DrawBig(x[measure.type], 15, 2, measure.Name().c_str(), Color::_1);

    measures[measure.type].Draw(27, 50, 4);

    Font::Text::DrawBig(68, 95, 2, measure.Units().c_str(), Color::_1);

    EndScene();
}


String<> Display::Measure::Name()
{
    static const pchar names[TypeMeasure::Count] =
    {
        "ДАВЛЕНИЕ",
        "ОСВЕЩЕННОСТЬ",
        "ТЕМПЕРАТУРА",
        "ВЛАЖНОСТЬ"
#ifdef TYPE_1
        , "СКОРОСТЬ",
#endif
        "ТОЧКА     РОСЫ"
    };

    return String<>(names[type]);
}

String<> Display::Measure::Units()
{
    static const pchar units[TypeMeasure::Count] =
    {
        "гПа",
        "лк",
        "ЁС",
        "%%"
#ifdef TYPE_1
        , "м/с",
#endif
        "ЁС"
    };

    return String<>(units[type]);
}

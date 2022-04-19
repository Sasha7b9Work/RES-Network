// Sasha7b9@tut.by (c)
#include "main.h"
#include "Display/Display.h"
#include "Hardware/Timer.h"
#include "Display/Colors.h"
#include "Modules/ST7735/ST7735.h"
#include "Display/Font/Font.h"
#include "Utils/Text/String.h"
#include "Utils/Text/Text.h"
#include <cstdlib>


namespace Display
{
    struct Measure
    {
        String<> old;
        String<> current;

        float value;                // ��������� ������������� ��������
        int position;               // ������� �������������� �������
        uint time;                  // ����� ���������� ��������� ������� �������������� �������

        Measure() : value(0.0f), position(0), time(0) {}

        void Draw(int x, int y);
    };

    static Measure measures[TypeMeasure::Count];

    static void DrawMeasures();


    namespace Buffer
    {
        static uint8 buffer[WIDTH * HEIGHT / 2];       // ������������ ����

        static uint8* FirstPixels()
        {
            return &buffer[0];
        }

        static void SetPoint(int x, int y)
        {
            if (x < 0) { return; }
            if (y < 0) { return; }
            if (x >= WIDTH) { return; }
            if (y >= HEIGHT) { return; }

            uint8* pixels = &buffer[(y * WIDTH + x) / 2];

            uint8 value = *pixels;

            if (x % 2)
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


void Display::Update()
{
    TimeMeterMS meter;
    static uint time = 0;

    Color::E color2 = Color::WHITE;
    Color::E color1 = Color::BLACK;

    BeginScene(Color::GRAY_25);

    Rectangle(140, 30).Fill(10, 5, Color::BLUE);

    static int y0 = 40;

    HLine(10).Draw(5, y0, color2);

    y0++;

    if (y0 == HEIGHT)
    {
        y0 = 40;
    }

    Font::Set(TypeFont::_8);

    String<>("�������� ������").Draw(40, 10, color1);

    DrawMeasures();

    String<>("%d ms", time).Draw(125, 25, color1);

    EndScene();

    time = meter.ElapsedTime();
}


void Display::BeginScene(Color::E color)
{
    Buffer::Fill(color);
}


void Display::EndScene()
{
    ST7735::WriteData(Buffer::FirstPixels());
}


void HLine::Draw(int x0, int y, Color::E color)
{
    Color::SetCurrent(color);

    for (int x = x0; x < x0 + width; x++)
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

    for (int x = x0; x < x0 + width; x++)
    {
        VLine(height).Draw(x, y0);
    }
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

    if (value == measure.value)
    {
        return;
    };

    measure.old.SetFormat(measure.current.c_str());

    measure.position = 0;
    measure.time = TIME_MS;
    measure.value = value;

    measure.current.SetFormat("%f", value);
}


void Display::DrawMeasures()
{
    int x0 = 20;
    int y0 = 45;
    int dY = 15;

    String<>("�������� :").Draw(x0, y0, Color::GREEN);
    String<>("������������ :").Draw(x0, y0 + dY);
    String<>("��������� :").Draw(x0, y0 + 4 * dY);
    String<>("�������� :").Draw(x0, y0 + 2 * dY);
    String<>("����������� :").Draw(x0, y0 + 3 * dY);

    for (int i = 0; i < TypeMeasure::Count; i++)
    {
        measures[i].Draw(90, y0 + i * dY);
    }
}


void Display::Measure::Draw(int x, int y)
{
    if (position >= current.Size())
    {
        current.Draw(x, y);
    }
    else
    {
        if (TIME_MS - time > 20)
        {
            time = TIME_MS;
            position++;
        }

        for (int i = 0; i < position; i++)
        {
            x = Char(current[i]).Draw(x, y) + 1;
        }

        Rectangle(8, 8).Fill(x, y);

        x += 8;

        for (int i = position; i < current.Size(); i++)
        {
            x = Char(old[i]).Draw(x, y) + 1;
        }
    }
}
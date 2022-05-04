// 2022/04/29 16:45:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Display/Display.h"
#include "Display/Colors.h"
#include "Frame.h"


namespace Display
{
    // ����� ������������ ��������
    wxBitmap bitmap(Display::WIDTH, Display::HEIGHT);

    // ����� ����� ��������
    static wxMemoryDC memDC;

    static wxPen pen = *wxWHITE_PEN;
    static wxBrush brush = *wxWHITE_BRUSH;

    void BeginScene(Color &);

    void DrawText(int x, int y, const wxString &, Color &);

    void EndScene();

    void SetColor(Color &);
}


void Display::Update()
{
    BeginScene(Color::BLACK);

    DrawText(50, 50, _("�������� ������"), Color::WHITE);

    EndScene();
}


void Display::BeginScene(Color &color)
{
    memDC.SelectObject(bitmap);

    SetColor(color);

    memDC.DrawRectangle(0, 0, WIDTH, HEIGHT);
}


void Display::DrawText(int x, int y, const wxString &text, Color &color)
{
    SetColor(color);

    const wxFont font = memDC.GetFont();

    wxFont new_font = font;
    new_font.SetPointSize(20);

    memDC.SetFont(new_font);

    memDC.DrawText(text, x, y);
}


void Display::EndScene()
{
    memDC.SelectObject(wxNullBitmap);

    if (Frame::Self())
    {
        Frame::Self()->Refresh();
    }
}


void Display::SetColor(Color &color)
{
    pen.SetColour(color.value);
    brush.SetColour(color.value);

    memDC.SetBrush(brush);
    memDC.SetPen(pen);

    memDC.SetTextForeground(color.value);
}


void Display::SetValue(int type, float value)
{
    static char buffer[1024];

    sprintf(buffer, "%d = %f\0x0d\0x0a", type, value);

    DWORD num_chars = 0;

//    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), buffer, std::strlen(buffer), &num_chars, NULL);
    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), L"Test string", std::strlen(buffer), &num_chars, NULL);
}

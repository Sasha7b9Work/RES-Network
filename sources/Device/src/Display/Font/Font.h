// 2022/02/11 15:58:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"


struct TypeFont
{
    enum E
    {
        _5,
        _8,
        Count
    };
};


struct Symbol
{
    uchar width;
    uchar bytes[8];
};


struct Font
{
    int height;
    Symbol symbol[256];

    static const Font *font;
    static const Font *fonts[TypeFont::Count];

    static TypeFont::E current;

    static int GetSize();

    static int GetLengthText(pchar text);

    static int GetHeightSymbol(char symbol);

    static int GetLengthSymbol(uchar symbol);

    static void Set(TypeFont::E);

    static int GetSpacing();
};


struct Symbol22
{
    uint8 width;
    int16 words[12];
};


// ����� ������������ �������� 2�2 �����
struct Font22
{
    int height;
    int width;

    Symbol22 symbol[256];
};

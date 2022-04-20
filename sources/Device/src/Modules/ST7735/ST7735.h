// 2022/03/12 09:24:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"


namespace ST7735
{
    void Init();

    void WriteBuffer(uint8 *);

    // �������� ����� ������. x � width ������ ���� �������
    void WriteBuffer(int x, int y, int width, int height);
}

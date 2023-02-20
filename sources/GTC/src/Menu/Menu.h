// 2022/05/05 12:28:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"


namespace Menu
{
    void ShortPress();

    void LongPress();

    void DoubleClick();

    void Draw();

    bool Opened();

    namespace Title
    {
        const int HEIGHT = 27;
    }
}

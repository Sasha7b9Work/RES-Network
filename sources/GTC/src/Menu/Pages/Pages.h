// 2022/05/05 12:27:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"
#include "Menu/MenuItemsDefs.h"


struct PageMain
{
    static const Page * const self;
};


struct PageDisplay
{
    static const Page * const self;

    struct PageMeasures
    {
        static const Page * const self;
    };
};


struct PageHC12
{
    static const Page * const self;
};


struct PageSystem
{
    static const Page * const self;
};

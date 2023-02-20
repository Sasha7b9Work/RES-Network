// 2022/08/05 15:08:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Text/String.h"


class Int
{
public:
    Int(int);
    String<> ToStirng() const;
private:
    int value;
};

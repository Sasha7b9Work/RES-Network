// 2022/08/05 15:09:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Values.h"


Int::Int(int _value) : value(_value)
{

}


String<> Int::ToStirng() const
{
    return String<>("%d", value);
}

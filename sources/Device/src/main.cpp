// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Device.h"


int main(void)
{
    Device::Init();

    while (1)
    {
        Device::Update();
    }
}

// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Device.h"

/*
*   ������ ������� �������� ���������� ID, ������� �������� ����� �������� ����� ����
*/


int main(void)
{
    Device::Init();

    while (true)
    {
        Device::Update();
    }
}

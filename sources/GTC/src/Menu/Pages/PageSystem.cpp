// 2022/08/04 14:32:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"


extern const DPage pageMain;


DEF_GOVERNOR(gSerialNumber,
    "С/Н",
    *PageSystem::self,
    0, (int)0x7FFFFFFF,
    gset.system.serial_number
)


void ClosePageSystem()
{
    PageSystem::self->Close();
}


DEF_BUTTN(bClosePageSystem,
    "Закрыть",
    *PageSystem::self,
    ClosePageSystem
);


DEF_PAGE_2(pageSystem, //-V1027
    "СИСТЕМА",
    pageMain,
    gSerialNumber,
    bClosePageSystem
)


const Page *const PageSystem::self = (const Page *)&pageSystem;

// 2022/05/05 12:26:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"


extern const DPage pageMain;


static void CloseMainPage()
{
    PageMain::self->Close();
}

DEF_BUTTN(bCloseMainPage,
    "Закрыть",
    pageMain,
    CloseMainPage
)

DEF_PAGE_4(pageMain, //-V1027
    "МЕНЮ",
    Page::Empty,
    *PageDisplay::self,
    *PageHC12::self,
    *PageSystem::self,
    bCloseMainPage
)

const Page * const PageMain::self = (const Page *)&pageMain;

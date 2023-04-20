// 2022/05/05 13:14:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"


extern const DPage pageMain;


DEF_CHOICE_2( chPressure,
    "��������",
    *PageDisplay::PageMeasures::self,
    gset.display.show_measure[TypeMeasure::Pressure],
    "����", "���"
)

DEF_CHOICE_2(chTemperature,
    "�����������",
    *PageDisplay::PageMeasures::self,
    gset.display.show_measure[TypeMeasure::Temperature],
    "����", "���"
)

DEF_CHOICE_2(chHumidity,
    "���������",
    *PageDisplay::PageMeasures::self,
    gset.display.show_measure[TypeMeasure::Humidity],
    "����", "���"
)

static void CloseMeasures()
{
    PageDisplay::PageMeasures::self->Close();
};

DEF_BUTTN( bCloseMeasures,
    "�������",
    *PageDisplay::PageMeasures::self,
    CloseMeasures
)

DEF_PAGE_4(pageMeasures, //-V1027
    "���������",
    *PageDisplay::self,
    chPressure,
    chTemperature,
    chHumidity,
    bCloseMeasures
)


void ClosePageDisplay()
{
    PageDisplay::self->Close();
}

DEF_BUTTN( bClosePageDisplay,
    "�������",
    *PageDisplay::self,
    ClosePageDisplay
)

DEF_PAGE_2(pageDisplay, //-V1027
    "�������",
    pageMain,
    pageMeasures,
    bClosePageDisplay
)

const Page * const PageDisplay::self = (const Page *)&pageDisplay;
const Page * const PageDisplay::PageMeasures::self = (const Page *)&pageMeasures;

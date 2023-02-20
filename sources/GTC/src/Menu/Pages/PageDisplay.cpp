// 2022/05/05 13:14:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"


extern const DPage pageMain;


DEF_CHOICE_2( chPressure,
    "Давление",
    *PageDisplay::PageMeasures::self,
    gset.display.show_measure[TypeMeasure::Pressure],
    "Откл", "Вкл"
)

DEF_CHOICE_2(chIllumination,
    "Освещённость",
    *PageDisplay::PageMeasures::self,
    gset.display.show_measure[TypeMeasure::Illumination],
    "Откл", "Вкл"
)

#ifdef TYPE_1
DEF_CHOICE_2(chVelocity,
    "Скорость",
    *PageDisplay::PageMeasures::self,
    gset.display.show_measure[TypeMeasure::Velocity],
    "Откл", "Вкл"
)
#endif

DEF_CHOICE_2(chTemperature,
    "Температура",
    *PageDisplay::PageMeasures::self,
    gset.display.show_measure[TypeMeasure::Temperature],
    "Откл", "Вкл"
)

DEF_CHOICE_2(chHumidity,
    "Влажность",
    *PageDisplay::PageMeasures::self,
    gset.display.show_measure[TypeMeasure::Humidity],
    "Откл", "Вкл"
)

static void CloseMeasures()
{
    PageDisplay::PageMeasures::self->Close();
};

DEF_BUTTN( bCloseMeasures,
    "Закрыть",
    *PageDisplay::PageMeasures::self,
    CloseMeasures
)

#ifdef TYPE_1
DEF_PAGE_6( pageMeasures, //-V1027
    "ИЗМЕРЕНИЯ",
    *PageDisplay::self,
    chPressure,
    chIllumination,
    chTemperature,
    chHumidity,
    chVelocity,
    bCloseMeasures
)
#endif

#ifdef TYPE_2
DEF_PAGE_5(pageMeasures, //-V1027
    "ИЗМЕРЕНИЯ",
    *PageDisplay::self,
    chPressure,
    chIllumination,
    chTemperature,
    chHumidity,
    bCloseMeasures
)
#endif


void ClosePageDisplay()
{
    PageDisplay::self->Close();
}

DEF_BUTTN( bClosePageDisplay,
    "Закрыть",
    *PageDisplay::self,
    ClosePageDisplay
)

DEF_PAGE_2(pageDisplay, //-V1027
    "ДИСПЛЕЙ",
    pageMain,
    pageMeasures,
    bClosePageDisplay
)

const Page * const PageDisplay::self = (const Page *)&pageDisplay;
const Page * const PageDisplay::PageMeasures::self = (const Page *)&pageMeasures;

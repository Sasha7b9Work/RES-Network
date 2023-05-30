// 2022/05/05 12:28:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Menu.h"
#include "Display/Display.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Display/Font/Font.h"


void Menu::ShortPress(Key::E key)
{
    if (!Opened())
    {
        if (key == Key::_1)
        {
            ++gset.display.typeDisplaydInfo;

            if (gset.display.typeDisplaydInfo.value == TypeDisplayedInformation::Menu)
            {
                gset.display.typeDisplaydInfo.value = TypeDisplayedInformation::MeasureTemperature;
            }
        }
        else if (key == Key::_2)
        {
            PageMain::self->Open();
        }
    }
    else
    {
        Item::Opened()->ShortPressure(key);
    }

    Display::need_redraw = true;
}


void Menu::LongPress(Key::E key)
{
    if (!Opened())
    {

    }
    else
    {
        Item::Opened()->LongPressure(key);
    }

    Display::need_redraw = true;
}


void Menu::DoubleClick(Key::E)
{
    if (Opened())
    {
        Item::Opened()->DoubleClick();
    }

    Display::need_redraw = true;
}


bool Menu::Opened()
{
    return (Item::Opened() != &Page::Empty);
}


void Menu::Draw()
{
    Display::BeginScene(Color::BLACK);

    Font::Set(TypeFont::_12_10);

    if (Item::Opened()->IsPage())
    {
        Item::Opened()->Draw(0, 0, true);
    }
    else
    {
        const Item *opened = Item::Opened();

        const Page *keeper = opened->Keeper();

        keeper->Draw(0, 0, false);

        Item::Opened()->DrawOpened(0, Menu::Title::HEIGHT, true);
    }

    Display::EndScene();
}

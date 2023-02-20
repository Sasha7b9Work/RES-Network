// 2022/05/05 12:27:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Pages.h"
#include "Display/Display.h"
#include "Utils/Text/String.h"
#include "Menu/Menu.h"
#include "Utils/Text/String.h"
#include "Utils/Values.h"
#include "Utils/Math.h"


Item Item::Empty;
const Item *Item::opened_item = &Item::Empty;
Governor::ActiveControl::E Governor::active_control = Governor::ActiveControl::Increase;


void Item::Open() const
{
    opened_item = this;
}


void Item::Close() const
{
    if (this == PageMain::self)
    {
        opened_item = &Page::Empty;
    }
    else
    {
        opened_item = ToDItem()->keeper;
    }
}


void Page::Close() const
{
    Item::Close();

    uint8 *currentItem = ToDPage()->currentItem;

    if (*currentItem == NumItems() - 1)
    {
        *currentItem = 0;
    }
}


void Item::Draw(int x, int y, bool active) const
{
    if (IsOpened())
    {
        DrawOpened(x, y, active);
    }
    else
    {
        DrawClosed(x, y, active);
    }
}


bool Item::IsOpened() const
{
    if (IsPage())
    {
        return (this == opened_item) || (Item::Opened()->Keeper() == this);
    }

    return (this == opened_item);
}


void Item::DrawOpened(int x, int y, bool active) const
{
    if (IsPage())
    {
        ToPage()->DrawOpened(x, y, active);
    }
    else if (IsGovernor())
    {
        ToGovernor()->DrawOpened(x, y, active);
    }
}


void Item::DrawClosed(int x, int y, bool active) const
{
    Color::E fill = Color::BLACK;
    Color::E draw = Color::WHITE;

    if (ToDItem()->keeper->CurrentItem() == this)
    {
        fill = Color::MenuItem();
    }

    if (Item::Opened() != Keeper())
    {
        draw = Color::GRAY_25;
        fill = Color::GRAY_10;
    }

    Rectangle(Item::WIDTH, Item::HEIGHT).DrawFilled(x, y, fill, draw);

    if (IsPage())
    {
        ToPage()->DrawClosed(x, y, active);
    }
    else if (IsChoice())
    {
        ToChoice()->DrawClosed(x, y, active);
    }
    else if (IsButton())
    {
        ToButton()->DrawClosed(x, y, active);
    }
    else if (IsGovernor())
    {
        ToGovernor()->DrawClosed(x, y, active);
    }
}


String<> Item::Title() const
{
    return String<>(ToDItem()->title);
}


void Page::DrawOpened(int x, int y, bool active) const
{
    DrawTitle(x, y);

    DrawItems(x, y + Menu::Title::HEIGHT, active);
}


void Page::DrawTitle(int x, int y) const
{
    Rectangle(Item::WIDTH, 27).DrawFilled(x, y, Color::BLACK, Color::WHITE);

    Title().Draw(x + 10, y + 10, Color::BLUE);
}


void Page::DrawItems(int x, int y, bool active) const
{
    for (int i = FirstItemOnScreen(); i < LastItemOnScreen(); i++)
    {
        ToDPage()->items[i]->DrawClosed(x, y, active);
        y += Item::HEIGHT;
    }
}


const Item *Page::CurrentItem() const
{
    const Item *const *items = ToDPage()->items;

    uint8 *currentItem = ToDPage()->currentItem;

    return items[*currentItem];
}


void Page::DrawClosed(int x, int y, bool active) const
{
    Title().Draw(x + 10, y + 5, Color::MenuLetters(active));
}


pchar Choice::CurrentName() const
{
    const DChoice *choice = ToDChoice();

    return choice->names[*choice->cell];
}


void Choice::DrawClosed(int x, int y, bool active) const
{
    Title().Draw(x + 10, y + 5, Color::MenuLetters(active));

    String<>(CurrentName()).Draw(x + 130, y + 5);
}


void Button::DrawClosed(int x, int y, bool active) const
{
    Title().Draw(x + 10, y + 5, Color::MenuLetters(active));
}


void Governor::DrawClosed(int x, int y, bool active) const
{
    Title().Draw(x + 10, y + 5, Color::MenuLetters(active));

    Int(*ToDGovernor()->value).ToStirng().DrawRelativelyRight(x + 150, y + 5, Color::MenuLetters(active));
}


void Governor::DrawOpened(int x, int y, bool active) const
{
    Color::E fill = Color::BLACK;
    Color::E draw = Color::WHITE;

    if (ToDItem()->keeper->CurrentItem() == this)
    {
        fill = Color::MenuItem();
    }

    if (!active && (Item::Opened() != Keeper()))
    {
        draw = Color::GRAY_25;
        fill = Color::GRAY_10;
    }

    Rectangle(Item::WIDTH, Item::HEIGHT).DrawFilled(x, y, fill, draw);

    Title().Draw(x + 10, y + 5, Color::MenuLetters(true));

    DrawControls(x, y);

    Int(*ToDGovernor()->value).ToStirng().DrawRelativelyRight(x + 150, y + 5, Color::MenuLetters(active));
}


void Governor::DrawControls(int x, int y) const
{
    DrawControl(x + 50, y + 3, String<>("\x95"), active_control == ActiveControl::Increase);

    DrawControl(x + 50, y + 12, String<>("\x85"), active_control == ActiveControl::Decrease);

    DrawControl(x + 80, y + 5, String<>("\x88"), active_control == ActiveControl::Close);
}


void Governor::DrawControl(int x, int y, const String<> &symbol, bool active) const
{
    Color::E color_draw = Color::MenuLetters(true);

    if (active)
    {
        int width = 10;
        int height = 6;

        if (active_control == ActiveControl::Close)
        {
            width = 11;
            height = 11;
        }

        Rectangle(width, height).Fill(x - 1, y - 1, Color::MenuLetters(true));
        color_draw = Color::MenuItem();
    }

    symbol.Draw(x, y, color_draw);
}


int Page::FirstItemOnScreen() const
{
    return (*ToDPage()->currentItem / Page::NUM_ITEMS_ON_SCREEN) * Page::NUM_ITEMS_ON_SCREEN;
}


int Page::LastItemOnScreen() const
{
    int result = FirstItemOnScreen() + Page::NUM_ITEMS_ON_SCREEN;

    if (result > NumItems())
    {
        result = NumItems();
    }

    return result;
}


int Page::NumItems() const
{
    for (int i = 0; ; i++)
    {
        if (ToDPage()->items[i] == nullptr)
        {
            return i;
        }
    }
}


void Page::ShortPressure() const
{
    uint8 *currentItem = ToDPage()->currentItem;

    *currentItem = (uint8)(*currentItem + 1);

    if (*currentItem == NumItems())
    {
        *currentItem = 0;
    }
}


void Page::LongPressure() const
{
    const Item *item = CurrentItem();

    if (item->IsPage())
    {
        item->ToPage()->Open();
    }
    else if (item->IsChoice())
    {
        item->ToChoice()->LongPressure();
    }
    else if (item->IsButton())
    {
        item->ToButton()->LongPressure();
    }
    else if (item->IsGovernor())
    {
        item->ToGovernor()->LongPressure();
    }
}


void Page::DoubleClick() const
{

}


void Choice::ShortPressure() const
{

}


void Button::ShortPressure() const
{

}


void Choice::LongPressure() const
{
    uint8 *cell = ToDChoice()->cell;

    *cell = (uint8)(*cell + 1);

    if (*cell == ToDChoice()->count)
    {
        *cell = 0;
    }
}


void Choice::DoubleClick() const
{

}


void Button::DoubleClick() const
{

}


void Item::ShortPressure() const
{
    switch (ToDItem()->type)
    {
    case TypeItem::Page:        ToPage()->ShortPressure();      break;
    case TypeItem::Choice:      ToChoice()->ShortPressure();    break;
    case TypeItem::Button:      ToButton()->ShortPressure();    break;
    case TypeItem::Governor:    ToGovernor()->ShortPressure();  break;
    case TypeItem::Count:                                       break;
    }
}


void Item::LongPressure() const
{
    switch (ToDItem()->type)
    {
    case TypeItem::Page:        ToPage()->LongPressure();       break;
    case TypeItem::Choice:      ToChoice()->LongPressure();     break;
    case TypeItem::Button:      ToButton()->LongPressure();     break;
    case TypeItem::Governor:    ToGovernor()->LongPressure();   break;
    case TypeItem::Count:                                       break;
    }
}


void Item::DoubleClick() const
{
    switch (ToDItem()->type)
    {
    case TypeItem::Page:        ToPage()->DoubleClick();        break;
    case TypeItem::Choice:      ToChoice()->DoubleClick();      break;
    case TypeItem::Button:      ToButton()->DoubleClick();      break;
    case TypeItem::Governor:    ToGovernor()->DoubleClick();    break;
    case TypeItem::Count:                                       break;
    }
}


void Governor::ShortPressure() const
{
    if (Item::Opened())
    {
        if (active_control == ActiveControl::Close)
        {
            active_control = ActiveControl::Increase;
            Close();
        }
        else
        {
            const DGovernor *data = ToDGovernor();

            int *value = data->value;

            if (active_control == ActiveControl::Increase && *value < data->max)
            {
                *value = *value + 1;
            }
            else if (active_control == ActiveControl::Decrease && *value > data->min)
            {
                *value = *value - 1;
            }
        }
    }
}


void Governor::LongPressure() const
{
    if (Item::Opened() != this)
    {
        Open();
    }
    else
    {
        Math::CircleIncrease<int>((int *)&active_control, 0, ActiveControl::Count - 1);
    }
}


void Governor::DoubleClick() const
{

}

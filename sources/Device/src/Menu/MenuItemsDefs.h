// 2022/05/06 13:37:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define DEF_BUTTN(name, title, keeper, function)    \
    static const DButton name =                     \
    {                                               \
        TypeItem::Button,                           \
        title,                                      \
        (const Page *)&keeper,                      \
        function                                    \
    };


#define DEF_ITEMS_1(name, item)                                                             \
    static const Item * const name[] = { (Item *)&item, nullptr };

#define DEF_ITEMS_3(name, item1, item2, item3)                                              \
    static const Item * const name[] = { (Item *)&item1,(Item *)&item2, (Item *)&item3, nullptr };

#define DEF_ITEMS_6(name, item1, item2, item3, item4, item5, item6)                         \
    static const Item * const name[] = { (Item *)&item1, (Item *)&item2, (Item *)&item3,    \
        (Item *)&item4, (Item *)&item5, (Item *)&item6, nullptr };


#define DEF_PAGE(name, title, keeper, items)                                                \
    static uint8 ci##name = 0;                                                              \
    const DPage name = { TypeItem::Page, title, (const Page *)&keeper, items, &ci##name };


#define DEF_PAGE_1(name, title, keeper, item1)  \
    DEF_ITEMS_1(items##name, item1)             \
    DEF_PAGE(name, title, keeper, items##name)

#define DEF_PAGE_3(name, title, keeper, item1, item2, item3)  \
    DEF_ITEMS_3(items##name, item1, item2, item3)             \
    DEF_PAGE(name, title, keeper, items##name)

#define DEF_PAGE_6(name, title, keeper, item1, item2, item3, item4, item5, item6)  \
    DEF_ITEMS_6(items##name, item1, item2, item3, item4, item5, item6)             \
    DEF_PAGE(name, title, keeper, items##name)

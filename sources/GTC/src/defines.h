// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

typedef unsigned char uint8;
typedef signed char int8;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned int uint;
typedef const char *pchar;
typedef unsigned char uchar;


// Промежуток между измерениями
#define TIME_MEASURE  500


#ifdef WIN32
    #define IN_MODE_TEST
#else
    #define nullptr 0 //-V1059
    #pragma diag_suppress 2748
#endif


#define TYPE_1          // Присутствуют все датчики
//#define TYPE_2          // Отсутствует датчик скорости воздуха

#ifdef TYPE_1
    #ifdef TYPE_2
        static_assert(0, "Нельзя оба типа определять");
    #endif
#endif

#ifndef TYPE_1
    #ifndef TYPE_2
        static_assert(0, "Определите тип - 1 или 2");
    #endif
#endif


union BitSet32
{
    uint   word;
    uint16 half_word[2];
    uint8  byte[4];
};


#define _GET_BIT(value, bit)   (((value) >> (bit)) & 0x01)
#define _SET_BIT(value, bit)   ((value) |= (1 << (bit)))
#define _CLEAR_BIT(value, bit) ((value) &= (~(1 << (bit))))


#define ERROR_VALUE_FLOAT   1.111e29f


#include "Log.h"

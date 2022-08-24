// Sasha7b9@tut.by (c)
#include "defines.h"
#include "Hardware/InterCom.h"
#include "Hardware/CDC/CDC.h"
#include "Modules/HC12/HC12.h"
#include "Modules/ST7735/ST7735.h"
#include "Display/Display.h"
#include "Utils/Text/String.h"
#include "Utils/Buffer.h"
#include "Utils/Math.h"

#ifdef GUI
    #include "Hardware/LAN/ClientTCP.h"
#endif


namespace InterCom
{
    /*
    *  ������ ���������.
    *  0      - 'A'
    *  1      - 'B'
    *  2      - 'C'
    *  3...6  - hash[7...11]
    *  7      - type
    *  8...11 - value
    */

    Direction::E direction = Direction::_None;

    Buffer<uint8, 12> CreateMessage(TypeMeasure::E type, float value)
    {
        Buffer<uint8, 12> message;

        message[0] = 'A';
        message[1] = 'B';
        message[2] = 'C';

        uint8 buffer[5];
        buffer[0] = (uint8)type;
        std::memcpy(&buffer[1], &value, 4);

        uint hash = Math::CalculateHash(buffer, 5);

        std::memcpy(&message[3], &hash, 4);

        std::memcpy(&message[7], buffer, 5);

        return message;
    }
}


void InterCom::SetDirection(Direction::E dir)
{
    direction = dir;
}


void InterCom::Send(TypeMeasure::E type, float measure)
{
    static const pchar names[TypeMeasure::Count] =
    {
        "Pressure",
        "Luminance",
        "Temperature",
        "Humidity"
#ifdef TYPE_1
        , "Velocity"
#endif
    };

    static const pchar units[TypeMeasure::Count] =
    {
        "hPa",
        "lk",
        "degress Celsius",
        "%%"
#ifdef TYPE_1
        , "m/s"
#endif
    };

    if (direction & Direction::Display)
    {
        Display::SetMeasure(type, measure);
    }

    Buffer<uint8, 12> data = CreateMessage(type, measure);

    String<> message("%s : %f %s", names[type], measure, units[type]);

    if (direction & Direction::CDC)
    {
        CDC::Transmit(message.c_str(), message.Size() + 1);
    }

    if (direction & Direction::HC12)
    {
//        HC12::Transmit(message.c_str(), message.Size() + 1);
        HC12::Transmit(data.Data(), 12);
    }

#ifdef GUI

    ClientTCP::Transmit(data.Data(), 12);

#endif
}

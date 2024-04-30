// 2024/01/11 11:02:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/HI50/HI50.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Hardware/CDC/CDC.h"


namespace HI50
{
//    static const uint8 TURN_ON = 0x4f;
//    static const uint8 MEAS_AUTO = 0x44;
//    static const uint8 MEAS_HI = 0x4D;

    struct State
    {
        enum E
        {
            IDLE,
            WAIT_TURN_ON,
            WAIT_MEASURE
        };
    };

    static State::E state = State::IDLE;

    static bool is_init = false;
}


bool HI50::Init()
{
    is_init = true;

    return true;
}


void HI50::Update()
{
    if (!is_init)
    {
        return;
    }

    switch (state)
    {
    case State::IDLE:
//        HAL_USART1::Send(TURN_ON);
        state = State::WAIT_TURN_ON;
        break;

    case State::WAIT_TURN_ON:
        break;

    case State::WAIT_MEASURE:
        break;
    }
}


void HI50::CallbackOnReceive(pchar /*message*/)
{
    switch (state)
    {
    case State::IDLE:
        break;

    case State::WAIT_TURN_ON:
//        HAL_USART1::Send(MEAS_HI);
        state = State::WAIT_MEASURE;
        break;

    case State::WAIT_MEASURE:
//        CDC::TransmitF("%s", message);
//        HAL_USART1::Send(MEAS_HI);
        break;
    }
}

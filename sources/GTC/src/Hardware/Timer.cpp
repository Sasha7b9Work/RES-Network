#include "defines.h"
#include "Hardware/Timer.h"
#include <stm32f1xx_hal.h>


uint Timer::CurrentTime()
{
    return HAL_GetTick();
}


TimeMeterMS::TimeMeterMS()
{
    Reset();
}


void TimeMeterMS::Reset()
{
    time_reset = TIME_MS;
    time_pause = 0;
}


void TimeMeterMS::Pause()
{
    time_pause = TIME_MS;
}


void TimeMeterMS::Continue()
{
    time_reset += (TIME_MS - time_pause);
}


uint TimeMeterMS::ElapsedTime()
{
    return TIME_MS - time_reset;
}


void TimeMeterMS::WaitMS(uint dT)
{
    uint time_enter = ElapsedTime();

    while (ElapsedTime() - time_enter < dT)
    {

    }
}

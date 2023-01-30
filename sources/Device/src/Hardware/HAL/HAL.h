// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Text/String.h"


struct PackedTime
{
    unsigned hours : 5;
    unsigned minutes : 6;
    unsigned seconds : 6;
    unsigned year : 16;
    unsigned month : 4;
    unsigned notUsed0 : 4;
    unsigned day : 5;
    unsigned ms : 27;
    PackedTime(uint h = 11, uint m = 11, uint s = 11, uint d = 11, uint mo = 11, uint y = 11) :
        hours(h), minutes(m), seconds(s), year(y), month(mo), notUsed0(0), day(d), ms(0) {};
    // »зменение значени€ пол€ на +/- 1
    void ChangeHours(int delta);
    void ChangeMinutes(int delta);
    void ChangeSeconds(int delta);
    void ChangeDay(int delta);
    void ChangeMonth(int delta);
    void ChangeYear(int delta);

    // ¬озвращает структура PackedTime, врем€ в которой отстоит в будущее на timeMS миллисекунд
    void AddTime(uint timeMS);

    String<> ToString() const;

private:

    // ƒобавить к дате hours часов
    void AddHours(uint hours);
    void AddMinutes(uint minutes);
    void AddSeconds(uint seconds);
    void AddMilliseconds(uint ms);
    void AddDays(uint days);
    void AddMonths(uint months);
};


namespace HAL
{
    void Init();

    void Delay(unsigned int timeMS);
}


namespace HAL_FLASH
{

}


namespace HAL_I2C1
{
    void Init();

    int8 Read(uint8 dev_id, uint8 reg_addr, uint8 *reg_data, uint16 len);
    int8 Read16(uint8 dev_id, uint8 *data);

    int8 Write(uint8 dev_id, uint8 reg_addr, uint8 *reg_data, uint16 len);
    int8 Write8(uint8 dev_id, uint8 data);
}


namespace HAL_RTC
{
    void Init();

    PackedTime GetPackedTime();

    bool SetPackedTime(const PackedTime &time);
}


#ifdef __cplusplus
extern "C" {
#endif 

    void NMI_Handler(void);
    void HardFault_Handler(void);
    void MemManage_Handler(void);
    void BusFault_Handler(void);
    void UsageFault_Handler(void);
    void SVC_Handler(void);
    void DebugMon_Handler(void);
    void PendSV_Handler(void);
    void SysTick_Handler(void);
    void USB_LP_CAN1_RX0_IRQHandler(void);
    void USART1_IRQHandler(void);

#ifdef __cplusplus
}
#endif

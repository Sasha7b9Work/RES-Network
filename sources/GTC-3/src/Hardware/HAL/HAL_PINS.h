// 2023/06/15 14:13:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <stm32f3xx_hal.h>


struct Pin
{
    Pin(GPIO_TypeDef *_gpio, uint16 _pin, uint _mode, uint _pull, uint _alternate = 0U) :
        gpio(_gpio), pin(_pin), mode(_mode), pull(_pull), alternate(_alternate) { }
    void Init();
    void ToLow();
    void ToHi();
    void Set(bool);
    bool IsHi() const;
    bool IsLow() const;
private:
    GPIO_TypeDef *gpio;
    uint16        pin;
    uint          mode;
    uint          pull;
    uint          alternate;
};


struct PinInput : public Pin
{
    PinInput(GPIO_TypeDef *_gpio, uint16 _pin, uint pull) : Pin(_gpio, _pin, GPIO_MODE_INPUT, pull) { }
};


struct PinOutputPP : public Pin
{
    PinOutputPP(GPIO_TypeDef *_gpio, uint16 _pin, uint pull) : Pin(_gpio, _pin, GPIO_MODE_OUTPUT_PP, pull) { }
};


struct PinAF_OD : public Pin
{
    PinAF_OD(GPIO_TypeDef *_gpio, uint16 _pin) : Pin(_gpio, _pin, GPIO_MODE_AF_OD, GPIO_PULLUP) { }
};


struct PinAF_PP : public Pin
{
    PinAF_PP(GPIO_TypeDef *_gpio, uint16 _pin, uint pull) : Pin(_gpio, _pin, GPIO_MODE_AF_PP, pull) { }
};


struct PinAnalog : public Pin
{
    PinAnalog(GPIO_TypeDef *_gpio, uint16 _pin) : Pin(_gpio, _pin, GPIO_MODE_ANALOG, GPIO_NOPULL) { }
};


extern Pin         pinWP;
extern PinOutputPP pinBEEP;
extern PinAnalog   pinADC;
extern PinAF_OD    pinSCL;
extern PinAF_OD    pinSDA;
extern PinAF_PP    pinSCK_SPI1;     // SPI1
extern PinAF_PP    pinMOSI_SPI1;    // SPI1
extern PinInput    pinMISO_SPI1;    // SPI1
extern PinOutputPP pinNSS_SPI1;     // SPI1
extern PinAF_PP    pinTX;           // HC12 TX
extern PinInput    pinRX;           // HC12 RX
extern PinInput    pinKey1;
extern PinInput    pinKey2;
extern PinOutputPP pinCS_HC12;
extern Pin         pinSCL_SPI2;     // SPI2
extern Pin         pinMOSI_SPI2;    // SPI2
extern PinOutputPP pinRESET_ST;     // ST7735
extern PinOutputPP pinDC_ST;        // ST7735
extern PinOutputPP pinCS_ST;        // ST7735

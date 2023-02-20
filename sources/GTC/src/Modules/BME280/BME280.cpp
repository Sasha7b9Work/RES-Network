// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/BME280/BME280.h"
#include "Modules/BME280/bme280_driver.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f1xx_hal.h>
#include <cstring>
#include <cstdlib>


static bme280_dev dev;

static unsigned int timeNext = 1;       // Время следующего измерения


void BME280::Init()
{
    dev.dev_id = BME280_I2C_ADDR_SEC;
    dev.intf = BME280_I2C_INTF;
    dev.read = HAL_I2C1::Read;
    dev.write = HAL_I2C1::Write;
    dev.delay_ms = HAL_Delay;

    bme280_init(&dev);

    uint8_t settings_sel;

    dev.settings.osr_h = BME280_OVERSAMPLING_1X;
    dev.settings.osr_p = BME280_OVERSAMPLING_16X;
    dev.settings.osr_t = BME280_OVERSAMPLING_2X;
    dev.settings.filter = BME280_FILTER_COEFF_16;
    dev.settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

    settings_sel = BME280_OSR_PRESS_SEL;
    settings_sel |= BME280_OSR_TEMP_SEL;
    settings_sel |= BME280_OSR_HUM_SEL;
    settings_sel |= BME280_STANDBY_SEL;
    settings_sel |= BME280_FILTER_SEL;

    bme280_set_sensor_settings(settings_sel, &dev);

    bme280_set_sensor_mode(BME280_NORMAL_MODE, &dev);

    /* Delay while the sensor completes a measurement */
    dev.delay_ms(70);
}


bool BME280::GetMeasures(float* temp, float* pressure, float* humidity)
{
    if(HAL_GetTick() < timeNext)
    {
        return false;
    }

    timeNext += TIME_MEASURE + (std::rand() % 100);

#ifdef IN_MODE_TEST

    static float value = 1.1f;

    value *= 7.1f;
    *temp = value / 100.0f;

    value *= 1.2f;
    *pressure = value / 100.0f;

    value *= 0.83f;
    *humidity = value / 99.28f;

    if (value > 1e4f)
    {
        value = 1.34f;
    }

    return true;

#else

    bme280_data comp_data;

    int8 result = bme280_get_sensor_data(BME280_ALL, &comp_data, &dev);

    if (result == BME280_OK)
    {
        *temp = (float)comp_data.temperature;
        *pressure = (float)comp_data.pressure / 100.0f;
        *humidity = (float)comp_data.humidity;
    }

    return (result == BME280_OK);

#endif
}

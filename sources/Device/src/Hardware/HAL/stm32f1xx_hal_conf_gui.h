#pragma once


#undef __HAL_RCC_GPIOD_CLK_ENABLE
#define __HAL_RCC_GPIOD_CLK_ENABLE()

#undef __HAL_RCC_GPIOA_CLK_ENABLE
#define __HAL_RCC_GPIOA_CLK_ENABLE()

#undef __HAL_RCC_GPIOB_CLK_ENABLE
#define __HAL_RCC_GPIOB_CLK_ENABLE()

#undef __HAL_RCC_SPI2_CLK_ENABLE
#define __HAL_RCC_SPI2_CLK_ENABLE()

#undef SPI2
extern SPI_TypeDef typeDefSPI2;
#define SPI2  (&typeDefSPI2)

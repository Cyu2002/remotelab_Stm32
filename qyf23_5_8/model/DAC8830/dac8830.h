#ifndef _DAC8830_H
#define _DAC8830_H

#include "main.h"




#define SPI_CLK(x) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,x)
#define SPI_SDI(x) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,x)
#define SPI_LE(x) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,x)

void DAC8830_Init(void);
void DAC8830_Write16Byte(u16 byte);
void delay_s_us(void);
void DAC8830_WriteDate(double date);

void DAC8830_WriteAll(void);


#endif
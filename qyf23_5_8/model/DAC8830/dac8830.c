#include "dac8830.h"


void delay_s_us(void){
	u8 i=48;
	while(i--);
}


void DAC8830_Init(void){
	 GPIO_InitTypeDef GPIO_InitStruct = {0};
	 
	 __HAL_RCC_GPIOC_CLK_ENABLE();
	 GPIO_InitStruct.Pin=GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	 GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
	 GPIO_InitStruct.Pull=GPIO_NOPULL;
	 GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_MEDIUM;
	 
	 HAL_GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	 SPI_LE(1);
	 SPI_CLK(0);
	 SPI_SDI(0);
	
}
void DAC8830_Write16Byte(u16 byte){
	u8 i=0;
	SPI_LE(0);
	delay_s_us();
	for(i=0;i<16;i++){
		if(byte&0x8000){
			SPI_SDI(1);
		}
		else{
			SPI_SDI(0);
		}
		byte<<=1;
		delay_s_us();
		SPI_CLK(1);
		delay_s_us();
		SPI_CLK(0);
	}
	SPI_LE(1);
}
void DAC8830_WriteDate(double date){
	u16 a=0;
	a=(u16)(65535.0*(date/2.76));
	DAC8830_Write16Byte(a);
}
void DAC8830_WriteAll(void){
	
	DAC8830_Write16Byte(30000);
	
}


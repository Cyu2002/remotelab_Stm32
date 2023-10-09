#ifndef _adf4002_h_
#define _adf4002_h_

#include "main.h"




#define SCL(x) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,x)
#define SDA(x) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,x)
#define LE(x) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,x)


#define R_Address 0X000000
#define N_Address 0X000001
#define F_Address 0X000002
#define I_Address 0X000003
#define Pre_R 0X000000          //X000,0000,DAT(14),00
#define Pre_N 0X000000          //XX0,DAT(13),XXXXXX,01


void adf4002_Init(void);
void SendDataPll(unsigned long int Data);
void Delay(unsigned int z);
void DelayMs(void);
void SetFrequency(uint32_t fvco);
void RDivideTest(uint16_t RData, uint16_t NN);
void testADF4002(void);


#endif

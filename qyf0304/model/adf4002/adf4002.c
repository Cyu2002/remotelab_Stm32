#include "adf4002.h"



long int ReadData;

//long int Reg0x00 = 0x001F40;//r
//////long int Reg0x01 = 0x0DAC01;//n

long int Reg0x02 = 0x0D8092; //function
long int Reg0x03 = 0x0D8093;//init
//long int Reg0x02 = 0x0D80B2; //function DVDD
//long int Reg0x03 = 0x0D80B3;//init DVDD

long int Reg0x02_LEDON  = 0x0D80B2; //function
long int Reg0x02_LEDOFF = 0x0D80F2; //function

void adf4002_Init(void){
	
	 GPIO_InitTypeDef GPIO_Initure1 = {0};
	 __HAL_RCC_GPIOB_CLK_ENABLE();
	 
	 GPIO_Initure1.Pin=GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_3;
	 GPIO_Initure1.Mode=GPIO_MODE_OUTPUT_PP;
	 GPIO_Initure1.Pull=GPIO_NOPULL;
	 GPIO_Initure1.Speed=GPIO_SPEED_FREQ_HIGH;
	 
	 HAL_GPIO_Init(GPIOB,&GPIO_Initure1);
	 
	 SCL(0);
	 SDA(0);
	 LE(0);
	 
	 
	SendDataPll(Reg0x03);//init
	SendDataPll(Reg0x02); //funtion
}



void SendDataPll(unsigned long int Data)          //?????
{
  unsigned char i;

	SCL(0);
  LE(0);  
  
  for(i = 0; i < 24; i++)
  {
    if(Data & 0x800000)
    {
      SDA(1);
    }
    else
    {
       SDA(0);
    }
    Data <<= 1;     
    SCL(1);
    
    Delay(100);
    
    SCL(0);
    Delay(100);
  }
  SDA(0);
  
  LE(0);
  Delay(100);
  LE(1);
}
void Delay(unsigned int z)
{
  unsigned int i,j;
  
  for(i = z; i > 0; i--)
    for(j = 10; j > 0; j--) ;
}
void DelayMs(void)
{
  unsigned int i, j;
  
  for(i = 0; i < 1000; i++)
  {
    for(j = 0; j < 1000; j++)
    {
      Delay(1000);
    }
  }
}
void SetFrequency(uint32_t fvco)
{
	uint16_t NN;
  double bl;
	uint16_t R = 1;//R??
	uint32_t S_R = 0, S_N = 0;
  bl = ((double)fvco)/100000000;
////  R = 1;
  NN = (uint16_t)(bl*R);
  S_R = Pre_R + (R<<2) + R_Address;
  S_N = Pre_N + (NN<<8) + N_Address;
  SendDataPll(S_R);     
  SendDataPll(S_N);
  SendDataPll(Reg0x02); //funtion
  SendDataPll(Reg0x03);//init
  SendDataPll(S_N);
  SendDataPll(S_R);
}

void RDivideTest(uint16_t RData, uint16_t NN)
{
	uint32_t S_R = 0, S_N = 0;
  S_R = Pre_R + (RData<<2) + R_Address;
  S_N = Pre_N + (NN<<8) + N_Address;
  SendDataPll(S_R);     
  SendDataPll(S_N);
  SendDataPll(Reg0x02); //funtion
  SendDataPll(Reg0x03);//init
  SendDataPll(S_N);
  SendDataPll(S_R);
}
void testADF4002(void)
{
  adf4002_Init();
  SendDataPll(Reg0x02_LEDOFF); Delay(15);
  SendDataPll(Reg0x02_LEDON); Delay(15);
  SendDataPll(Reg0x02_LEDOFF); Delay(15);
  SendDataPll(Reg0x02_LEDON); Delay(15);
  
}


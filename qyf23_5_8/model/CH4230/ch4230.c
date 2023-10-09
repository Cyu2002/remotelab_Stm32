#include "ch4230.h"
#include "ch423.h"




// OC[7:0], OC[15:8], IO[7:0]
uint8_t CH4230_portState[3] = {0xFF, 0xFF, 0xFF};

// working
// full bright with limited current
// push pull for OCPin
// interrupt disabled
// scanH disabled
// scanL disabled
// output for IOPin
uint8_t CH4230_config = 0x01;


void delay_us(u32 us){
	u32 i = 0;
	for(i=0;i<us;i++){
		u8 a = 10;
		while(a--);
	}
}


void CH4230_Init(void){
	GPIO_InitTypeDef GPIO_Initure1 = {0};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_Initure1.Pin=GPIO_PIN_11|GPIO_PIN_12;
	GPIO_Initure1.Mode=GPIO_MODE_OUTPUT_OD;
	GPIO_Initure1.Pull=GPIO_NOPULL;
	GPIO_Initure1.Speed=GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOA,&GPIO_Initure1);
	
}
void CH4230_IIC_Start(void){
	SCL_OUT(1);
	SDA_OUT(1);
	delay_us(2);
	SDA_OUT(0);
	delay_us(2);
	SCL_OUT(0);
	delay_us(2);
}
void CH4230_IIC_Stop(void){
	SCL_OUT(0);
	delay_us(2);
	SDA_OUT(0);
	delay_us(2);
	SCL_OUT(1);
	delay_us(2);
	SDA_OUT(1);
	delay_us(4);
}
void CH4230_IIC_SendByte(u8 txd){
	 uint8_t i;
    for (i = 0; i < 8; i++)
    {
        if (txd & 0x80)
        {
					SDA_OUT(1);
        }
        else
        {
            
					SDA_OUT(0);
        }
				delay_us(2);      
				SCL_OUT(1);
				delay_us(2); 
        SCL_OUT(0);
        if (i == 7)
        {
					SDA_OUT(1);
        }
        txd <<= 1;
				delay_us(4);
    }      
}
u8 CH4230_IIC_ReadByte(unsigned char ack){
	unsigned char i,receive=0;
	SDA_OUT(1);//SDA 
	for(i=0;i<8;i++ )
	{
		SCL_OUT(0);
		delay_us(2);
		SCL_OUT(1);
		receive<<=1;
		if(SDA_IN())receive++;
		delay_us(1);
	}
		if (!ack)
		CH4230_NACK();
		else
		CH4230_ACK();
		return receive;
}



void CH4230_ACK(void){
	SCL_OUT(0);
	delay_us(2);
	SDA_OUT(0);
	delay_us(2);
	SCL_OUT(1);
	delay_us(2); 
	SCL_OUT(0);
		
}
u8 CH4230_WaitACK(void){
	u8 ucErrTime=0;
	SDA_OUT(1); 
	SDA_OUT(1);delay_us(1);
	SCL_OUT(1);delay_us(1);
	while(SDA_IN())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			CH4230_IIC_Stop();
			return 1;
		}
	}
	SCL_OUT(0);
	return 0;
	
	
}
void CH4230_NACK(void){
	SCL_OUT(0);
	delay_us(2);
	SDA_OUT(1);
	delay_us(2);
	SCL_OUT(1);
	delay_us(2);
	SCL_OUT(0);
}











void SCL_OUT(u8 a){
	if(a==1){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
	}
	else if(a==0){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
	}
}
void SDA_OUT(u8 a){
	if(a==1){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
	}
	else if(a==0){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
	}
}
u8 SDA_IN(void){
	return HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12);
}












void CH4230_Write(uint8_t cmd, uint8_t data){
	CH4230_IIC_Start();
	CH4230_IIC_SendByte(cmd);
	CH4230_NACK();
	CH4230_IIC_SendByte(data);
	CH4230_NACK();
	CH4230_IIC_Stop();
}
	
	
uint8_t CH4230_Read(uint8_t cmd){
	u8 result;
	CH4230_IIC_Start();
	CH4230_IIC_SendByte(cmd);
	CH4230_NACK();
	result=CH4230_IIC_ReadByte(0);
	CH4230_IIC_Stop();
	return result;
}
void CH4230_WriteAll(uint32_t data){
	CH4230_portState[0] = data >> 0 & 0xFF;
  CH4230_portState[1] = data >> 8 & 0xFF;
  CH4230_portState[2] = data >> 16 & 0xFF;

  CH4230_Write(CH423_OCL_W, CH4230_portState[0]);
  CH4230_Write(CH423_OCH_W, CH4230_portState[1]);
  CH4230_Write(CH423_IO_W, CH4230_portState[2]);
}
void CH4230_WriteOC(uint16_t data){
	CH4230_portState[0] = data >> 0 & 0xFF;
  CH4230_portState[1] = data >> 8 & 0xFF;

  CH4230_Write(CH423_OCL_W, CH4230_portState[0]);
  CH4230_Write(CH423_OCH_W, CH4230_portState[1]);
	
}
void CH4230_WritePin(uint8_t id, uint8_t state){
	uint8_t grp = id / 8;
  id -= grp * 8;
  if (state)
    CH4230_portState[grp] |= (uint8_t)(1 << id);
  else
    CH4230_portState[grp] &= ~(uint8_t)(1 << id);

  if (grp == 0)
    CH4230_Write(CH423_OCL_W, CH4230_portState[0]);
  else if (grp == 1)
    CH4230_Write(CH423_OCH_W, CH4230_portState[1]);
  else if (grp == 2)
    CH4230_Write(CH423_IO_W, CH4230_portState[2]);
  else
    return;
}
uint8_t CH4230_ReadPin(uint8_t id){
	uint8_t grp = id / 8;
  id -= grp * 8;
  if (grp > 2)
    return 0xFF;
  else if (grp == 2)
    CH4230_portState[2] = CH4230_Read(CH423_IO_R);
  return CH4230_portState[grp] >> id & 0x1;
}

// CH423_IOPIN_IN / CH423_IOPIN_OUT
void CH4230_SetIOPinMode(uint8_t mode){
	mode &= CH423_IOPIN_MASK;
  CH4230_config &= ~CH423_IOPIN_MASK;
  CH4230_config |= mode;
  CH4230_Write(CH423_CONF, CH4230_config);
}

// CH423_OCPIN_PUSHPULL / CH423_OCPIN_OPENDRAIN
void CHD230_SetOCPinMode(uint8_t mode){
	mode &= CH423_OCPIN_MASK;
  CH4230_config &= ~CH423_OCPIN_MASK;
  CH4230_config |= mode;
  CH4230_Write(CH423_CONF, CH4230_config);
}








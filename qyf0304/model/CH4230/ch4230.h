#ifndef _CH4230_H
#define _CH4230_H

#include "main.h"

//PA11 clk
//PA12 da






void SCL_OUT(u8 a);
void SDA_OUT(u8 a);
u8 SDA_IN(void);


void delay_us(u32 us);

void CH4230_Init(void);
void CH4230_IIC_Start(void);
void CH4230_IIC_Stop(void);
void CH4230_IIC_SendByte(u8 txd);
u8 CH4230_IIC_ReadByte(unsigned char ack);
void CH4230_ACK(void);
u8 CH4230_WaitACK(void);
void CH4230_NACK(void);




void CH4230_Write(uint8_t cmd, uint8_t data);
uint8_t CH4230_Read(uint8_t cmd);
void CH4230_WriteAll(uint32_t data);
void CH4230_WriteOC(uint16_t data);
void CH4230_WritePin(uint8_t id, uint8_t state);
uint8_t CH4230_ReadPin(uint8_t id);

// CH423_IOPIN_IN / CH423_IOPIN_OUT
void CH4230_SetIOPinMode(uint8_t mode);

// CH423_OCPIN_PUSHPULL / CH423_OCPIN_OPENDRAIN
void CHD230_SetOCPinMode(uint8_t mode);


#endif

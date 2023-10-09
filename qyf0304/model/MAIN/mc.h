#ifndef _MAINCP_H
#define _MAINCP_H


#include "main.h"
#include "main.h"
#include "usart.h"
#include <math.h>
#include <stdlib.h>
#include "ch423.h"
#include "ch4230.h"
#include "adf4002.h"
#include "ad9833.h"
#include "dac8830.h"



#define PIN_A GPIO_PIN_4
#define PIN_B GPIO_PIN_1
#define PIN_C GPIO_PIN_0

////first ch423
#define RELAY_EN(x)  			 CH423_WritePin(0,x)
#define PLL_EN(x)					 CH423_WritePin(1,x)
#define TUN_AMP_EN(x) 		 CH423_WritePin(2,x)
#define FM_DEM_EN(x)			 CH423_WritePin(3,x)
#define POW_AMP_EN(x)			 CH423_WritePin(4,x)
#define MUL_EN(x)  				 CH423_WritePin(18,x);\
													 HAL_Delay(120);\
													 CH423_WritePin(8,x)
#define IN_OUT_EN(x)			 CH423_WritePin(19,x);\
													 HAL_Delay(120);\
													 CH423_WritePin(9,x)
#define PRO_AMP_EN(x)			 CH423_WritePin(10,x);\
													 HAL_Delay(120);\
													 CH423_WritePin(20,x)
#define WV_EN(x)    		   CH423_WritePin(14,x)
#define LV_EN(x)    			 CH423_WritePin(13,x)
#define ZSV_EN(x)    			 CH423_WritePin(12,x)
#define FSV_EN(x)    			 CH423_WritePin(11,x)
#define IN_OUT_2_EN(x)		 CH423_WritePin(16,x)
#define DDS_EN(x)    			 CH423_WritePin(17,x)
#define FM_MOD_EN(x) 		   CH423_WritePin(21,x)
#define OSC_EN(x)    		   CH423_WritePin(22,x)
#define AM_DEM_EN(x)   		 CH423_WritePin(23,x)

/////second ch423

#define MUL_CON_A(x)  			 CH4230_WritePin(0,x)
#define MUL_CON_B(x) 			 CH4230_WritePin(1,x)

#define AM_DEM_CON_A(x) 		 CH4230_WritePin(23,x)
#define AM_DEM_CON_B(x)		 CH4230_WritePin(2,x)

#define IN_OUT_2_IN_CON(x) 	  CH4230_WritePin(3,x)
#define IN_OUT_2_OUT_CON(x) 	CH4230_WritePin(12,x)

#define PLL_CON_A(x) CH4230_WritePin(4,x)
#define PLL_CON_B(x) CH4230_WritePin(11,x)
////////////////////////////!!!!!!!!!!!OSC other
#define OSC_CON_B(x) CH4230_WritePin(5,!x)
#define OSC_CON_A(x) CH4230_WritePin(6,!x)
#define OSC_CON_C(x) CH4230_WritePin(7,!x)
#define OSC_CON_D(x) CH4230_WritePin(8,!x)
#define OSC_CON_E(x) CH4230_WritePin(9,!x)
#define OSC_CON_F(x) CH4230_WritePin(10,!x)

#define A_OUT(x) CH4230_WritePin(15,x)
#define B_OUT(x) CH4230_WritePin(14,x)
#define C_OUT(x) CH4230_WritePin(16,x)
#define D_OUT(x) CH4230_WritePin(13,x)

#define TUN_AMP_CON_A(x) CH4230_WritePin(19,x)
#define TUN_AMP_CON_B(x) CH4230_WritePin(18,x)
#define TUN_AMP_CON_C(x) CH4230_WritePin(17,x)
#define TUN_AMP_CON_D(x) CH4230_WritePin(20,x)

#define FM_MOD_CON_A(x) CH4230_WritePin(21,x)
#define FM_MOD_CON_B(x) CH4230_WritePin(22,x)


/////out con
#define OSC_OUT             A_OUT(0);B_OUT(0);C_OUT(0);D_OUT(0)
#define FIR_DDS_OUT         A_OUT(1);B_OUT(1);C_OUT(0);D_OUT(0)
#define DIR_DDS_OUT         A_OUT(0);B_OUT(0);C_OUT(1);D_OUT(0)
#define lv_bo_OUT           A_OUT(1);B_OUT(0);C_OUT(1);D_OUT(0)
#define IN_OUT_CONNET       A_OUT(0);B_OUT(1);C_OUT(1);D_OUT(0)
#define DIO_AM_DEM_OUT      A_OUT(1);B_OUT(1);C_OUT(1);D_OUT(1)
#define PRO_AMP_OUT         A_OUT(0);B_OUT(0);C_OUT(0);D_OUT(1)
#define TUN_AMP_OUT					A_OUT(1);B_OUT(0);C_OUT(0);D_OUT(1)
#define AM_MOD_OUT_DEM_IN		A_OUT(0);B_OUT(1);C_OUT(0);D_OUT(1)
#define FM_MOD_OUT_DEM_IN   A_OUT(1);B_OUT(1);C_OUT(0);D_OUT(1)
#define FM_DEM_OUT					A_OUT(0);B_OUT(0);C_OUT(1);D_OUT(1)
#define FSK_DEM_OUT					A_OUT(1);B_OUT(0);C_OUT(1);D_OUT(0)
#define CHIP_AM_DEM_OUT			A_OUT(0);B_OUT(1);C_OUT(1);D_OUT(1)


//#define OSC_OUT             A_OUT(1);B_OUT(1);C_OUT(1);D_OUT(1)
//#define FIR_DDS_OUT         A_OUT(0);B_OUT(0);C_OUT(1);D_OUT(1)
//#define DIR_DDS_OUT         A_OUT(1);B_OUT(1);C_OUT(0);D_OUT(1)
//#define lv_bo_OUT           A_OUT(0);B_OUT(1);C_OUT(0);D_OUT(1)
//#define IN_OUT_CONNET       A_OUT(1);B_OUT(0);C_OUT(0);D_OUT(1)
//#define DIO_AM_DEM_OUT      A_OUT(0);B_OUT(0);C_OUT(0);D_OUT(0)
//#define PRO_AMP_OUT         A_OUT(1);B_OUT(1);C_OUT(1);D_OUT(0)
//#define TUN_AMP_OUT					A_OUT(0);B_OUT(1);C_OUT(1);D_OUT(0)
//#define AM_MOD_OUT_DEM_IN		A_OUT(1);B_OUT(0);C_OUT(1);D_OUT(0)
//#define FM_MOD_OUT_DEM_IN   A_OUT(0);B_OUT(0);C_OUT(1);D_OUT(0)
//#define FM_DEM_OUT					A_OUT(1);B_OUT(1);C_OUT(0);D_OUT(0)
//#define FSK_DEM_OUT					A_OUT(0);B_OUT(1);C_OUT(0);D_OUT(0)
//#define CHIP_AM_DEM_OUT			A_OUT(1);B_OUT(0);C_OUT(0);D_OUT(0)
//////in con
//#define 
#define SCREEN_EN(x)          HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,x)
#define U5_BUS_CON(x)					HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,x)
#define D12_BUS_CON(x)				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,x)
#define U12_BUS_CON(x)				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,x)






void DDS_Con(void);
void PLL_Con(void);
void yk_Con(void);
void AMT_Con(void);
void AMJ_Con(void);
void FMT_Con(void);
void FMJ_Con(void);
void tun_amp_Con(void);
void OSC_Con(void);
void POW_AMP_Con(void);
void lv10_7M(void);



#endif
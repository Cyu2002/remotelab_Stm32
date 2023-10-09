/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "ad9833.h"
#include "adf4002.h"
#include "ch423.h"
#include <math.h>
#include "delay.h"
#include "ch4230.h"
#include "mc.h"
#include "dac8830.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */






/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	int len,flag0=0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

	
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

	
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	Delay_Init(40);
	
	HAL_Delay(50);
	

	
	
	DAC8830_Init();
	//OCL(0-7)    OCH(9-15)  IO(16-23)(IO+16)
	CH423_Init(GPIOF,7, GPIOF,6);//ch423init (power)
	CH423_Write(CH423_CONF,0x1);
	
	
	//CH423_WriteAll(0);
	CH423_WriteOC(0x60);
	Delay_ms(100);
	CH423_WriteAll(0);

	CH4230_Init();//PA11(clk),PA12(da) (control)
	CH4230_Write(CH423_CONF,0x1);
	CH4230_WriteAll(0x17E8);
	U5_BUS_CON(0);
	D12_BUS_CON(0);
	U12_BUS_CON(0);
	
	HAL_Delay(500);

	
	
	MX_USART1_UART_Init();
  MX_USART2_UART_Init();
	SCREEN_EN(0);
	HAL_Delay(500);
	SCREEN_EN(1);
	
	
	uspp(2,"page main");
	uspp(2,"\xff\xff\xff");
	//HAL_GPIO_WritePin();(GPIOB,GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9,1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	//uspp(1,"page main");
		
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		//uspp(1,"lplp77\r\n");
		
		if(USART_RX_STA2&0x8000){
			len=USART_RX_STA2&0x3fff;
			switch(USART_RX_BUF2[0])
			{
				case'A':flag0=1;break;
				case'B':flag0=2;break;
				case'C':flag0=3;break;
				case'D':flag0=4;break;
				case'E':flag0=5;break;
				case'F':flag0=6;break;
				case'G':flag0=7;break;
				case'H':flag0=8;break;
				case'I':flag0=9;break;
				case'J':flag0=10;break;
				case'K':flag0=11;break;
				default:flag0=0;break;
			}
			USART_RX_STA2=0;
		}
		if(USART_RX_STA&0x8000){
			len=USART_RX_STA&0x3fff;	
			USART_RX_STA=0;
		}
		switch(flag0)
		{
			case 1:DDS_Con();break;
			case 2:PLL_Con();break;
			case 3:yk_Con();break;
			case 4:AMT_Con();break;
			case 5:AMJ_Con();break;
			case 6:FMT_Con();break;
			case 7:FMJ_Con();break;
			case 8:tun_amp_Con();break;
			case 9:OSC_Con();break;
			case 10:POW_AMP_Con();break;
			case 11:lv10_7M();break;
			default: break;
		}
		flag0=0;
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

//类似printf的函数（自创的）

void uspp(uint16_t usa,char words[]){
	int len;
	len=strlen((const char*)words);
	
	if(usa==1){
		HAL_UART_Transmit(&huart1,(u8*)words,len,0xffff);
	}
	else if(usa==2){
		HAL_UART_Transmit(&huart2,(u8*)words,len,0xffff);
	}
}
//////////////////////////////////////////////////////////////////








/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */


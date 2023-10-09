#include "mc.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

int breakup=0;
int i,j;
int flag=0,flagP=0;
int len0=0;
int dds1=0;
double ddsp=0.0;
double pll;
int pll0;
double yakong;
char cun[200];
char pllo[200];
int flag12=0;
int fact=0;
//OCL(0-7)    OCH(9-15)  IO(16-23)
//CH423_WritePin();
//CH4230_WritePin(16,0);
void DDS_Con(void){	
	
	
	U5_BUS_CON(1);
	D12_BUS_CON(1);
	U12_BUS_CON(1);
	HAL_Delay(10);
	FIR_DDS_OUT;


	
	LV_EN(1);
	HAL_Delay(120);
	WV_EN(1);
	HAL_Delay(120);
	FSV_EN(1);
	HAL_Delay(120);
	ZSV_EN(1);
	HAL_Delay(120);
	IN_OUT_EN(1);
	HAL_Delay(120);
	DDS_EN(1);
	HAL_Delay(120);
	
	
	breakup=1;
	while(breakup){
		//uspp(1,"454\r\n");
		if(USART_RX_STA2&0x8000){
			len0=USART_RX_STA2&0x3fff;
			if(USART_RX_BUF2[0]=='F'){
				ad9833_init();
				AD9833_SetFrequencyQuick(1,AD9833_OUT_SINUS);
				for(j=0;USART_RX_BUF2[j]!='P';j++);
				for(i=1;USART_RX_BUF2[i]!='P';i++){
					dds1=dds1+(int)pow(10,j-i-1)*(USART_RX_BUF2[i]-'0');
				}
				ddsp=(double)dds1/1000.0;
				if(USART_RX_BUF2[len0-1]=='1')AD9833_SetFrequencyQuick(ddsp,AD9833_OUT_SINUS);
				else if(USART_RX_BUF2[len0-1]=='2')AD9833_SetFrequencyQuick(ddsp,AD9833_OUT_TRIANGLE);
				else if(USART_RX_BUF2[len0-1]=='3')AD9833_SetFrequencyQuick(ddsp,AD9833_OUT_MSB);
				dds1=0;
			}
			else if(USART_RX_BUF2[0]=='b'){
				breakup=0;
				CH423_WriteAll(0);
				CH4230_WriteAll(0x17E8);
				U5_BUS_CON(0);
				D12_BUS_CON(0);
				U12_BUS_CON(0);
			}//breakout
			USART_RX_STA2=0;
		}
		
	}
}
void PLL_Con(void){	
	
	
	U5_BUS_CON(1);
//CH4230_WritePin(12,0);//2 channel
	HAL_Delay(10);
	IN_OUT_2_OUT_CON(0);
	
	
//	CH4230_WritePin(4,1);
	PLL_CON_A(1);
//	CH4230_WritePin(11,0);//kai
	PLL_CON_B(0);
	
	WV_EN(1);
				HAL_Delay(120);
	LV_EN(1);
				HAL_Delay(120);
//	CH423_WritePin(11,0);
	FSV_EN(0);
//	CH423_WritePin(12,1);
	ZSV_EN(1);
	HAL_Delay(50);
	IN_OUT_EN(0);
//	CH423_WritePin(9,0);
				HAL_Delay(120);
	
//	CH423_WritePin(16,1);
IN_OUT_2_EN(1);
				HAL_Delay(120);
//	CH423_WritePin(1,1);//power
PLL_EN(1);
				HAL_Delay(120);
	adf4002_Init();

	
	breakup=1;
	while(breakup){
		if(USART_RX_STA2&0x8000){
			pll=0.0;
			flagP=0;
			flag12=0;
			len0=USART_RX_STA2&0x3fff;
			if(USART_RX_BUF2[0]=='P'){
			for(j=0;USART_RX_BUF2[j]!='M';j++){
				if(USART_RX_BUF2[j]=='.'){
					flag=1;
				}
			}
			if(flag==1){
				for(j=0;USART_RX_BUF2[j]!='.';j++);

				for(i=j-1;i>0;i--){
					pll+=pow(10,j-i-1)*(USART_RX_BUF2[i]-'0');
				}
				
				if(USART_RX_BUF2[j+1]=='1'&&USART_RX_BUF2[j+2]=='2'&&USART_RX_BUF2[j+3]=='M'){
					pll0=pll*1000000+120000;
					flag12=1;
				}				
				for(i=j+1;i<len0-1;i++){
					pll+=pow(10,j-i)*(USART_RX_BUF2[i]-'0');
				}
			}
			else if(flag==0){
				for(i=1;USART_RX_BUF2[i]!='M';i++){
					pll=pll+pow(10,len0-i-2)*(USART_RX_BUF2[i]-'0');
				}
			}
			if(flag12==0){
				pll0=(int)(pll*1000000.0);
			}
			if((pll0%20000)!=0){
				uspp(2,"t3.txt=\"精度过高，实际频率：");
				fact=(pll0/20000)*20000;
				sprintf(pllo,"%d",fact);
				uspp(2,pllo);
				uspp(2,"HZ\"");
				uspp(2,"\xff\xff\xff");
				flagP=1;
			}
			RDivideTest(1250,pll0/20000);
			
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)&&flagP==0){
				uspp(2,"t3.txt=\"锁定检测：已锁\"");
				uspp(2,"\xff\xff\xff");
			}
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)==0&&flagP==0){
				uspp(2,"t3.txt=\"锁定检测：未锁\"");
				uspp(2,"\xff\xff\xff");
			}
		}
			if(USART_RX_BUF2[0]=='b'){
				PLL_CON_B(1);
				PLL_CON_A(0);
			breakup=0;
			CH423_WriteAll(0);
			CH4230_WriteAll(0x17E8);
			U5_BUS_CON(0);
			D12_BUS_CON(0);
			U12_BUS_CON(0);
		}
			USART_RX_STA2=0;
		}
		flag=0;
	}
	
	
	
}
void yk_Con(void){
//	CH423_WritePin(13,1);
//				HAL_Delay(50);
//	CH423_WritePin(14,1);
//				HAL_Delay(50);
//	CH4230_WritePin(14,1);
//	CH4230_WritePin(15,1);
//	CH4230_WritePin(16,1);//out
	U5_BUS_CON(1);
	D12_BUS_CON(1);
	U12_BUS_CON(1);
	PRO_AMP_OUT;
//	CH423_WritePin(11,1);
	FSV_EN(1);
	HAL_Delay(120);
//	CH423_WritePin(12,1);
	ZSV_EN(1);
	HAL_Delay(120);
//	CH423_WritePin(9,1);
	IN_OUT_EN(1);
	HAL_Delay(120);
//	CH423_WritePin(19,1);//power
//				HAL_Delay(120);
//	CH423_WritePin(20,1);
//				HAL_Delay(120);
//	CH423_WritePin(10,1);//power
//				HAL_Delay(120);
	PRO_AMP_EN(1);

	
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0|GPIO_PIN_1,1);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,0);//IN
	
	breakup=1;
	while(breakup){
	if(USART_RX_STA2&0x8000){
		len0=USART_RX_STA2&0x3fff;	
		if(USART_RX_BUF2[0]=='Y'){
			memset(cun,0,200);
			for(j=1;USART_RX_BUF2[j]!='P';j++){
				cun[j-1]=USART_RX_BUF2[j];
			}
			yakong=atof(cun);
			if(yakong>=2.0){
				yakong=2.0;
			}
			DAC8830_WriteDate(yakong);
		}
		else if(USART_RX_BUF2[0]=='b'){
			breakup=0;
			CH423_WriteAll(0);
			CH4230_WriteAll(0x17E8);
			U5_BUS_CON(0);
			D12_BUS_CON(0);
			U12_BUS_CON(0);
		}
		USART_RX_STA2=0;
		}
	}
}
void AMT_Con(void){//////////mo ren chip
	//screen com for com

	U5_BUS_CON(1);
	D12_BUS_CON(1);
	U12_BUS_CON(1);
	
	AM_MOD_OUT_DEM_IN;
	

	MUL_CON_A(1);
	MUL_CON_B(0);
	
	
	AM_DEM_CON_A(1);
	AM_DEM_CON_B(0);
	
	HAL_GPIO_WritePin(GPIOA,PIN_B,0);
	HAL_GPIO_WritePin(GPIOA,PIN_A,1);
	HAL_GPIO_WritePin(GPIOA,PIN_C,0);//IN

	FSV_EN(1);
	HAL_Delay(100);
	ZSV_EN(1);
	HAL_Delay(100);
	IN_OUT_EN(1);
	HAL_Delay(100);
	IN_OUT_2_EN(1);
	HAL_Delay(100);
	LV_EN(1);
	HAL_Delay(100);
	WV_EN(1);
	HAL_Delay(100);

	MUL_EN(1);
	AM_DEM_EN(1);

	breakup=1;
	while(breakup){
	if(USART_RX_STA2&0x8000){
		if(USART_RX_BUF2[0]=='Y'&&USART_RX_BUF2[1]=='T'){
			CHIP_AM_DEM_OUT;
			
			HAL_GPIO_WritePin(GPIOA,PIN_B,0);
			HAL_GPIO_WritePin(GPIOA,PIN_A,1);
			HAL_GPIO_WritePin(GPIOA,PIN_C,0);//IN

//			LV_EN(1);
//			HAL_Delay(50);
//			WV_EN(1);
//			HAL_Delay(50);
			
			
			MUL_EN(1);
			AM_DEM_EN(1);

					
			AM_DEM_CON_A(1);
			AM_DEM_CON_B(0);	
		}
		else if(USART_RX_BUF2[0]=='F'&&USART_RX_BUF2[1]=='T'){
	
			AM_MOD_OUT_DEM_IN;
					
			HAL_GPIO_WritePin(GPIOA,PIN_B,0);
			HAL_GPIO_WritePin(GPIOA,PIN_A,1);
			HAL_GPIO_WritePin(GPIOA,PIN_C,0);//IN
					
			AM_DEM_CON_A(1);
			AM_DEM_CON_B(0);	
			
			MUL_EN(1);
			AM_DEM_EN(1);
			
		}
		else if(USART_RX_BUF2[0]=='C'&&USART_RX_BUF2[1]=='H'){
			AM_DEM_CON_A(1);
			AM_DEM_CON_B(0);//chip		
			CHIP_AM_DEM_OUT;
		}
		else if(USART_RX_BUF2[0]=='D'&&USART_RX_BUF2[1]=='I'){
			AM_DEM_CON_A(0);
			AM_DEM_CON_B(1);//dio
			DIO_AM_DEM_OUT;
		}
		else if(USART_RX_BUF2[0]=='T'&&USART_RX_BUF2[1]=='Z'){
			if(USART_RX_BUF2[2]=='K'){
				
//					CH4230_WritePin(16,1);
//					CH4230_WritePin(15,1);
//					CH4230_WritePin(14,0);
//					CH4230_WritePin(13,0);//both
					AM_MOD_OUT_DEM_IN;
					MUL_CON_A(1);
					MUL_CON_B(0);

				
					AM_DEM_CON_A(1);
					AM_DEM_CON_B(0);//chip
					
					
					HAL_GPIO_WritePin(GPIOA,PIN_B,0);
					HAL_GPIO_WritePin(GPIOA,PIN_A,1);
					HAL_GPIO_WritePin(GPIOA,PIN_C,0);//IN

//					FSV_EN(1);
//					HAL_Delay(120);
//					ZSV_EN(1);
//					HAL_Delay(120);
//					IN_OUT_EN(1);
//					HAL_Delay(120);
//					IN_OUT_2_EN(1);
//					HAL_Delay(120);
					
					MUL_EN(1);
					AM_DEM_EN(1);
			}
			else {
					
					AM_DEM_CON_A(1);
					HAL_Delay(100);
					AM_DEM_CON_B(0);	//
					//out all0
					HAL_GPIO_WritePin(GPIOA,PIN_B,1);
					HAL_Delay(100);
					HAL_GPIO_WritePin(GPIOA,PIN_A,0);
					HAL_Delay(100);
					HAL_GPIO_WritePin(GPIOA,PIN_C,0);//IN
					
					CHIP_AM_DEM_OUT;;
//					LV_EN(1);
//					HAL_Delay(120);
//					WV_EN(1);
//					HAL_Delay(120);
//					FSV_EN(1);
//					HAL_Delay(120);
//					ZSV_EN(1);
//					HAL_Delay(120);
//					IN_OUT_EN(1);
//					HAL_Delay(120);
					
					MUL_EN(0);
					AM_DEM_EN(1);
			}
			
		}
		else if(USART_RX_BUF2[0]=='J'&&USART_RX_BUF2[1]=='T'){
			if(USART_RX_BUF2[2]=='K'){
					AM_DEM_CON_A(1);
				HAL_Delay(100);
					AM_DEM_CON_B(0);
				HAL_Delay(100);				//
					HAL_GPIO_WritePin(GPIOA,PIN_B,1);
				HAL_Delay(100);
					HAL_GPIO_WritePin(GPIOA,PIN_A,0);
				HAL_Delay(100);
					HAL_GPIO_WritePin(GPIOA,PIN_C,0);//IN
				HAL_Delay(100);
					CHIP_AM_DEM_OUT;
					
					MUL_EN(0);
					AM_DEM_EN(1);
			}
			else {
					AM_MOD_OUT_DEM_IN;
					
					MUL_CON_A(1);
					MUL_CON_B(0);//out
				
					AM_DEM_CON_A(1);
					AM_DEM_CON_B(0);
					
					HAL_GPIO_WritePin(GPIOA,PIN_B,0);
					HAL_GPIO_WritePin(GPIOA,PIN_A,1);
					HAL_GPIO_WritePin(GPIOA,PIN_C,0);//IN

//					CH423_WritePin(11,1);
//								HAL_Delay(120);
//					CH423_WritePin(12,1);
//								HAL_Delay(120);
//					CH423_WritePin(9,1);
//								HAL_Delay(120);
//					CH423_WritePin(19,1);//power
//								HAL_Delay(120);
//					CH423_WritePin(16,1);
//								HAL_Delay(120);
					
					MUL_EN(1);
					AM_DEM_EN(1);
			}
		}
		 if(USART_RX_BUF2[0]=='b'){
				breakup=0;
				CH423_WriteAll(0);
				CH4230_WriteAll(0x17E8);
			 	U5_BUS_CON(0);
				D12_BUS_CON(0);
				U12_BUS_CON(0);
				}//breakout
			USART_RX_STA2=0;
		}
		if(USART_RX_STA&0x8000){
			//len=USART_RX_STA&0x3fff;	
			if(USART_RX_BUF[0]=='0'){
				breakup=0;
				uspp(2,"page main");
				uspp(2,"\xff\xff\xff");
				CH423_WriteAll(0);
				CH4230_WriteAll(0x17E8);
			 	U5_BUS_CON(0);
				D12_BUS_CON(0);
				U12_BUS_CON(0);
			}
			USART_RX_STA=0;
		}
		
	}
}
void AMJ_Con(void){	
	CH4230_WritePin(23,0);
	CH4230_WritePin(2,1);	//
	//out all0
	
	HAL_GPIO_WritePin(GPIOA,PIN_B,1);
	HAL_GPIO_WritePin(GPIOA,PIN_A,0);
	HAL_GPIO_WritePin(GPIOA,PIN_C,0);//IN
	
	CH423_WritePin(13,1);
				HAL_Delay(120);
	CH423_WritePin(14,1);
				HAL_Delay(120);
	CH423_WritePin(11,1);
				HAL_Delay(120);
	CH423_WritePin(12,1);
				HAL_Delay(120);
	CH423_WritePin(9,1);
				HAL_Delay(120);
	CH423_WritePin(19,1);//power
				HAL_Delay(120);
	
	CH423_WritePin(18,1);
	HAL_Delay(120);
	CH423_WritePin(8,1);//power
	HAL_Delay(120);
	CH423_WritePin(23,1);
	
	

	breakup=1;
	while(breakup){
	
		if(USART_RX_STA2&0x8000){
		
			if(USART_RX_BUF2[0]=='Y'&&USART_RX_BUF2[1]=='T'){
					HAL_GPIO_WritePin(GPIOA,PIN_B,0);
					HAL_GPIO_WritePin(GPIOA,PIN_A,1);
					HAL_GPIO_WritePin(GPIOA,PIN_C,0);//IN
				
					CH4230_WritePin(23,0);
					CH4230_WritePin(2,1);//dio
				
				
			}
			else if(USART_RX_BUF2[0]=='F'&&USART_RX_BUF2[1]=='T'){
					CH4230_WritePin(23,0);
					CH4230_WritePin(2,1);//dio
				
					HAL_GPIO_WritePin(GPIOA,PIN_B,1);
					HAL_GPIO_WritePin(GPIOA,PIN_A,0);
					HAL_GPIO_WritePin(GPIOA,PIN_C,0);//IN				
				
			}
			else if(USART_RX_BUF2[0]=='d'&&USART_RX_BUF2[1]=='i'){
					CH4230_WritePin(23,0);
					CH4230_WritePin(2,1);//dio
				
					CH4230_WritePin(13,0);
					CH4230_WritePin(14,0);
					CH4230_WritePin(15,0);
					CH4230_WritePin(16,0);	
			}
			else if(USART_RX_BUF2[0]=='c'&&USART_RX_BUF2[1]=='h'){
					CH4230_WritePin(23,1);
					CH4230_WritePin(2,0);//dio
				
					CH4230_WritePin(13,0);
					CH4230_WritePin(14,0);
					CH4230_WritePin(15,1);
					CH4230_WritePin(16,0);
			}
		
			if(USART_RX_BUF2[0]=='b'){
					breakup=0;
					CH423_WriteAll(0);
					CH4230_WriteAll(0x17E8);
					U5_BUS_CON(0);
					D12_BUS_CON(0);
					U12_BUS_CON(0);
				}//breakout
			USART_RX_STA2=0;
		}
	}
}
void FMT_Con(void){	
	U5_BUS_CON(1);
	D12_BUS_CON(1);
	U12_BUS_CON(1);
	
	FM_MOD_OUT_DEM_IN;
	FM_MOD_CON_A(1);
	FM_MOD_CON_B(0);//out
	
	HAL_GPIO_WritePin(GPIOA,PIN_B,1);
	HAL_GPIO_WritePin(GPIOA,PIN_A,1);
	HAL_GPIO_WritePin(GPIOA,PIN_C,0);//IN
	
	FSV_EN(1);
	HAL_Delay(120);
	ZSV_EN(1);
	HAL_Delay(120);
	WV_EN(1);
	HAL_Delay(120);
	LV_EN(1);
	HAL_Delay(120);
	IN_OUT_EN(1);
	HAL_Delay(120);
	FM_MOD_EN(1);//power
	FM_DEM_EN(1);
	


	
	breakup=1;
	while(breakup){
		
		if(USART_RX_STA2&0x8000){
			if(USART_RX_BUF2[0]=='Y'&&USART_RX_BUF2[1]=='T'){
				
					FM_DEM_OUT;
				
				
					HAL_GPIO_WritePin(GPIOA,PIN_B,1);
					HAL_GPIO_WritePin(GPIOA,PIN_A,1);
					HAL_GPIO_WritePin(GPIOA,PIN_C,0);//IN
				
					FM_MOD_CON_A(1);
					FM_MOD_CON_B(0);//out
				
	
					FM_DEM_EN(1);
			}
			else if(USART_RX_BUF2[0]=='F'&&USART_RX_BUF2[1]=='T'){
//					CH4230_WritePin(16,1);
//					CH4230_WritePin(15,0);
//					CH4230_WritePin(14,0);
//					CH4230_WritePin(13,0);//out
					FM_DEM_OUT;
				
					CH4230_WritePin(21,1);
					CH4230_WritePin(22,0);//out
					
					HAL_GPIO_WritePin(GPIOA,PIN_B,1);
					HAL_GPIO_WritePin(GPIOA,PIN_A,1);
					HAL_GPIO_WritePin(GPIOA,PIN_C,0);//IN		
				
					FM_DEM_EN(1);
					FM_MOD_EN(1);				
			}
			else if(USART_RX_BUF2[0]=='T'&&USART_RX_BUF2[1]=='Z'){
				if(USART_RX_BUF2[2]=='K'){
//						CH4230_WritePin(16,1);
//						CH4230_WritePin(15,0);
//						CH4230_WritePin(14,0);
//						CH4230_WritePin(13,0);//out
						FM_MOD_OUT_DEM_IN;
						FM_MOD_CON_A(1);
						FM_MOD_CON_B(0);//out
						
						HAL_GPIO_WritePin(GPIOA,PIN_B,1);
						HAL_GPIO_WritePin(GPIOA,PIN_A,1);
						HAL_GPIO_WritePin(GPIOA,PIN_C,0);//IN
						
						
						FM_MOD_EN(1);//power
						FM_DEM_EN(1);//power
				}else {
						HAL_GPIO_WritePin(GPIOA,PIN_B,0);
						HAL_GPIO_WritePin(GPIOA,PIN_A,0);
						HAL_GPIO_WritePin(GPIOA,PIN_C,1);//IN
						
						FM_MOD_CON_A(0);       ////////hou qi bao hu
						FM_MOD_CON_B(1);

					
						FM_DEM_OUT;
					
						FM_DEM_EN(1);//power
						FM_MOD_EN(1);	
				}
			}
			else if(USART_RX_BUF2[0]=='J'&&USART_RX_BUF2[1]=='T'){
				if(USART_RX_BUF2[2]=='K'){
						HAL_GPIO_WritePin(GPIOA,PIN_B,0);
						HAL_GPIO_WritePin(GPIOA,PIN_A,0);
						HAL_GPIO_WritePin(GPIOA,PIN_C,1);//IN
						
//						CH4230_WritePin(21,0);       ////////hou qi bao hu
//						CH4230_WritePin(22,1);
					FM_MOD_CON_A(0);
					FM_MOD_CON_B(1);
					
//						CH4230_WritePin(13,0);
//						CH4230_WritePin(14,1);
//						CH4230_WritePin(15,1);//out
//						CH4230_WritePin(16,0);
//					
					
						FM_DEM_OUT;
					
					
					//	CH423_WritePin(13,1);
						WV_EN(1);
						HAL_Delay(120);
					//	CH423_WritePin(14,1);
						LV_EN(1);
						HAL_Delay(120);
						//CH423_WritePin(11,1);
						FSV_EN(1);
						HAL_Delay(120);
						//CH423_WritePin(12,1);
						ZSV_EN(1);
						HAL_Delay(120);
						//CH423_WritePin(9,1);
						IN_OUT_EN(1);
						HAL_Delay(120);
						//CH423_WritePin(19,1);//power
						//HAL_Delay(120);
						
						FM_DEM_EN(1);//power
						FM_MOD_EN(1);//power
				}else {
//						CH4230_WritePin(16,1);
//						CH4230_WritePin(15,0);
//						CH4230_WritePin(14,0);
//						CH4230_WritePin(13,0);//out
						
					
					FM_MOD_OUT_DEM_IN;
					
					//	CH4230_WritePin(21,1);
					FM_MOD_CON_A(1);
					//	CH4230_WritePin(22,0);//out
					FM_MOD_CON_B(0);
						
						HAL_GPIO_WritePin(GPIOA,PIN_B,1);
						HAL_GPIO_WritePin(GPIOA,PIN_A,1);
						HAL_GPIO_WritePin(GPIOA,PIN_C,0);//IN
						
						FM_DEM_EN(1);//power
						FM_MOD_EN(1);//power
				}
			}
			if(USART_RX_BUF2[0]=='b'){
				breakup=0;
				CH423_WriteAll(0);
				CH4230_WriteAll(0x17E8);
				U5_BUS_CON(0);
				D12_BUS_CON(0);
				U12_BUS_CON(0);
				}//breakout
			USART_RX_STA2=0;
		}
		if(USART_RX_STA&0x8000){
			//len=USART_RX_STA&0x3fff;	
			if(USART_RX_BUF[0]=='0'){
				breakup=0;
				uspp(2,"page main");
				uspp(2,"\xff\xff\xff");
				CH423_WriteAll(0);
				CH4230_WriteAll(0x17E8);
			 	U5_BUS_CON(0);
				D12_BUS_CON(0);
				U12_BUS_CON(0);
			}
			USART_RX_STA=0;
		}
	}
}
void FMJ_Con(void){	
	HAL_GPIO_WritePin(GPIOA,PIN_B,0);
	HAL_GPIO_WritePin(GPIOA,PIN_A,0);
	HAL_GPIO_WritePin(GPIOA,PIN_C,1);//IN
	
	////////hou qi bao hu
	CH4230_WritePin(13,0);
	CH4230_WritePin(14,1);
	CH4230_WritePin(15,1);//out
	CH4230_WritePin(16,0);
	
	CH423_WritePin(13,1);
				HAL_Delay(120);
	CH423_WritePin(14,1);
				HAL_Delay(120);
	CH423_WritePin(11,1);
				HAL_Delay(120);
	CH423_WritePin(12,1);
				HAL_Delay(120);
	CH423_WritePin(9,1);
				HAL_Delay(120);
	CH423_WritePin(19,1);//power
				HAL_Delay(120);
	
	CH423_WritePin(3,1);//power
	CH423_WritePin(21,1);
	
	
	breakup=1;
	while(breakup){
		if(USART_RX_STA2&0x8000){
			if(USART_RX_BUF2[0]=='Y'&&USART_RX_BUF2[1]=='T'){
					HAL_GPIO_WritePin(GPIOA,PIN_B,1);
					HAL_GPIO_WritePin(GPIOA,PIN_A,1);
					HAL_GPIO_WritePin(GPIOA,PIN_C,0);//IN		
				
					CH4230_WritePin(13,0);
					CH4230_WritePin(14,1);
					CH4230_WritePin(15,1);//out
					CH4230_WritePin(16,0);
			}
			else if(USART_RX_BUF2[0]=='F'&&USART_RX_BUF2[1]=='T'){
				HAL_GPIO_WritePin(GPIOA,PIN_B,0);
				HAL_GPIO_WritePin(GPIOA,PIN_A,0);
				HAL_GPIO_WritePin(GPIOA,PIN_C,1);//IN
				
				////////hou qi bao hu
				CH4230_WritePin(13,0);
				CH4230_WritePin(14,1);
				CH4230_WritePin(15,1);//out
				CH4230_WritePin(16,0);	
			}
		
		
		
		if(USART_RX_BUF2[0]=='b'){
				breakup=0;
				CH423_WriteAll(0);
				CH4230_WriteAll(0x17E8);
				U5_BUS_CON(0);
				D12_BUS_CON(0);
				U12_BUS_CON(0);
			}//breakout
			USART_RX_STA2=0;
		}
	}
}
void tun_amp_Con(void){
	////////hou qi jia
//	CH4230_WritePin(14,1);
//	CH4230_WritePin(16,1);//out
		U5_BUS_CON(1);
		D12_BUS_CON(1);
		U12_BUS_CON(1);

	TUN_AMP_OUT;
	
	HAL_GPIO_WritePin(GPIOA,PIN_B,0);
	HAL_GPIO_WritePin(GPIOA,PIN_A,1);
	HAL_GPIO_WritePin(GPIOA,PIN_C,1);//IN
	
	//CH423_WritePin(13,1);
	LV_EN(1);
	HAL_Delay(120);
	//CH423_WritePin(14,1);
	WV_EN(1);
	HAL_Delay(50);
	//CH423_WritePin(11,1);
	ZSV_EN(1);
				HAL_Delay(120);
	//CH423_WritePin(12,1);
	FSV_EN(1);
	HAL_Delay(120);
//	CH423_WritePin(9,1);
//				HAL_Delay(120);
//	CH423_WritePin(19,1);//power
IN_OUT_EN(1);
				HAL_Delay(120);
	
	TUN_AMP_EN(1);//power
	
	

	breakup=1;
	while(breakup){
		
		if(USART_RX_STA2&0x8000){
			if(USART_RX_BUF2[0]=='S'&&USART_RX_BUF2[1]=='W'){
			switch(USART_RX_BUF2[2]){
				case 'A':if(USART_RX_BUF2[4]=='0') TUN_AMP_CON_A(0);
								else TUN_AMP_CON_A(1);
								break;
				case 'B':if(USART_RX_BUF2[4]=='0') TUN_AMP_CON_B(0);
								else TUN_AMP_CON_B(1);
								break;
				case 'C':if(USART_RX_BUF2[4]=='0') TUN_AMP_CON_C(0);
								else TUN_AMP_CON_C(1);
								break;
				case 'D':if(USART_RX_BUF2[4]=='0') TUN_AMP_CON_D(0);
								else TUN_AMP_CON_D(1);
								break;
				default:break;
			}
		}	
		if(USART_RX_BUF2[0]=='b'){
				breakup=0;
				CH423_WriteAll(0);
				CH4230_WriteAll(0x17E8);
				U5_BUS_CON(0);
				D12_BUS_CON(0);
				U12_BUS_CON(0);
				}//breakout
			USART_RX_STA2=0;
		}
		
	}
	
	
}
void OSC_Con(void){
	U5_BUS_CON(1);
	D12_BUS_CON(1);
	U12_BUS_CON(1);
	
	
//	CH4230_WritePin(13,1);
//	CH4230_WritePin(14,1);
//	CH4230_WritePin(15,1);
//	CH4230_WritePin(16,1);//out
	OSC_OUT;
	
//	CH423_WritePin(13,1);
	LV_EN(1);
				HAL_Delay(120);
//	CH423_WritePin(14,1);
	WV_EN(1);
				HAL_Delay(120);
//	CH423_WritePin(11,1);
	ZSV_EN(1);
				HAL_Delay(120);
//	CH423_WritePin(12,1);
  FSV_EN(1);
				HAL_Delay(120);
//	CH423_WritePin(9,1);
//				HAL_Delay(120);
//	CH423_WritePin(19,1);//power
	IN_OUT_EN(1);
				HAL_Delay(120);
	
//	CH423_WritePin(22,1);//power
	OSC_EN(1);
	HAL_Delay(120);
//	CH423_WritePin(0,1);
RELAY_EN(1);
	

	
	
	breakup=1;
	while(breakup){
		
	if(USART_RX_STA2&0x8000){
		if(USART_RX_BUF2[0]=='S'&&USART_RX_BUF2[1]=='W'){
			switch(USART_RX_BUF2[2]){
				case 'A':if(USART_RX_BUF2[4]=='0')CH4230_WritePin(6,1);
									else CH4230_WritePin(6,0);
									break;
				case 'B':if(USART_RX_BUF2[4]=='0')CH4230_WritePin(5,1);
									else CH4230_WritePin(5,0);
									break;
				case 'C':if(USART_RX_BUF2[4]=='0')CH4230_WritePin(7,1);
									else CH4230_WritePin(7,0);
									break;
				case 'D':if(USART_RX_BUF2[4]=='0')CH4230_WritePin(8,1);
									else CH4230_WritePin(8,0);
									break;
				case 'E':if(USART_RX_BUF2[4]=='0')CH4230_WritePin(9,1);
									else CH4230_WritePin(9,0);
									break;
				case 'F':if(USART_RX_BUF2[4]=='0')CH4230_WritePin(10,1);
									else CH4230_WritePin(10,0);
									break;
				default:break;
			}
			
		}
		
		
		
		if(USART_RX_BUF2[0]=='b'){
				breakup=0;
				CH423_WriteAll(0);
				CH4230_WriteAll(0x17E8);
				U5_BUS_CON(0);
				D12_BUS_CON(0);
				U12_BUS_CON(0);
				}//breakout
			USART_RX_STA2=0;
		}
		
	}

}
void POW_AMP_Con(void){	
	U5_BUS_CON(1);
	
	
//CH4230_WritePin(12,1);
	IN_OUT_2_OUT_CON(1);
//CH4230_WritePin(14,1);
//CH4230_WritePin(15,1);
//CH4230_WritePin(16,1);//out
	
	PRO_AMP_OUT;
	//CH4230_WritePin(3,0);//in
	IN_OUT_2_IN_CON(0);
	
	//CH423_WritePin(13,1);
	LV_EN(1);
				HAL_Delay(100);
//	CH423_WritePin(14,1);
	WV_EN(1);
				HAL_Delay(120);
//	CH423_WritePin(11,1);
//				HAL_Delay(50);
//	CH423_WritePin(12,1);
//				HAL_Delay(50);
//	CH423_WritePin(9,1);
//				HAL_Delay(120);
//	CH423_WritePin(19,1);//power
//	IN_OUT_EN(1);
//				HAL_Delay(120);
	
	//CH423_WritePin(16,1);
	IN_OUT_2_EN(1);
				HAL_Delay(120);
	
	//CH423_WritePin(4,1);//power
	POW_AMP_EN(1);
	

	
	
	breakup=1;
	while(breakup){
		
	if(USART_RX_STA2&0x8000){

		
		
		if(USART_RX_BUF2[0]=='b'){
				breakup=0;
				CH423_WriteAll(0);
				CH4230_WriteAll(0x17E8);
				U5_BUS_CON(0);
				D12_BUS_CON(0);
				U12_BUS_CON(0);
				}//breakout
			USART_RX_STA2=0;
		}
	}
}
void lv10_7M(void){	
	U5_BUS_CON(1);
	D12_BUS_CON(1);
	U12_BUS_CON(1);
	
//	CH4230_WritePin(13,1);
//	CH4230_WritePin(14,1);//out
	
	lv_bo_OUT;
	
	HAL_GPIO_WritePin(GPIOA,PIN_B,0);
	HAL_GPIO_WritePin(GPIOA,PIN_A,0);
	HAL_GPIO_WritePin(GPIOA,PIN_C,0);//IN
	
//	CH423_WritePin(13,1);
	WV_EN(1);
				HAL_Delay(120);
//	CH423_WritePin(14,1);
	LV_EN(1);
				HAL_Delay(120);
//	CH423_WritePin(11,1);
	FSV_EN(1);
				HAL_Delay(120);
//	CH423_WritePin(12,1);
	ZSV_EN(1);
				HAL_Delay(120);
//	CH423_WritePin(9,1);
//				HAL_Delay(120);
//	CH423_WritePin(19,1);//power
	IN_OUT_EN(1);
				HAL_Delay(120);
	

	
	
	breakup=1;
	while(breakup){
		
		if(USART_RX_STA2&0x8000){
			
			if(USART_RX_BUF2[0]=='b'){
				breakup=0;
				CH423_WriteAll(0);
				CH4230_WriteAll(0x17E8);	
				U5_BUS_CON(0);
				D12_BUS_CON(0);
				U12_BUS_CON(0);				
			}	
			USART_RX_STA2=0;
		}
	}
}





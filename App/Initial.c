/***************************************************************
*版    权：苏州欧姆尼克新能源科技有限公司软件组
*文件名称：Initial.c
 * 修改人：沈仕强
 * 修改日期：2019.06.20
*描    述：初始化MCU
***************************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"
#include "stm32f0xx.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_pwr.h"
#include "I2C.h"
#include "Pcf8563.h"
#include "Display.h"
#include "Initial.h"
#include "Lib_Communication.h"
#include "Lib_Build.h"

/********************************************************************
 * Macros
 ********************************************************************/

/********************************************************************
 * Constant
 ********************************************************************/

/********************************************************************
 * Local Types
 ********************************************************************/
 
/********************************************************************
 * Global Types
 ********************************************************************/

/********************************************************************
 * Extern Global Types
 ********************************************************************/

/********************************************************************
 * Functions
 *******************************************************************/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void TIM16_Configuration(void);
void USART1_Configuration(void);
//void USART2_Configuration(void);
void I2C_Configuration(void);
void EXTI_Config(void);
//void USART2_Printf_Configuration(void); 

void sInitMCU(void)
{
	RCC_Configuration();
	GPIO_Configuration();
	TIM16_Configuration();
	USART1_Configuration();									//用于主副通讯的USART1初始化配置
	//USART2_Printf_Configuration();       //debug&升级
	I2C_Configuration();
	sInitComData();
	sInitPcf8563();	
	sReadAllEepromData();
	sInitDisplayTask();
}

void RCC_Configuration(void)
{
	/*Enable GPIOA and GPIOB clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC, ENABLE); 
	/*Enable TIM16 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16 | RCC_APB2Periph_TIM15 , ENABLE);
	/*Enable TIM3 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
}

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*
	PA0-RW/PA1-RS485DIR1/PA8-RS485DIR0/PA12-SPEAK is for output pushpull
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_8|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1); //485接收
	GPIO_ResetBits(GPIOA,GPIO_Pin_8); //485接收

	/*
	PA4-KEY0/PA5-KEY1/PA6-KEY2/PA7-KEY3 is for input pushpull
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*
	PB0-RS/PB1-RC/PB2-LCMEN/PB3-COMM/PB4-FAULT/PB5-NORMAL is for output pushpull
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Configure PB8/PB9/PB10/PB11/PB12/PB13/PB14/PB15 for LCD data in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | 		//按照TL2中为双向的I/O，st中不能配位双向I/O可以在需要为输入口的时候改变
								  GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void TIM16_Configuration(void)								//用于系统调度任务1ms定时
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/* Enable the TIM16 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM16_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* TIM16 DeInit */
	TIM_DeInit(TIM16);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Period = 100 - 1;									//10us * 100 = 1ms
	TIM_TimeBaseStructure.TIM_Prescaler = 480 - 1;							//48MHz/480 = 0.1MHz 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);
	/* clear overflow interrupt flag*/
	TIM_ClearFlag(TIM16,TIM_FLAG_Update);
	/* TIM Interrupts enable */
	TIM_ITConfig(TIM16,TIM_IT_Update, ENABLE);
	/* TIM16 enable counter */
	TIM_Cmd(TIM16, ENABLE);	
}


void USART1_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	/* Enable USART clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	/* Connect PA9 to USART1_Tx */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);  
	/* Connect PA10 to USART1_Rx */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
  
	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Enable the USART1 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* USARTx configuration ----------------------------------------------------*/
	/* USARTx configured as follow:
	- BaudRate = 115200 baud  
	- Word Length = 8 Bits
	- one Stop Bit
	- No parity
	- Hardware flow control disabled (RTS and CTS signals)
	- Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	/* When using Parity the word length must be configured to 9 bits */
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 
	 
	/* Enable USART Receive data register not empty interrupt */
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
  
	/* Enable USART */
	USART_Cmd(USART1, ENABLE);
	/* Clear Transmission complete flag */
	USART_ClearFlag(USART1, USART_FLAG_TC);
}


//void USART2_Printf_Configuration(void)
//{
//	USART_InitTypeDef USART_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;
//	NVIC_InitTypeDef  NVIC_InitStructure;
//	
//	/* Enable GPIO clock */
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
//	/* Enable USART clock */
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//	/* Connect PA2 to USART1_Tx */
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);  
//	/* Connect PA3 to USART1_Rx */
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
//  
//	/* Configure USART Tx and Rx as alternate function push-pull */
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	/* Enable the USART2 gloabal Interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPriority = 4;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

//	USART_InitStructure.USART_BaudRate = 9600;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	/* When using Parity the word length must be configured to 8 bits */
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	USART_Init(USART2, &USART_InitStructure); 

//	/* Enable USART */
//	USART_Cmd(USART2, ENABLE);
//	/* Enable USART Receive data register not empty interrupt */
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//空闲使能，用于判断数据帧接收完成
//	/* Clear Transmission complete flag */
//	USART_ClearFlag(USART2, USART_FLAG_TC);
//	USART_ClearFlag(USART2, USART_FLAG_IDLE);
//	USART_ClearFlag(USART2, USART_FLAG_RXNE);
//}

void I2C_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* Enable GPIOB(PB6/PB7) clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	/* Config I2C_SCL(PB6),I2C_SDA(PB7) */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}



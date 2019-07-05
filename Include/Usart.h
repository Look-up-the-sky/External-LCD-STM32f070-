/******************** (C) COPYRIGHT 2018 许瑞娟 ********************************
* File Name          :  Usart.h
* Author             :  许瑞娟
* CPU Type           :  GD32F130
* IDE                :  IAR 8.2
* Version            :  V1.0
* Date               :  03/13/2018
* Description        :  Usart header file
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H
#define __USART_H

/* Includes ------------------------------------------------------------------*/
#include "TypeDefine.h"


/* Private define ------------------------------------------------------------*/
#define ARRAYNUM(arr_nanme)      (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))
#define USART0_TDATA_ADDRESS     ((uint32_t)0x40013828)					// 130_150 device 串口数据发送地址
#define USART0_RDATA_ADDRESS     ((uint32_t)0x40013824)					// 130_150 device 串口数据接收地址



/* Private typedef -----------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
extern  void Usart_Variable_init(void);									// 串口变量初始化
extern  void Usart_COM_init(INT32U COM);									// 初始化UART口
extern  void Usart_Send_Data(INT8U *pData);								// USART发送数据
extern  void USART_Transfer_DMA_Config(void);							// USART DMA配置
extern  void USART_Receiver_DMA_Config(void);							// USART接收DMA配置
extern  void LOG_Output(void);
extern  void Usart_Timeout_config(void);								// 配置USART接收超时中断



#endif /* __USART_H */

/******************* (C) COPYRIGHT 2018 许瑞娟 **** END OF FILE ***************/


/******************** (C) COPYRIGHT 2018 ����� ********************************
* File Name          :  Usart.h
* Author             :  �����
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
#define USART0_TDATA_ADDRESS     ((uint32_t)0x40013828)					// 130_150 device �������ݷ��͵�ַ
#define USART0_RDATA_ADDRESS     ((uint32_t)0x40013824)					// 130_150 device �������ݽ��յ�ַ



/* Private typedef -----------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
extern  void Usart_Variable_init(void);									// ���ڱ�����ʼ��
extern  void Usart_COM_init(INT32U COM);									// ��ʼ��UART��
extern  void Usart_Send_Data(INT8U *pData);								// USART��������
extern  void USART_Transfer_DMA_Config(void);							// USART DMA����
extern  void USART_Receiver_DMA_Config(void);							// USART����DMA����
extern  void LOG_Output(void);
extern  void Usart_Timeout_config(void);								// ����USART���ճ�ʱ�ж�



#endif /* __USART_H */

/******************* (C) COPYRIGHT 2018 ����� **** END OF FILE ***************/


#ifndef _LED_STATE_H_
#define _LED_STATE_H_
#include "TypeDefine.h"
#include "stm32f0xx.h"
#include "App.h"
#define cNormalOn()		GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define cNormalOff()	GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define cFaultOn()		GPIO_SetBits(GPIOB,GPIO_Pin_4)
#define cFaultOff() 	GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define cCommLEDOn()	GPIO_SetBits(GPIOB,GPIO_Pin_3)
#define cCommLEDOff()	GPIO_ResetBits(GPIOB,GPIO_Pin_3)

//ÃÛ·äÄæ±äÆ÷×´Ì¬
#define  PowerOnMode           0x00
#define  WaitState						 0x01
#define  Checking							 0x02
#define  NormalState_N				 0x04
#define  NormalState_L				 0x05
#define  BoardDebugMode				 0x06
#define  OpenLoopMode					 0x07
#define  AdCalibrationMode		 0x08
#define  FaultState						 0x80
#define  Shutdown							 0x40
#define  FlashState						 0xF1

//»Æ·äÄæ±äÆ÷×´Ì¬
#define  HF_WaitState          0x01
#define  HF_Checking					 0x02
#define  HF_NormalState				 0x03
#define  HF_StopState				   0x04
#define  HF_FlashState				 0xF1


extern INT8U INV_StateFlag;
extern INT16S Comm_StateFlag;   //Í¨Ñ¶×´Ì¬
void sLedStateTask(void);
void INVErrTypeAnalysis(void);
#endif

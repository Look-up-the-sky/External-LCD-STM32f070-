/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Interrupt.c
 * 修改人：沈仕强
 * 修改日期：2019.06.20
 * 描述：中断函数
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"
#include "Lib_task.h"
#include "stm32f0xx_tim.h"
#include "modbus.h"
#include "Led_State.h"
#include "MF_Datecfg.h"
#include "HF_Datecfg.h"
#include "Lib_Communication.h"
#include "Variable_Transformation.h"
/********************************************************************
 * Global Types
 ********************************************************************/
extern INT32U CNT_1MS;
/********************************************************************
 * Functions
 ********************************************************************/
void sTimerSystem(void);
void SYSCLKConfig_STOP(void);

void TIM16_IRQHandler(void)				//系统任务调度定时中断 1MS
{
	static INT8U x = 0;
	sTimerSystem();
	c_1MS_Cnt++;
	CNT_1MS++;
	if((INV_StateFlag == 3)&&(x == 0))
	{
		CNT_1MS = 0;
		x = 1;
		MasterData.data.uwReconnectTime = __REV16(*INV_Set_Info.WaitStandupTime);
	}
	else
	{
		if((CNT_1MS >= 1000)&&(x == 1))
		{
			CNT_1MS = 0;
			MasterData.data.uwReconnectTime--;
			if(MasterData.data.uwReconnectTime >(__REV16(*INV_Set_Info.WaitStandupTime)) )
			{
				MasterData.data.uwReconnectTime = 0;
			}
			if(INV_StateFlag != 3)
			{
				x = 0;
				MasterData.data.uwReconnectTime = 0;
			}
		}
	}
}

void sTimerSystem(void)
{
	if(TIM_GetITStatus(TIM16,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM16,TIM_FLAG_Update);
		sTaskTimer();
	}
}


void USART1_IRQHandler(void)
{
	MODBUS_UART1_IRQHandler();
}


//void USART2_IRQHandler(void)
//{
//	MODBUS_UART2_IRQHandler();
//}


void NMI_Handler(void)
{
}


void HardFault_Handler(void)
{
  NVIC_SystemReset();
}


void SVC_Handler(void)
{
	NVIC_SystemReset();
}


void PendSV_Handler(void)
{
	NVIC_SystemReset();
}


void SysTick_Handler(void)
{
	NVIC_SystemReset();
}

void sHardFault(void)
{
	if((EXTI_GetITStatus(EXTI_Line11) != RESET))	//stop mode 唤醒中断线
	{
		SYSCLKConfig_STOP();
		/* Clear the EXTI line 15 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
}

void SYSCLKConfig_STOP(void)							//从stop mode恢复系统时钟
{
    /* After wake-up from STOP reconfigure the system clock */
    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);

    /* Wait till HSE is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
    {}

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {}
}

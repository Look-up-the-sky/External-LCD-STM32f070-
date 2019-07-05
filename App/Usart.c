/******************** (C) COPYRIGHT 2017 ����� ********************************
* File Name          :  Usart.c
* Author             :  �����
* CPU Type           :  GD32F130
* IDE                :  IAR 8.2
* Version            :  V1.0
* Date               :  03/13/2018
* Description        :  ����Ӧ�ó���
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "Usart.h"



/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
void Usart_Variable_init(void);											// ���ڱ�����ʼ��
void Usart_COM_init(INT32U COM);											// ��ʼ��UART��
void Usart_Send_Data(INT8U *pData);										// ���ڷ����ַ���
void Usart_Nvic_config(INT32U COM);										// �����жϳ�ʼ��
void USART_Transfer_DMA_Config(void);									// USART����DMA����
void USART_Receiver_DMA_Config(void);									// USART����DMA����
void LOG_Output(void);
void Usart_Timeout_config(void);										// ����USART���ճ�ʱ�ж�



/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
*                			�����@2018-03-13
* Function Name  : Usart_Variable_init
* Description    : ���ڱ�����ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Usart_Variable_init(void)
{

	// ��Log���ͽṹ��
	INT8U *BfrPtr = (INT8U *)&Log;
	
	memset(BfrPtr, 0, sizeof(Log_Typedef));

	// ����ջ���ṹ��
	BfrPtr = (unsigned char *)&USART_Rx;
	memset(BfrPtr, 0, sizeof(Usart_Rx_Typedef));
	
}// End of void Usart_Variable_init(void)

/*******************************************************************************
*                			�����@2018-03-13
* Function Name  : Usart_Init
* Description    : ���ڳ�ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Usart_COM_init(INT32U COM)
{
  	// �����жϳ�ʼ��
	Usart_Nvic_config(COM);

#ifdef LOG_ENABLE

	nvic_irq_enable(DMA_Channel1_2_IRQn,0,0);

#endif
	
	// ��ʼ��Usart��������Ϊ115200,8bit��һ��ֹͣλ������żУ��
	gd_eval_com_init(COM);

	// ʹ�ܴ��ڽ��ճ�ʱ�ж�
//	Usart_Timeout_config();

	// ʹ�ܴ����ж�
	usart_interrupt_enable(USER_UART_COM, USART_INT_RBNE);

	// USART DMA����
//	USART_DMA_Config();
	
}// End of void Usart_COM_init((INT32U COM))

/*******************************************************************************
*                			�����@2018-03-13
* Function Name  : Usart_Nvic_config
* Description    : �����жϳ�ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Usart_Timeout_config(void)
{
	// enable the USART receive timeout and configure the time of timeout
    usart_receiver_timeout_enable(USER_UART_COM);
    
    // ��ʱʱ������Ϊ10ms
    usart_receiver_timeout_config(USER_UART_COM, 1152);

    usart_interrupt_enable(USER_UART_COM, USART_INT_RT);

	
}// End of void Usart_Nvic_config(INT32U COM)


/*******************************************************************************
*                			�����@2018-03-13
* Function Name  : Usart_Send_Data
* Description    : ���ڳ�ʼ��
* Input          : INT8U *pData  ���ӡ������
* Output         : None
* Return         : None
*******************************************************************************/
void Usart_Send_Data(INT8U *pData)
{
	while(*pData != '\0')
	{
	  	while(RESET != usart_interrupt_flag_get(USER_UART_COM, USART_INT_FLAG_TBE));
		usart_data_transmit(USER_UART_COM, *pData);
		while(RESET == usart_flag_get(USER_UART_COM, USART_FLAG_TC));
		pData++;
	}
	
}// End of void Usart_Send_Data((INT32U COM))

/*******************************************************************************
*                			�����@2018-03-13
* Function Name  : Usart_Nvic_config
* Description    : �����жϳ�ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Usart_Nvic_config(INT32U COM)
{
	INT8U ucPort;
	
  	if(COM == USART0)
	{
		ucPort = USART0_IRQn;
	}
	else
	{
		ucPort = USART1_IRQn;
	}
	  
    nvic_irq_enable(ucPort, 0, 1);
	
}// End of void Usart_Nvic_config(INT32U COM)


/*******************************************************************************
*                			�����@2018-03-13
* Function Name  : Usart_Nvic_config
* Description    : �����жϳ�ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART_Transfer_DMA_Config(void)
{
	dma_parameter_struct dma_init_struct;
	
	// enable DMA clock */
	rcu_periph_clock_enable(RCU_DMA);
		
	/* deinitialize DMA channel1 */
	dma_deinit(USART_TRANSFER_DMA_CH);
	dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
	dma_init_struct.memory_addr = (uint32_t)Log.Buffer[Log.Transfer_Index];
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
	dma_init_struct.number = strlen((char *)Log.Buffer[Log.Transfer_Index]);
	dma_init_struct.periph_addr = USART0_TDATA_ADDRESS;
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
	
	dma_init(USART_TRANSFER_DMA_CH, dma_init_struct);
	/* configure DMA mode */
	dma_circulation_disable(USART_TRANSFER_DMA_CH);
	dma_memory_to_memory_disable(USART_TRANSFER_DMA_CH);
	
	usart_dma_transmit_config(USART0, USART_DENT_ENABLE);
	
	// enable DMA transfer complete interrupt
    dma_interrupt_enable(USART_TRANSFER_DMA_CH, DMA_CHXCTL_FTFIE);
    
    /* enable DMA channel3 */
    dma_channel_enable(USART_TRANSFER_DMA_CH);

	// �´���Ҫ���͵�log����λ��
	Log.Transfer_Index++;

	// ��ֵ����
	if (Log.Transfer_Index >= LOG_MAX_NUM)
	{
		Log.Transfer_Index = 0;
	}
	
}// End of void USART_DMA_Config(void)

/*******************************************************************************
*                			�����@2018-03-13
* Function Name  : USART_Receiver_DMA_Config
* Description    : �����жϳ�ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART_Receiver_DMA_Config(void)
{
	dma_parameter_struct dma_init_struct;

	/* deinitialize DMA channel2 */
	dma_deinit(USART_RECEIVER_DMA_CH);
	dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
	dma_init_struct.memory_addr = (uint32_t)(&USART_Rx.Buffer[1]);
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
	dma_init_struct.number = USART_DMA_RX_MAX_LEN;
	dma_init_struct.periph_addr = USART0_RDATA_ADDRESS;
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
	dma_init(USART_RECEIVER_DMA_CH, dma_init_struct);
	
	dma_circulation_disable(USART_RECEIVER_DMA_CH);
	dma_memory_to_memory_disable(USART_RECEIVER_DMA_CH);

	dma_channel_enable(USART_RECEIVER_DMA_CH);
	usart_dma_receive_config(USART0, USART_DENR_ENABLE);

	
}// End of void USART_DMA_Config(void)

/*******************************************************************************
*                			�����@2018-03-13
* Function Name  : LOG_Output
* Description    : �����жϳ�ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LOG_Output(void)
{
	// ����ַ���������
//	Log.Buffer[Log.Join_Index][ucLen] = 0;
	
	// log������ż�һ
	Log.Join_Index++;
	Log.Current_Num++;
	
	// ��ֵ����
	if (Log.Join_Index >= LOG_MAX_NUM)
	{
		Log.Join_Index = 0;
	}
	if (Log.Current_Num > LOG_MAX_NUM)
	{
		Log.Current_Num = LOG_MAX_NUM;
	}

	// log���ʹ��ڿ���״̬�����Է�����һ֡
	if (Log.Ready_To_Transfer == LOG_TRANSFER_IDLE)
	{
	
#ifdef LOG_ENABLE

		// ��PCBA��������²ſɷ���log
		if (Knob_Mode != KNOB_PCBA_TEST_MODE)
		{
			Log.Ready_To_Transfer = LOG_TRANSFER_BUSY;
			USART_Transfer_DMA_Config();
		}
#endif
	}
	
}// End of void LOG_Output(void)

/******************* (C) COPYRIGHT 2018 ����� *********** END OF FILE ********/





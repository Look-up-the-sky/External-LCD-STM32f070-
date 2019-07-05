#ifndef _MODBUS_H_
#define _MODBUS_H_
#include "stm32f0xx_usart.h"
#include "TypeDefine.h"
#include "App.h"
#define RS485_DIR1_OUT()	GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define RS485_DIR1_IN()	  GPIO_ResetBits(GPIOA,GPIO_Pin_8)
#define RS485_DIR2_OUT()	GPIO_SetBits(GPIOA,GPIO_Pin_1)
#define RS485_DIR2_IN()	  GPIO_ResetBits(GPIOA,GPIO_Pin_1)
extern INT8U UART_RX_RUNNING_FLAG;
extern INT32U c_1MS_Cnt;
extern INT16U PUSDATE_BUF[128];
/*Modbus ����ͷ��*/
typedef __packed struct{
	INT8U ID;
	INT8U func;
}modbus_head_t;

/*�����ּĴ������� 03*/
typedef __packed struct{
	INT8U ID;
	INT8U func;
	INT16U	addr;	/*�Ĵ�����ַ*/
	INT16U	size;	/*�Ĵ���*/
}modbus_holdreg_r_req;

/*�����ּĴ����ظ� 03*/
typedef struct{
	INT8U ID;
	INT8U func;
	INT8U	bytes;	/*�����ֽ���*/
	INT16U date[120];	/*����������*/
}modbus_holdreg_r_ack;
/*������Ĵ������� 04*/
typedef __packed struct{
	INT8U ID;
	INT8U func;
	INT16U	addr;	/*�Ĵ�����ַ*/
	INT16U	size;	/*�Ĵ���*/
}modbus_inputreg_r_req;

/*������Ĵ����ظ� 04*/
typedef  struct{
	INT8U ID;
	INT8U func;
	INT8U	bytes;	/*�����ֽ���*/
	INT16U	date[120];	/*����������*/
}modbus_inputreg_r_ack;

/*д���Ĵ������� 06*/
typedef __packed struct{
	INT8U ID;
	INT8U func;
	INT16U	addr;	/*�Ĵ�����ַ*/
	INT16U	value;	/*�Ĵ���ֵ*/
}modbus_holdreg_ws_req;

/*д���Ĵ����ظ� 06*/
typedef  struct{
	INT8U ID;
	INT8U func;
	INT16U	addr;	/*�Ĵ�����ַ*/
	INT16U	value;	/*�Ĵ���ֵ*/
}modbus_holdreg_ws_ack;

/*д��Ĵ������� 10*/
typedef  struct{
	INT8U ID;
	INT8U func;
	INT16U	addr;	/*�Ĵ�����ַ*/
	INT16U	size;	/*�������*/
	INT8U	bytes;	/*�ֽڼ���*/
	INT16U	date[120];	/*����������*/
}modbus_holdreg_wm_req;

/*д��Ĵ����ظ� 10*/
typedef __packed struct{
	INT8U ID;
	INT8U func;
	INT16U	addr;	/*�Ĵ�����ַ*/
	INT16U	size;	/*�������*/
}modbus_holdreg_wm_ack;

/*������Ӧ*/
typedef __packed struct{
	INT8U ID;
	INT8U func;
	INT8U  error;	//������
}modbus_error_ack;


#define SLAVE_ID          0x01
#define MODBUS_CRC_LEN		2
#define MAX_MODBUS_FUNNO	16
#define MAX_SNDBUF_LEN		256
#define MAX_RCVBUF_LEN		256
#define MAX_RCV_PERIOD    15

#define MIN_DEVID	1
#define MAX_DEVID	247


#define INVALID_FUNCNUM		1
#define INVALID_VALADDR		2
#define INVALID_VALUE			3

#define FLAG_FLASH_ADDRESS   0x0801FC00   //STM32F070
#define APP_FLASH_ADDRESS    0x08002800  //10KB
#define BOOT_TO_APP          1
#define APP_TO_BOOT          2
	

typedef struct{
	INT16U recvlen;
	INT16U sendlen;
	INT32U prevtime;
	INT8U modbus_snd_buf[MAX_SNDBUF_LEN];
	INT8U modbus_rcv_buf[MAX_RCVBUF_LEN];
}modbus_info_t;

typedef struct
{
	INT16U usStartAddr;
	INT16U usLen;
}WRITE_06_DATE;

typedef struct
{
	INT16U usStartAddr;
	INT16U usLen;
}WRITE_10_DATE;

extern modbus_info_t modbus;
extern WRITE_06_DATE write_06_date;
extern WRITE_10_DATE write_10_date;
extern INT8U FLAG_03_UPDATE;
extern INT8U FLAG_04_UPDATE;
extern INT8U FLAG_06_UPDATE;
extern INT8U FLAG_10_UPDATE;

void MODBUS_UART1_IRQHandler(void);
void MODBUS_UART2_IRQHandler(void);
void MODBUS_UARTSend(USART_TypeDef* USARTx, INT8U *BufferPtr, INT32U Length);
void sModbusTask(void);
void Auto_Identify_M(void);  //�Զ�ʶ����ͺ���
void read_03_update(INT8U ID,INT16U usStartAddr, INT16U usLen);
void read_04_update(INT8U ID,INT16U usStartAddr, INT16U usLen);
//void write_06_update(INT8U ID,INT16U usStartAddr, INT16U usLen);
void write_10_update(INT8U ID,INT16U usStartAddr, INT16U usLen);


#endif

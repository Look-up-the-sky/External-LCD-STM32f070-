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
/*Modbus 报文头部*/
typedef __packed struct{
	INT8U ID;
	INT8U func;
}modbus_head_t;

/*读保持寄存器请求 03*/
typedef __packed struct{
	INT8U ID;
	INT8U func;
	INT16U	addr;	/*寄存器地址*/
	INT16U	size;	/*寄存数*/
}modbus_holdreg_r_req;

/*读保持寄存器回复 03*/
typedef struct{
	INT8U ID;
	INT8U func;
	INT8U	bytes;	/*读出字节数*/
	INT16U date[120];	/*读到的数据*/
}modbus_holdreg_r_ack;
/*读输入寄存器请求 04*/
typedef __packed struct{
	INT8U ID;
	INT8U func;
	INT16U	addr;	/*寄存器地址*/
	INT16U	size;	/*寄存数*/
}modbus_inputreg_r_req;

/*读输入寄存器回复 04*/
typedef  struct{
	INT8U ID;
	INT8U func;
	INT8U	bytes;	/*读出字节数*/
	INT16U	date[120];	/*读到的数据*/
}modbus_inputreg_r_ack;

/*写单寄存器请求 06*/
typedef __packed struct{
	INT8U ID;
	INT8U func;
	INT16U	addr;	/*寄存器地址*/
	INT16U	value;	/*寄存器值*/
}modbus_holdreg_ws_req;

/*写单寄存器回复 06*/
typedef  struct{
	INT8U ID;
	INT8U func;
	INT16U	addr;	/*寄存器地址*/
	INT16U	value;	/*寄存器值*/
}modbus_holdreg_ws_ack;

/*写多寄存器请求 10*/
typedef  struct{
	INT8U ID;
	INT8U func;
	INT16U	addr;	/*寄存器地址*/
	INT16U	size;	/*输出数量*/
	INT8U	bytes;	/*字节计数*/
	INT16U	date[120];	/*读到的数据*/
}modbus_holdreg_wm_req;

/*写多寄存器回复 10*/
typedef __packed struct{
	INT8U ID;
	INT8U func;
	INT16U	addr;	/*寄存器地址*/
	INT16U	size;	/*输出数量*/
}modbus_holdreg_wm_ack;

/*错误响应*/
typedef __packed struct{
	INT8U ID;
	INT8U func;
	INT8U  error;	//错误码
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
void Auto_Identify_M(void);  //自动识别机型函数
void read_03_update(INT8U ID,INT16U usStartAddr, INT16U usLen);
void read_04_update(INT8U ID,INT16U usStartAddr, INT16U usLen);
//void write_06_update(INT8U ID,INT16U usStartAddr, INT16U usLen);
void write_10_update(INT8U ID,INT16U usStartAddr, INT16U usLen);


#endif

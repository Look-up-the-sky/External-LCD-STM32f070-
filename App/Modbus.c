/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：modbus.c
 * 修改人：沈仕强
 * 修改日期：2019.06.20
 * 描述：modbus函数
 *******************************************************/
#include "config.h"
#include "MF_Datecfg.h"
#include "HF_Datecfg.h"
#include "modbus.h"
#include "stm32f0xx_usart.h"
#include "Crc.h"
#include "Variable_Transformation.h"
#include "Led_State.h"


INT8U PRJ_NUMBER = Default_Model;//默认为0，通过通讯自动判定  
modbus_info_t modbus;
INT32U c_1MS_Cnt;
INT32U Timeout_Cnt = 0;
INT8U UART_RX_RUNNING_FLAG = 0;
INT8U UART2_RX_RUNNING_FLAG = 0;
INT16U PUSDATE_BUF[128];
INT8U  Uart2_Receive_BUF[20];
INT16U Uart2_Receive_Len;  //接收长度
//**************modbus任务标志位*************
INT8U FLAG_03_UPDATE = 1;
INT8U FLAG_04_UPDATE = 0;
INT8U FLAG_06_UPDATE = 0;
INT8U FLAG_10_UPDATE = 0;
INT8U FINISH_FLAG = 0;
INT8U CRC_Error_Flag = 0;
INT8U Read_Write_Change_Flag = 0;
INT8U Uart2_Receive_Finish_Flag = 0;
INT8U	Uart2_Receiving_Flag = 0;
//**************globle*****************
WRITE_06_DATE write_06_date;
WRITE_10_DATE write_10_date;
INT8U Auto_Identify[8] = {0x01,0x04,0x00,0x00,0x00,0x02,0x71,0xCB};
INT8U Set_Enable[13] = {0x01,0x10,0x20,0x00,0x00,0x02,0x04,0x45,0x41,0x53,0x54,0x12,0x79};
INT8U Country_Enable[13] = {0x01,0x10,0x21,0x02,0x00,0x02,0x04,0x45,0x41,0x53,0x54,0x9e,0x30};
INT8U Inv_Reset_Enable[13] = {0x01,0x10,0x21,0x06,0x00,0x02,0x04,0x52,0x45,0x43,0x46,0x56,0x7b};
INT8U History_Clear_Enable[13] = {0x01,0x10,0x21,0x09,0x00,0x02,0x04,0x48,0x43,0x4C,0x52,0xf4,0xdd};
INT8U PV_Mode_Enable[13] = {0x01,0x10,0x21,0x0c,0x00,0x02,0x04,0x4d,0x4f,0x44,0x45,0xb3,0xe3};
INT8U Total_Generation_En[13] = {0x01,0x10,0x21,0x0f,0x00,0x02,0x04,0x45,0x43,0x4f,0x4d,0x37,0x63};
INT8U Inv_Fast_Control_En[13] = {0x01,0x10,0x21,0x12,0x00,0x02,0x04,0x45,0x53,0x45,0x54,0x31,0x59};


void MODBUS_UART1_IRQHandler(void)
{
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)		//数据接收中断
	{
		if(UART_RX_RUNNING_FLAG == 0)
		{
			UART_RX_RUNNING_FLAG = 1;
			modbus.prevtime = c_1MS_Cnt;
		}
		/*超过最长MODBUS缓冲 丢弃*/
		if(modbus.recvlen >= MAX_RCVBUF_LEN )
		{
			modbus.modbus_rcv_buf[0]=USART1->RDR ;
			modbus.recvlen = 0;
			return;
		}
		Comm_StateFlag--;   //通讯计数，用来判断与逆变器通讯是否中断
		if(Comm_StateFlag != 0)
		{
			Comm_StateFlag = 0;
		}
		modbus.modbus_rcv_buf[modbus.recvlen++]=USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	

}

void MODBUS_UART2_IRQHandler(void)
{
		if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)        
	   {
			 Uart2_Receive_BUF[Uart2_Receive_Len++]=USART_ReceiveData(USART2);
			 USART_ClearITPendingBit(USART2, USART_IT_RXNE);
			 Uart2_Receiving_Flag = 1;
	   }	 
		else if((USART_GetFlagStatus(USART2,USART_FLAG_IDLE)!=RESET)&&(Uart2_Receiving_Flag == 1))
			{					
				USART_ClearFlag(USART2, USART_FLAG_IDLE);						
				Uart2_Receive_Finish_Flag = 1;
				Uart2_Receiving_Flag = 0;
      }	
			else
			{
				USART_ClearFlag(USART2, USART_FLAG_IDLE);	
			}
	
}

//***********************************发送函数**********************************
void MODBUS_UARTSend(USART_TypeDef* USARTx, INT8U *BufferPtr, INT32U Length)
{
	RS485_DIR1_OUT();
	while ( Length != 0 )
		{
			USART_SendData(USARTx, (unsigned char)*BufferPtr);
			while( USART_GetFlagStatus(USARTx,USART_FLAG_TC) == RESET);
			BufferPtr++;
			Length--;
		}
	Comm_StateFlag++;   //通讯计数，用来判断与逆变器通讯是否中断
	RS485_DIR1_IN();
}



void sModbusTask(void)
{
	static unsigned char CNT = 0,First_Flag =0,Second_Flag =0;
	INT16U cur_cnt,offset;
	//01 10 70 00 00 02 04 49 41 50 45 2d d6
	if(Uart2_Receive_Finish_Flag == 1)     //升级信号监听
	{
		Uart2_Receive_Finish_Flag = 0;
		Uart2_Receive_Len = 0;
		if((Uart2_Receive_BUF[0] == 0x01)&&(Uart2_Receive_BUF[1] == 0x10)&&(Uart2_Receive_BUF[2] == 0x70)
			&&(Uart2_Receive_BUF[3] == 0x00)&&(Uart2_Receive_BUF[4] == 0x00)&&(Uart2_Receive_BUF[5] == 0x02)
			&&(Uart2_Receive_BUF[6] == 0x04)&&(Uart2_Receive_BUF[7] == 0x49)&&(Uart2_Receive_BUF[8] == 0x41)
			&&(Uart2_Receive_BUF[9] == 0x50)&&(Uart2_Receive_BUF[10] == 0x45))
		{
			/* Clear All pending flags */
				FLASH_ClearFlag( FLASH_FLAG_BSY|FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR);
				FLASH_Unlock();
				FLASH_ErasePage(FLAG_FLASH_ADDRESS);
				if(FLASH_ProgramWord(FLAG_FLASH_ADDRESS,APP_TO_BOOT) == FLASH_COMPLETE)
				{	
					FLASH_Lock();
					__set_PRIMASK(1);
					NVIC_SystemReset();
				}
		}
	}
	else if(PRJ_NUMBER == Default_Model) //用于启动判断是黄蜂还是蜜蜂机型
	{	
			Auto_Identify_M();
			if(FINISH_FLAG == 1)
			{
				if((modbus.modbus_rcv_buf[3] == 0x45)&&(modbus.modbus_rcv_buf[4] == 0x41)
					&&(modbus.modbus_rcv_buf[5] == 0x53)&&(modbus.modbus_rcv_buf[6] == 0x54))
				{
					PRJ_NUMBER = SUNNYBEE;
				}
				else if((modbus.modbus_rcv_buf[3] == 0x48)&&(modbus.modbus_rcv_buf[4] == 0x52)
					&&(modbus.modbus_rcv_buf[5] == 0x4E)&&(modbus.modbus_rcv_buf[6] == 0x54))
				{
					PRJ_NUMBER = HORNET;
				}
				else
				{
					PRJ_NUMBER = Default_Model;
				}
			}		
	}
	else
	{
		if(FLAG_06_UPDATE == 1)
		{
			Read_Write_Change_Flag =1;
			FLAG_03_UPDATE = 0;
			FLAG_04_UPDATE = 0;
			FLAG_06_UPDATE = 0;
			//write_06_update(0x01,write_06_date.usStartAddr,write_06_date.usLen);
		}
		else
		{
			if(FLAG_10_UPDATE == 1)
			{
				Read_Write_Change_Flag = 1;
				FLAG_03_UPDATE = 0;
				FLAG_04_UPDATE = 0;
				
				if(PRJ_NUMBER == SUNNYBEE)
				{
					if(First_Flag == 0)
					{
						First_Flag = 1;
						memcpy(modbus.modbus_snd_buf,Set_Enable,13);
						MODBUS_UARTSend(USART1, modbus.modbus_snd_buf, 13);
						UART_RX_RUNNING_FLAG = 0;
					}
					else
					{
						if((UART_RX_RUNNING_FLAG == 1)&&(First_Flag == 1)&&(Second_Flag == 0))
						{
							cur_cnt = c_1MS_Cnt;
							offset = (cur_cnt>=modbus.prevtime)?(cur_cnt-modbus.prevtime):(0xFFFFFFFF-modbus.prevtime+cur_cnt);
							if(offset < MAX_RCV_PERIOD)
							return;
							Second_Flag =1;
							if((write_10_date.usStartAddr == 0x2104)||(write_10_date.usStartAddr == 0x2105))
							{
								memcpy(modbus.modbus_snd_buf,Country_Enable,13);
								MODBUS_UARTSend(USART1, modbus.modbus_snd_buf, 13);
							}
							if(write_10_date.usStartAddr == 0x2108)
							{
								memcpy(modbus.modbus_snd_buf,Inv_Reset_Enable,13);
								MODBUS_UARTSend(USART1, modbus.modbus_snd_buf, 13);
							}
							if(write_10_date.usStartAddr == 0x210b)
							{
								memcpy(modbus.modbus_snd_buf,History_Clear_Enable,13);
								MODBUS_UARTSend(USART1, modbus.modbus_snd_buf, 13);
							}
							if(write_10_date.usStartAddr == 0x210e)
							{
								memcpy(modbus.modbus_snd_buf,PV_Mode_Enable,13);
								MODBUS_UARTSend(USART1, modbus.modbus_snd_buf, 13);
							}
							if(write_10_date.usStartAddr == 0x2111)
							{
								memcpy(modbus.modbus_snd_buf,Total_Generation_En,13);
								MODBUS_UARTSend(USART1, modbus.modbus_snd_buf, 13);
							}
							if(write_10_date.usStartAddr >=0x2114)
							{
								memcpy(modbus.modbus_snd_buf,Inv_Fast_Control_En,13);
								MODBUS_UARTSend(USART1, modbus.modbus_snd_buf, 13);
							}
							UART_RX_RUNNING_FLAG = 0;
							modbus.recvlen = 0;
						}
						else
						{
							if((UART_RX_RUNNING_FLAG == 1)&&(First_Flag == 1)&&(Second_Flag == 1))
								{
									cur_cnt = c_1MS_Cnt;
									offset = (cur_cnt>=modbus.prevtime)?(cur_cnt-modbus.prevtime):(0xFFFFFFFF-modbus.prevtime+cur_cnt);
									if(offset < MAX_RCV_PERIOD)
									return;
									write_10_update(0x01,write_10_date.usStartAddr,write_10_date.usLen);
									UART_RX_RUNNING_FLAG =0;
									if(FINISH_FLAG == 1)
									{
										FLAG_03_UPDATE = 1;
										CNT = 0;
										FLAG_10_UPDATE = 0;
										First_Flag = 0;
										Second_Flag = 0;
									}
									else
									{
										if(CRC_Error_Flag ==1)
										{
											CRC_Error_Flag =0;
											First_Flag = 0;
											Second_Flag = 0;
										}
									}
								}							
						}
					}
				}
				else if(PRJ_NUMBER == HORNET)
				{
					write_10_update(0x01,write_10_date.usStartAddr,write_10_date.usLen);
					if(FINISH_FLAG == 1)
					{
						FLAG_03_UPDATE = 1;
						CNT = 0;
						FLAG_10_UPDATE = 0;
					}
				}
			}
			else
			{
				if(FLAG_03_UPDATE == 1)
				{
					switch(CNT)
					{
						case 0:
						{
							if(PRJ_NUMBER == SUNNYBEE)
								read_03_update(0x01,0x2000, 0x0c);
							else if(PRJ_NUMBER == HORNET)
								read_03_update(0x01,0x2080, 0x03);						
							if(FINISH_FLAG ==1)
							{
								CNT = 1;
							}
						
						}
						break;
						case 1:
						{
							if(PRJ_NUMBER == SUNNYBEE)
								read_03_update(0x01,0x2100, 0x25);
							else if(PRJ_NUMBER == HORNET)
								read_03_update(0x01,0x2100, 0x30);
							if(FINISH_FLAG ==1)
							{
								CNT = 2;
							}
						}
						break;
						case 2:
						{
							if(PRJ_NUMBER == SUNNYBEE)
								read_03_update(0x01,0x2125, 0x33);
							else if(PRJ_NUMBER == HORNET)
								read_03_update(0x01,0x2202, 0x04);
							if(FINISH_FLAG ==1)
							{
								if(PRJ_NUMBER == SUNNYBEE)
								{
									System_Time_Change_Flag = 1; //系统时间改变标志位
									CNT = 0;
									Read_Write_Change_Flag = 1;
									FLAG_04_UPDATE = 1;
									FLAG_03_UPDATE = 0;   //开机询问一次
								}
								else if(PRJ_NUMBER == HORNET)
								{
									CNT = 3;
								}
							}
						}
						break;
						case 3:
						{
							if(PRJ_NUMBER == HORNET)
							  read_03_update(0x01,0x2280, 0x08);
							if(FINISH_FLAG ==1)
							{
								CNT = 4;
							}
						}
						break;
						case 4:
						{
							if(PRJ_NUMBER == HORNET)
						    read_03_update(0x01,0x2300, 0x7A);
							if(FINISH_FLAG ==1)
							{
								CNT = 5;
							}
						}
						break;
						case 5:
						{
							if(PRJ_NUMBER == HORNET)
								read_03_update(0x01,0x237A, 0x69);
							if(FINISH_FLAG ==1)
							{
								CNT = 6;
							}
						}
						break;
						case 6:
						{
							if(PRJ_NUMBER == HORNET)
								read_03_update(0x01,0x2480, 0x13);
							if(FINISH_FLAG ==1)
							{
								System_Time_Change_Flag = 1; //系统时间改变标志位
								CNT = 0;
								Read_Write_Change_Flag = 1;
								FLAG_04_UPDATE = 1;
								FLAG_03_UPDATE = 0;   //开机询问一次
							}
						}
						default:
							{
							if(CNT > 6)
								CNT = 0;
							}
							break;
					}
				}
				else
				{
					if(FLAG_04_UPDATE == 1)  //定时更新
					{
						switch(CNT)
						{
							case 0:
							{
								if(PRJ_NUMBER == SUNNYBEE)
									read_04_update(0x01,0x0000,0x61);
								else if(PRJ_NUMBER == HORNET)
									read_04_update(0x01,0x0000,0x61);
								if(FINISH_FLAG == 1)
								{
									CNT = 1;
								}
							}
								break;
							case 1:
							{
								if(PRJ_NUMBER == SUNNYBEE)
									read_04_update(0x01,0x0100,0x35);
								else if(PRJ_NUMBER == HORNET)
									read_04_update(0x01,0x0100,0x07);
								if(FINISH_FLAG == 1)
								{
									CNT = 2;
								}
							}
								break;
							case 2:
							{
								if(PRJ_NUMBER == SUNNYBEE)
									read_04_update(0x01,0x0200,0x1c);
								else if(PRJ_NUMBER == HORNET)
									read_04_update(0x01,0x0180,0x10);
								if(FINISH_FLAG == 1)
								{
									CNT = 3;
								}
							}
								break;
							case 3:
							{
								if(PRJ_NUMBER == SUNNYBEE)
									read_04_update(0x01,0x0BB8,0x50);
								else if(PRJ_NUMBER == HORNET)
									read_04_update(0x01,0x0200,0x1A);
								if(FINISH_FLAG == 1)
								{
									CNT = 4;
								}
							}
								break;
							case 4:
							{
								if(PRJ_NUMBER == SUNNYBEE)
									read_04_update(0x01,0x0FA0,0x21);
								else if(PRJ_NUMBER == HORNET)
									read_04_update(0x01,0x0280,0x0F);
								if(FINISH_FLAG == 1)
								{
									CNT = 5;
								}
							}
								break;
							case 5:
							{
								if(PRJ_NUMBER == SUNNYBEE)
									read_04_update(0x01,0x1004,0x1b);
								else if(PRJ_NUMBER == HORNET)
									read_04_update(0x01,0x0300,0x64);
								if(FINISH_FLAG == 1)
								{
									CNT = 0;
								}
							}
								break;
							default:
							{
								if(CNT >5)
									CNT = 0;
							}
								break;
						}				
					}
				}
			}
		}
	}
}
void Auto_Identify_M()   //自动识别机型函数
{
	static INT8U SEND_Auto_FLAG = 0;
	INT16U cur_cnt,offset;
	INT16U crc,RX_CRC;
	if(SEND_Auto_FLAG == 0)                //询问指令未发送
	{

		//01 04 00 00 00 02 71 CB 
		MODBUS_UARTSend(USART1,Auto_Identify,8);
		SEND_Auto_FLAG = 1;
		modbus.recvlen = 0;
		FINISH_FLAG = 0;
	}
	else                                      //询问指令发送完成，等待接收
	{
		Timeout_Cnt++;
		if(Timeout_Cnt > 5)      //防止同逆变器一起上电，导致询问后永远等待逆变器回应
		{
			Timeout_Cnt = 0;
			SEND_Auto_FLAG = 0;
		}
		if(UART_RX_RUNNING_FLAG == 1)
		{
			cur_cnt = c_1MS_Cnt;
			offset = (cur_cnt>=modbus.prevtime)?(cur_cnt-modbus.prevtime):(0xFFFFFFFF-modbus.prevtime+cur_cnt);
			if(offset < MAX_RCV_PERIOD)
			return;
			//接收完成，更新数据    
			UART_RX_RUNNING_FLAG = 0;
			crc = crc16(modbus.modbus_rcv_buf,modbus.recvlen-MODBUS_CRC_LEN);
			/*CRC校验错误丢弃*/
			RX_CRC = (modbus.modbus_rcv_buf[modbus.recvlen-MODBUS_CRC_LEN])|(modbus.modbus_rcv_buf[modbus.recvlen-MODBUS_CRC_LEN+1]<<8);
			if(RX_CRC != crc)
			{
				modbus.recvlen = 0;
				SEND_Auto_FLAG = 0;//失败则继续询问
				return;
			}
			SEND_Auto_FLAG = 0;
			modbus.recvlen = 0;
			FINISH_FLAG =1;
			Timeout_Cnt = 0;
		}
	}
}
void read_03_update(INT8U ID,INT16U usStartAddr, INT16U usLen)
{	
	static INT8U SEND_03_FLAG = 0;
	INT16U cur_cnt,offset;
	INT16U rlen,crc,RX_CRC;
	modbus_holdreg_r_req r_req;
	if(Read_Write_Change_Flag ==1)
	{
		SEND_03_FLAG = 0;
		Read_Write_Change_Flag = 0;
	}
	if(SEND_03_FLAG == 0)                //询问指令未发送
	{
		modbus.recvlen = 0;
		r_req.ID = ID;
		r_req.func = 0x03;
		r_req.addr = __REV16(usStartAddr);
		r_req.size = __REV16(usLen);
		rlen = sizeof(modbus_holdreg_r_req);
		memcpy(modbus.modbus_snd_buf,&r_req,rlen);
		*(INT16U*)(&modbus.modbus_snd_buf[rlen]) = crc16(modbus.modbus_snd_buf,rlen);
		MODBUS_UARTSend(USART1, modbus.modbus_snd_buf, rlen+MODBUS_CRC_LEN);
		SEND_03_FLAG = 1;
		FINISH_FLAG = 0;
	}
	else                                      //询问指令发送完成，等待接收
	{
		Timeout_Cnt++;
		if(Timeout_Cnt > 5)      //防止同逆变器一起上电，导致询问后永远等待逆变器回应
		{
			Timeout_Cnt = 0;
			SEND_03_FLAG = 0;
		}
		if(UART_RX_RUNNING_FLAG == 1)
		{
			cur_cnt = c_1MS_Cnt;
			offset = (cur_cnt>=modbus.prevtime)?(cur_cnt-modbus.prevtime):(0xFFFFFFFF-modbus.prevtime+cur_cnt);
			if(offset < MAX_RCV_PERIOD)
			return;
			//接收完成，更新数据
			UART_RX_RUNNING_FLAG = 0;
			crc = crc16(modbus.modbus_rcv_buf,modbus.recvlen-MODBUS_CRC_LEN);
			/*CRC校验错误丢弃*/
			RX_CRC = (modbus.modbus_rcv_buf[modbus.recvlen-MODBUS_CRC_LEN])|(modbus.modbus_rcv_buf[modbus.recvlen-MODBUS_CRC_LEN+1]<<8);
			if(RX_CRC != crc)
			{
				modbus.recvlen = 0;
				SEND_03_FLAG = 0;//失败则继续询问
				FLAG_03_UPDATE = 1;
				return;
			}
			memcpy(PUSDATE_BUF,&modbus.modbus_rcv_buf[3],(modbus.recvlen-MODBUS_CRC_LEN-3));
			if(PRJ_NUMBER == SUNNYBEE)
				CFG_ParamCfg(g_astrParamCfgTblAttr, PUSDATE_BUF, usStartAddr, usLen);//写入MF参数
			else if(PRJ_NUMBER == HORNET)
				CFG_ParamCfg(HF_astrParamCfgTblAttr, PUSDATE_BUF, usStartAddr, usLen);//写入HF参数
			modbus.recvlen = 0;	
			SEND_03_FLAG = 0;
			FINISH_FLAG = 1;
			Timeout_Cnt = 0;
		}
	}
	
	
}
void read_04_update(INT8U ID,INT16U usStartAddr, INT16U usLen)
{
	static INT8U SEND_04_FLAG = 0;
	INT16U cur_cnt,offset;
	INT16U rlen,crc,RX_CRC;
	modbus_inputreg_r_req r_req;
	if(Read_Write_Change_Flag ==1)
	{
		SEND_04_FLAG = 0;
		Read_Write_Change_Flag = 0;
	}
	if(SEND_04_FLAG == 0)                //询问指令未发送
	{
		r_req.ID = ID;
		r_req.func = 0x04;
		r_req.addr = __REV16(usStartAddr);
		r_req.size = __REV16(usLen);
		rlen = sizeof(modbus_inputreg_r_req);
		memcpy(modbus.modbus_snd_buf,&r_req,rlen);
		*(INT16U*)(&modbus.modbus_snd_buf[rlen]) = crc16(modbus.modbus_snd_buf,rlen);
		MODBUS_UARTSend(USART1, modbus.modbus_snd_buf, rlen+MODBUS_CRC_LEN);
		SEND_04_FLAG = 1;
		modbus.recvlen = 0;
		FINISH_FLAG = 0;
	}
	else                                      //询问指令发送完成，等待接收
	{
		if(UART_RX_RUNNING_FLAG == 1)
		{
			cur_cnt = c_1MS_Cnt;
			offset = (cur_cnt>=modbus.prevtime)?(cur_cnt-modbus.prevtime):(0xFFFFFFFF-modbus.prevtime+cur_cnt);
			if(offset < MAX_RCV_PERIOD)
			return;
			//接收完成，更新数据    
			UART_RX_RUNNING_FLAG = 0;
			crc = crc16(modbus.modbus_rcv_buf,modbus.recvlen-MODBUS_CRC_LEN);
			/*CRC校验错误丢弃*/
			RX_CRC = (modbus.modbus_rcv_buf[modbus.recvlen-MODBUS_CRC_LEN])|(modbus.modbus_rcv_buf[modbus.recvlen-MODBUS_CRC_LEN+1]<<8);
			if(RX_CRC != crc)
			{
				modbus.recvlen = 0;
				SEND_04_FLAG = 0;//失败则继续询问
				FLAG_04_UPDATE = 1;
				return;
			}
			memcpy(PUSDATE_BUF,&modbus.modbus_rcv_buf[3],(modbus.recvlen-MODBUS_CRC_LEN-3));
			if(PRJ_NUMBER == SUNNYBEE)
				CFG_ParamCfg(g_astrParamSendTblAttr, PUSDATE_BUF, usStartAddr, usLen);//写入MF参数
			else if(PRJ_NUMBER == HORNET)
				CFG_ParamCfg(HF_astrParamSendTblAttr, PUSDATE_BUF, usStartAddr, usLen);//写入HF参数
			SEND_04_FLAG = 0;
			modbus.recvlen = 0;
			FINISH_FLAG =1;
	
		}
	}
}

//void write_06_update(INT8U ID,INT16U usStartAddr, INT16U usLen)
//{
//	static INT8U SEND_06_FLAG = 0;
//	INT16U cur_cnt,offset;
//	INT16U rlen,crc,RX_CRC;
//	modbus_holdreg_ws_req ws_req;
//	if(SEND_06_FLAG == 0)         //写参数信息未发送
//	{
//		modbus.recvlen = 0;
//		ws_req.ID = ID;
//		ws_req.func = 0x06;
//		ws_req.addr = __REV16(usStartAddr);
//		if(PRJ_NUMBER == SUNNYBEE)
//				CFG_LoadParam(PUSDATE_BUF, g_astrParamCfgTblAttr, usStartAddr,usLen, 0x03); //MF参数读取
//		else if(PRJ_NUMBER == HORNET)
//				CFG_LoadParam(PUSDATE_BUF, HF_astrParamCfgTblAttr, usStartAddr,usLen, 0x03); //HF参数读取
//		ws_req.value = PUSDATE_BUF[0];
//		rlen = sizeof(modbus_holdreg_ws_req);
//		memcpy(modbus.modbus_snd_buf,&ws_req,rlen);
//		*(INT16U*)(&modbus.modbus_snd_buf[rlen]) = crc16(modbus.modbus_snd_buf,rlen);
//		MODBUS_UARTSend(USART1, modbus.modbus_snd_buf, rlen+MODBUS_CRC_LEN);
//		SEND_06_FLAG = 1;
//	}
//	else                            //发送完成，等待接收回复
//	{
//		if(UART_RX_RUNNING_FLAG == 1)
//		{
//			cur_cnt = c_1MS_Cnt;
//			offset = (cur_cnt>=modbus.prevtime)?(cur_cnt-modbus.prevtime):(0xFFFFFFFF-modbus.prevtime+cur_cnt);
//			if(offset < MAX_RCV_PERIOD)
//			return;
//			//接收完成，判断是否写入成功
//			UART_RX_RUNNING_FLAG = 0;
//			crc = crc16(modbus.modbus_rcv_buf,modbus.recvlen-MODBUS_CRC_LEN);
//			/*CRC校验错误丢弃*/
//			RX_CRC = (modbus.modbus_rcv_buf[modbus.recvlen-MODBUS_CRC_LEN])|(modbus.modbus_rcv_buf[modbus.recvlen-MODBUS_CRC_LEN+1]<<8);
//			if(RX_CRC != crc)
//			{
//				modbus.recvlen = 0;
//				return;
//			}
//		}
//		modbus.recvlen = 0;
//	}
//}
//	
void write_10_update(INT8U ID,INT16U usStartAddr, INT16U usLen)
{
	static INT8U SEND_10_FLAG = 0;
	INT16U cur_cnt,offset;
	INT16U rlen,crc,RX_CRC;
	modbus_holdreg_wm_req wm_req;
	if(SEND_10_FLAG == 0)         //写参数信息未发送
	{
		modbus.recvlen = 0;
		FINISH_FLAG = 0;
		wm_req.ID = ID;
		wm_req.func = 0x10;
		wm_req.addr = __REV16(usStartAddr);	
		wm_req.size = __REV16(usLen);
		wm_req.bytes = usLen*2;
		if(PRJ_NUMBER == SUNNYBEE)
				CFG_LoadParam(PUSDATE_BUF, g_astrParamCfgTblAttr, usStartAddr,usLen, 0x03); //MF参数读取
		else if(PRJ_NUMBER == HORNET)
				CFG_LoadParam(PUSDATE_BUF, HF_astrParamCfgTblAttr, usStartAddr,usLen, 0x03); //HF参数读取
		memcpy(modbus.modbus_snd_buf,&wm_req,7);
		memcpy(&modbus.modbus_snd_buf[7],PUSDATE_BUF,usLen*2);
		rlen = usLen*2 + 7;
		crc = crc16(modbus.modbus_snd_buf,rlen);
		modbus.modbus_snd_buf[rlen] = crc;
		modbus.modbus_snd_buf[rlen+1] = crc>>8;
		MODBUS_UARTSend(USART1, modbus.modbus_snd_buf, rlen+MODBUS_CRC_LEN);
		SEND_10_FLAG = 1;
	}
	else                            //发送完成，等待接收回复
	{
		if(UART_RX_RUNNING_FLAG == 1)
		{
			cur_cnt = c_1MS_Cnt;
			offset = (cur_cnt>=modbus.prevtime)?(cur_cnt-modbus.prevtime):(0xFFFFFFFF-modbus.prevtime+cur_cnt);
			if(offset < MAX_RCV_PERIOD)
			return;
			//接收完成，判断是否写入成功
			UART_RX_RUNNING_FLAG = 0;
			crc = crc16(modbus.modbus_rcv_buf,modbus.recvlen-MODBUS_CRC_LEN);
			/*CRC校验错误丢弃*/
			RX_CRC = (modbus.modbus_rcv_buf[modbus.recvlen-MODBUS_CRC_LEN])|(modbus.modbus_rcv_buf[modbus.recvlen-MODBUS_CRC_LEN+1]<<8);
			if(RX_CRC != crc)
			{
				modbus.recvlen = 0;
				SEND_10_FLAG = 0;
				FLAG_10_UPDATE = 1;
				CRC_Error_Flag = 1;
				return;
			}
			SEND_10_FLAG = 0;
			FINISH_FLAG = 1;
			modbus.recvlen = 0;
		}		
	}
}


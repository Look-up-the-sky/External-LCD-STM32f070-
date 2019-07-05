/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：debug.c
 * 修改人：沈仕强
 * 修改日期：2019.06.20
 * 描述：调试函数
 *******************************************************/
#include "debug.h"
#include <stdarg.h>
#include "modbus.h"
#include "MF_Datecfg.h"
#include "HF_Datecfg.h"
#include "Lib_Communication.h"
#include "Variable_Transformation.h"
int fputc(int ch, FILE *f)
{
	/* 将Printf内容发往串口 */
	USART_SendData(USART2, (unsigned char) ch);
	while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);	
	return (ch);
}

static char *itoa(int value, char *string, int radix)
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;
	
	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
	    *ptr = 0;
	    return string;
	}
	
	if (!value)
	{
	    *ptr++ = 0x30;
	    *ptr = 0;
	    return string;
	}
	
	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
	    *ptr++ = '-';
	
	    /* Make the value positive. */
	    value *= -1;
	}
	
	for (i = 10000; i > 0; i /= 10)
	{
	    d = value / i;
	
	    if (d || flag)
	    {
	        *ptr++ = (char)(d + 0x30);
	        value -= (d * i);
	        flag = 1;
	    }
	}
	
	/* Null terminate the string. */
	*ptr = 0;
	
	return string;

} /* NCL_Itoa */

void USART2_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
{
	const char *s;
	int d;   
	char buf[16];
	
	va_list ap;
	va_start(ap, Data);
	
	while ( *Data != 0)     // 判断是否到达字符串结束符
	{				                          
		if ( *Data == 0x5c )  //'\'
	{									  
	switch ( *++Data )
	{
		case 'r':							          //回车符
			USART_SendData(USARTx, 0x0d);
			Data ++;
		break;
		
		case 'n':							          //换行符
			USART_SendData(USARTx, 0x0a);	
			Data ++;
		break;
		
		default:
			Data ++;
		break;
	}			 
	}
	else if ( *Data == '%')
	{									  //
	switch ( *++Data )
	{				
		case 's':										  //字符串
			s = va_arg(ap, const char *);
	for ( ; *s; s++) 
	{
		USART_SendData(USARTx,*s);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
		Data++;
		break;
	
	case 'd':										//十进制
	d = va_arg(ap, int);
	itoa(d, buf, 10);
	for (s = buf; *s; s++) 
	{
		USART_SendData(USARTx,*s);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
	Data++;
	break;
		 default:
				Data++;
		    break;
	}		 
	} /* end of else if */
	else USART_SendData(USARTx, *Data++);
	while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
}
void UART_2_Send(USART_TypeDef* USARTx, INT8U *BufferPtr, INT32U Length)
{
	RS485_DIR2_OUT();
	while ( Length != 0 )
		{
			USART_SendData(USARTx, (unsigned char)*BufferPtr);
			while( USART_GetFlagStatus(USARTx,USART_FLAG_TC) == RESET);
			BufferPtr++;
			Length--;
		}
	RS485_DIR2_IN();
}

void sDebugTask(void)
{
	INT8U BUF[50];
	memcpy(BUF,&MF_sPowerFactor,2);
	UART_2_Send(USART2,BUF,2);
}

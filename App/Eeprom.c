/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Eeprom.c
 * 修改人：沈仕强
 * 修改日期：2019.06.20
 * 描述：Eeprom读写
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"
#include "Eeprom.h"

/********************************************************************
 * Macros
 ********************************************************************/
#define Write  0xfe
#define Read   0x01
 
 /**************************************************************
*Function
**************************************************************/
void I2c_delay(unsigned int t)
{
	while(t--);
}

void I2c_delayms(unsigned char t)
{
	INT16U i,j;
	for(i = 0;i < t;i++)
	{
		for(j = 0;j <= 4600;j++)
		{
		}
	}
}

void I2c_start(void)
{
	SDA_out();
	SDA_H();
	I2c_delay(10);
	SCL_H();
	I2c_delay(20);
	SDA_L();
	I2c_delay(20);
	SCL_L();
	I2c_delay(10);
}

void I2c_stop(void)
{
	SDA_out();
	SDA_L();
	I2c_delay(10);
	SCL_H();
	I2c_delay(20);
	SDA_H();
	I2c_delay(20);
	SCL_L();
	I2c_delay(10);
}

unsigned char I2c_wait_ack(void)
{
	unsigned char i;			
	unsigned char I2c_bad;
	
	SDA_H();
	SDA_in();			
	I2c_delay(20);
	SCL_H();
	I2c_delay(10);
	for(i=0; i<3; i++)
	{
		if((SDA_High) == 0)
		{
			I2c_bad = 1;
			break;
		}
		else
		{
			I2c_delay(10);		
		}
	}
	SCL_L();
	I2c_delay(10);
	SDA_out();
	SDA_H();
	I2c_delay(10);
	return  I2c_bad;
}

void I2c_no_wait_ack(void)
{
	SDA_out();
	I2c_delay(20);
	SDA_H();
	I2c_delay(20);
	SCL_H();
	I2c_delay(20);
	SCL_L();
	I2c_delay(20);
}

void I2c_send_ack(void)
{
	SDA_out();
	I2c_delay(20);
	SDA_L();
	I2c_delay(20);
	SCL_H();
	I2c_delay(20);
	SCL_L();
	I2c_delay(20);
}

void I2C_send_no_ack(void)
{
	SDA_out();
	SDA_H();
	I2c_delay(10);				
	SCL_H();
	I2c_delay(10);				
	SCL_L();
	I2c_delay(10);				
}

void I2c_wr8bit(unsigned char c)
{
	unsigned char	i;			

	for(i=0; i<8; i++)
	{
		if(c&0x80)
			SDA_H();
		else	
			SDA_L();
		I2c_delay(30);
		SCL_H();
		I2c_delay(30);
		SCL_L();
		c=c<<1;		
		I2c_delay(30);
	}
	SDA_H();	
	I2c_delay(20);	
}

unsigned char I2c_rd8bit(void)
{
	unsigned char i, val=0;	
	SDA_in();
	I2c_delay(30);
	for(i=0x80; i>0;)             
	{
		SCL_H();                     
		I2c_delay(20);
		if(SDA_High)
			val=(val | i);   
		I2c_delay(20);
		SCL_L();
		i>>=1;
		I2c_delay(20);
	}
	I2c_delay(30);
	return val;
}

unsigned char I2c_write(unsigned char Addr,unsigned char* Data,unsigned char DeviceID)
{
	unsigned char Ack;

	I2c_start();
	I2c_wr8bit(DeviceID&Write);
	Ack = I2c_wait_ack();
	if(Ack == 1)
	{
		I2c_wr8bit(Addr);
		Ack = I2c_wait_ack();
		if(Ack == 1)
		{
			I2c_wr8bit(*Data);
			Ack = I2c_wait_ack();
		}
	}	
	I2c_stop();
	return Ack;
}

unsigned char I2c_read(unsigned char Addr,unsigned char* Data,unsigned char DeviceID)
{
	unsigned char Ack = 0;
	I2c_start();
	I2c_wr8bit(DeviceID&Write);
	Ack = I2c_wait_ack();
	if(Ack == 1)
	{
		I2c_wr8bit(Addr);
		Ack = I2c_wait_ack();
		if(Ack == 1)
		{
			I2c_start();
			I2c_wr8bit(DeviceID|Read); 
			Ack = I2c_wait_ack();
			if(Ack == 1)
			{
				*Data=I2c_rd8bit(); 
				I2c_no_wait_ack();
			}
		}
	}	 
	
	I2c_stop();
	return Ack;
}

void I2c_wr8bit_low(unsigned char c)
{
	char	i;			
	SDA_out();
	for(i=0; i<8; i++)
	{
		if(c&0x01)
			SDA_H();
		else	
			SDA_L();
		I2c_delay(10);
		SCL_H();
		I2c_delay(10);
		SCL_L();
		c>>=1;			
		I2c_delay(10);
	}
	SDA_H();			
}

void sWriteI2C(INT8U *lubData,INT16U luwI2CAddr,INT8U lubDataLen)
{
	INT8U Ack;
	INT8U Len;
	INT8U PageAddr;
	INT8U WriteTimes;
	
	for(Len = 0; Len < lubDataLen; Len++)
	{
		PageAddr = ((unsigned char)(luwI2CAddr>>8))&0x03;
		PageAddr <<= 1;
		for(WriteTimes = 0; WriteTimes < 10; WriteTimes++)
		{
			Ack = I2c_write(luwI2CAddr&0xff,(unsigned char *)lubData,0xA8 |(PageAddr&0x0f));
			if(Ack)
				break;
		}
		if(Ack)
		{
			luwI2CAddr++;
			lubData++;
		}
		else break;
	}
}

void sReadI2C(INT8U* lubData,INT16U luwI2CAddr,INT8U lubDataLen)
{
	INT8U Len;
	INT8U PageAddr;
	INT8U Ack;
	INT8U ReadTimes;

	for(Len = 0; Len < lubDataLen; Len++)
	{
		PageAddr = (unsigned char)((luwI2CAddr>>8)&0x03);
		PageAddr <<= 1;
		for(ReadTimes = 0; ReadTimes < 10; ReadTimes++)
		{
			Ack = I2c_read(luwI2CAddr&0xff,(unsigned char*)lubData,0xA8|(PageAddr&0x0f));
			if(Ack)
				break;
		}
		if(Ack)
		{
			luwI2CAddr++;
			lubData++;
		}
		else break;
	}
}

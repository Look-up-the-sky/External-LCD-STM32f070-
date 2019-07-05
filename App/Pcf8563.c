/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Pcf8563.c
 * 修改人：宋丙菊
 * 修改日期：2015.07.21
 * 描述：时钟读写
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "TypeDefine.h"
#include "App.h"
#include "Eeprom.h"
#include "Pcf8563.h"
#include "Lib_Communication.h"
#include "I2c.h"

/********************************************************************
 * Macros
 ********************************************************************/
#define PCF8563_Write  0xA2
#define PCF8563_Read   0xA3
#define PCF8563_Addr   0xA2

/********************************************************************
 * Constant
 ********************************************************************/

/********************************************************************
 * Local Types
 ********************************************************************/

/********************************************************************
 * Global Types
 ********************************************************************/
 
 /********************************************************************
  * Extern Global Types
  ********************************************************************/
 
/********************************************************************
 * Function
 *********************************************************************/

/********************************************************************
 * 函数名称：BCD码转换为十六进制数
 * 函数输入：BCD码表示的数
 * 函数返回值：十六进制数
 * 函数描述：将一个以BCD码表示的数转换为十六进制数
 ********************************************************************/
INT8U sBcdToHex(INT8U tBcd)
{
	INT8U tHex;
	
	tHex = (tBcd >> 4) * 10 + (tBcd & 0x0f);
	return tHex;
}
/********************************************************************
 * 函数名称：十六进制数转换为BCD码
 * 函数输入：十六进制数 
 * 函数返回值：BCD码表示的数
 * 函数描述：将一个十六进制数数转换为BCD码
 ********************************************************************/
INT8U sHexToBcd(INT8U tHex)
{
	INT8U tBcd;
	
	tBcd = ((tHex / 10) << 4);
	tBcd |= (tHex % 10);
	return tBcd;	
}

unsigned char I2c_Pcf8563write(unsigned char Addr,unsigned char Data)
{
	unsigned char Ack;

	I2c_start();
	I2c_wr8bit(PCF8563_Write);
	Ack = I2c_wait_ack();
	if(Ack == 1)
	{
		I2c_wr8bit(Addr);
		Ack = I2c_wait_ack();
		if(Ack == 1)
		{
			I2c_wr8bit(Data);
			Ack = I2c_wait_ack();
		}
	}	
	I2c_stop();
	return Ack;
}

unsigned char I2c_Pcf8563read(unsigned char Addr)
{
	unsigned char Ack = 0;
	unsigned char Data;
	I2c_start();
	I2c_wr8bit(PCF8563_Write);
	Ack = I2c_wait_ack();
	if(Ack == 1)
	{
		I2c_wr8bit(Addr);
		Ack = I2c_wait_ack();
		if(Ack == 1)
		{
			I2c_start();
			I2c_wr8bit(PCF8563_Read);
			Ack = I2c_wait_ack();
			if(Ack == 1)
			{
				Data=I2c_rd8bit(); 
				I2c_no_wait_ack();
			}
		}
	}	 
	
	I2c_stop();
	return Data;
}

void sWritePcf8563(INT8U *lubData,INT16U luwI2CAddr,INT8U lubDataLen)
{
	INT8U Len;
	
	for(Len = 0; Len < lubDataLen; Len++)
	{
		I2c_Pcf8563write(luwI2CAddr,sHexToBcd(lubData[Len]));
		luwI2CAddr--;
		if(luwI2CAddr == 0x06)
		{
			luwI2CAddr--;
		}
	}
}

void sReadPcf8563(INT8U* lubData)
{
	lubData[5] = sBcdToHex(I2c_Pcf8563read(Second_Addr) & 0x7f);
	lubData[4] = sBcdToHex(I2c_Pcf8563read(Minute_Addr) & 0x7f);
	lubData[3] = sBcdToHex(I2c_Pcf8563read(Hour_Addr) & 0x3f);
	lubData[2] = sBcdToHex(I2c_Pcf8563read(Date_Addr) & 0x3f);
	lubData[1] = sBcdToHex(I2c_Pcf8563read(Month_Addr) & 0x1f);
	lubData[0] = sBcdToHex(I2c_Pcf8563read(Year_Addr) & 0xff);
}

void sInitPcf8563(void)
{
	I2c_Pcf8563write(0x00,0x00);					//写控制寄存器1
	I2c_Pcf8563write(0x0d,0x00);					//禁止CLCKOUT输出
	I2c_Pcf8563write(0x0e,0x00);					//定时器无效
}

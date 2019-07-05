/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�Pcf8563.c
 * �޸��ˣ��α���
 * �޸����ڣ�2015.07.21
 * ������ʱ�Ӷ�д
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
 * �������ƣ�BCD��ת��Ϊʮ��������
 * �������룺BCD���ʾ����
 * ��������ֵ��ʮ��������
 * ������������һ����BCD���ʾ����ת��Ϊʮ��������
 ********************************************************************/
INT8U sBcdToHex(INT8U tBcd)
{
	INT8U tHex;
	
	tHex = (tBcd >> 4) * 10 + (tBcd & 0x0f);
	return tHex;
}
/********************************************************************
 * �������ƣ�ʮ��������ת��ΪBCD��
 * �������룺ʮ�������� 
 * ��������ֵ��BCD���ʾ����
 * ������������һ��ʮ����������ת��ΪBCD��
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
	I2c_Pcf8563write(0x00,0x00);					//д���ƼĴ���1
	I2c_Pcf8563write(0x0d,0x00);					//��ֹCLCKOUT���
	I2c_Pcf8563write(0x0e,0x00);					//��ʱ����Ч
}

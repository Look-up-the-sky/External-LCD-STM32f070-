/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�I2C.c
 * �޸��ˣ�����ǿ
 * �޸����ڣ�2019.06.20
 * ������I2C��д
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"
#include "Eeprom.h"
#include "Lib_Country.h"
#include "Lib_CRC.h"
#include "Lib_Communication.h"
#include "Lib_ClearRam.h"
#include "Lib_CalCoef.h"
#include "I2C.h"
#include "Lib_EnergyCal.h"
#include "Pcf8563.h"
#include "Variable_Transformation.h"

/********************************************************************
 * Global Types
 ********************************************************************/
IICFLAG IicFlag;

/********************************************************************
 * Functions
 *******************************************************************/
void sWriteInfoData(INT16U luwType);
void sReadInfoData(void);
void sWriteTime(void);
void sReadTime(void);
void sSetInitInfo(void);

/****************************************************************************
 * �������ƣ�I2C����
 * �������룺��
 * ��������ֵ����
 * ����������
 ****************************************************************************/
void sI2CTask(void)
{

	if(IicFlag.bit.mubTimeW ==1)
	{
		IicFlag.bit.mubTimeW = 0;
		IicFlag.bit.mubTimeR = 1;
		sWriteTime();
	}
	else
	{
		if(IicFlag.bit.mubTimeR ==1)
		{
			sReadTime();	
		}
	}
	if(IicFlag.bit.mubInfoW)
	{
		sWriteInfoData(0);
		sWriteInfoData(1);
		IicFlag.bit.mubInfoW = cFalse;
	}
	else if(IicFlag.bit.mubInfoR)
	{
		sReadInfoData();
		IicFlag.bit.mubInfoR = cFalse;
	}
	
}

/*****************************************************************************
 * �������ƣ�д������Ϣ����
 * �������룺��
 * ��������ֵ����
 * ������������������Ϣд��Eeprom����д����������¶�ȡ����У��ʧ�����û�����Ϣдʧ��
 * 			 ��־λ��Eeprom���ϱ�־λ��
 ****************************************************************************/
void sWriteInfoData(INT16U luwType)
{
	INFO ParamTemp;
	INT8U i;

	for(i = 0;i < 3;i++)
	{
		Info.data.uwCRC = fCrcCal8(Info.all,cInfoSize - 2);
		sWriteI2C(Info.all,cInfoAdrEE + luwType * cBackUpEE,cInfoSize);
		I2c_delayms(2);
		sReadI2C(ParamTemp.all,cInfoAdrEE + luwType * cBackUpEE,cInfoSize);
		if(ParamTemp.data.uwCRC == fCrcCal8(ParamTemp.all,cInfoSize - 2))
		{
			return;
		}
	}
	IicFlag.bit.mubInfoWFail = cTrue;
}

/*****************************************************************************
 * �������ƣ���������Ϣ����
 * �������룺��
 * ��������ֵ����
 * ������������ȡEeprom�еĻ�����Ϣ���������ͷ�������У�鲻ͨ���������sWriteInfoData()
 * 			 ������Ĭ�ϻ�����Ϣд��Eeprom�У������û�����Ϣ��ȡʧ�ܱ�־λ��
 ****************************************************************************/
void sReadInfoData(void)
{
	INT8U i;

	for(i = 0;i < 3;i++)
	{
		sReadI2C(Info.all,cInfoAdrEE,cInfoSize);
		if(Info.data.uwCRC == fCrcCal8(Info.all,cInfoSize - 2))
		{
			return;
		}
	}
	
	for(i = 0;i < 3;i++)
	{
		sReadI2C(Info.all,cInfoAdrBackUpEE,cInfoSize);
		if(Info.data.uwCRC == fCrcCal8(Info.all,cInfoSize - 2))
		{
			sWriteInfoData(0);
			return;
		}
	}

	sSetInitInfo();
	sWriteInfoData(0);
	sWriteInfoData(1);
	IicFlag.bit.mubInfoRFail = cTrue;
}

/*****************************************************************************
 * �������ƣ�дʱ�亯��
 * �������룺��
 * ��������ֵ����
 * ����������
 ****************************************************************************/
void sWriteTime(void)
{
	SYSTEMTIME ParamTemp;
	INT8U i;

	for(i = 0;i < 3;i++)
	{
		sWritePcf8563(SystemTime.all,Year_Addr,cTimeSize);
		I2c_delayms(1);
		sReadPcf8563(ParamTemp.all);
		if(SystemTime.data.ubYear == ParamTemp.data.ubYear && SystemTime.data.ubHour == ParamTemp.data.ubHour
		&& SystemTime.data.ubMonth == ParamTemp.data.ubMonth && SystemTime.data.ubDate == ParamTemp.data.ubDate)
		{
			return;
		}
	}
	IicFlag.bit.mubTimeWFail = cTrue;
}

/*****************************************************************************
 * �������ƣ���ʱ�亯��
 * �������룺��
 * ��������ֵ����
 * ����������
 ****************************************************************************/
void sReadTime(void)
{
	INT16U i;
	SYSTEMTIME ParamTemp;

	for(i = 0;i < 3;i++)
	{
		sReadPcf8563(ParamTemp.all);
		if(ParamTemp.data.ubMinute < 60 && ParamTemp.data.ubSecond < 60	&& ParamTemp.data.ubHour < 24 && ParamTemp.data.ubDate < 32
		&& ParamTemp.data.ubMonth < 13)
		{
			SystemTime = ParamTemp;
			return;
		}
	}
	
	SystemTime.data.ubMonth = 1;
	SystemTime.data.ubDate = 1;
	IicFlag.bit.mubTimeRFail = cTrue;
}

/*****************************************************************************
 * �������ƣ���ʼ��������Ϣ����
 * �������룺��
 * ��������ֵ����
 * ����������
 ****************************************************************************/
void sSetInitInfo(void)
{
	Info.data.ublanguage = cEnglish;
	Info.data.ubFirstTurnOn = cTrue;
	Info.data.ubPasswordH = 0x50;
	Info.data.ubPasswordM = 0x00;
	Info.data.ubPasswordL = 0x05;
}

/*****************************************************************************
 * �������ƣ���Eeporm�е��������ݺ���
 * �������룺��
 * ��������ֵ����
 * ��������������sReadInfoData()
 ****************************************************************************/
void sReadAllEepromData(void)
{	
	IicFlag.all = 0;
	sReadInfoData();
}

/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：I2C.c
 * 修改人：沈仕强
 * 修改日期：2019.06.20
 * 描述：I2C读写
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
 * 函数名称：I2C任务
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：
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
 * 函数名称：写机型信息函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：将机型信息写入Eeprom，将写入的数据重新读取，若校验失败则置机型信息写失败
 * 			 标志位和Eeprom故障标志位。
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
 * 函数名称：读机型信息函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：读取Eeprom中的机型信息，如果数据头错误或者校验不通过，则调用sWriteInfoData()
 * 			 函数将默认机型信息写入Eeprom中，并且置机型信息读取失败标志位。
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
 * 函数名称：写时间函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：
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
 * 函数名称：读时间函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：
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
 * 函数名称：初始化机器信息函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：
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
 * 函数名称：读Eeporm中的所有数据函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：调用sReadInfoData()
 ****************************************************************************/
void sReadAllEepromData(void)
{	
	IicFlag.all = 0;
	sReadInfoData();
}

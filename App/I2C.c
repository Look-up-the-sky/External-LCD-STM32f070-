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
 * Macros
 ********************************************************************/


/********************************************************************
 * Constant
 ********************************************************************/

/********************************************************************
 * Local Types
 ********************************************************************/
INT8U lubSystemDatePre;
INT8U lub1minCnt;

INT16U luwCountry;

/********************************************************************
 * Global Types
 ********************************************************************/
INT8U gubResetCnt;

INT16U guwGridVoltCoef[cPhaseNum];
INT16U guwGridCurrCoef;

IICFLAG IicFlag;

/********************************************************************
 * Extern Global Types
 ********************************************************************/


/********************************************************************
 * Functions
 *******************************************************************/
void sWriteSaftyData(INT16U luwType);
void sReadSaftyData(void);
void sWriteRatioData(INT16U luwType);
void sReadRatioData(void);
void sWriteEngyData(INT16U luwType);
void sReadEngyData(void);
void sWriteInfoData(INT16U luwType);
void sReadInfoData(void);
void sWriteErrorData(INT16U luwType);
void sReadErrorData(void);
void sWriteTime(void);
void sReadTime(void);
void sSaveErrorMessage(INT32U luwtErrorCode);
void sSetInitRatio(void);
void sSetInitSafty(void);
void sSetInitInfo(void);
void sInfoDataCheck(void);
void sCoefDataCheck(void);
void sSaftyDataCheck(void);

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
		sInfoDataCheck();
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
 * 函数名称：故障存储函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：故障时刻记录信息有：时间（年月日时分秒，故障码）
 ****************************************************************************/
void sSaveErrorMessage(INT32U luwtErrorCode)
{
	INT8U i;
	
	luwtErrorCode &= 0xEFFFFF93;

	if(luwtErrorCode != ((INT32U)ErrorRecord.data.ErrorMessage[0].data.ErrorCodeH << 16) + ErrorRecord.data.ErrorMessage[0].data.ErrorCodeL)
	{
		if(luwtErrorCode)
		{
			for(i = 14; i > 0; i--)
			{
				ErrorRecord.data.ErrorMessage[i] = ErrorRecord.data.ErrorMessage[i - 1];
			}
			ErrorRecord.data.ErrorMessage[0].data.Date = SystemTime.data.ubDate;
			ErrorRecord.data.ErrorMessage[0].data.Hour = SystemTime.data.ubHour;
			ErrorRecord.data.ErrorMessage[0].data.Minute = SystemTime.data.ubMinute;
			ErrorRecord.data.ErrorMessage[0].data.Second = SystemTime.data.ubSecond;
			ErrorRecord.data.ErrorMessage[0].data.Year = SystemTime.data.ubYear;
			ErrorRecord.data.ErrorMessage[0].data.Month = SystemTime.data.ubMonth;
			ErrorRecord.data.ErrorMessage[0].data.ErrorCodeH = luwtErrorCode >> 16;
			ErrorRecord.data.ErrorMessage[0].data.ErrorCodeL = luwtErrorCode;
			IicFlag.bit.mubErrorW = cTrue;
		}
	}
}

/*****************************************************************************
 * 函数名称：写安规函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：将安规保护值写入Eeprom，将写入的数据重新读取，若校验失败则置安规写失败
 * 			 标志位和Eeprom故障标志位。
 ****************************************************************************/
void sWriteSaftyData(INT16U luwType)
{
	SAFTY ParamTemp;
	INT8U i;
	
	if(luwCountry != Safty.data.uwCountry)										//并网国家值被改变
	{
		sResetProtect(Safty.data.uwCountry);									//重设并网参数
		luwCountry = Safty.data.uwCountry;
	}

	for(i = 0;i < 3;i++)
	{
		Safty.data.uwCRC = fCrcCal8(Safty.all,cSaftySize - 2);
		sWriteI2C(Safty.all,cSaftyAdrEE + luwType * cBackUpEE,cSaftySize);
		I2c_delayms(2);
		sReadI2C(ParamTemp.all,cSaftyAdrEE + luwType * cBackUpEE,cSaftySize);
		if(ParamTemp.data.uwCRC == fCrcCal8(ParamTemp.all,cSaftySize - 2))
		{
			return;
		}
	}
		
	IicFlag.bit.mubSaftyWFail = cTrue;
//	Error.bit.mubEepromFault = cTrue;
}

/*****************************************************************************
 * 函数名称：读安规函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：读取Eeprom中的安规数据，如果数据头错误或者校验不通过，则调用sWriteSaftyData()
 * 			 函数将默认安规写入Eeprom中，并且置安规读取失败标志位。
 ****************************************************************************/
void sReadSaftyData(void)
{
	INT8U i;

	for(i = 0;i < 3;i++)
	{
		sReadI2C(Safty.all,cSaftyAdrEE,cSaftySize);
		if(Safty.data.uwCRC == fCrcCal8(Safty.all,cSaftySize - 2))
		{
			return;
		}
	}
	
	for(i = 0;i < 3;i++)
	{
		sReadI2C(Safty.all,cSaftyAdrBackUpEE,cSaftySize);
		if(Safty.data.uwCRC == fCrcCal8(Safty.all,cSaftySize - 2))
		{
			sWriteSaftyData(0);
			return;
		}
	}

	sSetInitSafty();
	sWriteSaftyData(0);
	sWriteSaftyData(1);
	IicFlag.bit.mubSaftyRFail = cTrue;
}

/*****************************************************************************
 * 函数名称：写矫正系数函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：将保护系数写入Eeprom，将写入的数据重新读取，若校验失败则置校验系数写失败
 * 			 标志位和Eeprom故障标志位。
 ****************************************************************************/
void sWriteRatioData(INT16U luwType)
{
	COEF ParamTemp;
	INT8U i;

	for(i = 0;i < 3;i++)
	{
		Coef.data.uwCRC = fCrcCal8(Coef.all,cCoefSize - 2);
		sWriteI2C(Coef.all,cCoefAdrEE + luwType * cBackUpEE,cCoefSize);
		I2c_delayms(2);
		sReadI2C(ParamTemp.all,cCoefAdrEE + luwType * cBackUpEE,cCoefSize);
		if(ParamTemp.data.uwCRC == fCrcCal8(ParamTemp.all,cCoefSize - 2))
		{
			return;
		}
	}
	IicFlag.bit.mubRatioWFail = cTrue;
//	Error.bit.mubEepromFault = cTrue;
}

/*****************************************************************************
 * 函数名称：读校验系数函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：读取Eeprom中的校验系数，如果数据头错误或者校验不通过，则调用sWriteRatioData()
 * 			 函数将默认校验系数写入Eeprom中，并且置校验系数读取失败标志位。
 ****************************************************************************/
void sReadRatioData(void)
{
	INT8U i;

	for(i = 0;i < 3;i++)
	{
		sReadI2C(Coef.all,cCoefAdrEE,cCoefSize);
		if(Coef.data.uwCRC == fCrcCal8(Coef.all,cCoefSize - 2))
		{
			return;
		}
	}
	
	for(i = 0;i < 3;i++)
	{
		sReadI2C(Coef.all,cCoefAdrBackUpEE,cCoefSize);
		if(Coef.data.uwCRC == fCrcCal8(Coef.all,cCoefSize - 2))
		{
			sWriteRatioData(0);
			return;
		}
	}

	sSetInitRatio();
	sWriteRatioData(0);
	sWriteRatioData(1);
	IicFlag.bit.mubRatioRFail = cTrue;
}

/*****************************************************************************
 * 函数名称：写发电量函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：将发电量写入Eeprom，将写入的数据重新读取，若校验失败则置发电量写失败
 * 			 标志位和Eeprom故障标志位。
 ****************************************************************************/
void sWriteEngyData(INT16U luwType)
{
	ENERGY ParamTemp;
	INT8U i;

	for(i = 0;i < 3;i++)
	{
		Energy.data.uwCRC = fCrcCal8(Energy.all,cEnergySize - 2);
		sWriteI2C(Energy.all,cEngyAdrEE + luwType * cBackUpEE,cEnergySize);
		I2c_delayms(2);
		sReadI2C(ParamTemp.all,cEngyAdrEE + luwType * cBackUpEE,cEnergySize);
		if(ParamTemp.data.uwCRC == fCrcCal8(ParamTemp.all,cEnergySize - 2))
		{
			return;
		}
	}
	IicFlag.bit.mubEngyWFail = cTrue;
//	Error.bit.mubEepromFault = cTrue;
}

/*****************************************************************************
 * 函数名称：读发电量函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：读取Eeprom中的发电量，如果数据头错误或者校验不通过，则调用sWriteEngyData()
 * 			 函数将Eeprom中的发电量数据清0，并且置校验系数读取失败标志位。
 ****************************************************************************/
void sReadEngyData(void)
{
	INT8U i;

	for(i = 0;i < 3;i++)
	{
		sReadI2C(Energy.all,cEngyAdrEE,cEnergySize);
		if(Energy.data.uwCRC == fCrcCal8(Energy.all,cEnergySize - 2))
		{
			return;
		}
	}
	
	for(i = 0;i < 3;i++)
	{
		sReadI2C(Energy.all,cEngyAdrBackUpEE,cEnergySize);
		if(Energy.data.uwCRC == fCrcCal8(Energy.all,cEnergySize - 2))
		{
			sWriteEngyData(0);
			return;
		}
	}

	sClearRamByte(Energy.all,cEnergySize);
	sWriteEngyData(0);
	sWriteEngyData(1);
	IicFlag.bit.mubEngyRFail = cTrue;
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
//	Error.bit.mubEepromFault = cTrue;
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
 * 函数名称：写故障信息函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：将故障信息写入Eeprom，将写入的数据重新读取，若校验失败则置故障信息写失败
 * 			 标志位和Eeprom故障标志位。
 ****************************************************************************/
void sWriteErrorData(INT16U luwType)
{
	ERRORRECORD ParamTemp;
	INT8U i;

	for(i = 0;i < 3;i++)
	{
		ErrorRecord.data.uwCRC = fCrcCal8(ErrorRecord.all,cErrorRecordSize - 2);
		sWriteI2C(ErrorRecord.all,cErrorAdrEE + luwType * cBackUpEE,cErrorRecordSize);
		I2c_delayms(5);
		sReadI2C(ParamTemp.all,cErrorAdrEE + luwType * cBackUpEE,cErrorRecordSize);
		if(ParamTemp.data.uwCRC == fCrcCal8(ParamTemp.all,cErrorRecordSize - 2))
		{
			return;
		}
	}
	IicFlag.bit.mubErrorWFail = cTrue;
//	Error.bit.mubEepromFault = cTrue;
}

/*****************************************************************************
 * 函数名称：读故障信息函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：读取Eeprom中的故障信息，如果数据头错误或者校验不通过，则调用sWriteErrorData()
 * 			 函数清除Eeprom中的故障信息，并且置故障信息读取失败标志位。
 ****************************************************************************/
void sReadErrorData(void)
{
	INT16U i;

	for(i = 0;i < 3;i++)
	{
		sReadI2C(ErrorRecord.all,cErrorAdrEE,cErrorRecordSize);
		if(ErrorRecord.data.uwCRC == fCrcCal8(ErrorRecord.all,cErrorRecordSize - 2))
		{
			return;
		}
	}
	
	for(i = 0;i < 3;i++)
	{
		sReadI2C(ErrorRecord.all,cErrorAdrBackUpEE,cErrorRecordSize);
		if(ErrorRecord.data.uwCRC == fCrcCal8(ErrorRecord.all,cErrorRecordSize - 2))
		{
			sWriteErrorData(0);
			return;
		}
	}

	sClearRamByte(ErrorRecord.all,cErrorRecordSize);
	sWriteErrorData(0);
	sWriteErrorData(1);
	IicFlag.bit.mubErrorRFail = cTrue;
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
	INT8U i;
	Info.data.ubManufacture[0] = 'O';
	Info.data.ubManufacture[1] = 'm';
	Info.data.ubManufacture[2] = 'n';
	Info.data.ubManufacture[3] = 'i';
	Info.data.ubManufacture[4] = 'k';
	for(i = 5;i < 32;i++)
	{
		Info.data.ubManufacture[i] = ' ';
	}
	Info.data.uwMaxCurrent = cMaxInvCurr;
if(PRJ_NUMBER == SUNNYBEE)
{
	Info.data.uwRatedPower = 1500;
	Info.data.uwkWModel = 1500;
	Info.data.ubInputNumber = 1;
}
else if(PRJ_NUMBER == HORNET)
{
	Info.data.uwRatedPower = 3000;
	Info.data.uwkWModel = 3000;
	Info.data.ubInputNumber = 2;
}
else
{
	Info.data.uwRatedPower = 3000;
	Info.data.uwkWModel = 3000;
	Info.data.ubInputNumber = 1;
}

if(PRJ_NUMBER == SUNNYBEE )
	Info.data.ubGeneration = 2;
else
	Info.data.ubGeneration = 3;

	Info.data.ublanguage = cEnglish;
	Info.data.ubFirstTurnOn = cTrue;
	Info.data.ubPasswordH = 0x50;
	Info.data.ubPasswordM = 0x00;
	Info.data.ubPasswordL = 0x05;
}

/*****************************************************************************
 * 函数名称：初始化保护功能函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：
 ****************************************************************************/
void sSetInitSafty(void)
{
	Safty.data.uwCountry = cGermany;
	sResetProtect(cGermany);
if(PRJ_NUMBER == SUNNYBEE)
{
	Safty.data.uwPVStartVolt = c250V;
	Safty.data.uwTemperatureStart = c90Degree;
	Safty.data.uwTemperatureEnd = c100Degree;
}
else if(PRJ_NUMBER == HORNET)
{
	Safty.data.uwPVStartVolt = c250V;
	Safty.data.uwTemperatureStart = c80Degree;
	Safty.data.uwTemperatureEnd = c90Degree;
}
else
{
	Safty.data.uwPVStartVolt = c150V;
	Safty.data.uwTemperatureStart = c85Degree;
	Safty.data.uwTemperatureEnd = c95Degree;
}
	Safty.data.ubConstCurrEnable = 0x55;
	Safty.data.ubWorkEnable = 0xAA;
	Safty.data.uwScanMpptPeriod = 240;
	Safty.data.uw10minVoltHigh = c253V;
	Safty.data.ubPowerPerMinute = 10;
	Safty.data.ubPfDelay = 0;
	Safty.data.ubQvDelay = 5;
}

/*****************************************************************************
 * 函数名称：初始化系数函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：将所有系数初始化为1000。
 * 			1000 = 0x03E8
 ****************************************************************************/
void sSetInitRatio(void)
{
	INT8U i;
	
	for(i = 0;i < cCoefSize / 2;i++)
	{
		Coef.all[2 * i] = 0xE8;
		Coef.all[2 * i + 1] = 0x03;
	}
}

/*****************************************************************************
 * 函数名称：根据国家设置保护功能函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：
 ****************************************************************************/
void sResetProtect(INT16U luwCountry)
{
	INT8U i;
	
	if(luwCountry >= cMaxSaftyCode)
	{
		luwCountry = cGermany;
		Safty.data.uwCountry = luwCountry;
	}

	for(i = 0;i < 30;i++)
	{
		Safty.all[2 * i + 2] = mSafetyLawDefaultData[luwCountry][i];
		Safty.all[2 * i + 3] = mSafetyLawDefaultData[luwCountry][i] >> 8;
	}
}

/*****************************************************************************
 * 函数名称：逆变器信息有效性检测函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：
 ****************************************************************************/
void sInfoDataCheck(void)
{
	INT16U luwValue;
	
	luwValue = cMaxInvCurr;
	if(Info.data.uwMaxCurrent > luwValue)
	{
		Info.data.uwMaxCurrent = luwValue;
	}
	
	if(Info.data.ubInputNumber > 2 || Info.data.ubInputNumber == 0)
	{
		Info.data.ubInputNumber = 2;
	}

if(PRJ_NUMBER == SUNNYBEE)
{
	if(Info.data.uwkWModel != 4000 && Info.data.uwkWModel != 6000 && Info.data.uwkWModel != 5000)
	{
		Info.data.uwkWModel = 5000;
	}
}
else if(PRJ_NUMBER == HORNET)
{
	if(Info.data.uwkWModel != 15000 && Info.data.uwkWModel != 13000 && Info.data.uwkWModel != 12000 && Info.data.uwkWModel != 10000 && Info.data.uwkWModel != 8000)
	{
		Info.data.uwkWModel = 10000;
	}
}
else
{
	if(Info.data.uwkWModel != 3000 && Info.data.uwkWModel != 2500 && Info.data.uwkWModel != 2000)
	{
		Info.data.uwkWModel = 2000;
	}
}


	if(Info.data.uwRatedPower > Info.data.uwkWModel)
	{
		Info.data.uwRatedPower = Info.data.uwkWModel;
	}
}

/*****************************************************************************
 * 函数名称：系数有效性检测函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：
 ****************************************************************************/
void sCoefDataCheck(void)
{
	INT8U i;
	INT16U luwCoef;
	
	for(i = 0;i < cCoefSize / 2 - 1;i++)
	{
		luwCoef = (Coef.all[i * 2 + 1] << 8) + Coef.all[i * 2];
		if(luwCoef > cCoefMax || luwCoef < cCoefMin)
		{
			Coef.all[2 * i] = cCoefDef & 0xFF;
			Coef.all[2 * i + 1] = cCoefDef >> 8;
			IicFlag.bit.mubRatioW = cTrue;
		}
	}
}

/*****************************************************************************
 * 函数名称：系数有效性检测函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：
 ****************************************************************************/
void sSaftyDataCheck(void)
{
	INT8U i;
	INT16U luwValueMax,luwValueMin;
	
	if(Safty.data.uwCountry >= cMaxSaftyCode)
	{
		Safty.data.uwCountry = cGermany;
		for(i = 0;i < 30;i++)
		{
			Safty.all[i * 2 + 2] = mSafetyLawDefaultData[cGermany][i];
			Safty.all[i * 2 + 3] = mSafetyLawDefaultData[cGermany][i] >> 8;
		}
	}
	else
	{
		if(Safty.data.uwStartDelayTime < 10)
		{
			Safty.data.uwStartDelayTime = 10;
		}
		else if(Safty.data.uwStartDelayTime > 300)
		{
			Safty.data.uwStartDelayTime = 300;
		}
		
		/*************电压限制**************/
		if(Safty.data.uwGridVoltHigh[0] < c240V)
		{
			Safty.data.uwGridVoltHigh[0] = c240V;
		}
		else if(Safty.data.uwGridVoltHigh[0] > c280V)
		{
			Safty.data.uwGridVoltHigh[0] = c280V;
		}
		
		if(Safty.data.uwGridVoltHigh[1] < Safty.data.uwGridVoltHigh[0])
		{
			Safty.data.uwGridVoltHigh[1] = Safty.data.uwGridVoltHigh[0];
		}
		else if(Safty.data.uwGridVoltHigh[1] > c280V)
		{
			Safty.data.uwGridVoltHigh[1] = c280V;
		}
		
		if(Safty.data.uwGridVoltHighCnt[0] < 6)
		{
			Safty.data.uwGridVoltHighCnt[0] = 6;
		}
		if(Safty.data.uwGridVoltHighCnt[1] < 2)
		{
			Safty.data.uwGridVoltHighCnt[1] = 2;
		}
		
		if(Safty.data.uwGridVoltLow[0] < c110V)
		{
			Safty.data.uwGridVoltLow[0] = c110V;
		}
		else if(Safty.data.uwGridVoltLow[0] > c220V)
		{
			Safty.data.uwGridVoltLow[0] = c220V;
		}
		if(Safty.data.uwGridVoltLow[1] < c80V)
		{
			Safty.data.uwGridVoltLow[1] = c80V;
		}
		else if(Safty.data.uwGridVoltLow[1] > Safty.data.uwGridVoltLow[0])
		{
			Safty.data.uwGridVoltLow[1] = Safty.data.uwGridVoltLow[0];
		}
		
		if(Safty.data.uwGridVoltLowCnt[0] < 6)
		{
			Safty.data.uwGridVoltLowCnt[0] = 6;
		}
		if(Safty.data.uwGridVoltLowCnt[1] < 6)
		{
			Safty.data.uwGridVoltLowCnt[1] = 6;
		}
		
		if(Safty.data.uwGridVoltBackHigh < c230V)
		{
			Safty.data.uwGridVoltBackHigh = c230V;
		}
		else if(Safty.data.uwGridVoltBackHigh > Safty.data.uwGridVoltHigh[0])
		{
			Safty.data.uwGridVoltBackHigh = Safty.data.uwGridVoltHigh[0];
		}

		if(Safty.data.uwGridVoltBackLow < Safty.data.uwGridVoltLow[0])
		{
			Safty.data.uwGridVoltBackLow = Safty.data.uwGridVoltLow[0];
		}
		else if(Safty.data.uwGridVoltBackLow > c230V)
		{
			Safty.data.uwGridVoltBackLow = c230V;
		}
		
		if(Safty.data.uwGridVoltBackCnt < 20)
		{
			Safty.data.uwGridVoltBackCnt = 20;
		}
		else if(Safty.data.uwGridVoltBackCnt > 60000)
		{
			Safty.data.uwGridVoltBackCnt = 60000;
		}
		
		/*************频率限制**************/
		if(Safty.data.Protection.bit.ub60Hz)
		{
			luwValueMax = c65Hz;
			luwValueMin = c60Hz1;
		}
		else
		{
			luwValueMax = c55Hz;
			luwValueMin = c50Hz1;
		}
		
		if(Safty.data.uwFreqHigh[0] < luwValueMin)
		{
			Safty.data.uwFreqHigh[0] = luwValueMin;
		}
		else if(Safty.data.uwFreqHigh[0] > luwValueMax)
		{
			Safty.data.uwFreqHigh[0] = luwValueMax;
		}
		
		if(Safty.data.uwFreqHigh[1] < Safty.data.uwFreqHigh[0])
		{
			Safty.data.uwFreqHigh[1] = Safty.data.uwFreqHigh[0];
		}
		else if(Safty.data.uwFreqHigh[1] > luwValueMax)
		{
			Safty.data.uwFreqHigh[1] = luwValueMax;
		}
		
		if(Safty.data.uwFreqHighCnt[0] < 6)
		{
			Safty.data.uwFreqHighCnt[0] = 6;
		}
		if(Safty.data.uwFreqHighCnt[1] < 6)
		{
			Safty.data.uwFreqHighCnt[1] = 6;
		}
		
		if(Safty.data.Protection.bit.ub60Hz)
		{
			luwValueMax = c59Hz9;
			luwValueMin = c55Hz;
		}
		else
		{
			luwValueMax = c49Hz9;
			luwValueMin = c45Hz;
		}
		if(Safty.data.uwFreqLow[0] < luwValueMin)
		{
			Safty.data.uwFreqLow[0] = luwValueMin;
		}
		else if(Safty.data.uwFreqLow[0] > luwValueMax)
		{
			Safty.data.uwFreqLow[0] = luwValueMax;
		}
		if(Safty.data.uwFreqLow[1] < luwValueMin)
		{
			Safty.data.uwFreqLow[1] = luwValueMin;
		}
		else if(Safty.data.uwFreqLow[1] > Safty.data.uwFreqLow[0])
		{
			Safty.data.uwFreqLow[1] = Safty.data.uwFreqLow[0];
		}
		
		if(Safty.data.uwFreqLowCnt[0] < 6)
		{
			Safty.data.uwFreqLowCnt[0] = 6;
		}
		if(Safty.data.uwFreqLowCnt[1] < 6)
		{
			Safty.data.uwFreqLowCnt[1] = 6;
		}
		
		if(Safty.data.Protection.bit.ub60Hz)
		{
			luwValueMax = c59Hz9;
			luwValueMin = c60Hz1;
		}
		else
		{
			luwValueMax = c49Hz9;
			luwValueMin = c50Hz03;
		}
		if(Safty.data.uwFreqBackHigh < luwValueMin)
		{
			Safty.data.uwFreqBackHigh = luwValueMin;
		}
		else if(Safty.data.uwFreqBackHigh > Safty.data.uwFreqHigh[0])
		{
			Safty.data.uwFreqBackHigh = Safty.data.uwFreqHigh[0];
		}

		if(Safty.data.uwFreqBackLow < Safty.data.uwFreqLow[0])
		{
			Safty.data.uwFreqBackLow = Safty.data.uwFreqLow[0];
		}
		else if(Safty.data.uwFreqBackLow > luwValueMax)
		{
			Safty.data.uwFreqBackLow = luwValueMax;
		}
		
		if(Safty.data.uwFreqBackCnt < 20)
		{
			Safty.data.uwFreqBackCnt = 20;
		}
		else if(Safty.data.uwFreqBackCnt > 60000)
		{
			Safty.data.uwFreqBackCnt = 60000;
		}
		
		/*************直流分量限制**************/
		if(Safty.data.swDCILimit[0] < 20)
		{
			Safty.data.swDCILimit[0] = 20;
		}
		else if(Safty.data.swDCILimit[0] > 1000)
		{
			Safty.data.swDCILimit[0] = 1000;
		}
		
		if(Safty.data.swDCILimit[1] < Safty.data.swDCILimit[0])
		{
			Safty.data.swDCILimit[1] = Safty.data.swDCILimit[0];
		}
		else if(Safty.data.swDCILimit[1] > 1000)
		{
			Safty.data.swDCILimit[1] = 1000;
		}
		
		if(Safty.data.swDCICnt[0] < 6)
		{
			Safty.data.swDCICnt[0] = 6;
		}
		if(Safty.data.swDCICnt[1] < 6)
		{
			Safty.data.swDCICnt[1] = 6;
		}
		
		if(Safty.data.swDCIBack < 20)
		{
			Safty.data.swDCIBack = 20;
		}
		else if(Safty.data.swDCIBack > Safty.data.swDCILimit[0])
		{
			Safty.data.swDCIBack = Safty.data.swDCILimit[0];
		}
		
		if(Safty.data.uwDCIBackCnt < 20)
		{
			Safty.data.uwDCIBackCnt = 20;
		}
		else if(Safty.data.uwDCIBackCnt > 600)
		{
			Safty.data.uwDCIBackCnt = 600;
		}
	}
	
	/*************启动电压限制**************/
#if(PRJ_NUMBER == SUNNYBEE)
	Safty.data.uwPVStartVolt = c250V;
#elif(PRJ_NUMBER == HORNET)
	Safty.data.uwPVStartVolt = c250V;
#else
	Safty.data.uwPVStartVolt = c150V;
#endif
	
	/*************MPP扫描周期限制**************/
	if(Safty.data.uwScanMpptPeriod > 240)
	{
		Safty.data.uwScanMpptPeriod = 240;
	}
	else if(Safty.data.uwScanMpptPeriod < 20)
	{
		Safty.data.uwScanMpptPeriod = 20;
	}
	
	/*************过温降载点限制**************/
	luwValueMax = c110Degree;
	luwValueMin = c90Degree;
	if(Safty.data.uwTemperatureEnd > luwValueMax)
	{
		Safty.data.uwTemperatureEnd = luwValueMax;
	}
	else if(Safty.data.uwTemperatureEnd < luwValueMin)
	{
		Safty.data.uwTemperatureEnd = luwValueMin;
	}
	
	luwValueMax = Safty.data.uwTemperatureEnd - c10Degree;
	luwValueMin = c80Degree;
	if(Safty.data.uwTemperatureStart < luwValueMin)
	{
		Safty.data.uwTemperatureStart = luwValueMin;
	}
	else if(Safty.data.uwTemperatureStart > luwValueMax)
	{
		Safty.data.uwTemperatureStart = luwValueMax;
	}
	
	if(Safty.data.uw10minVoltHigh > c280V)
	{
		Safty.data.uw10minVoltHigh  = c280V;
	}
	else if(Safty.data.uw10minVoltHigh < c240V)
	{
		Safty.data.uw10minVoltHigh = c240V;
	}
	
	if(Safty.data.ubPowerPerMinute > 20)
	{
		Safty.data.ubPowerPerMinute = 20;
	}
	else if(Safty.data.ubPowerPerMinute < 5)
	{
		Safty.data.ubPowerPerMinute = 5;
	}
	
	if(Safty.data.uwCountry == cGermany)
	{
		Safty.data.uwSoftStartType.bit.ubFreqHigh = 1;
	}
	else
	{
		Safty.data.uwSoftStartType.Word = 0x0F;
	}
	
	if(Safty.data.ubPfDelay > 200)
	{
		Safty.data.ubPfDelay = 200;
	}
	if(Safty.data.ubQvDelay > 200)
	{
		Safty.data.ubQvDelay = 200;	
	}
}

/*****************************************************************************
 * 函数名称：读Eeporm中的所有数据函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：调用sReadEngyData()、sReadErrorData()、sReadInfoData()、sReadRatioData()
 * 			 、sReadSaftyData()和sReadTime()函数读取Eeporm中的数据
 ****************************************************************************/
void sReadAllEepromData(void)
{
	
	IicFlag.all = 0;
	sReadInfoData();
	sInfoDataCheck();
}

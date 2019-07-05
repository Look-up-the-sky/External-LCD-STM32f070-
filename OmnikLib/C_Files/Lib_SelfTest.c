/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Lib_SelfTest.c
 * 修改人：宋丙菊
 * 修改日期：2014.12.15
 * 描述：根据上位机下发的命令进行电压、频率自检测试
 * 修改内容：
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"
#include "Lib_GridVoltProtect.h"
#include "Lib_GridFreqProtect.h"
#include "Lib_Communication.h"
#include "Lib_SelfTest.h"

/********************************************************************
 * Macros
 ********************************************************************/
#define cNoSelfTest		0
#define cLimitRecover	1
#define cVoltHigh1		2
#define cVoltHigh2		3
#define cVoltLow1		4
#define cVoltLow2		5
#define cFreqHigh1		6
#define cFreqHigh2		7
#define cFreqLow1		8
#define cFreqLow2		9
#define cVoltHigh1Fast	12
#define cVoltHigh2Fast	13
#define cVoltLow1Fast	14
#define cVoltLow2Fast	15
#define cFreqHigh1Fast	16
#define cFreqHigh2Fast	17
#define cFreqLow1Fast	18
#define cFreqLow2Fast	19

/********************************************************************
 * Local Types
 ********************************************************************/
typedef union
{
	INT16U all;
	struct
	{
		INT16U ubVoltFaultStart:1;
		INT16U ubFreqFaultStart:1;
		INT16U ubSaveValue:1;
		INT16U ubReserve:13;
	}bit;
}SELFTESTFLAG;

INT8U lubSelfTestCount;
INT8U lubSelfTestpre;
INT8U lubStepPre;
INT8U lubDisplay;
INT8U lubSelfTestStep;

INT16U luw1msCnt;
INT16U luwDataSave[2];

float lfVoltTemp[4];
float lfFreqTemp[4];
float lfFaultValue;

SELFTESTFLAG SelfTestFlag;

/********************************************************************
 * Global Types
 ********************************************************************/
#ifdef FloatEnable
#else
INT16U guwFreqTemp;
#endif

#ifdef FloatEnable
/**************************************************************************
*函数功能:自检控制函数
*函数输入:自检标志位
*函数返回值:无
*函数描述:bSelfTestCmd为0代表不进行自检;bSelfTestCmd为1、11代表自检准备,清
*		  电网故障;bSelfTestCmd为2-9代表相应的故障自检;bSelfTestCmd为12
*		 -19代表快速故障自检;bSelfTestCmd为10、20代表自检结束,清故障检测数据；
*************************************************************************/
void sSelfTestControl(INT8U lubSelfTest,INT8U lubState)
{
	float lfTemp;

	if(lubSelfTest == 0)										//不进行自检
	{
	}
	else if(lubSelfTest == 2 || lubSelfTest == 12)				//over voltage self test		过压1
	{
		if(lubSelfTestpre != lubSelfTest)
		{
			lfVoltTemp[cHigh1] = sGetVoltLimit(cHigh1);			//存储过压值
		}

		if(lubState == cNormalState)
		{
			lubSelfTestCount++;
			if(lubSelfTestCount >= 10)
			{
				lfTemp = sGetVoltLimit(cHigh1);
				lfTemp -= c1V;
				sSetVoltLimit(cHigh1,lfTemp);					//the threshold voltage descrease 1V/200ms
				lubSelfTestCount = 0;
			}
		}
	}
	else if(lubSelfTest == 3 || lubSelfTest == 13)				//over voltage self test	过压2
	{
		if(lubSelfTestpre != lubSelfTest)
		{
			lfVoltTemp[cHigh2] = sGetVoltLimit(cHigh2);			//存储过压值
		}

		if(lubState == cNormalState)
		{
			lubSelfTestCount++;
			if(lubSelfTestCount >= 10)
			{
				lfTemp = sGetVoltLimit(cHigh2);
				lfTemp -= c1V;
				sSetVoltLimit(cHigh2,lfTemp);					//the threshold voltage descrease 1V/200ms
				lubSelfTestCount = 0;
			}
		}
	}
	else if(lubSelfTest == 4 || lubSelfTest == 14)				//over voltage self test	欠压1
	{
		if(lubSelfTestpre != lubSelfTest)
		{
			lfVoltTemp[cLow1] = sGetVoltLimit(cLow1);			//存储欠压值
		}

		if(lubState == cNormalState)
		{
			lubSelfTestCount++;
			if(lubSelfTestCount >= 10)
			{
				lfTemp = sGetVoltLimit(cLow1);
				lfTemp += c1V;
				sSetVoltLimit(cLow1,lfTemp);					//the threshold voltage descrease 1V/200ms
				lubSelfTestCount = 0;
			}
		}
	}
	else if(lubSelfTest == 5 || lubSelfTest == 15)				//over voltage self test	欠压2
	{
		if(lubSelfTestpre != lubSelfTest)
		{
			lfVoltTemp[cLow2] = sGetVoltLimit(cLow2);			//存储欠压值
		}

		if(lubState == cNormalState)
		{
			lubSelfTestCount++;
			if(lubSelfTestCount >= 10)
			{
				lfTemp = sGetVoltLimit(cLow2);
				lfTemp += c1V;
				sSetVoltLimit(cLow2,lfTemp);					//the threshold voltage descrease 1V/200ms
				lubSelfTestCount = 0;
			}
		}
	}
	else if(lubSelfTest == 6 || lubSelfTest == 16)				//over voltage self test	过频1
	{
		if(lubSelfTestpre != lubSelfTest)
		{
			lfFreqTemp[cHigh1] = sGetFreqLimit(cHigh1);			//存储过频值
		}

		if(lubState == cNormalState)
		{
			lubSelfTestCount++;
			if(lubSelfTestCount >= 20)
			{
				lfTemp = sGetFreqLimit(cHigh1);
				lfTemp -= c0Hz1;								//Over Freq self test		0.01Hz/20ms
				sSetFreqLimit(cHigh1,lfTemp);
				lubSelfTestCount = 0;
			}
		}
	}
	else if(lubSelfTest == 7 || lubSelfTest == 17)				//over voltage self test	过频2
	{
		if(lubSelfTestpre != lubSelfTest)
		{
			lfFreqTemp[cHigh2] = sGetFreqLimit(cHigh2);			//存储过频值
		}

		if(lubState == cNormalState)
		{
			lubSelfTestCount++;
			if(lubSelfTestCount >= 20)
			{
				lfTemp = sGetFreqLimit(cHigh2);
				lfTemp -= c0Hz1;								//Over Freq self test		0.01Hz/20ms
				sSetFreqLimit(cHigh2,lfTemp);
				lubSelfTestCount = 0;
			}
		}
	}
	else if(lubSelfTest == 8 || lubSelfTest == 18)				//over voltage self test	欠频1
	{
		if(lubSelfTestpre != lubSelfTest)
		{
			lfFreqTemp[cLow1] = sGetFreqLimit(cLow1);			//存储欠频值
		}

		if(lubState == cNormalState)
		{
			lubSelfTestCount++;
			if(lubSelfTestCount >= 20)
			{
				lfTemp = sGetFreqLimit(cLow1);
				lfTemp += c0Hz1;								//Low Freq self test		0.01Hz/20ms
				sSetFreqLimit(cLow1,lfTemp);
				lubSelfTestCount = 0;
			}
		}
	}
	else if(lubSelfTest == 9 || lubSelfTest == 19)				//over voltage self test	欠频2
	{
		if(lubSelfTestpre != lubSelfTest)
		{
			lfFreqTemp[cLow2] = sGetFreqLimit(cLow2);			//存储欠频值
		}

		if(lubState == cNormalState)
		{
			lubSelfTestCount++;
			if(lubSelfTestCount >= 20)
			{
				lfTemp = sGetFreqLimit(cLow2);
				lfTemp += c0Hz1;								//Low Freq self test		0.01Hz/20ms
				sSetFreqLimit(cLow2,lfTemp);
				lubSelfTestCount = 0;
			}
		}
	}
	else														//自检结束，恢复保护值
	{
		if(lubSelfTestpre >= 2 && lubSelfTestpre <= 5)
		{
			sSetVoltLimit(lubSelfTestpre - 2,lfVoltTemp[lubSelfTestpre - 2]);
		}
		else if(lubSelfTestpre >= 12 && lubSelfTestpre <= 15)
		{
			sSetVoltLimit(lubSelfTestpre - 12,lfVoltTemp[lubSelfTestpre - 12]);
		}
		else if(lubSelfTestpre >= 6 && lubSelfTestpre <= 9)
		{
			sSetFreqLimit(lubSelfTestpre - 6,lfFreqTemp[lubSelfTestpre - 6]);
		}
		else if(lubSelfTestpre >= 16 && lubSelfTestpre <= 19)
		{
			sSetFreqLimit(lubSelfTestpre - 16,lfFreqTemp[lubSelfTestpre - 16]);
		}

		lfFaultValue = 0.0f;
	}

	lubSelfTestpre = lubSelfTest;
}

void sSetSelfTestFlag(INT8U lubVoltorFreq)
{
	if(lubVoltorFreq)
	{
		SelfTestFlag.bit.ubVoltFaultStart = 1;
	}
	else
	{
		SelfTestFlag.bit.ubFreqFaultStart = 1;
	}
}

void sClearSelfTestFlag(INT8U lubVoltorFreq)
{
	if(lubVoltorFreq)
	{
		SelfTestFlag.bit.ubVoltFaultStart = 0;
	}
	else
	{
		SelfTestFlag.bit.ubFreqFaultStart = 0;
	}
}

INT16U sGetFaultDelay(INT8U lubSelfTest,INT8U lubState,float lfGridVolt,float lfGridFreq)
{
	if(lubSelfTest != 0)
	{
		if(lubState == cNormalState)
		{
			if(SelfTestFlag.bit.ubVoltFaultStart || SelfTestFlag.bit.ubFreqFaultStart)
			{
				luw1msCnt++;
			}
			else
			{
				luw1msCnt = 0;
			}
			SelfTestFlag.bit.ubSaveValue = 0;
			return(0);
		}
		else
		{
			if((lubSelfTest >= 2 && lubSelfTest <= 5) || (lubSelfTest >= 12 && lubSelfTest <= 15))
			{
				if(lubState == cFaultState && SelfTestFlag.bit.ubSaveValue == 0)
				{
					lfFaultValue = 10.0f * lfGridVolt;
					SelfTestFlag.bit.ubSaveValue = 1;
				}
			}
			else if((lubSelfTest >= 6 && lubSelfTest <= 9) || (lubSelfTest >= 16 && lubSelfTest <= 19))
			{
				if(lubState == cFaultState && SelfTestFlag.bit.ubSaveValue == 0)
				{
					lfFaultValue = 100.0f * lfGridFreq;
					SelfTestFlag.bit.ubSaveValue = 1;
				}
			}
			return(luw1msCnt);
		}
	}

	return(0);
}

INT16U sGetFaultValue(void)
{
	return((INT16U)lfFaultValue);
}
#else

#endif

void sInitControl2Data(void)
{
	lubDisplay = cSelfTestStart;
	lubSelfTestStep = cNoSelfTest;
}

INT8U sGetSelfTestStep(void)
{
	return(lubSelfTestStep);
}

INT16U sGetSelfTestData(INT8U i)
{
	if(i < 2)
	{
		return(luwDataSave[i]);
	}
	else
	{
		return(0);
	}
}

/**************************************************************************
*函数功能:自检控制函数2
*函数输入:
*函数返回值:无
*函数描述:；
*************************************************************************/
INT8U sSelfTestControl2(INT8U lubButtonState,INT16U luwValue,INT16U luwTime)
{
	if(lubSelfTestStep == cNoSelfTest)										//未按Enter开始自检
	{
		if(lubButtonState == 1)
		{
			lubSelfTestStep = cVoltHigh1;
			lubDisplay = cWaitVoltHigh1;
		}
		else if(lubButtonState == 2)
		{
			lubSelfTestStep = cVoltHigh1Fast;
			lubDisplay = cWaitVoltHigh1;
		}
	}
	else if(lubSelfTestStep == cLimitRecover)								//自检数值恢复
	{
		if(luwValue == 0)													//自检数值已恢复
		{
			if(lubStepPre != cNoSelfTest)
			{
				if(lubStepPre == cFreqLow2 || lubStepPre == cFreqLow2Fast)	//自检最后一步
				{
					if(lubButtonState)
					{
						lubSelfTestStep = cNoSelfTest;
						lubDisplay = cSelfTestEnd;
					}
				}
				else
				{
					if(lubButtonState)
					{
						lubSelfTestStep = lubStepPre + 1;
						lubDisplay++;
					}
				}
			}
			else
			{
				lubSelfTestStep = cNoSelfTest;
				lubDisplay = cSelfTestEnd;
			}
		}
	}
	else if((lubSelfTestStep >= cVoltHigh1 && lubSelfTestStep <= cFreqLow2)	//各个自检阶段
		 || (lubSelfTestStep >= cVoltHigh1Fast && lubSelfTestStep <= cFreqLow2Fast))
	{
		if(luwValue != 0)													//当前自检阶段已出结果
		{
			if(1)															//TODO：数值判断
			{
				lubStepPre = lubSelfTestStep;
				lubSelfTestStep = cLimitRecover;
				lubDisplay++;
				luwDataSave[cSelfTestValue] = luwValue;
				luwDataSave[cSelfTestTime] = luwTime;
			}
			else
			{
				lubStepPre = cNoSelfTest;
				lubSelfTestStep = cLimitRecover;
				lubDisplay = cTimeFault;
			}
		}
	}
	else
	{
		lubSelfTestStep = cNoSelfTest;
	}

	return(lubDisplay);
}

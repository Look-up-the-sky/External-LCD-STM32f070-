/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�Lib_SelfTest.c
 * �޸��ˣ��α���
 * �޸����ڣ�2014.12.15
 * ������������λ���·���������е�ѹ��Ƶ���Լ����
 * �޸����ݣ�
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
*��������:�Լ���ƺ���
*��������:�Լ��־λ
*��������ֵ:��
*��������:bSelfTestCmdΪ0���������Լ�;bSelfTestCmdΪ1��11�����Լ�׼��,��
*		  ��������;bSelfTestCmdΪ2-9������Ӧ�Ĺ����Լ�;bSelfTestCmdΪ12
*		 -19������ٹ����Լ�;bSelfTestCmdΪ10��20�����Լ����,����ϼ�����ݣ�
*************************************************************************/
void sSelfTestControl(INT8U lubSelfTest,INT8U lubState)
{
	float lfTemp;

	if(lubSelfTest == 0)										//�������Լ�
	{
	}
	else if(lubSelfTest == 2 || lubSelfTest == 12)				//over voltage self test		��ѹ1
	{
		if(lubSelfTestpre != lubSelfTest)
		{
			lfVoltTemp[cHigh1] = sGetVoltLimit(cHigh1);			//�洢��ѹֵ
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
	else if(lubSelfTest == 3 || lubSelfTest == 13)				//over voltage self test	��ѹ2
	{
		if(lubSelfTestpre != lubSelfTest)
		{
			lfVoltTemp[cHigh2] = sGetVoltLimit(cHigh2);			//�洢��ѹֵ
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
	else if(lubSelfTest == 4 || lubSelfTest == 14)				//over voltage self test	Ƿѹ1
	{
		if(lubSelfTestpre != lubSelfTest)
		{
			lfVoltTemp[cLow1] = sGetVoltLimit(cLow1);			//�洢Ƿѹֵ
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
	else if(lubSelfTest == 5 || lubSelfTest == 15)				//over voltage self test	Ƿѹ2
	{
		if(lubSelfTestpre != lubSelfTest)
		{
			lfVoltTemp[cLow2] = sGetVoltLimit(cLow2);			//�洢Ƿѹֵ
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
	else if(lubSelfTest == 6 || lubSelfTest == 16)				//over voltage self test	��Ƶ1
	{
		if(lubSelfTestpre != lubSelfTest)
		{
			lfFreqTemp[cHigh1] = sGetFreqLimit(cHigh1);			//�洢��Ƶֵ
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
	else if(lubSelfTest == 7 || lubSelfTest == 17)				//over voltage self test	��Ƶ2
	{
		if(lubSelfTestpre != lubSelfTest)
		{
			lfFreqTemp[cHigh2] = sGetFreqLimit(cHigh2);			//�洢��Ƶֵ
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
	else if(lubSelfTest == 8 || lubSelfTest == 18)				//over voltage self test	ǷƵ1
	{
		if(lubSelfTestpre != lubSelfTest)
		{
			lfFreqTemp[cLow1] = sGetFreqLimit(cLow1);			//�洢ǷƵֵ
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
	else if(lubSelfTest == 9 || lubSelfTest == 19)				//over voltage self test	ǷƵ2
	{
		if(lubSelfTestpre != lubSelfTest)
		{
			lfFreqTemp[cLow2] = sGetFreqLimit(cLow2);			//�洢ǷƵֵ
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
	else														//�Լ�������ָ�����ֵ
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
*��������:�Լ���ƺ���2
*��������:
*��������ֵ:��
*��������:��
*************************************************************************/
INT8U sSelfTestControl2(INT8U lubButtonState,INT16U luwValue,INT16U luwTime)
{
	if(lubSelfTestStep == cNoSelfTest)										//δ��Enter��ʼ�Լ�
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
	else if(lubSelfTestStep == cLimitRecover)								//�Լ���ֵ�ָ�
	{
		if(luwValue == 0)													//�Լ���ֵ�ѻָ�
		{
			if(lubStepPre != cNoSelfTest)
			{
				if(lubStepPre == cFreqLow2 || lubStepPre == cFreqLow2Fast)	//�Լ����һ��
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
	else if((lubSelfTestStep >= cVoltHigh1 && lubSelfTestStep <= cFreqLow2)	//�����Լ�׶�
		 || (lubSelfTestStep >= cVoltHigh1Fast && lubSelfTestStep <= cFreqLow2Fast))
	{
		if(luwValue != 0)													//��ǰ�Լ�׶��ѳ����
		{
			if(1)															//TODO����ֵ�ж�
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

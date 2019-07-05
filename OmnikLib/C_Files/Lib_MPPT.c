/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Lib_MPPT.c
 * 修改人：陈波
 * 描述：最大功率追踪
 * 初版日期：2016.08.09
 * 更改内容：1.1--最大功率点扫描过程中存储前一步PV电压、功率；
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"

/********************************************************************
 * Local Types
 ********************************************************************/
#ifdef FloatEnable
float lfPVRatedPower[cPVInNum];
float lfPVVoltPre[cPVInNum];
float lfPVPowerPre[cPVInNum];
float lfPVVoltBase[cPVInNum];
float lfMPPMin[cPVInNum];
float lfMpptWatMax[cPVInNum];
float lfMPPVolt[cPVInNum];
#else
INT16S lswPVRatedPower[cPVInNum];
INT16S lswPVVoltPre[cPVInNum];
INT16S lswPVPowerPre[cPVInNum];
INT16S lswPVVoltBase[cPVInNum];
INT16S lswMPPMin[cPVInNum];
INT16S lswMpptWatMax[cPVInNum];
INT16S lswMPPVolt[cPVInNum];
#endif

INT8U lubPVChangeCnt[cPVInNum];

/********************************************************************
 * Extern Global Types
 ********************************************************************/
#ifdef FloatEnable
extern float gfRatedPower;
#else
extern INT16U guwRatedPower;
#endif

extern INT8U gubCurrLimitCnt[cPVInNum];
extern INT8U gubCurrLimitCnt2[cPVInNum];

#ifdef FloatEnable
void sCalDeltVoltSingle(INT8U lubInputType,float lfPowerAC,float *lfPowerDC,float lfPowerACMax,float *lfPVVolt,float *lfPVRef)
{
	float lfDeltVolt[cPVInNum];
	float lf4PerRatedPower;
	float lfTemp;
	
	lf4PerRatedPower = gfRatedPower * 0.02f;
	lubInputType &= 0x01;
	StateFlg.bit.ubMPPTFlag = 0;
	
	if(lfPowerAC > lfPowerACMax + lf4PerRatedPower)
	{
		StateFlg.bit.ubMPPTFlag = 2;
		if(lfPowerACMax > gfRatedPower * 0.5f)
		{
			lfDeltVolt[lubInputType] = (lfPowerAC - lfPowerACMax - lf4PerRatedPower) * lfPVVolt[lubInputType] / 8.0f / lfPowerAC;
		}
		else
		{
			lfDeltVolt[lubInputType] = (lfPowerAC - lfPowerACMax - lf4PerRatedPower) * lfPVVolt[lubInputType] / 4.0f / gfRatedPower;
		}
	}
	else if(lfPowerDC[lubInputType] > lfPVRatedPower[lubInputType])
	{
		StateFlg.bit.ubMPPTFlag = 2;
		lfDeltVolt[lubInputType] = (lfPowerDC[lubInputType] - lfPVRatedPower[lubInputType]) * lfPVVolt[lubInputType] / 8.0f / lfPVRatedPower[lubInputType];
	}
	else if(gubCurrLimitCnt[lubInputType] >= 100 || gubCurrLimitCnt2[lubInputType] >= 100)
	{
		lfDeltVolt[lubInputType] = lfPVVolt[lubInputType] - lfPVRef[lubInputType] - c1PerPVVoltMax;
	}
	else if(lfPowerAC > lfPowerACMax)
	{
		StateFlg.bit.ubMPPTFlag = 1;
	}
	else if((lfPVRef[lubInputType] > lfPVVolt[lubInputType] + cPVReverseVolt) && (lfPowerDC[lubInputType] < cPVReversePower))		//mppt反追时,当PVref大于开路电压且输入功率很小进入
	{
		if(lubPVChangeCnt[lubInputType] == 0)
		{
			lubPVChangeCnt[lubInputType]++;
			lfPVVoltBase[lubInputType] = lfPVVolt[lubInputType];
		}
		else if(lfPVVoltBase[lubInputType] > lfPVVolt[lubInputType] + cPVRvsBackVolt || lfPVVoltBase[lubInputType] + cPVRvsBackVolt < lfPVVolt[lubInputType])
		{
			lubPVChangeCnt[lubInputType] = 0;
		}
		else if(++lubPVChangeCnt[lubInputType] >= 5)
		{
			lfPVRef[lubInputType] = lfPVVolt[lubInputType] - c1PerPVVoltMax;
			lubPVChangeCnt[lubInputType] = 0;
		}
	}
	else if(lfPowerAC > 0.9f * lfPowerACMax)
	{
		lfTemp = (lfPVPowerPre[lubInputType] - lfPowerDC[lubInputType]) / (lfPVVoltPre[lubInputType] - lfPVVolt[lubInputType]);
		lfTemp *= cPVVoltMax / lfPVRatedPower[lubInputType];
		lfTemp *= (lfPowerACMax - lfPowerAC) / lfPowerACMax;
		lfDeltVolt[lubInputType] = lfTemp * lfPVVolt[lubInputType] / 200.0f;
	}
	else
	{
		lfTemp = (lfPVPowerPre[lubInputType] - lfPowerDC[lubInputType]) / (lfPVVoltPre[lubInputType] - lfPVVolt[lubInputType]);
		lfTemp *= cPVVoltMax / lfPVRatedPower[lubInputType];
		lfDeltVolt[lubInputType] = lfTemp * lfPVVolt[lubInputType] / 2000.0f;
	}
	
	if(StateFlg.bit.ubMPPTFlag == 1)
	{
		lfDeltVolt[lubInputType] = c0V;
	}
	else if(lfDeltVolt[lubInputType] > c0V)
	{
		if(lfDeltVolt[lubInputType] > c1PerPVVoltMax)
		{
			lfDeltVolt[lubInputType] = c1PerPVVoltMax;
		}
		else if(StateFlg.bit.ubMPPTFlag == 2)
		{
			if(lfDeltVolt[lubInputType] < c0Per02PVVoltMax)
			{
				lfDeltVolt[lubInputType] = c0Per02PVVoltMax;
			}
		}
		else
		{
			if(lfDeltVolt[lubInputType] < c0Per1PVVoltMax)
			{
				lfDeltVolt[lubInputType] = c0Per1PVVoltMax;
			}
		}
	}
	else
	{
		if(lfDeltVolt[lubInputType] < -c1PerPVVoltMax)
		{
			lfDeltVolt[lubInputType] = -c1PerPVVoltMax;
		}
		else if(StateFlg.bit.ubMPPTFlag == 2)
		{
			if(lfDeltVolt[lubInputType] > -c0Per02PVVoltMax)
			{
				lfDeltVolt[lubInputType] = -c0Per02PVVoltMax;
			}
		}
		else
		{
			if(lfDeltVolt[lubInputType] > -c0Per1PVVoltMax)
			{
				lfDeltVolt[lubInputType] = -c0Per1PVVoltMax;
			}
		}
	}
	
	lfPVVoltPre[lubInputType] = lfPVVolt[lubInputType];
	lfPVPowerPre[lubInputType] = lfPowerDC[lubInputType];
	gubCurrLimitCnt[lubInputType] = 0;
	gubCurrLimitCnt2[lubInputType] = 0;
	lfPVRef[lubInputType] += lfDeltVolt[lubInputType];
	if(lfPVRef[lubInputType] < cMinMPPVolt)
	{
		lfPVRef[lubInputType] = cMinMPPVolt;
	}
	else if(lfPVRef[lubInputType] > cMaxMPPVolt)
	{
		lfPVRef[lubInputType] =  cMaxMPPVolt;
	}
}

void sCalDeltVoltBoth(float lfPowerAC,float *lfPowerDC,float lfPowerACMax,float *lfPVVolt,float *lfPVRef)
{
	float lfDeltVolt[cPVInNum];
	float lf4PerRatedPower;
	float lfTemp;
	
	lf4PerRatedPower = gfRatedPower * 0.02f;
	StateFlg.bit.ubMPPTFlag = 0;
	
	if(lfPowerAC > lfPowerACMax + lf4PerRatedPower)
	{
		StateFlg.bit.ubMPPTFlag = 2;
		if((lfPowerDC[cPVIn1] > lfPVRatedPower[cPVIn1] && lfPowerDC[cPVIn2] > lfPVRatedPower[cPVIn2])
		|| (lfPowerDC[cPVIn1] < lfPVRatedPower[cPVIn1] && lfPowerDC[cPVIn2] < lfPVRatedPower[cPVIn2]))
		{
			if(lfPowerACMax > gfRatedPower * 0.5f)
			{
				lfTemp = (lfPowerAC - lfPowerACMax - lf4PerRatedPower) / 8.0f / lfPowerAC;
			}
			else
			{
				lfTemp = (lfPowerAC - lfPowerACMax - lf4PerRatedPower) / 4.0f / gfRatedPower;
			}
			lfDeltVolt[cPVIn1] = lfTemp * lfPVVolt[cPVIn1];
			lfDeltVolt[cPVIn2] = lfTemp * lfPVVolt[cPVIn2];
		}
		else if(lfPowerDC[cPVIn1] > lfPVRatedPower[cPVIn1])
		{
			if(lfPowerACMax > gfRatedPower * 0.5f)
			{
				lfDeltVolt[cPVIn1] = (lfPowerAC - lfPowerACMax - lf4PerRatedPower) * lfPVVolt[cPVIn1] / 8.0f / lfPowerAC;
			}
			else
			{
				lfDeltVolt[cPVIn1] = (lfPowerAC - lfPowerACMax - lf4PerRatedPower) * lfPVVolt[cPVIn1] / 4.0f / gfRatedPower;
			}
			lfDeltVolt[cPVIn2] = c0V;
		}
		else
		{
			lfDeltVolt[cPVIn1] = c0V;
			if(lfPowerACMax > gfRatedPower * 0.5f)
			{
				lfDeltVolt[cPVIn2] = (lfPowerAC - lfPowerACMax - lf4PerRatedPower) * lfPVVolt[cPVIn2] / 8.0f / lfPowerAC;
			}
			else
			{
				lfDeltVolt[cPVIn2] = (lfPowerAC - lfPowerACMax - lf4PerRatedPower) * lfPVVolt[cPVIn2] / 4.0f / gfRatedPower;
			}
		}
	}
	else if(lfPowerAC > lfPowerACMax)
	{
		StateFlg.bit.ubMPPTFlag = 1;
		if(lfPowerDC[cPVIn1] > lfPVRatedPower[cPVIn1])
		{
			lfDeltVolt[cPVIn1] = (lfPowerDC[cPVIn1] - lfPVRatedPower[cPVIn1]) * lfPVVolt[cPVIn1] / 8.0f / lfPVRatedPower[cPVIn1];
		}
		else
		{
			lfDeltVolt[cPVIn1] = c0V;
		}
		
		if(lfPowerDC[cPVIn2] > lfPVRatedPower[cPVIn2])
		{
			lfDeltVolt[cPVIn2] = (lfPowerDC[cPVIn2] - lfPVRatedPower[cPVIn2]) * lfPVVolt[cPVIn2] / 8.0f / lfPVRatedPower[cPVIn2];
		}
		else
		{
			lfDeltVolt[cPVIn2] = c0V;
		}
	}
	else if(lfPowerAC > 0.9f * lfPowerACMax)
	{
		if(lfPowerDC[cPVIn1] > lfPVRatedPower[cPVIn1])
		{
			lfDeltVolt[cPVIn1] = (lfPowerDC[cPVIn1] - lfPVRatedPower[cPVIn1]) * lfPVVolt[cPVIn1] / 8.0f / lfPVRatedPower[cPVIn1];
		}
		else if(gubCurrLimitCnt[cPVIn1] >= 100 || gubCurrLimitCnt2[cPVIn1] >= 100)
		{
			lfDeltVolt[cPVIn1] = lfPVVolt[cPVIn1] - lfPVRef[cPVIn1] - c1PerPVVoltMax;
		}
		else if((lfPVRef[cPVIn1] > lfPVVolt[cPVIn1] + cPVReverseVolt) && (lfPowerDC[cPVIn1] < cPVReversePower))		//mppt反追时,当PVref大于开路电压且输入功率很小进入
		{
			if(lubPVChangeCnt[cPVIn1] == 0)
			{
				lubPVChangeCnt[cPVIn1]++;
				lfPVVoltBase[cPVIn1] = lfPVVolt[cPVIn1];
			}
			else if(lfPVVoltBase[cPVIn1] > lfPVVolt[cPVIn1] + cPVRvsBackVolt || lfPVVoltBase[cPVIn1] + cPVRvsBackVolt < lfPVVolt[cPVIn1])
			{
				lubPVChangeCnt[cPVIn1] = 0;
			}
			else if(++lubPVChangeCnt[cPVIn1] >= 5)
			{
				lfPVRef[cPVIn1] = lfPVVolt[cPVIn1] - c1PerPVVoltMax;
				lubPVChangeCnt[cPVIn1] = 0;
			}
		}
		else
		{
			lfTemp = (lfPVPowerPre[cPVIn1] - lfPowerDC[cPVIn1]) / (lfPVVoltPre[cPVIn1] - lfPVVolt[cPVIn1]);
			lfTemp *= cPVVoltMax / lfPVRatedPower[cPVIn1];
			lfTemp *= (lfPowerACMax - lfPowerAC) / lfPowerACMax;
			lfDeltVolt[cPVIn1] = lfTemp * lfPVVolt[cPVIn1] / 200.0f;
		}
		
		if(lfPowerDC[cPVIn2] > lfPVRatedPower[cPVIn2])
		{
			lfDeltVolt[cPVIn2] = (lfPowerDC[cPVIn2] - lfPVRatedPower[cPVIn2]) * lfPVVolt[cPVIn2] / 8.0f / lfPVRatedPower[cPVIn2];
		}
		else if(gubCurrLimitCnt[cPVIn2] >= 100 || gubCurrLimitCnt2[cPVIn2] >= 100)
		{
			lfDeltVolt[cPVIn2] = lfPVVolt[cPVIn2] - lfPVRef[cPVIn2] - c1PerPVVoltMax;
		}
		else if((lfPVRef[cPVIn2] > lfPVVolt[cPVIn2] + cPVReverseVolt) && (lfPowerDC[cPVIn2] < cPVReversePower))		//mppt反追时,当PVref大于开路电压且输入功率很小进入
		{
			if(lubPVChangeCnt[cPVIn2] == 0)
			{
				lubPVChangeCnt[cPVIn2]++;
				lfPVVoltBase[cPVIn2] = lfPVVolt[cPVIn2];
			}
			else if(lfPVVoltBase[cPVIn2] > lfPVVolt[cPVIn2] + cPVRvsBackVolt || lfPVVoltBase[cPVIn2] + cPVRvsBackVolt < lfPVVolt[cPVIn2])
			{
				lubPVChangeCnt[cPVIn2] = 0;
			}
			else if(++lubPVChangeCnt[cPVIn2] >= 5)
			{
				lfPVRef[cPVIn2] = lfPVVolt[cPVIn2] - c1PerPVVoltMax;
				lubPVChangeCnt[cPVIn2] = 0;
			}
		}
		else
		{
			lfTemp = (lfPVPowerPre[cPVIn2] - lfPowerDC[cPVIn2]) / (lfPVVoltPre[cPVIn2] - lfPVVolt[cPVIn2]);
			lfTemp *= cPVVoltMax / lfPVRatedPower[cPVIn2];
			lfTemp *= (lfPowerACMax - lfPowerAC) / lfPowerACMax;
			lfDeltVolt[cPVIn2] = lfTemp * lfPVVolt[cPVIn2] / 200.0f;
		}
	}
	else
	{
		if(lfPowerDC[cPVIn1] > lfPVRatedPower[cPVIn1])
		{
			lfDeltVolt[cPVIn1] = (lfPowerDC[cPVIn1] - lfPVRatedPower[cPVIn1]) * lfPVVolt[cPVIn1] / 8.0f / lfPVRatedPower[cPVIn1];
		}
		else if(gubCurrLimitCnt[cPVIn1] >= 100 || gubCurrLimitCnt2[cPVIn1] >= 100)
		{
			lfDeltVolt[cPVIn1] = lfPVVolt[cPVIn1] - lfPVRef[cPVIn1] - c1PerPVVoltMax;
		}
		else if((lfPVRef[cPVIn1] > lfPVVolt[cPVIn1] + cPVReverseVolt) && (lfPowerDC[cPVIn1] < cPVReversePower))		//mppt反追时,当PVref大于开路电压且输入功率很小进入
		{
			if(lubPVChangeCnt[cPVIn1] == 0)
			{
				lubPVChangeCnt[cPVIn1]++;
				lfPVVoltBase[cPVIn1] = lfPVVolt[cPVIn1];
			}
			else if(lfPVVoltBase[cPVIn1] > lfPVVolt[cPVIn1] + cPVRvsBackVolt || lfPVVoltBase[cPVIn1] + cPVRvsBackVolt < lfPVVolt[cPVIn1])
			{
				lubPVChangeCnt[cPVIn1] = 0;
			}
			else if(++lubPVChangeCnt[cPVIn1] >= 5)
			{
				lfPVRef[cPVIn1] = lfPVVolt[cPVIn1] - c1PerPVVoltMax;
				lubPVChangeCnt[cPVIn1] = 0;
			}
		}
		else
		{
			lfTemp = (lfPVPowerPre[cPVIn1] - lfPowerDC[cPVIn1]) / (lfPVVoltPre[cPVIn1] - lfPVVolt[cPVIn1]);
			lfTemp *= cPVVoltMax / lfPVRatedPower[cPVIn1];
			lfDeltVolt[cPVIn1] = lfTemp * lfPVVolt[cPVIn1] / 2000.0f;
		}
		
		if(lfPowerDC[cPVIn2] > lfPVRatedPower[cPVIn2])
		{
			lfDeltVolt[cPVIn2] = (lfPowerDC[cPVIn2] - lfPVRatedPower[cPVIn2]) * lfPVVolt[cPVIn2] / 8.0f / lfPVRatedPower[cPVIn2];
		}
		else if(gubCurrLimitCnt[cPVIn2] >= 100 || gubCurrLimitCnt2[cPVIn2] >= 100)
		{
			lfDeltVolt[cPVIn2] = lfPVVolt[cPVIn2] - lfPVRef[cPVIn2] - c1PerPVVoltMax;
		}
		else if((lfPVRef[cPVIn2] > lfPVVolt[cPVIn2] + cPVReverseVolt) && (lfPowerDC[cPVIn2] < cPVReversePower))		//mppt反追时,当PVref大于开路电压且输入功率很小进入
		{
			if(lubPVChangeCnt[cPVIn2] == 0)
			{
				lubPVChangeCnt[cPVIn2]++;
				lfPVVoltBase[cPVIn2] = lfPVVolt[cPVIn2];
			}
			else if(lfPVVoltBase[cPVIn2] > lfPVVolt[cPVIn2] + cPVRvsBackVolt || lfPVVoltBase[cPVIn2] + cPVRvsBackVolt < lfPVVolt[cPVIn2])
			{
				lubPVChangeCnt[cPVIn2] = 0;
			}
			else if(++lubPVChangeCnt[cPVIn2] >= 5)
			{
				lfPVRef[cPVIn2] = lfPVVolt[cPVIn2] - c1PerPVVoltMax;
				lubPVChangeCnt[cPVIn2] = 0;
			}
		}
		else
		{
			lfTemp = (lfPVPowerPre[cPVIn2] - lfPowerDC[cPVIn2]) / (lfPVVoltPre[cPVIn2] - lfPVVolt[cPVIn2]);
			lfTemp *= cPVVoltMax / lfPVRatedPower[cPVIn2];
			lfDeltVolt[cPVIn2] = lfTemp * lfPVVolt[cPVIn2] / 2000.0f;
		}
	}
	
	if(StateFlg.bit.ubMPPTFlag == 1)
	{
		lfDeltVolt[cPVIn1] = c0V;
		lfDeltVolt[cPVIn2] = c0V;
	}
	else
	{
		if(lfDeltVolt[cPVIn1] > c0V)
		{
			if(lfDeltVolt[cPVIn1] > c1PerPVVoltMax)
			{
				lfDeltVolt[cPVIn1] = c1PerPVVoltMax;
			}
			else if(StateFlg.bit.ubMPPTFlag == 2)				//允许小步长调整
			{
				if(lfDeltVolt[cPVIn1] < c0Per02PVVoltMax)
				{
					lfDeltVolt[cPVIn1] = c0Per02PVVoltMax;
				}
			}
			else
			{
				if(lfDeltVolt[cPVIn1] < c0Per1PVVoltMax)
				{
					lfDeltVolt[cPVIn1] = c0Per1PVVoltMax;
				}
			}
		}
		else
		{
			if(lfDeltVolt[cPVIn1] < -c1PerPVVoltMax)
			{
				lfDeltVolt[cPVIn1] = -c1PerPVVoltMax;
			}
			else if(StateFlg.bit.ubMPPTFlag == 2)
			{
				if(lfDeltVolt[cPVIn1] > -c0Per02PVVoltMax)
				{
					lfDeltVolt[cPVIn1] = -c0Per02PVVoltMax;
				}
			}
			else
			{
				if(lfDeltVolt[cPVIn1] > -c0Per1PVVoltMax)
				{
					lfDeltVolt[cPVIn1] = -c0Per1PVVoltMax;
				}
			}
		}
		
		if(lfDeltVolt[cPVIn2] > c0V)
		{
			if(lfDeltVolt[cPVIn2] > c1PerPVVoltMax)
			{
				lfDeltVolt[cPVIn2] = c1PerPVVoltMax;
			}
			else if(StateFlg.bit.ubMPPTFlag == 2)
			{
				if(lfDeltVolt[cPVIn2] < c0Per02PVVoltMax)
				{
					lfDeltVolt[cPVIn2] = c0Per02PVVoltMax;
				}
			}
			else
			{
				if(lfDeltVolt[cPVIn2] < c0Per1PVVoltMax)
				{
					lfDeltVolt[cPVIn2] = c0Per1PVVoltMax;
				}
			}
		}
		else
		{
			if(lfDeltVolt[cPVIn2] < -c1PerPVVoltMax)
			{
				lfDeltVolt[cPVIn2] = -c1PerPVVoltMax;
			}
			else if(StateFlg.bit.ubMPPTFlag == 2)
			{
				if(lfDeltVolt[cPVIn2] > -c0Per02PVVoltMax)
				{
					lfDeltVolt[cPVIn2] = -c0Per02PVVoltMax;
				}
			}
			else
			{
				if(lfDeltVolt[cPVIn2] > -c0Per1PVVoltMax)
				{
					lfDeltVolt[cPVIn2] = -c0Per1PVVoltMax;
				}
			}
		}
	}
	
	lfPVVoltPre[cPVIn1] = lfPVVolt[cPVIn1];
	lfPVVoltPre[cPVIn2] = lfPVVolt[cPVIn2];
	lfPVPowerPre[cPVIn1] = lfPowerDC[cPVIn1];
	lfPVPowerPre[cPVIn2] = lfPowerDC[cPVIn2];
	gubCurrLimitCnt[cPVIn1] = 0;
	gubCurrLimitCnt[cPVIn2] = 0;
	gubCurrLimitCnt2[cPVIn1] = 0;
	gubCurrLimitCnt2[cPVIn2] = 0;
	lfPVRef[cPVIn1] += lfDeltVolt[cPVIn1];
	lfPVRef[cPVIn2] += lfDeltVolt[cPVIn2];
	if(lfPVRef[cPVIn1] <  cMinMPPVolt)
	{
		lfPVRef[cPVIn1] = cMinMPPVolt;
	}
	else if(lfPVRef[cPVIn1] > cMaxMPPVolt)
	{
		lfPVRef[cPVIn1] =  cMaxMPPVolt;
	}
	if(lfPVRef[cPVIn2] <  cMinMPPVolt)
	{
		lfPVRef[cPVIn2] = cMinMPPVolt;
	}
	else if(lfPVRef[cPVIn2] > cMaxMPPVolt)
	{
		lfPVRef[cPVIn2] =  cMaxMPPVolt;
	}
}

void sCalDeltVolt(INT8U lubInputType,float lfPowerAC,float *lfPowerDC,float lfPowerACMax,float *lfPVVolt,float *lfPVRef)
{
	INT8U lubMPPScanStop;
	
	if(PowerDecreaseFlg.bit.ubMPPScan == 1)
	{
		if(PowerDecreaseFlg.bit.ubMPPScanStep == 0)	//第一阶段，关闭所有驱动
		{
			PowerDecreaseFlg.bit.ubMPPScanStep = 1;
		}
		else if(PowerDecreaseFlg.bit.ubMPPScanStep == 1)	//第二阶段，设置初始参考电压、最小扫描电压，最大功率清0
		{
			lfPVRef[cPVIn1] = lfPVVolt[cPVIn1] - cMPPScanStep;
			lfPVRef[cPVIn2] = lfPVVolt[cPVIn2] - cMPPScanStep;
			lfMPPMin[cPVIn1] = lfPVVolt[cPVIn1] * 0.6f;
			lfMPPMin[cPVIn2] = lfPVVolt[cPVIn2] * 0.6f;
			lfMpptWatMax[cPVIn1] = c0W;
			lfMpptWatMax[cPVIn2] = c0W;
			lfMPPVolt[cPVIn1] = cMaxMPPVolt;
			lfMPPVolt[cPVIn2] = cMaxMPPVolt;

			if(lfMPPMin[cPVIn1] < cMinMPPVolt)
			{
				lfMPPMin[cPVIn1] = cMinMPPVolt;
			}
			else if(lfMPPMin[cPVIn1] > cMaxMPPVolt)
			{
				lfMPPMin[cPVIn1] = cMaxMPPVolt;
			}
			
			if(lfMPPMin[cPVIn2] < cMinMPPVolt)
			{
				lfMPPMin[cPVIn2] = cMinMPPVolt;
			}
			else if(lfMPPMin[cPVIn2] > cMaxMPPVolt)
			{
				lfMPPMin[cPVIn2] = cMaxMPPVolt;
			}
/*******************************************************************/
			cInvOn();
			StateFlg.bit.ub_mpptscan_invoff = 0;
			
/*******************************************************************/
			PowerDecreaseFlg.bit.ubMPPScanStep = 2;
		}
		else if(PowerDecreaseFlg.bit.ubMPPScanStep == 2)	//第三阶段，以2.5V为步长减小PV参考电压，记录最大功率点电压
		{
			StateFlg.bit.ub_mpptscan_invoff = 0;
			if(lfPowerDC[cPVIn1] > lfMpptWatMax[cPVIn1])
			{
				lfMpptWatMax[cPVIn1] = lfPowerDC[cPVIn1];
				lfMPPVolt[cPVIn1] = lfPVRef[cPVIn1];
			}
			if(lfPowerDC[cPVIn2] > lfMpptWatMax[cPVIn2])
			{
				lfMpptWatMax[cPVIn2] = lfPowerDC[cPVIn2];
				lfMPPVolt[cPVIn2] = lfPVRef[cPVIn2];
			}
			
			lubMPPScanStop = 0;
			if(lfPowerDC[cPVIn1] < lfPVRatedPower[cPVIn1])
			{
				lfPVRef[cPVIn1] -= cMPPScanStep;
				lubMPPScanStop++;
			}
			if(lfPowerDC[cPVIn2] < lfPVRatedPower[cPVIn2])
			{
				lfPVRef[cPVIn2] -= cMPPScanStep;
				lubMPPScanStop++;
			}

			if((lfPVRef[cPVIn1] < lfMPPMin[cPVIn1] && lfPVRef[cPVIn2] < lfMPPMin[cPVIn2])
			 || lfPowerAC > gfRatedPower || lubMPPScanStop == 0)
			{
				PowerDecreaseFlg.bit.ubMPPScanStep = 3;
			}
		}
		else	//第四阶段，PV参考电压以10V为步长回升至最大功率点电压
		{
			lfPVRef[cPVIn1] += cMPPScanBackStep;
			lfPVRef[cPVIn2] += cMPPScanBackStep;
			if(lfPVRef[cPVIn1] > lfMPPVolt[cPVIn1])
			{
				lfPVRef[cPVIn1] = lfMPPVolt[cPVIn1];
				if(lfPVRef[cPVIn2] > lfMPPVolt[cPVIn2])
				{
					lfPVRef[cPVIn2] = lfMPPVolt[cPVIn2];
					PowerDecreaseFlg.bit.ubMPPScan = 0;
					PowerDecreaseFlg.bit.ubMPPScanStep = 0;
				}
			}
			else if(lfPVRef[cPVIn2] > lfMPPVolt[cPVIn2])
			{
				lfPVRef[cPVIn2] = lfMPPVolt[cPVIn2];
			}
		}

		if(lfPVRef[cPVIn1] < lfMPPMin[cPVIn1])
		{
			lfPVRef[cPVIn1] = lfMPPMin[cPVIn1];
		}
		else if(lfPVRef[cPVIn1] > cMaxMPPVolt)
		{
			lfPVRef[cPVIn1] = cMaxMPPVolt;
		}
		
		if(lfPVRef[cPVIn2] < lfMPPMin[cPVIn2])
		{
			lfPVRef[cPVIn2] = lfMPPMin[cPVIn2];
		}
		else if(lfPVRef[cPVIn2] > cMaxMPPVolt)
		{
			lfPVRef[cPVIn2] = cMaxMPPVolt;
		}
		
		lfPVVoltPre[cPVIn1] = lfPVVolt[cPVIn1];
		lfPVVoltPre[cPVIn2] = lfPVVolt[cPVIn2];
		lfPVPowerPre[cPVIn1] = lfPowerDC[cPVIn1];
		lfPVPowerPre[cPVIn2] = lfPowerDC[cPVIn2];
	}
	else if(lubInputType <= cPVIn2)
	{
		sCalDeltVoltSingle(lubInputType,lfPowerAC,lfPowerDC,lfPowerACMax,lfPVVolt,lfPVRef);
	}
	else
	{
		sCalDeltVoltBoth(lfPowerAC,lfPowerDC,lfPowerACMax,lfPVVolt,lfPVRef);
	}
}

void sSetMPPTPre(INT8U lubInputType,float lfPVVoltPreTemp,float lfPVPowerPreTemp)
{
	lfPVVoltPre[lubInputType] = lfPVVoltPreTemp;
	lfPVPowerPre[lubInputType] = lfPVPowerPreTemp;
}

void sSetPVRatedPower(float lfPV1RatedPower,float lfPV2RatedPower)
{
	lfPVRatedPower[cPVIn1] = lfPV1RatedPower;
	lfPVRatedPower[cPVIn2] = lfPV2RatedPower;
}
#else
void sCalDeltVoltSingle(INT8U lubInputType,INT16S lswPowerAC,INT16S *lswPowerDC,INT16S lswPowerACMax,INT16S *lswPVVolt,INT16U *luwPVRef)
{
	INT16S lswDeltVolt[cPVInNum];
	INT16S lsw4PerRatedPower;
	INT32S lslTemp;
	
	lsw4PerRatedPower = guwRatedPower / 50;
	lubInputType &= 0x01;
	StateFlg.bit.ubMPPTFlag = 0;

	if(lswPowerAC < c1W)
	{
		lswPowerAC = c1W;
	}
	
	if(lswPowerAC > lswPowerACMax + lsw4PerRatedPower)
	{
		StateFlg.bit.ubMPPTFlag = 2;
		if(lswPowerACMax > guwRatedPower / 2)
		{
			lswDeltVolt[lubInputType] = (INT32S)(lswPowerAC - lswPowerACMax - lsw4PerRatedPower) * lswPVVolt[lubInputType] / lswPowerAC >> 3;
		}
		else
		{
			lswDeltVolt[lubInputType] = (INT32S)(lswPowerAC - lswPowerACMax - lsw4PerRatedPower) * lswPVVolt[lubInputType] / guwRatedPower >> 2;
		}
	}
	else if(lswPowerDC[lubInputType] > lswPVRatedPower[lubInputType])
	{
		StateFlg.bit.ubMPPTFlag = 2;
		lswDeltVolt[lubInputType] = (INT32S)(lswPowerDC[lubInputType] - lswPVRatedPower[lubInputType]) * lswPVVolt[lubInputType] / lswPVRatedPower[lubInputType] >> 3;
	}
	else if(gubCurrLimitCnt[lubInputType] >= 100 || gubCurrLimitCnt2[lubInputType] >= 100)
	{
		lswDeltVolt[lubInputType] = lswPVVolt[lubInputType] - luwPVRef[lubInputType] - c1PerPVVoltMax;
	}
	else if(lswPowerAC > lswPowerACMax)
	{
		StateFlg.bit.ubMPPTFlag = 1;
	}
	else if((luwPVRef[lubInputType] > lswPVVolt[lubInputType] + cPVReverseVolt) && (lswPowerDC[lubInputType] < cPVReversePower))		//mppt反追时,当PVref大于开路电压且输入功率很小进入
	{
		if(lubPVChangeCnt[lubInputType] == 0)
		{
			lubPVChangeCnt[lubInputType]++;
			lswPVVoltBase[lubInputType] = lswPVVolt[lubInputType];
		}
		else if(lswPVVoltBase[lubInputType] > lswPVVolt[lubInputType] + cPVRvsBackVolt || lswPVVoltBase[lubInputType] + cPVRvsBackVolt < lswPVVolt[lubInputType])
		{
			lubPVChangeCnt[lubInputType] = 0;
		}
		else if(++lubPVChangeCnt[lubInputType] >= 5)
		{
			luwPVRef[lubInputType] = lswPVVolt[lubInputType] - c1PerPVVoltMax;
			lubPVChangeCnt[lubInputType] = 0;
		}
	}
	else if(lswPowerAC > (INT32S)9 * lswPowerACMax / 10)
	{
		if(lswPVVoltPre[lubInputType] != lswPVVolt[lubInputType])
		{
			lslTemp = (INT32S)cPVVoltMax * (lswPVPowerPre[lubInputType] - lswPowerDC[lubInputType]) / (lswPVVoltPre[lubInputType] - lswPVVolt[lubInputType]);
		}
		else
		{
			lslTemp = (INT32S)cPVVoltMax * (lswPVPowerPre[lubInputType] - lswPowerDC[lubInputType]);
		}
		lslTemp /= lswPVRatedPower[lubInputType];
		lslTemp *= lswPowerACMax - lswPowerAC;
		if(lswPowerACMax)
		{
			lslTemp /= lswPowerACMax;
		}
		lswDeltVolt[lubInputType] = lslTemp * lswPVVolt[lubInputType] / 200;
	}
	else
	{
		if(lswPVVoltPre[lubInputType] != lswPVVolt[lubInputType])
		{
			lslTemp = (INT32S)cPVVoltMax * (lswPVPowerPre[lubInputType] - lswPowerDC[lubInputType]) / (lswPVVoltPre[lubInputType] - lswPVVolt[lubInputType]);
		}
		else
		{
			lslTemp = (INT32S)cPVVoltMax * (lswPVPowerPre[lubInputType] - lswPowerDC[lubInputType]);
		}
		lslTemp /= lswPVRatedPower[lubInputType];
		lswDeltVolt[lubInputType] = lslTemp * lswPVVolt[lubInputType] / 2000;
	}

	if(StateFlg.bit.ubMPPTFlag == 1)
	{
		lswDeltVolt[lubInputType] = c0V;
	}
	else if(lswDeltVolt[lubInputType] > c0V)
	{
		if(lswDeltVolt[lubInputType] > c1PerPVVoltMax)
		{
			lswDeltVolt[lubInputType] = c1PerPVVoltMax;
		}
		else if(StateFlg.bit.ubMPPTFlag == 2)
		{
			if(lswDeltVolt[lubInputType] < c0Per02PVVoltMax)
			{
				lswDeltVolt[lubInputType] = c0Per02PVVoltMax;
			}
		}
		else
		{
			if(lswDeltVolt[lubInputType] < c0Per1PVVoltMax)
			{
				lswDeltVolt[lubInputType] = c0Per1PVVoltMax;
			}
		}
	}
	else
	{
		if(lswDeltVolt[lubInputType] < -c1PerPVVoltMax)
		{
			lswDeltVolt[lubInputType] = -c1PerPVVoltMax;
		}
		else if(StateFlg.bit.ubMPPTFlag == 2)
		{
			if(lswDeltVolt[lubInputType] > -c0Per02PVVoltMax)
			{
				lswDeltVolt[lubInputType] = -c0Per02PVVoltMax;
			}
		}
		else
		{
			if(lswDeltVolt[lubInputType] > -c0Per1PVVoltMax)
			{
				lswDeltVolt[lubInputType] = -c0Per1PVVoltMax;
			}
		}
	}
	
	lswPVVoltPre[lubInputType] = lswPVVolt[lubInputType];
	lswPVPowerPre[lubInputType] = lswPowerDC[lubInputType];
	gubCurrLimitCnt[lubInputType] = 0;
	gubCurrLimitCnt2[lubInputType] = 0;
	luwPVRef[lubInputType] += lswDeltVolt[lubInputType];
	if(luwPVRef[lubInputType] < cMinMPPVolt)
	{
		luwPVRef[lubInputType] = cMinMPPVolt;
	}
	else if(luwPVRef[lubInputType] > cMaxMPPVolt)
	{
		luwPVRef[lubInputType] =  cMaxMPPVolt;
	}
}

void sCalDeltVoltBoth(INT16S lswPowerAC,INT16S *lswPowerDC,INT16S lswPowerACMax,INT16S *lswPVVolt,INT16U *luwPVRef)
{
	INT16S lswDeltVolt[cPVInNum];
	INT16S lsw4PerRatedPower;
	INT32S lslTemp;
	INT8U  lubTmpJudg[cPVInNum];
	
	lsw4PerRatedPower = guwRatedPower / 50;
	StateFlg.bit.ubMPPTFlag = 0;

	if(lswPowerAC < c1W)
	{
		lswPowerAC = c1W;
	}
	
	if(lswPowerAC > lswPowerACMax + lsw4PerRatedPower)
	{
		StateFlg.bit.ubMPPTFlag = 2;
		if((lswPowerDC[cPVIn1] > lswPVRatedPower[cPVIn1] && lswPowerDC[cPVIn2] > lswPVRatedPower[cPVIn2])
		|| (lswPowerDC[cPVIn1] < lswPVRatedPower[cPVIn1] && lswPowerDC[cPVIn2] < lswPVRatedPower[cPVIn2]))
		{
			if(lswPowerACMax > guwRatedPower / 2)
			{
				lslTemp = ((INT32S)(lswPowerAC - lswPowerACMax - lsw4PerRatedPower) << 7) / lswPowerAC;
			}
			else
			{
				lslTemp = ((INT32S)(lswPowerAC - lswPowerACMax - lsw4PerRatedPower) << 8) / guwRatedPower;
			}
			lswDeltVolt[cPVIn1] = lslTemp * lswPVVolt[cPVIn1] >> 10;
			lswDeltVolt[cPVIn2] = lslTemp * lswPVVolt[cPVIn2] >> 10;
			lubTmpJudg[cPVIn1] = 1;
			lubTmpJudg[cPVIn2] = 1;
		}
		else if(lswPowerDC[cPVIn1] > lswPVRatedPower[cPVIn1])
		{
			if(lswPowerACMax > guwRatedPower / 2)
			{
				lswDeltVolt[cPVIn1] = (INT32S)(lswPowerAC - lswPowerACMax - lsw4PerRatedPower) * lswPVVolt[cPVIn1] / lswPowerAC >> 3;
			}
			else
			{
				lswDeltVolt[cPVIn1] = (INT32S)(lswPowerAC - lswPowerACMax - lsw4PerRatedPower) * lswPVVolt[cPVIn1] / guwRatedPower >> 2;
			}
			lswDeltVolt[cPVIn2] = c0V;
			lubTmpJudg[cPVIn1] = 1;
			lubTmpJudg[cPVIn2] = 2;
		}
		else
		{
			lswDeltVolt[cPVIn1] = c0V;
			if(lswPowerACMax > guwRatedPower / 2)
			{
				lswDeltVolt[cPVIn2] = (INT32S)(lswPowerAC - lswPowerACMax - lsw4PerRatedPower) * lswPVVolt[cPVIn2] / lswPowerAC >> 3;
			}
			else
			{
				lswDeltVolt[cPVIn2] = (INT32S)(lswPowerAC - lswPowerACMax - lsw4PerRatedPower) * lswPVVolt[cPVIn2] / guwRatedPower >> 2;
			}
			lubTmpJudg[cPVIn1] = 2;
			lubTmpJudg[cPVIn2] = 1;
		}
	}
	else if(lswPowerAC > lswPowerACMax)
	{
		StateFlg.bit.ubMPPTFlag = 1;
		if(lswPowerDC[cPVIn1] > lswPVRatedPower[cPVIn1])
		{
			lswDeltVolt[cPVIn1] = (INT32S)(lswPowerDC[cPVIn1] - lswPVRatedPower[cPVIn1]) * lswPVVolt[cPVIn1] / lswPVRatedPower[cPVIn1] >> 3;
			lubTmpJudg[cPVIn1] = 1;
		}
		else
		{
			lswDeltVolt[cPVIn1] = c0V;
			lubTmpJudg[cPVIn1] = 2;
		}
		
		if(lswPowerDC[cPVIn2] > lswPVRatedPower[cPVIn2])
		{
			lswDeltVolt[cPVIn2] = (INT32S)(lswPowerDC[cPVIn2] - lswPVRatedPower[cPVIn2]) * lswPVVolt[cPVIn2] / lswPVRatedPower[cPVIn2] >> 3;
			lubTmpJudg[cPVIn2] = 1;
		}
		else
		{
			lswDeltVolt[cPVIn2] = c0V;
			lubTmpJudg[cPVIn2] = 2;
		}
	}
	else if(lswPowerAC > (INT32S)9 * lswPowerACMax / 10)
	{
		if(lswPowerDC[cPVIn1] > lswPVRatedPower[cPVIn1])
		{
			lswDeltVolt[cPVIn1] = (INT32S)(lswPowerDC[cPVIn1] - lswPVRatedPower[cPVIn1]) * lswPVVolt[cPVIn1] / lswPVRatedPower[cPVIn1] >> 3;
			lubTmpJudg[cPVIn1] = 1;
		}
		else if(gubCurrLimitCnt[cPVIn1] >= 100 || gubCurrLimitCnt2[cPVIn1] >= 100)
		{
			lswDeltVolt[cPVIn1] = lswPVVolt[cPVIn1] - luwPVRef[cPVIn1] - c1PerPVVoltMax;
			lubTmpJudg[cPVIn1] = 0;
		}
		else if((luwPVRef[cPVIn1] > lswPVVolt[cPVIn1] + cPVReverseVolt) && (lswPowerDC[cPVIn1] < cPVReversePower))		//mppt反追时,当PVref大于开路电压且输入功率很小进入
		{
			if(lubPVChangeCnt[cPVIn1] == 0)
			{
				lubPVChangeCnt[cPVIn1]++;
				lswPVVoltBase[cPVIn1] = lswPVVolt[cPVIn1];
			}
			else if(lswPVVoltBase[cPVIn1] > lswPVVolt[cPVIn1] + cPVRvsBackVolt || lswPVVoltBase[cPVIn1] + cPVRvsBackVolt < lswPVVolt[cPVIn1])
			{
				lubPVChangeCnt[cPVIn1] = 0;
			}
			else if(++lubPVChangeCnt[cPVIn1] >= 5)
			{
				luwPVRef[cPVIn1] = lswPVVolt[cPVIn1] - c1PerPVVoltMax;
				lubPVChangeCnt[cPVIn1] = 0;
			}
		}
		else
		{
			if(lswPVVoltPre[cPVIn1] != lswPVVolt[cPVIn1])
			{
				lslTemp = (INT32S)cPVVoltMax * (lswPVPowerPre[cPVIn1] - lswPowerDC[cPVIn1]) / (lswPVVoltPre[cPVIn1] - lswPVVolt[cPVIn1]);
			}
			else
			{
				lslTemp = (INT32S)cPVVoltMax * (lswPVPowerPre[cPVIn1] - lswPowerDC[cPVIn1]);
			}

			if(lslTemp > 0)
			{
				lubTmpJudg[cPVIn1] = 1;
			}
			else
			{
				lubTmpJudg[cPVIn1] = 0;
			}
			lslTemp /= lswPVRatedPower[cPVIn1];
			lslTemp *= lswPowerACMax - lswPowerAC;
			if(lswPowerACMax != 0)
			{
				lslTemp /= lswPowerACMax;
			}
			lswDeltVolt[cPVIn1] = lslTemp * lswPVVolt[cPVIn1] / 200;
		}
		
		if(lswPowerDC[cPVIn2] > lswPVRatedPower[cPVIn2])
		{
			lswDeltVolt[cPVIn2] = (INT32S)(lswPowerDC[cPVIn2] - lswPVRatedPower[cPVIn2]) * lswPVVolt[cPVIn2] / lswPVRatedPower[cPVIn2] >> 3;
			lubTmpJudg[cPVIn2] = 1;
		}
		else if(gubCurrLimitCnt[cPVIn2] >= 100 || gubCurrLimitCnt2[cPVIn2] >= 100)
		{
			lswDeltVolt[cPVIn2] = lswPVVolt[cPVIn2] - luwPVRef[cPVIn2] - c1PerPVVoltMax;
			lubTmpJudg[cPVIn2] = 0;
		}
		else if((luwPVRef[cPVIn2] > lswPVVolt[cPVIn2] + cPVReverseVolt) && (lswPowerDC[cPVIn2] < cPVReversePower))		//mppt反追时,当PVref大于开路电压且输入功率很小进入
		{
			if(lubPVChangeCnt[cPVIn2] == 0)
			{
				lubPVChangeCnt[cPVIn2]++;
				lswPVVoltBase[cPVIn2] = lswPVVolt[cPVIn2];
			}
			else if(lswPVVoltBase[cPVIn2] > lswPVVolt[cPVIn2] + cPVRvsBackVolt || lswPVVoltBase[cPVIn2] + cPVRvsBackVolt < lswPVVolt[cPVIn2])
			{
				lubPVChangeCnt[cPVIn2] = 0;
			}
			else if(++lubPVChangeCnt[cPVIn2] >= 5)
			{
				luwPVRef[cPVIn2] = lswPVVolt[cPVIn2] - c1PerPVVoltMax;
				lubPVChangeCnt[cPVIn2] = 0;
			}
		}
		else
		{
			if(lswPVVoltPre[cPVIn2] != lswPVVolt[cPVIn2])
			{
				lslTemp = (INT32S)cPVVoltMax * (lswPVPowerPre[cPVIn2] - lswPowerDC[cPVIn2]) / (lswPVVoltPre[cPVIn2] - lswPVVolt[cPVIn2]);
			}
			else
			{
				lslTemp = (INT32S)cPVVoltMax * (lswPVPowerPre[cPVIn2] - lswPowerDC[cPVIn2]);
			}

			if(lslTemp > 0)
			{
				lubTmpJudg[cPVIn2] = 1;
			}
			else
			{
				lubTmpJudg[cPVIn2] = 0;
			}
			lslTemp /= lswPVRatedPower[cPVIn2];
			lslTemp *= lswPowerACMax - lswPowerAC;
			if(lswPowerACMax != 0)
			{
				lslTemp /= lswPowerACMax;
			}
			lswDeltVolt[cPVIn2] = lslTemp * lswPVVolt[cPVIn2] / 200;
		}
	}
	else
	{
		if(lswPowerDC[cPVIn1] > lswPVRatedPower[cPVIn1])
		{
			lswDeltVolt[cPVIn1] = (INT32S)(lswPowerDC[cPVIn1] - lswPVRatedPower[cPVIn1]) * lswPVVolt[cPVIn1] / lswPVRatedPower[cPVIn1] >> 3;
			lubTmpJudg[cPVIn1] = 1;
		}
		else if(gubCurrLimitCnt[cPVIn1] >= 100 || gubCurrLimitCnt2[cPVIn1] >= 100)
		{
			lswDeltVolt[cPVIn1] = lswPVVolt[cPVIn1] - luwPVRef[cPVIn1] - c1PerPVVoltMax;
			lubTmpJudg[cPVIn1] = 0;
		}
		else if((luwPVRef[cPVIn1] > lswPVVolt[cPVIn1] + cPVReverseVolt) && (lswPowerDC[cPVIn1] < cPVReversePower))		//mppt反追时,当PVref大于开路电压且输入功率很小进入
		{
			if(lubPVChangeCnt[cPVIn1] == 0)
			{
				lubPVChangeCnt[cPVIn1]++;
				lswPVVoltBase[cPVIn1] = lswPVVolt[cPVIn1];
			}
			else if(lswPVVoltBase[cPVIn1] > lswPVVolt[cPVIn1] + cPVRvsBackVolt || lswPVVoltBase[cPVIn1] + cPVRvsBackVolt < lswPVVolt[cPVIn1])
			{
				lubPVChangeCnt[cPVIn1] = 0;
			}
			else if(++lubPVChangeCnt[cPVIn1] >= 5)
			{
				luwPVRef[cPVIn1] = lswPVVolt[cPVIn1] - c1PerPVVoltMax;
				lubPVChangeCnt[cPVIn1] = 0;
			}
		}
		else
		{
			if(lswPVVoltPre[cPVIn1] != lswPVVolt[cPVIn1])
			{
				lslTemp = (INT32S)cPVVoltMax * (lswPVPowerPre[cPVIn1] - lswPowerDC[cPVIn1]) / (lswPVVoltPre[cPVIn1] - lswPVVolt[cPVIn1]);
			}
			else
			{
				lslTemp = (INT32S)cPVVoltMax * (lswPVPowerPre[cPVIn1] - lswPowerDC[cPVIn1]);
			}

			if(lslTemp > 0)
			{
				lubTmpJudg[cPVIn1] = 1;
			}
			else
			{
				lubTmpJudg[cPVIn1] = 0;
			}
			lslTemp /= lswPVRatedPower[cPVIn1];
			lswDeltVolt[cPVIn1] = lslTemp * lswPVVolt[cPVIn1] / 2000;
		}
		
		if(lswPowerDC[cPVIn2] > lswPVRatedPower[cPVIn2])
		{
			lswDeltVolt[cPVIn2] = (INT32S)(lswPowerDC[cPVIn2] - lswPVRatedPower[cPVIn2]) * lswPVVolt[cPVIn2] / lswPVRatedPower[cPVIn2] >> 3;
			lubTmpJudg[cPVIn2] = 1;
		}
		else if(gubCurrLimitCnt[cPVIn2] >= 100 || gubCurrLimitCnt2[cPVIn2] >= 100)
		{
			lswDeltVolt[cPVIn2] = lswPVVolt[cPVIn2] - luwPVRef[cPVIn2] - c1PerPVVoltMax;
			lubTmpJudg[cPVIn2] = 0;
		}
		else if((luwPVRef[cPVIn2] > lswPVVolt[cPVIn2] + cPVReverseVolt) && (lswPowerDC[cPVIn2] < cPVReversePower))		//mppt反追时,当PVref大于开路电压且输入功率很小进入
		{
			if(lubPVChangeCnt[cPVIn2] == 0)
			{
				lubPVChangeCnt[cPVIn2]++;
				lswPVVoltBase[cPVIn2] = lswPVVolt[cPVIn2];
			}
			else if(lswPVVoltBase[cPVIn2] > lswPVVolt[cPVIn2] + cPVRvsBackVolt || lswPVVoltBase[cPVIn2] + cPVRvsBackVolt < lswPVVolt[cPVIn2])
			{
				lubPVChangeCnt[cPVIn2] = 0;
			}
			else if(++lubPVChangeCnt[cPVIn2] >= 5)
			{
				luwPVRef[cPVIn2] = lswPVVolt[cPVIn2] - c1PerPVVoltMax;
				lubPVChangeCnt[cPVIn2] = 0;
			}
		}
		else
		{
			if(lswPVVoltPre[cPVIn2] != lswPVVolt[cPVIn2])
			{
				lslTemp = (INT32S)cPVVoltMax * (lswPVPowerPre[cPVIn2] - lswPowerDC[cPVIn2]) / (lswPVVoltPre[cPVIn2] - lswPVVolt[cPVIn2]);
			}
			else
			{
				lslTemp = (INT32S)cPVVoltMax * (lswPVPowerPre[cPVIn2] - lswPowerDC[cPVIn2]);
			}

			if(lslTemp > 0)
			{
				lubTmpJudg[cPVIn2] = 1;
			}
			else	
			{
				lubTmpJudg[cPVIn2] = 0;
			}
			lslTemp /= lswPVRatedPower[cPVIn2];
			lswDeltVolt[cPVIn2] = lslTemp * lswPVVolt[cPVIn2] / 2000;
		}
	}
	
	if(StateFlg.bit.ubMPPTFlag == 1)
	{
		lswDeltVolt[cPVIn1] = c0V;
		lswDeltVolt[cPVIn2] = c0V;
	}
	else
	{
		if(lubTmpJudg[cPVIn1] == 2)
		{
			lswDeltVolt[cPVIn1] = c0V;
		}
		else if(lswDeltVolt[cPVIn1] > c0V || (lswDeltVolt[cPVIn1] == c0V && lubTmpJudg[cPVIn1] == 1))
		{
			if(lswDeltVolt[cPVIn1] > c1PerPVVoltMax)
			{
				lswDeltVolt[cPVIn1] = c1PerPVVoltMax;
			}
			else if(StateFlg.bit.ubMPPTFlag == 2)				//允许小步长调整
			{
				if(lswDeltVolt[cPVIn1] < c0Per02PVVoltMax)
				{
					lswDeltVolt[cPVIn1] = c0Per02PVVoltMax;
				}
			}
			else
			{
				if(lswDeltVolt[cPVIn1] < c0Per1PVVoltMax)
				{
					lswDeltVolt[cPVIn1] = c0Per1PVVoltMax;
				}
			}
		}
		else
		{
			if(lswDeltVolt[cPVIn1] < -c1PerPVVoltMax)
			{
				lswDeltVolt[cPVIn1] = -c1PerPVVoltMax;
			}
			else if(StateFlg.bit.ubMPPTFlag == 2)
			{
				if(lswDeltVolt[cPVIn1] > -c0Per02PVVoltMax)
				{
					lswDeltVolt[cPVIn1] = -c0Per02PVVoltMax;
				}
			}
			else
			{
				if(lswDeltVolt[cPVIn1] > -c0Per1PVVoltMax)
				{
					lswDeltVolt[cPVIn1] = -c0Per1PVVoltMax;
				}
			}
		}

		if(lubTmpJudg[cPVIn2] == 2)
		{
			lswDeltVolt[cPVIn2] = c0V;
		}
		else if(lswDeltVolt[cPVIn2] > c0V || (lswDeltVolt[cPVIn2] == c0V && lubTmpJudg[cPVIn2] == 1))
		{
			if(lswDeltVolt[cPVIn2] > c1PerPVVoltMax)
			{
				lswDeltVolt[cPVIn2] = c1PerPVVoltMax;
			}
			else if(StateFlg.bit.ubMPPTFlag == 2)
			{
				if(lswDeltVolt[cPVIn2] < c0Per02PVVoltMax)
				{
					lswDeltVolt[cPVIn2] = c0Per02PVVoltMax;
				}
			}
			else
			{
				if(lswDeltVolt[cPVIn2] < c0Per1PVVoltMax)
				{
					lswDeltVolt[cPVIn2] = c0Per1PVVoltMax;
				}
			}
		}
		else
		{
			if(lswDeltVolt[cPVIn2] < -c1PerPVVoltMax)
			{
				lswDeltVolt[cPVIn2] = -c1PerPVVoltMax;
			}
			else if(StateFlg.bit.ubMPPTFlag == 2)
			{
				if(lswDeltVolt[cPVIn2] > -c0Per02PVVoltMax)
				{
					lswDeltVolt[cPVIn2] = -c0Per02PVVoltMax;
				}
			}
			else
			{
				if(lswDeltVolt[cPVIn2] > -c0Per1PVVoltMax)
				{
					lswDeltVolt[cPVIn2] = -c0Per1PVVoltMax;
				}
			}
		}
	}
	
	lswPVVoltPre[cPVIn1] = lswPVVolt[cPVIn1];
	lswPVVoltPre[cPVIn2] = lswPVVolt[cPVIn2];
	lswPVPowerPre[cPVIn1] = lswPowerDC[cPVIn1];
	lswPVPowerPre[cPVIn2] = lswPowerDC[cPVIn2];
	gubCurrLimitCnt[cPVIn1] = 0;
	gubCurrLimitCnt[cPVIn2] = 0;
	gubCurrLimitCnt2[cPVIn1] = 0;
	gubCurrLimitCnt2[cPVIn2] = 0;
	luwPVRef[cPVIn1] += lswDeltVolt[cPVIn1];
	luwPVRef[cPVIn2] += lswDeltVolt[cPVIn2];
	if(luwPVRef[cPVIn1] <  cMinMPPVolt)
	{
		luwPVRef[cPVIn1] = cMinMPPVolt;
	}
	else if(luwPVRef[cPVIn1] > cMaxMPPVolt)
	{
		luwPVRef[cPVIn1] =  cMaxMPPVolt;
	}
	if(luwPVRef[cPVIn2] <  cMinMPPVolt)
	{
		luwPVRef[cPVIn2] = cMinMPPVolt;
	}
	else if(luwPVRef[cPVIn2] > cMaxMPPVolt)
	{
		luwPVRef[cPVIn2] =  cMaxMPPVolt;
	}
}

void sCalDeltVolt(INT8U lubInputType,INT16S lswPowerAC,INT16S *lswPowerDC,INT16S lswPowerACMax,INT16S *lswPVVolt,INT16U *luwPVRef)
{
	INT8U lubMPPScanStop;
	
	if(PowerDecreaseFlg.bit.ubMPPScan == 1)
	{
		if(PowerDecreaseFlg.bit.ubMPPScanStep == 0)	//第一阶段，关闭所有驱动
		{
			PowerDecreaseFlg.bit.ubMPPScanStep = 1;
		}
		else if(PowerDecreaseFlg.bit.ubMPPScanStep == 1)	//第二阶段，设置初始参考电压、最小扫描电压，最大功率清0
		{
			luwPVRef[cPVIn1] = lswPVVolt[cPVIn1] - cMPPScanStep;
			luwPVRef[cPVIn2] = lswPVVolt[cPVIn2] - cMPPScanStep;
			lswMPPMin[cPVIn1] = lswPVVolt[cPVIn1] / 5 * 3;
			lswMPPMin[cPVIn2] = lswPVVolt[cPVIn2] / 5 * 3;
			lswMpptWatMax[cPVIn1] = c0W;
			lswMpptWatMax[cPVIn2] = c0W;
			lswMPPVolt[cPVIn1] = cMaxMPPVolt;
			lswMPPVolt[cPVIn2] = cMaxMPPVolt;

			if(lswMPPMin[cPVIn1] < cMinMPPVolt)
			{
				lswMPPMin[cPVIn1] = cMinMPPVolt;
			}
			else if(lswMPPMin[cPVIn1] > cMaxMPPVolt)
			{
				lswMPPMin[cPVIn1] = cMaxMPPVolt;
			}
			
			if(lswMPPMin[cPVIn2] < cMinMPPVolt)
			{
				lswMPPMin[cPVIn2] = cMinMPPVolt;
			}
			else if(lswMPPMin[cPVIn2] > cMaxMPPVolt)
			{
				lswMPPMin[cPVIn2] = cMaxMPPVolt;
			}
/*******************************************************************/
			cInvOn();
			StateFlg.bit.ub_mpptscan_invoff = 0;
			
/*******************************************************************/
			PowerDecreaseFlg.bit.ubMPPScanStep = 2;
		}
		else if(PowerDecreaseFlg.bit.ubMPPScanStep == 2)	//第三阶段，以2.5V为步长减小PV参考电压，记录最大功率点电压
		{
			StateFlg.bit.ub_mpptscan_invoff = 0;
			if(lswPowerDC[cPVIn1] > lswMpptWatMax[cPVIn1])
			{
				lswMpptWatMax[cPVIn1] = lswPowerDC[cPVIn1];
				lswMPPVolt[cPVIn1] = luwPVRef[cPVIn1];
			}
			if(lswPowerDC[cPVIn2] > lswMpptWatMax[cPVIn2])
			{
				lswMpptWatMax[cPVIn2] = lswPowerDC[cPVIn2];
				lswMPPVolt[cPVIn2] = luwPVRef[cPVIn2];
			}
			
			lubMPPScanStop = 0;
			if(lswPowerDC[cPVIn1] < lswPVRatedPower[cPVIn1])
			{
				luwPVRef[cPVIn1] -= cMPPScanStep;
				lubMPPScanStop++;
			}
			if(lswPowerDC[cPVIn2] < lswPVRatedPower[cPVIn2])
			{
				luwPVRef[cPVIn2] -= cMPPScanStep;
				lubMPPScanStop++;
			}

			if((luwPVRef[cPVIn1] < lswMPPMin[cPVIn1] && luwPVRef[cPVIn2] < lswMPPMin[cPVIn2])
			 || lswPowerAC > guwRatedPower || lubMPPScanStop == 0)
			{
				PowerDecreaseFlg.bit.ubMPPScanStep = 3;
			}
		}
		else	//第四阶段，PV参考电压以10V为步长回升至最大功率点电压
		{
			luwPVRef[cPVIn1] += cMPPScanBackStep;
			luwPVRef[cPVIn2] += cMPPScanBackStep;
			if(luwPVRef[cPVIn1] > lswMPPVolt[cPVIn1])
			{
				luwPVRef[cPVIn1] = lswMPPVolt[cPVIn1];
				if(luwPVRef[cPVIn2] > lswMPPVolt[cPVIn2])
				{
					luwPVRef[cPVIn2] = lswMPPVolt[cPVIn2];
					PowerDecreaseFlg.bit.ubMPPScan = 0;
					PowerDecreaseFlg.bit.ubMPPScanStep = 0;
				}
			}
			else if(luwPVRef[cPVIn2] > lswMPPVolt[cPVIn2])
			{
				luwPVRef[cPVIn2] = lswMPPVolt[cPVIn2];
			}
		}

		if(luwPVRef[cPVIn1] < lswMPPMin[cPVIn1])
		{
			luwPVRef[cPVIn1] = lswMPPMin[cPVIn1];
		}
		else if(luwPVRef[cPVIn1] > cMaxMPPVolt)
		{
			luwPVRef[cPVIn1] = cMaxMPPVolt;
		}
		
		if(luwPVRef[cPVIn2] < lswMPPMin[cPVIn2])
		{
			luwPVRef[cPVIn2] = lswMPPMin[cPVIn2];
		}
		else if(luwPVRef[cPVIn2] > cMaxMPPVolt)
		{
			luwPVRef[cPVIn2] = cMaxMPPVolt;
		}
		
		lswPVVoltPre[cPVIn1] = lswPVVolt[cPVIn1];
		lswPVVoltPre[cPVIn2] = lswPVVolt[cPVIn2];
		lswPVPowerPre[cPVIn1] = lswPowerDC[cPVIn1];
		lswPVPowerPre[cPVIn2] = lswPowerDC[cPVIn2];
	}
	else if(lubInputType <= cPVIn2)
	{
		sCalDeltVoltSingle(lubInputType,lswPowerAC,lswPowerDC,lswPowerACMax,lswPVVolt,luwPVRef);
	}
	else
	{
		sCalDeltVoltBoth(lswPowerAC,lswPowerDC,lswPowerACMax,lswPVVolt,luwPVRef);
	}
}

void sSetMPPTPre(INT8U lubInputType,INT16S lswPVVoltPreTemp,INT16S lswPVPowerPreTemp)
{
	lswPVVoltPre[lubInputType] = lswPVVoltPreTemp;
	lswPVPowerPre[lubInputType] = lswPVPowerPreTemp;
}

void sSetPVRatedPower(INT16S lswPV1RatedPower,INT16S lswPV2RatedPower)
{
	lswPVRatedPower[cPVIn1] = lswPV1RatedPower;
	lswPVRatedPower[cPVIn2] = lswPV2RatedPower;

	if(lswPVRatedPower[cPVIn1] < c1W)
	{
		lswPVRatedPower[cPVIn1] = c1W;
	}
	if(lswPVRatedPower[cPVIn2] < c1W)
	{
		lswPVRatedPower[cPVIn2] = c1W;
	}
}
#endif

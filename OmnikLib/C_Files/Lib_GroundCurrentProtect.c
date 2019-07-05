/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：GroundCurrentProtect.c
 * 修改人：陈波
 * 描述：漏电流自检、漏电流保护
 * 初版日期：2014.10.29
 * 修改内容：
 * 1.1版--变量定义时，不附值；附值非零值在初始化函数中     舒吉祥
 * 1.2版--常量定义修改    	    舒吉祥20140305
 * 1.3版--漏电流平均值计算处更改    舒吉祥20140324
 * 1.4版--定点形式保护处逻辑错误更改    舒吉祥20140325
 * 1.5版--将0mA偏置的计算时间缩短至100个周期
 * 1.6版--修正了漏电流故障无法恢复的bug和漏电流交流成分计算错误的bug
 * 1.7版--增加整型的快速保护功能
 * 1.8版--include目录改为在项目中设定；
 * 1.9版--定义一个函数指针，指向开方函数；
 * 2.0版--修改状态标志位名称,将计数上限改为宏定义值。
 * 2.1版-- cGround300msLimit：300ms的计数限值
 * 		   cGround150msLimit：150ms的计数限值
 * 		   cGroundCurrSaveNum：漏电流历史数据长度
 * 		   cGround30mAPreNum：30mA比较时用的历史数据位置，建议为cGround300msLimit + 3(10ms为3，20ms为2)
 * 		   cGround60mAPreNum：60mA比较时用的历史数据位置，建议为cGround150msLimit + 3(10ms为3，20ms为2)
 * 		   cGround150mAPreNum：150mA比较时用的历史数据位置，建议为3(10ms为3，20ms为2)
 * 2.2版--修改30mA，60mA,150mA跳变判断条件
 *			150mA跳变条件判断方法(30mA,60mA同理)
 *			判断前漏电流平均值x1；当前漏电流平均值x1； 判断前漏电流交流分量y1；当前漏电流交流分量y2；
 *			x2>0 && x1<0	x2+y2-x1-y1 > 150mA;
 *			x2>0 && x1>0	x2+y2-x1-y1 > 150mA && x2+y2>(-x1)+y2;
 *			x2<0 && x1<0    (-x2)+y2+x1-y1 > 150mA;
 *			x2<0 && x1>0	(-x2)+y2+x1-y1 > 150mA && (-x2)+y2>x1+y1;
 ********************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"

/********************************************************************
 * Local Types
 ********************************************************************/
#ifdef FloatEnable
float lfGroundIAC;
float lfGroundAvg;
float lfGroundIACOld[cGroundCurrSaveNum];
float lfGroundAvgOld[cGroundCurrSaveNum];
float lfGround0mASum;
float lfGround0mAAvg;
#else
INT16U luwGroundIAC;
INT16U luwGroundRmsPlusAvg;
INT16U luwGroundRmsPlusAvgOld[cGroundCurrSaveNum];
INT16S lswGround0mAAvg;
INT32S lslGround0mASum;
#endif

INT8U lubGround300msCnt;
INT8U lubGround150msCnt;
INT8U lubGroundAvg300msCnt;
INT8U lubGFCI0mAFaultCnt;
INT8U lubGFCI50mAFaultCnt;
INT8U lub0mACnt;

INT8S lsbGroundCnt;

INT16U luwGroundCurrBackCnt;

#ifdef FloatEnable
/********************************************************************
 * Global Types
 ********************************************************************/
pFuncf sSqrtFunc;
#else
INT16U swRoot(INT32U lulInput);
#endif

#ifdef FloatEnable

/*****************************************************************************
 * 函数功能：0mA漏电流自检。
 * 函数输入：漏电流的有效值。
 * 函数描述：如果漏电流的有效值连续超过40mA，则置漏电流设备故障标志位和设备故障标志
 * 			 位。
 ****************************************************************************/
void sGFCI0mAChk(float lfGroundIRms)
{
	if(lfGroundIRms > cGround0mATestLimit)
	{
		lubGFCI0mAFaultCnt++;
		if(lubGFCI0mAFaultCnt > 5)
		{
			Error.bit.mubGFCIDeviveFault = 1;
			lubGFCI0mAFaultCnt = 0;
		}
	}
	else
	{
		lubGFCI0mAFaultCnt = 0;
	}
}

/*****************************************************************************
 * 函数功能：50mA漏电流自检。
 * 函数输入：漏电流的有效值。
 * 函数描述：如果漏电流的有效值连续超过70mA或者低于30mA，则置漏电流设备故障标志位
 * 			 和设备故障标志位。
 ****************************************************************************/
void sGFCI50mAChk(float lfGroundIRms)
{
	if((lfGroundIRms > cGround50mATestLimitU) || (lfGroundIRms < cGround50mATestLimitD))
	{
		lubGFCI50mAFaultCnt++;
		if(lubGFCI50mAFaultCnt > 5)
		{
			Error.bit.mubGFCIDeviveFault = 1;
			lubGFCI50mAFaultCnt = 0;
		}
	}
	else
	{
		lubGFCI50mAFaultCnt = 0;
	}
}

/*****************************************************************************
 * 函数功能：计算漏电流值。
 * 函数输入：漏电流的有效值和漏电流的平均值。
 * 函数描述：根据漏电流的有效值和平均值，计算漏电流的交流分量有效值，并移位存储漏电
 * 			 流交流分量和直流分量的和。
 ****************************************************************************/
void sCalGroundCurrent(float lfGroundIRms, float lfGroundIAvg)
{
	if(lfGroundIRms > lfGroundIAvg && lfGroundIRms > -lfGroundIAvg)
	{
		lfGroundIAC = lfGroundIRms * lfGroundIRms - lfGroundIAvg * lfGroundIAvg;
		lfGroundIAC = sSqrtFunc(lfGroundIAC);
	}
	else
	{
		lfGroundIAC = cGround0mA;
	}
	
	lfGroundAvg = lfGroundIAvg;

	for(lsbGroundCnt = cGround30mAPreNum;lsbGroundCnt > 0;lsbGroundCnt--)
	{
		lfGroundIACOld[lsbGroundCnt] = lfGroundIACOld[lsbGroundCnt - 1];
		lfGroundAvgOld[lsbGroundCnt] = lfGroundAvgOld[lsbGroundCnt - 1];
	}
	
	lfGroundIACOld[0] = lfGroundIAC;
	lfGroundAvgOld[0] = lfGroundAvg;
}

/*****************************************************************************
 * 函数功能：150mA漏电流突变量保护。
 * 函数输入：无。
 * 函数描述：当前漏电流的交流分量和直流分量的和的突变量超过120mA时，置漏电流故障标
 * 			 志位。
 ****************************************************************************/
void sGFCIProtectFast(void)
{
	INT8U lubStatus;
	float lfGround150mAJudge;
	float lfDeltIACPlusIAvg150mA;
	
	if(Error.bit.mubGroundCurrentFault == 0)
	{
		if(lfGroundAvg > cGround0mA)
		{	
			lubStatus = 1;
			lfGround150mAJudge = lfGroundIAC + lfGroundAvg - lfGroundAvgOld[cGround150mAPreNum] - lfGroundIACOld[cGround150mAPreNum];
			if(lfGroundAvgOld[cGround150mAPreNum] < cGround0mA)
			{
				lubStatus = 3;
				lfDeltIACPlusIAvg150mA = lfGroundIAC  + lfGroundAvg + lfGroundAvgOld[cGround150mAPreNum] - lfGroundIACOld[cGround150mAPreNum];
			}
		}
		else
		{
			lubStatus = 2;
			lfGround150mAJudge = lfGroundIAC - lfGroundAvg + lfGroundAvgOld[cGround150mAPreNum] - lfGroundIACOld[cGround150mAPreNum];
			if(lfGroundAvgOld[cGround150mAPreNum] > cGround0mA)
			{
				lubStatus = 4;
				lfDeltIACPlusIAvg150mA = lfGroundIAC  - lfGroundAvg - lfGroundAvgOld[cGround150mAPreNum] - lfGroundIACOld[cGround150mAPreNum];
			}
		}

		if((lubStatus <= 2 && lfGround150mAJudge > cGround150mALimit) || (lubStatus >= 3 && lfDeltIACPlusIAvg150mA > cGround0mA && lfGround150mAJudge > cGround150mALimit))
			//150mA  40ms
		{
			Error.bit.mubGroundCurrentFault = 1;
		}
	}
}

/*****************************************************************************
 * 函数功能：300mA漏电流保护、30mA漏电流突变量保护、60mA漏电流突变量保护。
 * 函数输入：漏电流的有效值。
 * 函数描述：无漏电流故障时，漏电流的有效值连续8个控制周期超过120mA时，置漏电流故障
 * 			 标志位；漏电流的交流分量和直流分量的和变化量超过26mA时，置漏电流故障标
 * 			 志位；漏电流的交流分量和直流分量的和变化量超过52mA时，置漏电流故障标志
 * 			 位；发生漏电流故障时，漏电流的有效值连续低于20mA时，清漏电流故障标志位。
 ****************************************************************************/
void sGFCIProtect(float lfGroundIRms)
{
	INT8U lubStatus;
	float lfGround30mAJudge;
	float lfGround60mAJudge;
	float lfDeltIACPlusIAvg30mA;
	float lfDeltIACPlusIAvg60mA;
	
	if(Error.bit.mubGroundCurrentFault == 0)
	{
		if(lfGroundIRms >= cGround300mALimit)
			//300mA  300ms
		{
			lubGroundAvg300msCnt++;
			if(lubGroundAvg300msCnt >= cGround300msLimit)
			{
				Error.bit.mubGroundCurrentFault = 1;
				lubGroundAvg300msCnt = 0;
			}
		}
		else
		{
			lubGroundAvg300msCnt = 0;
		}
		//30mA跳变条件计算
		if(lfGroundAvg > cGround0mA)
		{	
			lubStatus = 1;
			lfGround30mAJudge = lfGroundIAC + lfGroundAvg - lfGroundAvgOld[cGround30mAPreNum] - lfGroundIACOld[cGround30mAPreNum];
			if(lfGroundAvgOld[cGround30mAPreNum] < cGround0mA)
			{
				lubStatus = 3;
				lfDeltIACPlusIAvg30mA = lfGroundIAC  + lfGroundAvg + lfGroundAvgOld[cGround30mAPreNum] - lfGroundIACOld[cGround30mAPreNum];
			}
		}
		else
		{
			lubStatus = 2;
			lfGround30mAJudge = lfGroundIAC - lfGroundAvg + lfGroundAvgOld[cGround30mAPreNum] - lfGroundIACOld[cGround30mAPreNum];
			if(lfGroundAvgOld[cGround30mAPreNum] > cGround0mA)
			{
				lubStatus = 4;
				lfDeltIACPlusIAvg30mA = lfGroundIAC  - lfGroundAvg - lfGroundAvgOld[cGround30mAPreNum] - lfGroundIACOld[cGround30mAPreNum];
			}
		}

		if((lubStatus <= 2 && lfGround30mAJudge > cGround30mALimit) || (lubStatus >= 3 && lfDeltIACPlusIAvg30mA > cGround0mA && lfGround30mAJudge > cGround30mALimit))
			//30mA  300ms
		{
			lubGround300msCnt++;
			if(lubGround300msCnt >= cGround300msLimit)
			{
				Error.bit.mubGroundCurrentFault = 1;
				lubGround300msCnt = 0;
			}
		}
		else if(lubGround300msCnt > 0)
		{
			lubGround300msCnt--;
		}
		
		//60mA跳变条件计算
		if(lfGroundAvg > cGround0mA)
		{	
			lubStatus = 1;
			lfGround60mAJudge = lfGroundIAC + lfGroundAvg - lfGroundAvgOld[cGround60mAPreNum] - lfGroundIACOld[cGround60mAPreNum];
			if(lfGroundAvgOld[cGround60mAPreNum] < cGround0mA)
			{
				lubStatus = 3;
				lfDeltIACPlusIAvg60mA = lfGroundIAC  + lfGroundAvg + lfGroundAvgOld[cGround60mAPreNum] - lfGroundIACOld[cGround60mAPreNum];
			}
		}
		else
		{
			lubStatus = 2;
			lfGround60mAJudge = lfGroundIAC - lfGroundAvg + lfGroundAvgOld[cGround60mAPreNum] - lfGroundIACOld[cGround60mAPreNum];
			if(lfGroundAvgOld[cGround60mAPreNum] > cGround0mA)
			{
				lubStatus = 4;
				lfDeltIACPlusIAvg60mA = lfGroundIAC  - lfGroundAvg - lfGroundAvgOld[cGround60mAPreNum] - lfGroundIACOld[cGround60mAPreNum];
			} 
		}
		
		if((lubStatus <= 2 && lfGround60mAJudge > cGround60mALimit) || (lubStatus >= 3 && lfDeltIACPlusIAvg60mA > cGround0mA && lfGround60mAJudge > cGround60mALimit))
			//60mA  150ms
		{
			lubGround150msCnt++;
			if(lubGround150msCnt >= cGround150msLimit)
			{
				Error.bit.mubGroundCurrentFault = 1;
				lubGround150msCnt = 0;
			}
		}
		else if(lubGround150msCnt > 0)
		{
			lubGround150msCnt--;
		}
		luwGroundCurrBackCnt = 0;
	}
}

/*****************************************************************************
 * 函数功能：初始化漏电流计算和保护的变量。
 * 函数输入：无。
 * 函数描述：将漏电流计算和保护所用的变量、计数器全部赋0。
 ****************************************************************************/
void sInitialGroundCurrCal(void)
{
	for(lsbGroundCnt = 0;lsbGroundCnt < cGroundCurrSaveNum;lsbGroundCnt++)
	{
		lfGroundIACOld[lsbGroundCnt] = cGround0mA;
		lfGroundAvgOld[lsbGroundCnt] = cGround0mA;
	}
	lfGroundIAC = cGround0mA;
	lfGroundAvg = cGround0mA;
	lubGround300msCnt = 0;
	lubGround150msCnt = 0;
	lubGroundAvg300msCnt = 0;
	luwGroundCurrBackCnt = 0;
	lubGFCI0mAFaultCnt = 0;
	lubGFCI50mAFaultCnt = 0;
	lub0mACnt = 0;

	lfGround0mASum = cGround0mA;
	lfGround0mAAvg = cGround0mA;
}

void sGFCICheck(float lfGroundIRms, float lfGroundIAvg)
{
	if(StateFlg.bit.ubGFCIStep == 0)
	{
		sInitialGroundCurrCal();
		StateFlg.bit.ubGFCIStep = 1;
		cGFCI50mATestOff();
	}
	else if(StateFlg.bit.ubGFCIStep == 1)
	{
		if(Error.bit.mubGroundCurrentFault)
		{
			if(lfGroundIRms < cGround0mATestLimit)
			{
				luwGroundCurrBackCnt++;
				if(luwGroundCurrBackCnt > 275)
				{
					Error.bit.mubGroundCurrentFault = 0;
				}
			}
		}
		if(++lub0mACnt <= 100)
		{
			lfGround0mASum += lfGroundIAvg;
		}
		else
		{
			lfGround0mAAvg = lfGround0mASum / 100;
			lub0mACnt = 101;
		}
	}
	else if(StateFlg.bit.ubGFCIStep == 2)
	{
		sGFCI0mAChk(lfGroundIRms);
	}
	else if(StateFlg.bit.ubGFCIStep == 3)
	{
		cGFCI50mATestOn();
	}
	else if(StateFlg.bit.ubGFCIStep == 4)
	{
		sGFCI50mAChk(lfGroundIRms);
	}
	else if(StateFlg.bit.ubGFCIStep == 5)
	{
		cGFCI50mATestOff();
	}
	else if(StateFlg.bit.ubGFCIStep == 6)
	{
		sGFCI0mAChk(lfGroundIRms);
	}
	else if(StateFlg.bit.ubGFCIStep == 7)
	{
	}
	else
	{
		sGFCIProtect(lfGroundIRms);
		sGFCIProtectFast();
	}
}
#else
void sCalGroundCurrent(INT16U luwGroundIRms, INT16S lswGroundIAvg)
{
	INT32U lultemp;
	INT16S lswDeltGroundI;

	//Bug DS-239：解决漏电流交流分量计算错误的bug
	if(lswGroundIAvg >= 0)
	{
		lswDeltGroundI = (INT16S)luwGroundIRms - lswGroundIAvg;
	}
	else
	{
		lswDeltGroundI = (INT16S)luwGroundIRms + lswGroundIAvg;
	}

	if(lswDeltGroundI > 0)
	{
		lultemp = (INT32S)luwGroundIRms * luwGroundIRms - (INT32S)lswGroundIAvg * lswGroundIAvg;
		luwGroundIAC = swRoot(lultemp);
	}
	else
	{
		luwGroundIAC = cGround0mA;
	}

	if(lswGroundIAvg > cGround0mA)
	{
		luwGroundRmsPlusAvg = luwGroundIAC + lswGroundIAvg;
	}
	else
	{
		luwGroundRmsPlusAvg = luwGroundIAC - lswGroundIAvg;
	}

	for(lsbGroundCnt = cGround30mAPreNum;lsbGroundCnt >= 0;lsbGroundCnt--)
	{
		luwGroundRmsPlusAvgOld[lsbGroundCnt + 1] = luwGroundRmsPlusAvgOld[lsbGroundCnt];
	}
	luwGroundRmsPlusAvgOld[0] = luwGroundRmsPlusAvg;
}

void sGFCIProtectFast(void)
{
	if(luwGroundRmsPlusAvg > luwGroundRmsPlusAvgOld[cGround150mAPreNum] + cGround150mALimit)
	{
		Error.bit.mubGroundCurrentFault = 1;
	}
}

void sGFCIProtect(INT16U luwGroundIRms)
{
	if(Error.bit.mubGroundCurrentFault == 0)
	{
		if(luwGroundIRms >= cGround300mALimit)
			//300mA  300ms
		{
			lubGroundAvg300msCnt++;
			if(lubGroundAvg300msCnt >= cGround300msLimit)
			{
				Error.bit.mubGroundCurrentFault = 1;
				lubGroundAvg300msCnt = 0;
			}
		}
		else
		{
			lubGroundAvg300msCnt = 0;
		}

		if(luwGroundRmsPlusAvg > luwGroundRmsPlusAvgOld[cGround30mAPreNum] + cGround30mALimit)
		{
			lubGround300msCnt++;
			if(lubGround300msCnt >= cGround300msLimit)
			{
				Error.bit.mubGroundCurrentFault = 1;
				lubGround300msCnt = 0;
			}
		}
		else
		{
			lubGround300msCnt = 0;
		}

		if(luwGroundRmsPlusAvg > luwGroundRmsPlusAvgOld[cGround60mAPreNum] + cGround60mALimit)
		{
			lubGround150msCnt++;
			if(lubGround150msCnt >= cGround150msLimit)
			{
				Error.bit.mubGroundCurrentFault = 1;
				lubGround150msCnt = 0;
			}
		}
		else
		{
			lubGround150msCnt = 0;
		}
		luwGroundCurrBackCnt = 0;
	}
}

void sInitialGroundCurrCal(void)
{
	for(lsbGroundCnt = 0;lsbGroundCnt < cGroundCurrSaveNum;lsbGroundCnt++)
	{
		luwGroundRmsPlusAvgOld[lsbGroundCnt] = cGround0mA;
	}
	luwGroundIAC = cGround0mA;
	luwGroundRmsPlusAvg = cGround0mA;
	lubGround300msCnt = 0;
	lubGround150msCnt = 0;
	lubGroundAvg300msCnt = 0;
	luwGroundCurrBackCnt = 0;
	lubGFCI0mAFaultCnt = 0;
	lubGFCI50mAFaultCnt = 0;
	lub0mACnt = 0;

	lslGround0mASum = cGround0mA;
	lswGround0mAAvg = cGround0mA;
}

void sGFCI0mAChk(INT16U luwGroundIRms)
{
	if(luwGroundIRms > cGround0mATestLimit)
	{
		lubGFCI0mAFaultCnt++;
		if(lubGFCI0mAFaultCnt > 5)
		{
			Error.bit.mubGFCIDeviveFault = 1;
			lubGFCI0mAFaultCnt = 0;
		}
	}
	else
	{
		lubGFCI0mAFaultCnt = 0;
	}
}

void sGFCI50mAChk(INT16U luwGroundIRms)
{
	if((luwGroundIRms > cGround50mATestLimitU) || (luwGroundIRms < cGround50mATestLimitD))
	{
		lubGFCI50mAFaultCnt++;
		if(lubGFCI50mAFaultCnt > 5)
		{
			Error.bit.mubGFCIDeviveFault = 1;
			lubGFCI50mAFaultCnt = 0;
		}
	}
	else
	{
		lubGFCI50mAFaultCnt = 0;
	}
}

void sGFCICheck(INT16U luwGroundIRms, INT16S lswGroundIAvg)
{
	if(StateFlg.bit.ubGFCIStep == 0)
	{
		sInitialGroundCurrCal();
		StateFlg.bit.ubGFCIStep = 1;
		cGFCI50mATestOff();
	}
	else if(StateFlg.bit.ubGFCIStep == 1)
	{
		//Bug DS-240：解决漏电流故障后无法回复的bug
		if(Error.bit.mubGroundCurrentFault)
		{
			if(luwGroundIRms < cGround0mATestLimit)
			{
				luwGroundCurrBackCnt++;
				if(luwGroundCurrBackCnt > 275)
				{
					Error.bit.mubGroundCurrentFault = 0;
				}
			}
		}
		if(++lub0mACnt <= 100)
		{
			lslGround0mASum += lswGroundIAvg;
		}
		else
		{
			lswGround0mAAvg = lslGround0mASum / 100;
			lub0mACnt = 101;
		}
	}
	else if(StateFlg.bit.ubGFCIStep == 2)
	{
		sGFCI0mAChk(luwGroundIRms);
	}
	else if(StateFlg.bit.ubGFCIStep == 3)
	{
		cGFCI50mATestOn();
	}
	else if(StateFlg.bit.ubGFCIStep == 4)
	{
		sGFCI50mAChk(luwGroundIRms);
	}
	else if(StateFlg.bit.ubGFCIStep == 5)
	{
		cGFCI50mATestOff();
	}
	else if(StateFlg.bit.ubGFCIStep == 6)
	{
		sGFCI0mAChk(luwGroundIRms);
	}
	else if(StateFlg.bit.ubGFCIStep == 7)
	{
	}
	else
	{
		sGFCIProtect(luwGroundIRms);
		sGFCIProtectFast();
	}
}
#endif

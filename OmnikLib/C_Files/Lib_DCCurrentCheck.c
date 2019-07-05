/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Lib_DCCurrentCheck.c
 * 修改人：陈波
 * 修改日期：2014.10.29
 * 描述：并网电流直流分量检测
 * 修改内容：
 * 1.1版--将变量全部设为局部变量；
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"

/********************************************************************
 * Local Types
 ********************************************************************/
#ifdef FloatEnable
float lfDCAvg;
float lfDCIZeroCrossAvg;
float lfDCISum;
float lfDCLimit1;
float lfDCLimit2;
float lfDCBack;
#else
INT16S lswDCAvg;
INT16S lswDCIZeroCrossAvg;
INT32S lslDCISum;
INT16S lswDCLimit1;
INT16S lswDCLimit2;
INT16S lswDCBack;
#endif

INT16U luwDCFault1Cnt[cPhaseNum];
INT16U luwDCFault2Cnt[cPhaseNum];
INT16U luwDCFaultBackCnt;
INT16U luwDCFaultCntLimit1;
INT16U luwDCFaultCntLimit2;
INT16U luwDCFaultBackCntLimit;

/*****************************************************************************
 * 函数名称：设置直流分量保护时间函数。
 * 函数输入：直流分量保护时间1，直流分量保护时间2，直流分量恢复时间。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sSetDCLimitTime(INT16U luwTime1,INT16U luwTime2,INT16U luwBackTime)
{
	luwDCFaultCntLimit1 = luwTime1;
	luwDCFaultCntLimit2 = luwTime2;
	luwDCFaultBackCntLimit = luwBackTime;
}

#ifdef FloatEnable
/*****************************************************************************
 * 函数名称：设置直流分量保护阈值函数。
 * 函数输入：直流分量保护阈值1，直流分量保护阈值2，直流分量恢复阈值。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sSetDCLimit(float lfLimit1,float lflimit2,float lfBackLimit)
{
	lfDCLimit1 = lfLimit1;
	lfDCLimit2 = lflimit2;
	lfDCBack = lfBackLimit;
}

/*****************************************************************************
 * 函数功能：并网电流直流分量保护。
 * 函数输入：无。
 * 函数描述：判断并网电流直流分量是否超过保护阈值，连续超过阈值后，置直流分量故障标
 * 			 志位。
 ****************************************************************************/
INT8U sCurrentDCCheck(const float *lfDCCurr, INT8U lubPhaseNum,INT8U lubError)
{
	INT8U lubPhase;
	
	if(lubError == 0)
	{
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			if(lfDCCurr[lubPhase] > lfDCLimit2 || lfDCCurr[lubPhase] < -lfDCLimit2)
			{
				luwDCFault2Cnt[lubPhase]++;
				luwDCFault1Cnt[lubPhase]++;
				if(luwDCFault2Cnt[lubPhase] >= luwDCFaultCntLimit2)
				{
					luwDCFault2Cnt[lubPhase] = 0;
					luwDCFault1Cnt[lubPhase] = 0;
					return(1);
				}
			}
			else if(lfDCCurr[lubPhase] > lfDCLimit1 || lfDCCurr[lubPhase] < -lfDCLimit1)
			{
				luwDCFault2Cnt[lubPhase] = 0;
				luwDCFault1Cnt[lubPhase]++;
				if(luwDCFault1Cnt[lubPhase] >= luwDCFaultCntLimit1)
				{
					luwDCFault1Cnt[lubPhase] = 0;
					return(1);
				}
			}
			else
			{
				luwDCFault2Cnt[lubPhase] = 0;
				luwDCFault1Cnt[lubPhase] = 0;
			}
		}
		return(0);
	}
	else
	{
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			if(lfDCCurr[lubPhase] > lfDCBack || lfDCCurr[lubPhase] < -lfDCBack)
			{
				luwDCFaultBackCnt = 0;
			}
		}
		
		if(++luwDCFaultBackCnt >= luwDCFaultBackCntLimit)
		{
			luwDCFaultBackCnt = 0;
			return(0);
		}
		return(1);
	}
}
#else
/*****************************************************************************
 * 函数名称：设置直流分量保护阈值函数。
 * 函数输入：直流分量保护阈值1，直流分量保护阈值2，直流分量恢复阈值。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sSetDCLimit(INT16S lswLimit1,INT16S lswlimit2,INT16S lswBackLimit)
{
	lswDCLimit1 = lswLimit1;
	lswDCLimit2 = lswlimit2;
	lswDCBack = lswBackLimit;
}

/*****************************************************************************
 * 函数功能：并网电流直流分量保护。
 * 函数输入：无。
 * 函数描述：判断并网电流直流分量是否超过保护阈值，连续超过阈值后，置直流分量故障标
 * 			 志位。
 ****************************************************************************/
INT8U sCurrentDCCheck(const INT16S *lswDCCurr, INT8U lubPhaseNum,INT8U lubError)
{
	INT8U lubPhase;
	
	if(lubError == 0)
	{
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			if(lswDCCurr[lubPhase] > lswDCLimit2 || lswDCCurr[lubPhase] < -lswDCLimit2)
			{
				luwDCFault2Cnt[lubPhase]++;
				luwDCFault1Cnt[lubPhase]++;
				if(luwDCFault2Cnt[lubPhase] >= luwDCFaultCntLimit2)
				{
					luwDCFault2Cnt[lubPhase] = 0;
					luwDCFault1Cnt[lubPhase] = 0;
					return(1);
				}
			}
			else if(lswDCCurr[lubPhase] > lswDCLimit1 || lswDCCurr[lubPhase] < -lswDCLimit1)
			{
				luwDCFault2Cnt[lubPhase] = 0;
				luwDCFault1Cnt[lubPhase]++;
				if(luwDCFault1Cnt[lubPhase] >= luwDCFaultCntLimit1)
				{
					luwDCFault1Cnt[lubPhase] = 0;
					return(1);
				}
			}
			else
			{
				luwDCFault2Cnt[lubPhase] = 0;
				luwDCFault1Cnt[lubPhase] = 0;
			}
		}
		return(0);
	}
	else
	{
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			if(lswDCCurr[lubPhase] > lswDCBack || lswDCCurr[lubPhase] < -lswDCBack)
			{
				luwDCFaultBackCnt = 0;
			}
		}
		
		if(++luwDCFaultBackCnt >= luwDCFaultBackCntLimit)
		{
			luwDCFaultBackCnt = 0;
			return(0);
		}
		return(1);
	}
}
#endif

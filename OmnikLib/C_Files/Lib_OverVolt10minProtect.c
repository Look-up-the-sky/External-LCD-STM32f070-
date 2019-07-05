/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：OverVolt10minProtect.c
 * 修改人：陈波
 * 描述：电网10分钟过压保护
 * 初版日期：2014.10.29
 * 更改内容：
 * 1.1版--定点逻辑与浮点逻辑保持一致     舒吉祥20140305
 * 1.2版--c0V应为c0VAC
 * 1.3版--include目录改为在项目中设定；
 * 1.4版--优化算法；
 *******************************************************/

#include "App.h"

#ifdef FloatEnable
#ifndef DebugEnable						//10分钟过压保护用800个字节的ram，如果同时开启Debug模式，ram不够用
#define cMaxVoltNum	200

INT16U luw3sGridVoltCnt;
INT16U luwGridVoltSumCount;

float lfGridVoltBuffer[cPhaseNum][cMaxVoltNum];
float lfGridVolt3sAvg[cPhaseNum];
float lfGridVolt3sSum[cPhaseNum];
float lf10MinGridHigh;
#endif
#else
#define cMaxVoltNum	16

INT16U luw30sGridVoltCnt;
INT16U luwGridVoltSumCount;

INT16U luwGridVoltBuffer[cPhaseNum][cMaxVoltNum];
INT32U lulGridVolt30sAvg[cPhaseNum];
INT32U lulGridVolt30sSum[cPhaseNum];
INT16U luw10MinGridHigh;
#endif

#ifdef FloatEnable
/*****************************************************************************
 * 函数名称：10分钟过压保护值设置函数。
 * 函数输入：10分钟过压保护阈值。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sSet10MinLimit(float lfLimit)
{
#ifndef DebugEnable
	lf10MinGridHigh = lfLimit;
#endif
}

/*****************************************************************************
 * 函数功能：初始化10分钟过压保护的变量。
 * 函数输入：无。
 * 函数描述：将10分钟过压保护的计时器附0，30s平均值附正常的AC值。
 ****************************************************************************/
void s10minVoltInitial(INT8U lubPhaseNum)
{
#ifndef DebugEnable
	INT8U lubPhase;

	for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
	{
		for(luw3sGridVoltCnt = 0;luw3sGridVoltCnt < cMaxVoltNum;luw3sGridVoltCnt++)
		{
			lfGridVoltBuffer[lubPhase][luw3sGridVoltCnt] = cNormalACVolt;
		}
		lfGridVolt3sSum[lubPhase] = c0V;
	}
	luw3sGridVoltCnt = 0;
	luwGridVoltSumCount = 0;
#endif
}

/*****************************************************************************
 * 函数功能：10分钟过压保护。
 * 函数输入：电网电压，电网相数，故障位。
 * 函数描述：计算30s电网电压平均值，移位存储在长度为cMaxVoltNum的数组里，数组的平均
 * 			 值如果超过保护设定值，则置电网电压故障标志位，如果30s的平均值已经恢复到
 * 			 正常范围，则清电网电压故障标志位。
 ****************************************************************************/
INT8U s10minVoltCheck(const float *lfGridVolt, INT8U lubPhaseNum,INT8U lubError)
{
#ifndef DebugEnable
	INT8U i;
	INT8U lubPhase;
	INT8U lubPhaseCount;
	
	if(luwGridVoltSumCount < cT3s)
	{
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			lfGridVolt3sSum[lubPhase] += lfGridVolt[lubPhase];
		}
		luwGridVoltSumCount++;
		return(lubError);
	}
	else
	{
		if(luw3sGridVoltCnt >= cMaxVoltNum)
		{
			luw3sGridVoltCnt =0;
		}
		lubPhaseCount = 0;
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			lfGridVoltBuffer[lubPhase][luw3sGridVoltCnt] = lfGridVolt3sSum[lubPhase] / luwGridVoltSumCount;
			lfGridVolt3sSum[lubPhase] = 0;
			lfGridVolt3sAvg[lubPhase] = 0;
			
			for(i = 0;i < cMaxVoltNum;i++)
			{
				lfGridVolt3sAvg[lubPhase] += lfGridVoltBuffer[lubPhase][i];
			}
			lfGridVolt3sAvg[lubPhase] = lfGridVolt3sAvg[lubPhase] / cMaxVoltNum;
			if((lfGridVoltBuffer[lubPhase][luw3sGridVoltCnt] <= lf10MinGridHigh) && (lubError == 1))
			{
				lubPhaseCount++;
			}
			if(lubPhaseCount >= lubPhaseNum)
			{
				s10minVoltInitial(lubPhaseNum);
				return(0);				
			}		
		}
		
		luwGridVoltSumCount = 0;		
		luw3sGridVoltCnt++;
		
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			if(lfGridVolt3sAvg[lubPhase] > lf10MinGridHigh)
			{
				return(1);
			}
		}
		return(0);
	}
#else
	return(0);
#endif
}
#else
/*****************************************************************************
 * 函数名称：10分钟过压保护值设置函数。
 * 函数输入：10分钟过压保护阈值。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sSet10MinLimit(INT16U luwLimit)
{
	luw10MinGridHigh = luwLimit;
}

/*****************************************************************************
 * 函数功能：初始化10分钟过压保护的变量。
 * 函数输入：无。
 * 函数描述：将10分钟过压保护的计时器附0，30s平均值附正常的AC值。
 ****************************************************************************/
void s10minVoltInitial(INT8U lubPhaseNum)
{
	INT8U lubPhase;

	for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
	{
		for(luw30sGridVoltCnt = 0;luw30sGridVoltCnt < cMaxVoltNum;luw30sGridVoltCnt++)
		{
			luwGridVoltBuffer[lubPhase][luw30sGridVoltCnt] = cNormalACVolt;
		}
		lulGridVolt30sSum[lubPhase] = c0V;
	}
	luw30sGridVoltCnt = 0;
	luwGridVoltSumCount = 0;
}

/*****************************************************************************
 * 函数功能：10分钟过压保护。
 * 函数输入：电网电压，电网相数，故障位。
 * 函数描述：计算30s电网电压平均值，移位存储在长度为cMaxVoltNum的数组里，数组的平均
 * 			 值如果超过保护设定值，则置电网电压故障标志位，如果30s的平均值已经恢复到
 * 			 正常范 围，则清电网电压故障标志位。
 ****************************************************************************/
INT8U s10minVoltCheck(const INT16U *luwGridVolt, INT8U lubPhaseNum,INT8U lubError)
{
	INT8U i;
	INT8U lubPhase;
	INT8U lubPhaseCount;
	
	if(luwGridVoltSumCount < cT30s)
	{
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			lulGridVolt30sSum[lubPhase] += luwGridVolt[lubPhase];
		}
		luwGridVoltSumCount++;
		return(lubError);
	}
	else
	{
		if(luw30sGridVoltCnt >= cMaxVoltNum)
		{
			luw30sGridVoltCnt = 0;
		}
		lubPhaseCount = 0;		
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			luwGridVoltBuffer[lubPhase][luw30sGridVoltCnt] = (INT16U)(lulGridVolt30sSum[lubPhase] / luwGridVoltSumCount);
			lulGridVolt30sSum[lubPhase] = 0;
			lulGridVolt30sAvg[lubPhase] = 0;
			for(i = 0;i < cMaxVoltNum;i++)
			{
				lulGridVolt30sAvg[lubPhase] += luwGridVoltBuffer[lubPhase][i];
			}
			lulGridVolt30sAvg[lubPhase] = lulGridVolt30sAvg[lubPhase] / cMaxVoltNum;
			if((lfGridVoltBuffer[lubPhase][luw30sGridVoltCnt] <= luw10MinGridHigh ) && (lubError == 1))
			{
				lubPhaseCount++;
			}
			if(lubPhaseCount >= lubPhaseNum)
			{
				s10minVoltInitial(lubPhaseNum);
				return(0);				
			}			
		}
		luwGridVoltSumCount =0;
		luw30sGridVoltCnt++;		
		
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			if(lulGridVolt30sAvg[lubPhase] > luw10MinGridHigh)
			{
				return(1);
			}
		}
		
		return(0);
	}
}
#endif

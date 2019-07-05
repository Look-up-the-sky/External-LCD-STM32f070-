/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：GridFreqProtect.c
 * 修改人：陈波
 * 修改日期：2014.10.29
 * 描述：过欠频保护
 * 修改内容：
 * 1.1版--将变量全部设为局部变量；
 * 1.2版--取消1s时间标准；
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"
#include "Lib_VoltFreq.h"
#include "Lib_SelfTest.h"

/********************************************************************
 * Local Types
 ********************************************************************/
INT16U luwOverFreqCnt;
INT16U luwOverFreqCnt2;
INT16U luwUnderFreqCnt;
INT16U luwUnderFreqCnt2;
INT16U luwFreqBackCnt;

#ifdef FloatEnable
float lfGridFreqValueLimit[6];
#else
INT16U luwGridFreqHigh1;
INT16U luwGridFreqHigh2;
INT16U luwGridFreqLow1;
INT16U luwGridFreqLow2;
INT16U luwGridFreqBackLow;
INT16U luwGridFreqBackHigh;
#endif

INT16U luwGridFreqTimeLimit[5];

/*****************************************************************************
 * 函数名称：获得频率保护时间函数。
 * 函数输入：保护选择。
 * 函数返回值：保护时间。
 * 函数描述：
 ****************************************************************************/
INT16U sGetFreqTime(INT8U lubTimeNum)
{
	if(lubTimeNum <= cBack)
	{
		return(luwGridFreqTimeLimit[lubTimeNum]);
	}
	else
	{
		return(0);
	}
}

/*****************************************************************************
 * 函数名称：设置频率保护时间函数。
 * 函数输入：保护选择，时间。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sSetFreqTime(INT8U lubTimeNum,INT16U luwTime)
{
	if(lubTimeNum <= cBack)
	{
		luwGridFreqTimeLimit[lubTimeNum] = luwTime;
	}
}
#ifdef FloatEnable
/*****************************************************************************
 * 函数功能：电网频率保护。
 * 函数输入：电网频率，保护时间类型，故障位。
 * 函数描述：当电网频率不在正常范围内时，计时器工作，时间超过规定值后，置电网频率故
 * 			 障标志位；在计时过程中如果电网频率恢复，则清计时器。电网频率故障后，若
 * 			 频率恢复到正常范围，则清除电网频率故障标志位。
 ****************************************************************************/
INT8U sFreqCheck(float lfGridFreq,INT8U lubError,INT8U lubEnableFlag)
{
	if(lubError == 0)
	{
		if(lfGridFreq < lfGridFreqValueLimit[cLow2] && (lubEnableFlag & 0x02))
		{
			luwUnderFreqCnt++;
			luwUnderFreqCnt2++;
			luwOverFreqCnt = 0;
			luwOverFreqCnt2 = 0;
			if(luwUnderFreqCnt2 >= luwGridFreqTimeLimit[cLow2])
			{
				luwUnderFreqCnt2 = 0;
				luwUnderFreqCnt = 0;
				return(1);
			}
		}
		else if(lfGridFreq < lfGridFreqValueLimit[cLow1] && (lubEnableFlag & 0x01))
		{
			luwUnderFreqCnt++;
			luwUnderFreqCnt2 = 0;
			luwOverFreqCnt = 0;
			luwOverFreqCnt2 = 0;
			if(luwUnderFreqCnt >= luwGridFreqTimeLimit[cLow1])
			{
				luwUnderFreqCnt = 0;
				return(1);
			}
		}
		else if(lfGridFreq > lfGridFreqValueLimit[cHigh2] && (lubEnableFlag & 0x02))
		{
			luwUnderFreqCnt = 0;
			luwUnderFreqCnt2 = 0;
			luwOverFreqCnt++;
			luwOverFreqCnt2++;
			if(luwOverFreqCnt2 >= luwGridFreqTimeLimit[cHigh2])
			{
				luwOverFreqCnt = 0;
				luwOverFreqCnt2 = 0;
				return(1);
			}
		}
		else if(lfGridFreq > lfGridFreqValueLimit[cHigh1] && (lubEnableFlag & 0x01))
		{
			luwUnderFreqCnt = 0;
			luwUnderFreqCnt2 = 0;
			luwOverFreqCnt++;
			luwOverFreqCnt2 = 0;
			if(luwOverFreqCnt >= luwGridFreqTimeLimit[cHigh1])
			{
				luwOverFreqCnt = 0;
				return(1);
			}
		}
		else
		{
			sClearSelfTestFlag(0);
			luwUnderFreqCnt = 0;
			luwUnderFreqCnt2 = 0;
			luwOverFreqCnt = 0;
			luwOverFreqCnt2  = 0;
		}
		return(0);
	}
	else
	{
		if((lfGridFreq > lfGridFreqValueLimit[cBackLow]) && (lfGridFreq < lfGridFreqValueLimit[cBackHigh]))
		{
			luwFreqBackCnt++;
			if(luwFreqBackCnt >= luwGridFreqTimeLimit[cBack])
			{
				luwFreqBackCnt = 0;
				return(0);
			}
		}
		else
		{
			luwFreqBackCnt = 0;
		}
		return(1);
	}
}

/*****************************************************************************
 * 函数名称：获得频率阈值函数。
 * 函数输入：保护选择。
 * 函数返回值：保护阈值。
 * 函数描述：
 ****************************************************************************/
float sGetFreqLimit(INT8U lubTimeNum)
{
	if(lubTimeNum <= cBackHigh)
	{
		return(lfGridFreqValueLimit[lubTimeNum]);
	}
	else
	{
		return(0.0f);
	}
}

/*****************************************************************************
 * 函数名称：设置频率阈值函数。
 * 函数输入：保护选择，频率阈值。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sSetFreqLimit(INT8U lubTimeNum,float lfFreq)
{
	if(lubTimeNum <= cBackHigh)
	{
		lfGridFreqValueLimit[lubTimeNum] = lfFreq;
	}
}

#else
/*****************************************************************************
 * 函数功能：电网频率保护。
 * 函数输入：电网频率，保护时间类型，故障位。
 * 函数描述：当电网频率不在正常范围内时，计时器工作，时间超过规定值后，置电网频率故
 * 			 障标志位；在计时过程中如果电网频率恢复，则清计时器。电网频率故障后，若
 * 			 频率恢复到正常范围，则清除电网频率故障标志位。
 ****************************************************************************/
INT8U sFreqCheck(INT16U luwGridFreq,INT8U lubError,INT8U lubEnableFlag)
{
	if(lubError == 0)
	{
		if(luwGridFreq < luwGridFreqLow2)
		{
			luwUnderFreqCnt++;
			luwUnderFreqCnt2++;
			luwOverFreqCnt = 0;
			luwOverFreqCnt2 = 0;
			if(luwUnderFreqCnt2 >= luwGridFreqTimeLimit[cLow2])
			{
				luwUnderFreqCnt = 0;
				luwUnderFreqCnt2 = 0;
				return(1);
			}
		}
		else if(luwGridFreq < luwGridFreqLow1)
		{
			luwUnderFreqCnt++;
			luwUnderFreqCnt2 = 0;
			luwOverFreqCnt = 0;
			luwOverFreqCnt2 = 0;
			if(luwUnderFreqCnt >= luwGridFreqTimeLimit[cLow1])
			{
				luwUnderFreqCnt = 0;
				return(1);
			}
		}
		else if(luwGridFreq > luwGridFreqHigh2)
		{
			luwUnderFreqCnt = 0;
			luwUnderFreqCnt2 = 0;
			luwOverFreqCnt++;
			luwOverFreqCnt2++;
			if(luwOverFreqCnt2 >= luwGridFreqTimeLimit[cHigh2])
			{
				luwOverFreqCnt = 0;
				luwOverFreqCnt2 = 0;
				return(1);
			}
		}
		else if(luwGridFreq > luwGridFreqHigh1)
		{
			luwUnderFreqCnt = 0;
			luwUnderFreqCnt2 = 0;
			luwOverFreqCnt++;
			luwOverFreqCnt2 = 0;
			if(luwOverFreqCnt >= luwGridFreqTimeLimit[cHigh1])
			{
				luwOverFreqCnt = 0;
				return(1);
			}
		}
		else
		{
			luwOverFreqCnt   = 0;
			luwOverFreqCnt2  = 0;
			luwUnderFreqCnt  = 0;
			luwUnderFreqCnt2 = 0;
		}
		return(0);
	}
	else
	{
		if((luwGridFreq > luwGridFreqBackLow) && (luwGridFreq < luwGridFreqBackHigh))
		{
			luwFreqBackCnt++;
			if(luwFreqBackCnt >= luwGridFreqTimeLimit[cBack])
			{
				luwFreqBackCnt = 0;
				return(0);
			}
		}
		else
		{
			luwFreqBackCnt = 0;
		}

		return(1);
	}
}

/*****************************************************************************
 * 函数名称：设置过频阈值函数。
 * 函数输入：过频阈值。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sSetFreqHigh(INT16U luwFreq1,INT16U luwFreq2)
{
	luwGridFreqHigh1 = luwFreq1;
	luwGridFreqHigh2 = luwFreq2;
}

/*****************************************************************************
 * 函数名称：设置欠频阈值函数。
 * 函数输入：欠频阈值。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sSetFreqLow(INT16U luwFreq1,INT16U luwFreq2)
{
	luwGridFreqHigh1 = luwFreq1;
	luwGridFreqHigh2 = luwFreq2;
}

/*****************************************************************************
 * 函数名称：设置恢复频率阈值函数。
 * 函数输入：恢复频率阈值。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sSetFreqBack(INT16U luwFreqLow,INT16U luwFreqHigh)
{
	luwGridFreqBackLow = luwFreqLow;
	luwGridFreqBackHigh = luwFreqHigh;
}
#endif

/*****************************************************************************
 * 函数功能：电网频率保护变量初始化。
 * 函数输入：无。
 * 函数描述：初始化电网频率保护函数中的局部变量。
 ****************************************************************************/
void sFreqCheckInitial(void)
{
}

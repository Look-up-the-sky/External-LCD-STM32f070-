/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Lib_PowerDecreaseByFreq.c
 * 修改人：陈波
 * 修改日期：2014.10.29
 * 描述：过频降载功能
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"
#include "Lib_Communication.h"

/********************************************************************
 * Local Types
 ********************************************************************/
typedef union
{
	INT8U all;
	struct
	{
		INT8U ubFreqDecreaseFlg:1;	//过频降载标志位
		INT8U ubPowerBackByFFlg:1;	//过频功率恢复标志位
		INT8U ubPowerFreezeFlg:1;	//功率冻结标志位
		INT8U ubFreqDecrease2Flg:1;	//欠频降载标志位
		INT8U ubPowerBackByF2Flg:1;	//频功率恢复标志位
		INT8U ubPowerFreeze2Flg:1;	//功率冻结标志位
		INT8U ubReserve:2;
	}bit;
}OVERFREQ;
OVERFREQ OverFreq;

#ifdef FloatEnable
INT16U luwPowerDelayTime;
INT16U luwPowerDelayCnt;

float lfStartFreq;
float lfDecreaseRatio;
float lfIncreaseRatio;
float lfRecoverFreq;
float lfPowerByFreq;
float lfPowerByFreqPre;
float lfPowerFreeze;
float lfPowerFreeze2;
#else
#endif

INT8U lubFreqDecCnt1[2];
INT8U lubFreqDecCnt2[2];

INT16U luwPowerBackByFreqCnt[2];

/********************************************************************
 * Global Types
 ********************************************************************/


/********************************************************************
 * Extern Global Types
 ********************************************************************/


#ifdef FloatEnable
/*****************************************************************************
 * 函数功能：设置过频降载点。
 * 函数输入：过频降载点、过频降载恢复点、降载速率、恢复速率、恢复延时时间。
 * 函数描述：
 ****************************************************************************/
void sSetPowerByFreqLimit(float lfStartFreqTemp, float lfRecoverFreqTemp, float lfDecreaseRatioTemp, float lfIncreaseRatioTemp, INT16U luwPowerDelayTimeTemp)
{
	lfStartFreq = lfStartFreqTemp;
	lfDecreaseRatio = lfDecreaseRatioTemp;
	lfIncreaseRatio = lfIncreaseRatioTemp;
	lfRecoverFreq = lfRecoverFreqTemp;
	luwPowerDelayTime = luwPowerDelayTimeTemp;
}

/*****************************************************************************
 * 函数功能：过频降载初始化。
 * 函数输入：无
 * 函数描述：
 ****************************************************************************/
void sIniPowerByFreq(void)
{
	OverFreq.bit.ubFreqDecreaseFlg = cFalse;
	OverFreq.bit.ubPowerBackByFFlg = cFalse;
	luwPowerDelayCnt = 0;
}

/*****************************************************************************
 * 函数功能：过频降载。
 * 函数输入：电网频率、当前功率百分比。
 * 函数描述：电网频率超过过频降载点时，记录当前功率设为冻结功率，同时，输出功率随着
 * 			 频率的上升按降载速率下降，当频率恢复到过频降载点以下时，功率按照一定的
 * 			 功率斜率上升。
 ****************************************************************************/
float sPowerByFreq(float lfGridFreq, float lfACPower)
{
	if(lfGridFreq > lfStartFreq)
	{
		lubFreqDecCnt2[0] = 0;
		if(++lubFreqDecCnt1[0] >= 3)
		{
			lubFreqDecCnt1[0] = 3;
			OverFreq.bit.ubFreqDecreaseFlg = cTrue;
			OverFreq.bit.ubPowerBackByFFlg = cTrue;
			if(OverFreq.bit.ubPowerFreezeFlg == cFalse)
			{
				lfPowerFreeze = lfACPower;
				lfPowerByFreqPre = lfACPower;
				OverFreq.bit.ubPowerFreezeFlg = cTrue;
				if(lfPowerFreeze > c100Percent)
				{
					lfPowerFreeze = c100Percent;
				}
				lfPowerFreeze2 = lfPowerFreeze;
			}
			luwPowerDelayCnt = 0;
		}
	}
	else if(lfGridFreq < lfStartFreq - c0Hz03)
	{
		lubFreqDecCnt1[0] = 0;
		if(++lubFreqDecCnt2[0] >= 3)
		{
			lubFreqDecCnt2[0] = 3;
			OverFreq.bit.ubFreqDecreaseFlg = cFalse;
			OverFreq.bit.ubPowerFreezeFlg = cFalse;
		}
	}

	if(OverFreq.bit.ubFreqDecreaseFlg)		//过频降载阶段，功率随着频率上升线性下降；
	{
		if(lfGridFreq > lfRecoverFreq)
		{
			lfPowerByFreq = lfPowerFreeze2 - (lfGridFreq - lfStartFreq) * lfDecreaseRatio * lfPowerFreeze2;
			if(lfPowerByFreq > lfPowerByFreqPre)
			{
				lfPowerByFreq = lfPowerByFreqPre;
			}
			lfPowerByFreqPre = lfPowerByFreq;
			lfPowerFreeze = lfPowerByFreq;
		}
		else
		{
			lfPowerByFreq = lfPowerFreeze - (lfGridFreq - lfStartFreq) * lfDecreaseRatio * lfPowerFreeze;
		}
		
		if(lfPowerByFreq > lfPowerFreeze)
		{
			lfPowerByFreq = lfPowerFreeze;
		}
		else if(lfPowerByFreq < c0Percent)
		{
			lfPowerByFreq = c0Percent;
		}
		
		return(lfPowerByFreq);
	}
	else if(OverFreq.bit.ubPowerBackByFFlg)	//过频恢复阶段，功率按一定斜率上升
	{
		if(lfGridFreq < lfRecoverFreq)
		{
			if(++luwPowerDelayCnt >= luwPowerDelayTime)
			{
				luwPowerDelayCnt = luwPowerDelayTime;
				lfPowerByFreq += lfIncreaseRatio / 6000.0f;
				if(lfPowerByFreq > c100Percent)
				{
					lfPowerByFreq = c100Percent;
					OverFreq.bit.ubPowerBackByFFlg = cFalse;
				}
				else if(lfPowerByFreq < c0Percent)
				{
					lfPowerByFreq = c0Percent;
				}
			}
		}
		return(lfPowerByFreq);
	}
	else
	{
		return(c100Percent);
	}
}

/*****************************************************************************
 * 函数功能：欠频降载。
 * 函数输入：电网频率、欠频降载点、降载速率。
 * 函数描述：电网频率低于频降载点时，记录当前功率设为冻结功率，同时，输出功率随着
 * 			 频率的下降按降载速率下降，当频率恢复到过频降载点一下时，功率按照一定的
 * 			 功率斜率上升。
 ****************************************************************************/
float sPowerByFreq2(float lfGridFreq, float lfStartFreq, float lfDecreaseRatio, float lfACPower)
{
	float lfPowerLimit;
	
	if(lfGridFreq < lfStartFreq)
	{
		lubFreqDecCnt2[1] = 0;
		if(++lubFreqDecCnt1[1] >= 3)
		{
			lubFreqDecCnt1[1] = 3;
			OverFreq.bit.ubFreqDecrease2Flg = cTrue;
			OverFreq.bit.ubPowerBackByF2Flg = cTrue;
			luwPowerBackByFreqCnt[1] = 0;
			if(OverFreq.bit.ubPowerFreeze2Flg == 0)
			{
				lfPowerFreeze = lfACPower;
				OverFreq.bit.ubPowerFreeze2Flg = 1;
			}
			if(lfPowerFreeze > c100Percent)
			{
				lfPowerFreeze = c100Percent;
			}
		}
	}
	else
	{
		lubFreqDecCnt1[1] = 0;
		if(++lubFreqDecCnt2[1] >= 3)
		{
			lubFreqDecCnt2[1] = 3;
			OverFreq.bit.ubFreqDecrease2Flg = cFalse;
			OverFreq.bit.ubPowerFreeze2Flg = 0;
		}
	}

	if(OverFreq.bit.ubFreqDecrease2Flg)		//欠频降载阶段，功率随着频率上升线性下降；
	{
		lfPowerByFreq = lfPowerFreeze - (lfStartFreq - lfGridFreq) * lfDecreaseRatio * lfPowerFreeze;
		if(lfPowerByFreq > c100Percent)
		{
			lfPowerByFreq = c100Percent;
		}
		else if(lfPowerByFreq < c0Percent)
		{
			lfPowerByFreq = c0Percent;
		}
		return(lfPowerByFreq);
	}
	else if(OverFreq.bit.ubPowerBackByF2Flg)	//过频恢复阶段，功率按一定斜率上升
	{
		lfPowerLimit = lfPowerFreeze + luwPowerBackByFreqCnt[1] / 60000.0f;

		if(MasterData.data.ubState == cNormalState)
		{
			luwPowerBackByFreqCnt[1]++;
		}
		else
		{
			luwPowerBackByFreqCnt[1] = 0;
		}
		if(luwPowerBackByFreqCnt[1] >= 60000)
		{
			luwPowerBackByFreqCnt[1] = 0;
			OverFreq.bit.ubPowerBackByF2Flg = cFalse;
		}
		
		if(lfPowerLimit > c100Percent)
		{
			lfPowerLimit = c100Percent;
		}
		else if(lfPowerLimit < c0Percent)
		{
			lfPowerLimit = c0Percent;
		}
		
		return(lfPowerLimit);
	}
	else
	{
		return(c100Percent);
	}
}

#else
#endif

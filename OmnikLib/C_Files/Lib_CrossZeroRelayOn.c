/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：CrossZeroRelayOn.c
 * 修改人：蒋骊冬
 * 描述：继电器过零点吸合控制
 * 初版日期：2017.06.22
 * 更改内容：
 *******************************************************/
 
/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"
#include "Lib_CrossZeroRelayOn.h"
#include "Lib_Communication.h"

/********************************************************************
 * Local Types
 ********************************************************************/
INT16U luwCrossZeroDelayCnt;
INT16U luwRelayDelayOnCnt;
INT16U luwRelayDelayOnAdjCnt;

/*****************************************************************************
 * 函数名称：电网过零检测
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：检测继电器吸合延时吸合标志位，在过零中断中将过零标志位置位
 *****************************************************************************/
void sCrossZeroChk(void)
{
	if(StateFlg.bit.ubNRelayDelayOn)
	{
		StateFlg.bit.ubCrossZeroFlg = cTrue;
	}
}

/*****************************************************************************
 * 函数名称：计算继电器过零吸合延时时间
 * 函数输入：电网频率(Hz)、继电器动作时间(us)、延时补偿(us)
 * 函数返回值：无
 * 函数描述：
 *****************************************************************************/
void sCalCrossZeroRelayOn(float lfGridFreq, INT16U luwRelayAction, INT16S lswRelayDelayOnAdj)
{
	INT16U luwTemp;
	luwTemp = 1e+6 / (INT16U)lfGridFreq;
	luwTemp -= luwRelayAction;
	luwTemp -= lswRelayDelayOnAdj;
	luwTemp /= 1e+3 / c20KHz;
	luwRelayDelayOnCnt = luwTemp;
}

/*****************************************************************************
 * 函数名称：继电器过零吸合执行
 * 函数输入：电网频率(Hz)、继电器两端电压(V)
 * 函数返回值：
 * 函数描述：
 *****************************************************************************/
void sActCrossZeroRelayOn(float lfGridFreq, float lfRelayVolt)
{
	float lfTemp;
	/************开始计数，计数结束后，继电器吸合************/
	if(StateFlg.bit.ubNRelayDelayOn == cTrue && StateFlg.bit.ubCrossZeroFlg == cTrue)
	{
		/************计从过零点开始到吸合继电器的时间************/
		luwCrossZeroDelayCnt++;
		if(luwCrossZeroDelayCnt >= luwRelayDelayOnCnt)
		{
			/************吸合继电器************/
			cNRelayOn();
			/************根据继电器两端电压得到继电器吸合时间，判断是否需要补偿************/
			/************判断继电器吸合时机，是否接近0V************/
			if(lfRelayVolt >= -c10V && lfRelayVolt <= c10V)
			{
				luwRelayDelayOnAdjCnt++;
				if(luwRelayDelayOnAdjCnt >= c300usCnt)//检测连续300us(6次)在0点附近
				{
					lfTemp = (float)luwCrossZeroDelayCnt;
					lfTemp -= c300usCnt / 2;
					lfTemp *= 1e+3 / c20KHz;//换算成时间，单位us
					lfTemp -= (float)1e+6 / lfGridFreq;//减去一个周期的时间，约20ms
					if(lfTemp < -c200us || lfTemp > c200us)//如果相差0.2ms以上，做调整，存入Eeprom
					{
						lfTemp /= 2;
						Safty.data.swNRelayDelayOnAdj += (INT16S)lfTemp;
						sNRelayDelayOnAdj(cTrue);
					}
					sNRelayDelayOn(cFalse);
				}
			}
			else
			{
				luwRelayDelayOnAdjCnt = 0;
			}
		}
	}
	else
	{
		luwCrossZeroDelayCnt = 0;
		StateFlg.bit.ubCrossZeroFlg = cFalse;
	}	
}

/*****************************************************************************
 * 函数名称：继电器延时吸合使能
 * 函数输入：标志位
 * 函数返回值：无
 * 函数描述：
 *****************************************************************************/
void sNRelayDelayOn(INT8U lubEnFlg)
{
	StateFlg.bit.ubNRelayDelayOn = lubEnFlg;
}

/*****************************************************************************
 * 函数名称：继电器延时补偿使能
 * 函数输入：标志位
 * 函数返回值：无
 * 函数描述：
 *****************************************************************************/
void sNRelayDelayOnAdj(INT8U lubEnFlg)
{
	StateFlg.bit.ubNRelayDelayOnAdj = lubEnFlg;
}

/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Lib_PVBusProtect.c
 * 修改人：陈波
 * 修改日期：2015.05.07
 * 描述：PV、Bus电压保护
 * 修改内容：
 * 1.1版--修改状态标志位名称；
 * 1.2版--加快PV、Bus的保护速度；
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "app.h"

/********************************************************************
 * Macros
 ********************************************************************/


/********************************************************************
 * Macros
 ********************************************************************/


/********************************************************************
 * Local Types
 ********************************************************************/
INT8U lubPVVoltLowCnt;
INT8U lubPVVoltHighCnt;
INT8U lubPVVoltNormalCnt;
INT8U lubBusVoltLowCnt;
INT8U lubPVVoltLowCnt_SinglePV;
INT8U lubPVVoltNormalCnt_SinglePV;

/********************************************************************
 * Global Types
 ********************************************************************/

#ifdef FloatEnable
/*****************************************************************************
 * 函数名称：PV电压检测函数
 * 函数输入：PV电压，输入路数，PV电压下限
 * 函数返回值：无
 * 函数描述：
 ****************************************************************************/
void sPVVoltCheck(float *lfPVVolt,INT8U lubPVNum,float lfPVVoltLow)
{
	INT8U i;
	float lfMaxPVVolt;
	
	lfMaxPVVolt = c0V;
	
	for(i = 0;i < lubPVNum;i++)
	{
		if(lfMaxPVVolt < lfPVVolt[i])
		{
			lfMaxPVVolt = lfPVVolt[i];
		}
	}
	
	if(lfMaxPVVolt < lfPVVoltLow)
	{
		if(++lubPVVoltLowCnt >= 2)
		{
			Error.bit.mubPVVoltLow = cTrue;
			Error.bit.mubPVVoltHigh = cFalse;
			lubPVVoltLowCnt = 0;
		}
		lubPVVoltNormalCnt = 0;
	}
	else if(lfMaxPVVolt > cMaxPVVolt)
	{
		if(++lubPVVoltHighCnt >= 2)
		{
			Error.bit.mubPVVoltHigh = cTrue;
			Error.bit.mubPVVoltLow = cFalse;
			lubPVVoltHighCnt = 0;
		}
		lubPVVoltNormalCnt = 0;
	}
	else
	{
		if(Error.bit.mubPVVoltLow == cTrue || Error.bit.mubPVVoltHigh == cTrue)
		{
			if(++lubPVVoltNormalCnt >= 250)
			{
				lubPVVoltLowCnt = 0;
				lubPVVoltHighCnt = 0;
				Error.bit.mubPVVoltLow = cFalse;
				Error.bit.mubPVVoltHigh = cFalse;
			}
		}
	}
}

/*****************************************************************************
 * 函数名称：单路PV电压低故障判断函数
 * 函数输入：某一路PV电压，PV电压下限
 * 函数返回值：无
 * 函数描述：
 ****************************************************************************/
void sPVVoltLowCheck_SinglePV(float lfPVVolt,float lfPVVoltLow)
{
	if(lfPVVolt < lfPVVoltLow)
	{
		if(++lubPVVoltLowCnt_SinglePV >= 2)
		{
			Error.bit.mubPVVoltLow = cTrue;
			lubPVVoltLowCnt_SinglePV = 0;
		}
		lubPVVoltNormalCnt_SinglePV = 0;
	}
	else
	{
		if(Error.bit.mubPVVoltLow == cTrue)
		{
			if(++lubPVVoltNormalCnt_SinglePV >= 250)
			{
				lubPVVoltLowCnt_SinglePV = 0;
				Error.bit.mubPVVoltLow = cFalse;
			}
		}
	}
}

/*****************************************************************************
 * 函数名称：Bus电压低检测函数
 * 函数输入：Bus电压，Bus电压下限
 * 函数返回值：无
 * 函数描述：
 ****************************************************************************/
void sBusVoltLowCheck(float lfBusVolt,float lfBusVoltLow)
{
	if(lfBusVolt < lfBusVoltLow)
	{
		if(++lubBusVoltLowCnt >= 100)
		{
			Error.bit.mubBusVoltLow = cTrue;
			lubBusVoltLowCnt = 0;
		}
	}
	else
	{
		lubBusVoltLowCnt = 0;
	}
}
#else
/*****************************************************************************
 * 函数名称：PV电压检测函数
 * 函数输入：PV电压，输入路数，PV电压下限
 * 函数返回值：无
 * 函数描述：
 ****************************************************************************/
void sPVVoltCheck(INT16U *luwPVVolt,INT8U lubPVNum,INT16U luwPVVoltLow)
{
	INT8U i;
	INT16U luwMaxPVVolt;
	
	luwMaxPVVolt = c0V;
	
	for(i = 0;i < lubPVNum;i++)
	{
		if(luwMaxPVVolt < luwPVVolt[i])
		{
			luwMaxPVVolt = luwPVVolt[i];
		}
	}
	
	if(luwMaxPVVolt < luwPVVoltLow)
	{
		if(++lubPVVoltLowCnt >= 2)
		{
			Error.bit.mubPVVoltLow = cTrue;
			Error.bit.mubPVVoltHigh = cFalse;
			lubPVVoltLowCnt = 0;
		}
		lubPVVoltNormalCnt = 0;
	}
	else if(luwMaxPVVolt > cMaxPVVolt)
	{
		if(++lubPVVoltHighCnt >= 2)
		{
			Error.bit.mubPVVoltHigh = cTrue;
			Error.bit.mubPVVoltLow = cFalse;
			lubPVVoltHighCnt = 0;
		}
		lubPVVoltNormalCnt = 0;
	}
	else
	{
		if(Error.bit.mubPVVoltLow == cTrue || Error.bit.mubPVVoltHigh == cTrue)
		{
			if(++lubPVVoltNormalCnt >= 250)
			{
				lubPVVoltLowCnt = 0;
				lubPVVoltHighCnt = 0;
				Error.bit.mubPVVoltLow = cFalse;
				Error.bit.mubPVVoltHigh = cFalse;
			}
		}
	}
}

/*****************************************************************************
 * 函数名称：PV&Bus电压检测函数
 * 函数输入：Bus电压，Bus电压下限
 * 函数返回值：无
 * 函数描述：
 ****************************************************************************/
void sBusVoltLowCheck(INT16U luwBusVolt,INT16U luwBusVoltLow)
{
	if(luwBusVolt < luwBusVoltLow)
	{
		if(++lubBusVoltLowCnt >= 100)
		{
			Error.bit.mubBusVoltLow = cTrue;
			lubBusVoltLowCnt = 0;
		}
	}
	else
	{
		lubBusVoltLowCnt = 0;
	}
}
#endif

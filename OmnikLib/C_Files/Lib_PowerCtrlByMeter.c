/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Lib_PowerCtrlByMeter.c
 * 修改人：陈波
 * 修改日期：2014.11.21
 * 描述：根据电表功率值和当前的功率值计算设定功率
 * 修改内容：
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"
#include "Lib_Communication.h"

/********************************************************************
 * Local Types
 ********************************************************************/
INT8U lubMeterLoss;

#ifdef FloatEnable
float lfPowerByMeter;
#else
INT16S lswPowerByMeter;
#endif

INT16S lswMeterPowerPre;

PQDDATA PQDDataPre;

/********************************************************************
 * Extern Global Types
 ********************************************************************/
#ifdef FloatEnable
extern float gfRatedPower;
#else
extern INT16U guwRatedPower;
#endif

#ifdef FloatEnable
/*****************************************************************************
 * 函数功能：防逆流功能函数。
 * 函数输入：电表总功率，逆变器总功率。
 * 函数描述：运行状态下1s调用一次
 ****************************************************************************/
float sSetPowerByMeter(PQDDATA *PQDDataTemp, float lfInvPower)
{
	INT16U i;
	
	for(i = 0;i < cPQDDataSize;i++)
	{
		if(PQDDataTemp->all[i] != PQDDataPre.all[i])
		{
			break;											//新数据跟前一帧数据不一样，说明有更新
		}
	}
	
	if(i != cPQDDataSize)
	{
		lfPowerByMeter = lfInvPower + (float)(PQDDataTemp->data.swActivePower[cPhaseR] + PQDDataTemp->data.swActivePower[cPhaseS] + PQDDataTemp->data.swActivePower[cPhaseT]) / 2;
		lubMeterLoss = 0;
	}
	else if(++lubMeterLoss >= 10)							//连续10个周期得不到新的数据，认为PQD已断开
	{
		lubMeterLoss = 10;
		lfPowerByMeter = gfRatedPower;
	}
	
	for(i = 0;i < cPQDDataSize;i++)
	{
		PQDDataPre.all[i] = PQDDataTemp->all[i];
	}
	
	if(lfPowerByMeter > gfRatedPower)
	{
		lfPowerByMeter = gfRatedPower;
	}
	else if(lfPowerByMeter < c0W)
	{
		lfPowerByMeter = c0W;
	}
	
	return(lfPowerByMeter);
}

/*****************************************************************************
 * 函数功能：防逆流功能数据初始化函数。
 * 函数输入：无
 * 函数描述：非运行状态下调用
 ****************************************************************************/
void sPowerByMeterInit(void)
{
	INT16U i;

	lfPowerByMeter = gfRatedPower;
	for(i = 0;i < cPQDDataSize;i++)
	{
		PQDDataPre.all[i] = c0W;
	}
	lubMeterLoss = 0;
}
#else
INT16U sSetPowerByMeter(PQDDATA *PQDDataTemp, INT16U luwInvPower)
{
	INT16U i;
	
	for(i = 0;i < cPQDDataSize;i++)
	{
		if(PQDDataTemp->all[i] != PQDDataPre.all[i])
		{
			break;											//新数据跟前一帧数据不一样，说明有更新
		}
	}
	
	if(i != cPQDDataSize)
	{
		lswPowerByMeter = (INT16S)luwInvPower + (PQDDataTemp->data.swActivePower[cPhaseR] + PQDDataTemp->data.swActivePower[cPhaseS] + PQDDataTemp->data.swActivePower[cPhaseT]) / 2;
		lubMeterLoss = 0;
	}
	else if(++lubMeterLoss >= 10)							//连续10个周期得不到新的数据，认为PQD已断开
	{
		lubMeterLoss = 10;
		lswPowerByMeter = guwRatedPower;
	}
	
	for(i = 0;i < cPQDDataSize;i++)
	{
		PQDDataPre.all[i] = PQDDataTemp->all[i];
	}
	
	if(lswPowerByMeter > (INT16S)guwRatedPower)
	{
		lswPowerByMeter = (INT16S)guwRatedPower;
	}
	else if(lswPowerByMeter < c0W)
	{
		lswPowerByMeter = c0W;
	}
	
	return(lswPowerByMeter);
}

/*****************************************************************************
 * 函数功能：防逆流功能数据初始化函数。
 * 函数输入：无
 * 函数描述：非运行状态下调用
 ****************************************************************************/
void sPowerByMeterInit(void)
{
	INT16U i;

	lswPowerByMeter = (INT16S)guwRatedPower;
	for(i = 0;i < cPQDDataSize;i++)
	{
		PQDDataPre.all[i] = c0W;
	}
	lubMeterLoss = 0;
}
#endif

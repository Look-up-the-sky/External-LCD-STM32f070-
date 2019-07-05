/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Lib_PowerDecreaseByVolt.c
 * 修改人：陈波
 * 修改日期：2015.06.24
 * 描述：过压降载
 * 修改内容：
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "app.h"

/********************************************************************
 * Local Types
 ********************************************************************/
#ifdef FloatEnable
float lfVoltPoint[2];
#else
INT16U luwDcrsVolt;
INT16U luwKeepVolt;
INT16U luwVoltPoint[2];
#endif

#ifdef FloatEnable
/*****************************************************************************
 * 函数名称：过压降载函数。
 * 函数输入：电网电压。
 * 函数返回值：并网最大功率百分比。
 * 函数描述：
 ****************************************************************************/
float sPowerByVolt(float lfGridVolt)
{
	if(lfGridVolt < lfVoltPoint[0])
	{
		return(c100Percent);
	}
	else if(lfGridVolt < lfVoltPoint[1])
	{
		return((lfVoltPoint[1] - lfGridVolt) / (lfVoltPoint[1] - lfVoltPoint[0]));
	}
	else
	{
		return(c0Percent);
	}
}

/*****************************************************************************
 * 函数名称：降载电压设定函数。
 * 函数输入：降载电压值。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sSetPowerByVoltLimit(float lfVolt1,float lfVolt2)
{
	lfVoltPoint[0] = lfVolt1;
	lfVoltPoint[1] = lfVolt2;
	
	if(lfVoltPoint[0] < c230V)
	{
		lfVoltPoint[0] = c230V;
	}
	if(lfVoltPoint[1] < lfVoltPoint[0] + c2V)
	{
		lfVoltPoint[1] = lfVoltPoint[0] + c2V;
	}
}
#else
/*****************************************************************************
 * 函数名称：过压降载函数。
 * 函数输入：电网电压。
 * 函数返回值：并网最大功率值。
 * 函数描述：
 ****************************************************************************/
INT8U sPowerByVolt(INT16U luwGridVolt)
{
	if(luwGridVolt > luwDcrsVolt)
	{
		return(2);
	}
	else if(luwGridVolt > luwKeepVolt)
	{
		return(1);
	}
	else
	{
		return(0);
	}
}

/*****************************************************************************
 * 函数名称：降载电压设定函数。
 * 函数输入：降载电压值。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sSetPowerByVoltLimit(INT16U luwVolt1,INT16U luwVolt2)
{
	luwDcrsVolt = luwVolt1;
	luwKeepVolt = luwVolt2;

	if(luwDcrsVolt < c253VAC)
	{
		luwDcrsVolt = c253VAC;
	}
	else if(luwDcrsVolt > c276VAC)
	{
		luwDcrsVolt = c276VAC;
	}

	if(luwKeepVolt + c5VAC > luwDcrsVolt)
	{
		luwKeepVolt = luwDcrsVolt - c5VAC;
	}
	else if(luwKeepVolt + c10VAC < luwDcrsVolt)
	{
		luwKeepVolt = luwDcrsVolt - c10VAC;
	}
}
#endif

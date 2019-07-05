/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Lib_CalCoef.c
 * 修改人：陈波
 * 修改日期：2014.12.26
 * 描述：AD采样系数的初始化和矫正
 * 修改内容：
 * 		1.1--所有系数默认值均为1000；
 *******************************************************/

#include "App.h"
#include "Lib_CalCoef.h"

/*****************************************************************************
 * 函数功能：采样系数计算
 * 函数输入：
 * 函数描述：矫正采样系数
 ****************************************************************************/
INT16U sCalCoef(INT16U *luwCoef,INT16U *luwMeterData,INT16U *luwMachineData,INT16U luwLength)
{
	INT16U i;

	if(luwLength > cMaxATEDataNum)
	{
		return(cFalse);
	}

	for(i = 0;i < luwLength;i++)
	{
		if(luwMachineData[i] == 0)
		{
			continue;
		}

		luwCoef[i] = (INT32U)luwCoef[i] * luwMeterData[i] / luwMachineData[i];

		if(luwCoef[i] > cCoefMax || luwCoef[i] < cCoefMin)
		{
			luwCoef[i] = cCoefDef;
		}
	}

	return(cTrue);
}

/*****************************************************************************
 * 函数功能：采样系数初始化
 * 函数输入：
 * 函数描述：采样系数初始化，若系数超范围则将其恢复为默认值
 ****************************************************************************/
void sCoefInit(INT16U *luwCoef,INT16U luwLength)
{
	INT16U i;

	if(luwLength > cMaxATEDataNum)
	{
		luwLength = cMaxATEDataNum;
	}

	for(i = 0;i < luwLength;i++)
	{
		if(luwCoef[i] > cCoefMax || luwCoef[i] < cCoefMin)
		{
			luwCoef[i] = cCoefDef;
		}
	}
}

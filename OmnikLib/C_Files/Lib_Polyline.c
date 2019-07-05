/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Lib_QByV.c
 * 修改人：陈波
 * 修改日期：2015.10.27
 * 描述：取折线上的某一点
 * 修改内容：
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"

/*****************************************************************************
 * 函数名称：折线点数值函数。
 * 函数输入：X轴百分比点，Y轴百分比点，当前X轴百分比。
 * 函数返回值：当前Y轴百分比。
 * 函数描述：百分比设定共分四个点，输入第一个变量为横坐标四个点的首地址，输入
 * 			 第二个变量为纵坐标四个点的首地址。当当前X轴百分比在四个点之外，则
 *			 返回值取端点值，当当前X轴百分比在四个点之内，则返回值取线性插值的
 *			 值。
 ****************************************************************************/
float sGetPolylinePoint(INT8U *lubXPercentSet,INT8S *lsbYPercentSet,float lfXPercent)
{
	float lfTemp;
	float lfXPercentSet[4];
	float lfYPercentSet[4];
	INT8U i;

	if(lubXPercentSet[0] <= lubXPercentSet[1] && lubXPercentSet[1] <= lubXPercentSet[2] && lubXPercentSet[2] <= lubXPercentSet[3])
	{
		for(i = 0;i < 4;i++)
		{
			lfXPercentSet[i] = (float)lubXPercentSet[i] / 100.0f;
			lfYPercentSet[i] = (float)lsbYPercentSet[i] / 100.0f;
		}

		if(lfXPercent < lfXPercentSet[0])
		{
			lfTemp = lfYPercentSet[0];
		}
		else if(lfXPercent < lfXPercentSet[1])
		{
			/*********************************************
			 * y = y0 + (x - x0) * (y1 - y0) / (x1 - x0)
			 *********************************************/
			lfTemp = lfXPercent - lfXPercentSet[0];
			lfTemp *= lfYPercentSet[1] - lfYPercentSet[0];
			lfTemp /= lfXPercentSet[1] - lfXPercentSet[0];
			lfTemp += lfYPercentSet[0];
		}
		else if(lfXPercent < lfXPercentSet[2])
		{
			lfTemp = lfXPercent - lfXPercentSet[1];
			lfTemp *= lfYPercentSet[2] - lfYPercentSet[1];
			lfTemp /= lfXPercentSet[2] - lfXPercentSet[1];
			lfTemp += lfYPercentSet[1];
		}
		else if(lfXPercent < lfXPercentSet[3])
		{
			lfTemp = lfXPercent - lfXPercentSet[2];
			lfTemp *= lfYPercentSet[3] - lfYPercentSet[2];
			lfTemp /= lfXPercentSet[3] - lfXPercentSet[2];
			lfTemp += lfYPercentSet[2];
		}
		else
		{
			lfTemp = lfYPercentSet[3];
		}

		return(lfTemp);
	}
	else
	{
		return(0.0f);
	}
}

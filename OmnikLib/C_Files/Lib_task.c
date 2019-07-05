/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Lib_task.c
 * 修改人：沈仕强
 * 修改日期：2015.03.26
 * 描述：任务调度
 * 修改内容：
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "app.h"		//app.h中定义cTaskNum
#include "Lib_task.h"

/********************************************************************
 * Macros
 ********************************************************************/
#define cMaxUnfinishedNum	10

/********************************************************************
 * Macros
 ********************************************************************/


/********************************************************************
 * Local Types
 ********************************************************************/
INT16U luwTaskNum;
INT16U luwTaskTimer[cTaskNum];
INT16U luwTaskPeriod[cTaskNum];

pFunc sTask[cTaskNum];

/********************************************************************
 * Global Types
 ********************************************************************/
INT16U guwTaskFinish[cTaskNum];

/*****************************************************************************
 * 函数名称：初始化任务地址函数。
 * 函数输入：任务优先级，任务地址。
 * 函数返回值：无。
 * 函数描述：设定此优先级任务的地址。
 ****************************************************************************/
void sInitTaskAdr(INT16U luwTaskNum, pFunc sFunction)
{
	sTask[luwTaskNum] = sFunction;
}

/*****************************************************************************
 * 函数名称：初始化任务周期函数。
 * 函数输入：任务优先级，任务周期，任务起始时间。
 * 函数返回值：无。
 * 函数描述：设定此优先级任务的周期。非周期任务，周期值赋0。
 ****************************************************************************/
void sInitTaskPeriod(INT16U luwTaskNum, INT16U luwPeriod, INT16U luwStartTime)
{
	luwTaskPeriod[luwTaskNum] = luwPeriod;
	luwTaskTimer[luwTaskNum] = luwStartTime;
}

/*****************************************************************************
 * 函数名称：任务调度系统启动函数。
 * 函数输入：无。
 * 函数返回值：无。
 * 函数描述：循环查询带执行任务，并跳转至相应地址执行任务，并清除任务带执行次数。
 ****************************************************************************/
void sTaskStart(void)
{
	while(1)
	{
		for(luwTaskNum = 0; luwTaskNum < cTaskNum; luwTaskNum++)
		{
			if(guwTaskFinish[luwTaskNum] != 0)
			{
				guwTaskFinish[luwTaskNum] = 0;
				sTask[luwTaskNum]();
				break;
			}
		}
	}
}

/*****************************************************************************
 * 函数名称：任务调度系统计时函数。
 * 函数输入：无。
 * 函数返回值：无。
 * 函数描述：循环累加个任务计时器，任务计时器到任务周期后，任务带执行次数加1。
 * 			此函数由MCU Timer的1ms中断函数调用。
 ****************************************************************************/
INT8U sTaskTimer(void)
{
	INT16U luwTemp;
	
	for(luwTemp = 0; luwTemp < cTaskNum; luwTemp++)
	{
		if(luwTaskPeriod[luwTemp] == 0)
		{
		}
		else if(++luwTaskTimer[luwTemp] >= luwTaskPeriod[luwTemp])
		{
			luwTaskTimer[luwTemp] = 0;
			if(++guwTaskFinish[luwTemp] >= cMaxUnfinishedNum)
			{
				return(1);
			}
		}
	}
	
	return(0);
}

/*****************************************************************************
 * 函数名称：非周期性任务启动函数。
 * 函数输入：任务号。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sSetTask(INT16U luwTaskNum)
{
	if(luwTaskPeriod[luwTaskNum] == 0)
	{
		if(++guwTaskFinish[luwTaskNum] >= cMaxUnfinishedNum)
		{
			guwTaskFinish[luwTaskNum] = cMaxUnfinishedNum;
		}
	}
}

/*****************************************************************************
 * 函数名称：任务延时启动函数。
 * 函数输入：任务号，延时时间。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sSetTaskDelay(INT16U luwTaskNum,INT8U lubDelay)
{
	if(luwTaskNum >= cTaskNum)
	{
		return;
	}
	
	if(lubDelay)
	{
		if(lubDelay <= luwTaskPeriod[luwTaskNum])
		{
			luwTaskTimer[luwTaskNum] = luwTaskPeriod[luwTaskNum] - lubDelay;
		}
	}
	else if(++guwTaskFinish[luwTaskNum] >= cMaxUnfinishedNum)
	{
		guwTaskFinish[luwTaskNum] = cMaxUnfinishedNum;
	}
}

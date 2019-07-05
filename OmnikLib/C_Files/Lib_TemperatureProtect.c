/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：TemperatureProtect.c
 * 修改人：陈波
 * 修改日期：2014.10.29
 * 描述：过温降载
 * 修改内容：
 * 1.1版--温度故障恢复后，清恢复计数器；
 * 1.2版--修正整型的变量名称；
 * 1.3版--温度恢复后功率为110%的额定功率；
 * 1.4版--include目录改为在项目中设定；
 * 1.5版--将返回值改为额定功率的百分比；
 *******************************************************/

#include "App.h"

#ifdef FloatEnable
float lfPwrPercentByT;
float lfTempS;
float lfTempE;
#else
INT16U luwPwrPercentByT;
INT16U luwTempS;
INT16U luwTempE;
#endif

INT16U luwTempBackCnt;

#ifdef FloatEnable
/*****************************************************************************
 * 函数名称：过温降载温度点设置函数。
 * 函数输入：过温降载起始温度，过温降载截止温度。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sSetTPoint(float lfTemp1,float lfTemp2)
{
	lfTempS = lfTemp1;
	lfTempE = lfTemp2;
}

/*****************************************************************************
 * 函数功能：过温保护。
 * 函数输入：逆变器温度。
 * 函数描述：无过温故障时，判断当前温度，如果超过温度上限，则置过温故障标志位，返回
 * 			 零功率，若在降载范围内，则返回计算的降载功率，若低于降载范围，则返回额
 * 			 定功率；当已经发生过温故障时，若当前温度低于降载范围，则清过温故障标志
 * 			 位，返回额定功率，若高于降载范围，则清计数器，返回额定功率。
 ****************************************************************************/
float sCalPowerByTemperature(float lfTemperature,INT8U lubError)
{
	if(lubError == 0)
	{
		if(lfTemperature > lfTempE)
		{
			lfPwrPercentByT = c0Percent;
		}
		else if(lfTemperature > lfTempS)
		{
			lfPwrPercentByT = (lfTempE - lfTemperature) / (lfTempE - lfTempS);
		}
		else
		{
			lfPwrPercentByT = c100Percent;
		}
	}
	else
	{
		if(lfTemperature < lfTempS)
		{
			if(++luwTempBackCnt >= 20)
			{
				luwTempBackCnt = 0;
				lfPwrPercentByT = c100Percent;
			}
			else
			{
				lfPwrPercentByT = c0Percent;
			}
		}
		else
		{
			luwTempBackCnt = 0;
			lfPwrPercentByT = c0Percent;
		}
	}

	return(lfPwrPercentByT);
}

/*****************************************************************************
 * 函数功能：
 * 函数输入：无。
 * 函数描述：
 ****************************************************************************/
void sCalPowerByTemperatureInitial(void)
{
	lfPwrPercentByT = c100Percent;
	luwTempBackCnt  = 0;
}
#else
/*****************************************************************************
 * 函数名称：过温降载温度点设置函数。
 * 函数输入：过温降载起始温度，过温降载截止温度。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sSetTPoint(INT16U luwTemp1,INT16U luwTemp2)
{
	luwTempS = luwTemp1;
	luwTempE = luwTemp2;
}

/*****************************************************************************
 * 函数功能：过温保护。
 * 函数输入：逆变器温度。
 * 函数描述：无过温故障时，判断当前温度，如果超过温度上限，则置过温故障标志位，返回
 * 			 零功率，若在降载范围内，则返回计算的降载功率，若低于降载范围，则返回额
 * 			 定功率；当已经发生过温故障时，若当前温度低于降载范围，则清过温故障标志
 * 			 位，返回额定功率，若高于降载范围，则清计数器，返回额定功率。
 ****************************************************************************/
INT16U sCalPowerByTemperature(INT16U luwTemperature,INT8U lubError)
{
	if(lubError == 0)
	{
		if(luwTemperature > luwTempE)
		{
			luwPwrPercentByT = 0;
		}
		else if(luwTemperature > luwTempS)
		{
			luwPwrPercentByT = (INT32U)100 * (luwTempE - luwTemperature) / (luwTempE - luwTempS);
		}
		else
		{
			luwPwrPercentByT = 100;
		}
	}
	else
	{
		if(luwTemperature < luwTempS)
		{
			if(++luwTempBackCnt >= 20)
			{
				luwTempBackCnt = 0;
				luwPwrPercentByT = 100;
			}
			else
			{
				luwPwrPercentByT = 0;
			}
		}
		else
		{
			luwTempBackCnt = 0;
			luwPwrPercentByT = 0;
		}
	}

	return(luwPwrPercentByT);
}

/*****************************************************************************
 * 函数功能：
 * 函数输入：无。
 * 函数描述：
 ****************************************************************************/
void sCalPowerByTemperatureInitial(void)
{
	luwPwrPercentByT = 100;
	luwTempBackCnt  = 0;
}

#endif

/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Lib_GridVoltProtect.c
 * 修改人：陈波
 * 描述：过欠压保护
 * 初版日期：2014.10.29
 * 修改内容：
 * 1.1版--将变量全部设为局部变量；
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"
#include "Lib_VoltFreq.h"
#include "Lib_SelfTest.h"
#include "Lib_Communication.h"

/********************************************************************
 * Global Types
 ********************************************************************/


/********************************************************************
 * Local Types
 ********************************************************************/
INT8U  lub_t;         // 突变时间t
INT16U luwOverVoltCnt[cPhaseNum];
INT16U luwOverVoltCnt2[cPhaseNum];
INT16U luwUnderVoltCnt[cPhaseNum];
INT16U luwUnderVoltCnt2[cPhaseNum];
INT16U luwVoltBackCnt;
INT16U luwProtectCnt; // 保护时间 


#ifdef FloatEnable
float lfGridVoltValueLimit[6];
float lfGridVolt_A[cPhaseNum];
float lfGridVolt_U[cPhaseNum];
float lfGridVolt_B[cPhaseNum];
#else
INT16U luwGridVoltHigh1;
INT16U luwGridVoltHigh2;
INT16U luwGridVoltLow1;
INT16U luwGridVoltLow2;
INT16U luwGridVoltBackLow;
INT16U luwGridVoltBackHigh;
#endif

INT16U luwGridVoltTimeLimit[5];

/*****************************************************************************
 * 函数功能：电网电压保护变量初始化。
 * 函数输入：无。
 * 函数描述：初始化电网电压保护函数中的局部变量。
 ****************************************************************************/
void sVoltCheckInitial(INT16U lubPhaseNum)
{
	INT8U lubPhase;
	for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
	{
		luwOverVoltCnt[lubPhase] = 0;
		luwOverVoltCnt2[lubPhase] = 0;
		luwUnderVoltCnt[lubPhase] = 0;
		luwUnderVoltCnt2[lubPhase] = 0;
	}
	luwVoltBackCnt = 0;
	luwProtectCnt = 0;
}

/*****************************************************************************
 * 函数名称：获得电压保护时间函数。
 * 函数输入：保护选择。
 * 函数返回值：保护时间。
 * 函数描述：
 ****************************************************************************/
INT16U sGetVoltTime(INT8U lubTimeNum)
{
	if(lubTimeNum <= cBack)
	{
		return(luwGridVoltTimeLimit[lubTimeNum]);
	}
	else
	{
		return(0);
	}
}

/*****************************************************************************
 * 函数名称：设置电压保护时间函数。
 * 函数输入：保护选择，时间。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sSetVoltTime(INT8U lubTimeNum,INT16U luwTime)
{
	if(lubTimeNum <= cBack)
	{
		luwGridVoltTimeLimit[lubTimeNum] = luwTime*10;
	}
}

#ifdef FloatEnable
/*****************************************************************************
 * 函数功能：电网电压保护。
 * 函数输入：电网电压，相，故障位。
 * 函数描述：当电网电压不在正常范围内时，计时器工作，时间超过规定值后，置电网电压故
 * 			 障标志位；在计时过程中如果电网电压恢复，则清计时器。若电压恢复到正常范
 * 			 围，则清除电网电压故障标志位。
 * 改进方向：当保护时间较长时，例如个别国家有10s甚至更长的情况，需要针对由于采样误差
 * 			 造成的保护异常提供解决方案。
 ****************************************************************************/
INT16U sVoltCheck(const float *lfGridVolt, INT8U lubPhaseNum, INT8U lubError, float lfGridFreq)
{
	INT8U lubPhase;
	INT8U lubSelfTestFlagTemp;
	INT16U luwtimedelta;
	luwtimedelta = 0;
	lubSelfTestFlagTemp = 0;

	if(lubError == 0)
	{
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			/********************************************************************
			* lfGridVolt_A,U,B 是连续的三个周期的电网电压有效值;
			* lfGridVolt_A为电网电压突变前一个周期内的有效值;
			* lfGridVolt_U为电网电压突变中一个周期内的有效值;
			* lfGridVolt_B为电网电压突变后一个周期内的有效值;
			* lub_t为突变时间，其计算公式为：t = ((B^2 - U^2)/(B^2 - A^2)) * T
			*********************************************************************/
			lfGridVolt_A[lubPhase] = lfGridVolt_U[lubPhase];
			lfGridVolt_U[lubPhase] = lfGridVolt_B[lubPhase];
			lfGridVolt_B[lubPhase] = lfGridVolt[lubPhase];
			lub_t =(((lfGridVolt_B[lubPhase]*lfGridVolt_B[lubPhase] - lfGridVolt_U[lubPhase]*lfGridVolt_U[lubPhase])/(lfGridVolt_B[lubPhase]*lfGridVolt_B[lubPhase] - lfGridVolt_A[lubPhase]*lfGridVolt_A[lubPhase]))/(lfGridFreq)) * 1000;
			
			if(lfGridVolt[lubPhase] > lfGridVoltValueLimit[cHigh1] && StateFlg.bit.ubVoltProtect!= 1) 	   //过压1
			{
				luwProtectCnt = luwGridVoltTimeLimit[cHigh1];
				luwtimedelta = luwProtectCnt - lub_t;
				StateFlg.bit.ubVoltProtect = 1; 
			}
			else if(lfGridVolt[lubPhase] > lfGridVoltValueLimit[cHigh2] && StateFlg.bit.ubVoltProtect!= 1) //过压2
			{
				luwProtectCnt = luwGridVoltTimeLimit[cHigh2];
				luwtimedelta = luwProtectCnt - lub_t;
				StateFlg.bit.ubVoltProtect = 1; 
			}
			else if(lfGridVolt[lubPhase] < lfGridVoltValueLimit[cLow1] && StateFlg.bit.ubVoltProtect!= 1) //欠压1
			{
				luwProtectCnt = luwGridVoltTimeLimit[cLow1];
				luwtimedelta = luwProtectCnt - lub_t;
				StateFlg.bit.ubVoltProtect = 1; 
			}
			else if(lfGridVolt[lubPhase] < lfGridVoltValueLimit[cLow2] && StateFlg.bit.ubVoltProtect!= 1)  //欠压2
			{
				luwProtectCnt = luwGridVoltTimeLimit[cLow2];
				luwtimedelta = luwProtectCnt - lub_t;
				StateFlg.bit.ubVoltProtect = 1; 
			}
			else if ((lfGridVolt[lubPhase]  < lfGridVoltValueLimit[cHigh1]) && (lfGridVolt[lubPhase]  > lfGridVoltValueLimit[cLow1]))
			{
				StateFlg.bit.ubVoltProtect = 0; //电压有效值在正常范围内，清除标志位置
				return (0);
			}
		}
		if(lubSelfTestFlagTemp == 0)
		{
			sClearSelfTestFlag(1);
		}
		
		return luwtimedelta;;
	}
	else
	{
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			if((lfGridVolt[lubPhase] < lfGridVoltValueLimit[cBackLow]) || (lfGridVolt[lubPhase] > lfGridVoltValueLimit[cBackHigh]))
			{
				luwVoltBackCnt = 0;
			}
		}

		if(++luwVoltBackCnt > luwGridVoltTimeLimit[cBack])
		{
			luwVoltBackCnt = 0;
			sVoltCheckInitial(lubPhaseNum);
			Error.bit.mubGridVoltFault = 0;
		}
		return(0);
	}
}

/*****************************************************************************
 * 函数名称：获得电压阈值函数。
 * 函数输入：保护选择。
 * 函数返回值：保护阈值。
 * 函数描述：
 ****************************************************************************/
float sGetVoltLimit(INT8U lubTimeNum)
{
	if(lubTimeNum <= cBackHigh)
	{
		return(lfGridVoltValueLimit[lubTimeNum]);
	}
	else
	{
		return(0.0f);
	}
}

/*****************************************************************************
 * 函数名称：设置电压阈值函数。
 * 函数输入：保护选择，电压阈值。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sSetVoltLimit(INT8U lubTimeNum,float lfVolt)
{
	if(lubTimeNum <= cBackHigh)
	{
		lfGridVoltValueLimit[lubTimeNum] = lfVolt;
	}
}

#else
/*****************************************************************************
 * 函数功能：电网电压保护。
 * 函数输入：电网电压，相，故障位。
 * 函数描述：当电网电压不在正常范围内时，计时器工作，时间超过规定值后，置电网电压故
 * 			 障标志位；在计时过程中如果电网电压恢复，则清计时器。若电压恢复到正常范
 * 			 围，则清除电网电压故障标志位。
 * 改进方向：当保护时间较长时，例如个别国家有10s甚至更长的情况，需要针对由于采样误差
 * 			 造成的保护异常提供解决方案。
 ****************************************************************************/
INT8U sVoltCheck(const INT16U *luwGridVolt, INT8U lubPhaseNum,INT8U lubError)
{
	INT8U lubPhase;

	if(lubError == 0)
	{
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			if(luwGridVolt[lubPhase] < luwGridVoltLow2)
			{
				luwUnderVoltCnt2[lubPhase]++;
				luwUnderVoltCnt[lubPhase]++;
				luwOverVoltCnt[lubPhase] = 0;
				luwOverVoltCnt2[lubPhase] = 0;
				if(luwUnderVoltCnt2[lubPhase] >= luwGridVoltTimeLimit[cLow2])
				{
					luwUnderVoltCnt2[lubPhase] = 0;
					luwUnderVoltCnt[lubPhase] = 0;
					return(1);
				}
			}
			else if(luwGridVolt[lubPhase] < luwGridVoltLow1)
			{
				luwUnderVoltCnt2[lubPhase] = 0;
				luwUnderVoltCnt[lubPhase]++;
				luwOverVoltCnt[lubPhase] = 0;
				luwOverVoltCnt2[lubPhase] = 0;
				if(luwUnderVoltCnt[lubPhase] >= luwGridVoltTimeLimit[cLow1])
				{
					luwUnderVoltCnt[lubPhase] = 0;
					return(1);
				}
			}
			else if(luwGridVolt[lubPhase] > luwGridVoltHigh2)
			{
				luwUnderVoltCnt2[lubPhase] = 0;
				luwUnderVoltCnt[lubPhase] = 0;
				luwOverVoltCnt[lubPhase]++;
				luwOverVoltCnt2[lubPhase]++;
				if(luwOverVoltCnt2[lubPhase] >= luwGridVoltTimeLimit[cHigh2])
				{
					luwOverVoltCnt[lubPhase] = 0;
					luwOverVoltCnt2[lubPhase] = 0;
					return(1);
				}
			}
			else if(luwGridVolt[lubPhase] > luwGridVoltHigh1)
			{
				luwUnderVoltCnt2[lubPhase] = 0;
				luwUnderVoltCnt[lubPhase] = 0;
				luwOverVoltCnt[lubPhase]++;
				luwOverVoltCnt2[lubPhase] = 0;
				if(luwOverVoltCnt[lubPhase] >= luwGridVoltTimeLimit[cHigh1])
				{
					luwOverVoltCnt[lubPhase] = 0;
					return(1);
				}
			}
			else
			{
				luwOverVoltCnt[lubPhase] = 0;
				luwUnderVoltCnt[lubPhase] = 0;
				luwOverVoltCnt2[lubPhase] = 0;
				luwUnderVoltCnt2[lubPhase] = 0;
			}
		}
		return(0);
	}
	else
	{
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			if((luwGridVolt[lubPhase] < luwGridVoltBackLow) || (luwGridVolt[lubPhase] > luwGridVoltBackHigh))
			{
				luwVoltBackCnt = 0;
			}
		}

		if(++luwVoltBackCnt > luwGridVoltTimeLimit[cBack])
		{
			luwVoltBackCnt = 0;
			sVoltCheckInitial(lubPhaseNum);
			return(0);
		}
		return(1);
	}
}

/*****************************************************************************
 * 函数名称：设置过压阈值函数。
 * 函数输入：过压阈值。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sSetVoltHigh(INT16U luwVolt1,INT16U luwVolt2)
{
	luwGridVoltHigh1 = luwVolt1;
	luwGridVoltHigh2 = luwVolt2;
}

/*****************************************************************************
 * 函数名称：设置欠压阈值函数。
 * 函数输入：欠压阈值。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sSetVoltLow(INT16U luwVolt1,INT16U luwVolt2)
{
	luwGridVoltLow1 = luwVolt1;
	luwGridVoltLow2 = luwVolt2;
}

/*****************************************************************************
 * 函数名称：设置恢复电压阈值函数。
 * 函数输入：恢复电压阈值。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sSetVoltBack(INT16U luwVoltLow,INT16U luwVoltHigh)
{
	luwGridVoltBackLow = luwVoltLow;
	luwGridVoltBackHigh = luwVoltHigh;
}
#endif

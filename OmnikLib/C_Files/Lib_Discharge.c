/**************************************************************************
	文件名称：Lib_Discharge.c
	功能描述：用逆变部分的开关损耗对直流母线放电
	版权所有：苏州欧姆尼克新能源科技有限公司软件组
***************************************************************************
	变更纪录：
	变更日期 |  作者   |变更说明
	=========|=========|==================
	20180223 | 邢文超  |初次建立此文件
	20180515 | 邢文超  |正弦波产生方法改为查表，不用arm_math函数
	
********************************分割线*************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"
#include "AdcInterrupt.h"
#include "StateChange.h"
#include "Lib_Communication.h"
//#include "arm_math.h"
#include "stm32f30x_tim.h"
#include "Lib_Discharge.h"

/********************************************************************
 * Macros
 ********************************************************************/
#define c_50us_step 	(c2Pi/400.0f)		//人造正弦量的角度累加步长，加个括号更安全
#define cMinPulse		9
#define cSinTabNum		720

/********************************************************************
 * Constant
 ********************************************************************/


/********************************************************************
 * Local Types
 ********************************************************************/


/********************************************************************
 * Global Types
 ********************************************************************/


/********************************************************************
 * Extern Global Types
 ********************************************************************/
extern INT16S lsw_Volt_Comp;
extern const float gfSinTabArray[cSinTabNum];

/********************************************************************
 * Functions
 *******************************************************************/
 
/*****************************************************************************
 * 函数名称：s_lib_discharge_func
 * 函数输入：头文件中的结构体变量地址
 * 函数返回值：无
 * 函数描述：控制逆变给直流母线放电
 ****************************************************************************/
void s_lib_discharge_func(LIB_DISCHARGE_ST *p)
{
	float lf_relay_volt[cPhaseNum];
	float lf_sine_ref[cPhaseNum];
	float lf_voltage_ref[cPhaseNum];
	INT8U lubPhase;
/********************************************************************/
	cLRelayOff();						//断开并网接触器
	cNRelayOff();
	StateFlg.bit.ubSlaveRelay = 0;		//cSlaveRelayOff
	sTurnOffBoostPWM();					//封锁斩波器脉冲

/********************************************************************/
//判断接触器是否已分断。如果接触器两侧电压瞬时值的差的绝对值，连续100次大于50V，则判断为接触器已断开。
	
	p->lub_discharge_relayoff_all = 1;//先设为1，如果有接触器未分断，则由下面的位与运算来清零
	
	for(lubPhase = cPhaseR;lubPhase < cPhaseNum;lubPhase++)
	{
//		lf_relay_volt[lubPhase] = fabs(p->lf_inv_volt[lubPhase] - p->lf_grid_volt[lubPhase]);
		lf_relay_volt[lubPhase] = (p->lf_inv_volt[lubPhase] > p->lf_grid_volt[lubPhase]) ? p->lf_inv_volt[lubPhase] - p->lf_grid_volt[lubPhase] : p->lf_grid_volt[lubPhase] - p->lf_inv_volt[lubPhase];
		if(lf_relay_volt[lubPhase] > c50V)
		{
			p->lub_relay_disconnect_cnt[lubPhase] += 1;
		}
		else
		{
			p->lub_relay_disconnect_cnt[lubPhase] = 0;
		}
	
		if(p->lub_relay_disconnect_cnt[lubPhase] > 100)
		{
			p->lub_relay_disconnect_cnt[lubPhase] = 101;
			p->lub_discharge_relayoff_phase[lubPhase] = 1;//此标志位为1，表示对应相的接触器已断开。此标志位在放电结束后清零
		}
		p->lub_discharge_relayoff_all &= p->lub_discharge_relayoff_phase[lubPhase];//如果有接触器未分断，则清零总标志位
	}

/********************************************************************/
	if(p->lub_discharge_relayoff_all == 1)	//如果接触器已断开，则可以给BUS放电。
	{
		sTurnOnInvPWM();	//使能逆变的PWM
		cInvOn();
/********************************************************************/
//人为制造一个工频正弦波给定
		for(lubPhase = cPhaseR;lubPhase < cPhaseNum;lubPhase++)
		{
			p->lf_angle[lubPhase] += c_50us_step;		//角度累加
			if(p->lf_angle[lubPhase] >= c2Pi)
			{
				p->lf_angle[lubPhase] -= c2Pi;
			}

//			lf_sine_ref[lubPhase] = arm_sin_f32(p->lf_angle[lubPhase]);					//人造正弦
			lf_sine_ref[lubPhase] = gfSinTabArray[(INT16U)(p->lf_angle[lubPhase] * 114.5f)];//改为查表，以节省内存
			lf_voltage_ref[lubPhase] = 0.9f * cINVPWMPeriodf * lf_sine_ref[lubPhase];	//电压给定
			p->lf_current_ref[lubPhase] = lf_sine_ref[lubPhase];						//假装是电流

			if(lf_voltage_ref[lubPhase] > cINVPWMPeriodf - cMinPulse)
			{
				lf_voltage_ref[lubPhase] = cINVPWMPeriodf - cMinPulse;
			}
			else if(lf_voltage_ref[lubPhase] < -cINVPWMPeriodf + cMinPulse)
			{
				lf_voltage_ref[lubPhase] = -cINVPWMPeriodf + cMinPulse;
			}

			if(lf_voltage_ref[lubPhase] > 0.0f)
			{
				p->lsw_voltage_ref[lubPhase] = lf_voltage_ref[lubPhase] + 0.5f;
			}
			else
			{
				p->lsw_voltage_ref[lubPhase] = lf_voltage_ref[lubPhase] - 0.5f;
			}
		}
	}
}

/*****************************************************************************
 * 函数名称：s_lib_discharge_init
 * 函数输入：头文件中的结构体变量地址
 * 函数返回值：无
 * 函数描述：控制逆变给直流母线放电的初始化
 ****************************************************************************/
void s_lib_discharge_init(LIB_DISCHARGE_ST *p)
{
	INT8U lubPhase;
	for(lubPhase = cPhaseR;lubPhase < cPhaseNum;lubPhase++)
	{
		p->lf_inv_volt[lubPhase] = 0;
		p->lf_grid_volt[lubPhase] = 0;

		p->lub_relay_disconnect_cnt[lubPhase] = 0;
		p->lub_discharge_relayoff_phase[lubPhase] = 0;
		p->lf_angle[lubPhase] = (float)lubPhase * c2PiDivide3;	//三相初值分别为 0, 2pi/3, 4pi/3

		p->lf_current_ref[lubPhase] = 0;
		p->lsw_voltage_ref[lubPhase] = 0;
	}
	p->lub_discharge_relayoff_all = 1;	//初始值应设为1，若没断开接触器，程序里会将其清零。

	StateFlg.bit.ub_discharge_volt_low = 1;
	StateFlg.bit.ub_discharge_flag = 0;
	lsw_Volt_Comp = 0;
}

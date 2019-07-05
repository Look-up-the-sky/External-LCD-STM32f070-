/**************************************************************************
	文件名称：Lib_Discharge.h
	功能描述：用逆变部分的开关损耗对直流母线放电
	版权所有：苏州欧姆尼克新能源科技有限公司软件组
***************************************************************************
	变更纪录：
	变更日期 |  作者   |变更说明
	=========|=========|==================
	20180223 | 邢文超  |初次建立此文件
	20180308 | 邢文超  |XXXXXXXXXXXXXXX

********************************分割线*************************************/
#ifndef _LIB_DISCHARGE_H_
#define _LIB_DISCHARGE_H_

/*下面是结构体类型定义*/
typedef struct
{
	float	lf_inv_volt[cPhaseNum];		//输入，逆变侧交流电压（单相或三相）
	float	lf_grid_volt[cPhaseNum];	//输入，电网侧交流电压（单相或三相）
	
	INT8U	lub_relay_disconnect_cnt[cPhaseNum];	//模块内部自用变量
	INT8U	lub_discharge_relayoff_phase[cPhaseNum];//模块内部自用变量。此位为1表示接触器已断开，但此位为零不表示接触器闭合。
	INT8U	lub_discharge_relayoff_all;				//模块内部自用变量。此位为1表示接触器已断开，但此位为零不表示接触器闭合。
	float	lf_angle[cPhaseNum];					//模块内部自用变量。人为制造的正弦给定的角度,取值范围0~2PI

	float	lf_current_ref[cPhaseNum];	//输出
	INT16S	lsw_voltage_ref[cPhaseNum];	//输出
	
}LIB_DISCHARGE_ST;//建议结构体类型名全部用大写字母，以区别于对应的变量名(只是建议哈)

/**********************************分割线**********************************/
/*下面是函数声明*/
void s_lib_discharge_func(LIB_DISCHARGE_ST *);
void s_lib_discharge_init(LIB_DISCHARGE_ST *);
#endif
/*到此结束*/


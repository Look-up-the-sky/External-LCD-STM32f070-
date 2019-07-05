/**************************************************************************
	�ļ����ƣ�Lib_Discharge.h
	��������������䲿�ֵĿ�����Ķ�ֱ��ĸ�߷ŵ�
	��Ȩ���У�����ŷķ�������Դ�Ƽ����޹�˾�����
***************************************************************************
	�����¼��
	������� |  ����   |���˵��
	=========|=========|==================
	20180223 | ���ĳ�  |���ν������ļ�
	20180308 | ���ĳ�  |XXXXXXXXXXXXXXX

********************************�ָ���*************************************/
#ifndef _LIB_DISCHARGE_H_
#define _LIB_DISCHARGE_H_

/*�����ǽṹ�����Ͷ���*/
typedef struct
{
	float	lf_inv_volt[cPhaseNum];		//���룬���ཻ����ѹ����������ࣩ
	float	lf_grid_volt[cPhaseNum];	//���룬�����ཻ����ѹ����������ࣩ
	
	INT8U	lub_relay_disconnect_cnt[cPhaseNum];	//ģ���ڲ����ñ���
	INT8U	lub_discharge_relayoff_phase[cPhaseNum];//ģ���ڲ����ñ�������λΪ1��ʾ�Ӵ����ѶϿ�������λΪ�㲻��ʾ�Ӵ����պϡ�
	INT8U	lub_discharge_relayoff_all;				//ģ���ڲ����ñ�������λΪ1��ʾ�Ӵ����ѶϿ�������λΪ�㲻��ʾ�Ӵ����պϡ�
	float	lf_angle[cPhaseNum];					//ģ���ڲ����ñ�������Ϊ��������Ҹ����ĽǶ�,ȡֵ��Χ0~2PI

	float	lf_current_ref[cPhaseNum];	//���
	INT16S	lsw_voltage_ref[cPhaseNum];	//���
	
}LIB_DISCHARGE_ST;//����ṹ��������ȫ���ô�д��ĸ���������ڶ�Ӧ�ı�����(ֻ�ǽ����)

/**********************************�ָ���**********************************/
/*�����Ǻ�������*/
void s_lib_discharge_func(LIB_DISCHARGE_ST *);
void s_lib_discharge_init(LIB_DISCHARGE_ST *);
#endif
/*���˽���*/


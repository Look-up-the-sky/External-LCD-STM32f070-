/**************************************************************************
	�ļ����ƣ�Lib_Discharge.c
	��������������䲿�ֵĿ�����Ķ�ֱ��ĸ�߷ŵ�
	��Ȩ���У�����ŷķ�������Դ�Ƽ����޹�˾�����
***************************************************************************
	�����¼��
	������� |  ����   |���˵��
	=========|=========|==================
	20180223 | ���ĳ�  |���ν������ļ�
	20180515 | ���ĳ�  |���Ҳ�����������Ϊ�������arm_math����
	
********************************�ָ���*************************************/

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
#define c_50us_step 	(c2Pi/400.0f)		//�����������ĽǶ��ۼӲ������Ӹ����Ÿ���ȫ
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
 * �������ƣ�s_lib_discharge_func
 * �������룺ͷ�ļ��еĽṹ�������ַ
 * ��������ֵ����
 * ������������������ֱ��ĸ�߷ŵ�
 ****************************************************************************/
void s_lib_discharge_func(LIB_DISCHARGE_ST *p)
{
	float lf_relay_volt[cPhaseNum];
	float lf_sine_ref[cPhaseNum];
	float lf_voltage_ref[cPhaseNum];
	INT8U lubPhase;
/********************************************************************/
	cLRelayOff();						//�Ͽ������Ӵ���
	cNRelayOff();
	StateFlg.bit.ubSlaveRelay = 0;		//cSlaveRelayOff
	sTurnOffBoostPWM();					//����ն��������

/********************************************************************/
//�жϽӴ����Ƿ��ѷֶϡ�����Ӵ��������ѹ˲ʱֵ�Ĳ�ľ���ֵ������100�δ���50V�����ж�Ϊ�Ӵ����ѶϿ���
	
	p->lub_discharge_relayoff_all = 1;//����Ϊ1������нӴ���δ�ֶϣ����������λ������������
	
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
			p->lub_discharge_relayoff_phase[lubPhase] = 1;//�˱�־λΪ1����ʾ��Ӧ��ĽӴ����ѶϿ����˱�־λ�ڷŵ����������
		}
		p->lub_discharge_relayoff_all &= p->lub_discharge_relayoff_phase[lubPhase];//����нӴ���δ�ֶϣ��������ܱ�־λ
	}

/********************************************************************/
	if(p->lub_discharge_relayoff_all == 1)	//����Ӵ����ѶϿ�������Ը�BUS�ŵ硣
	{
		sTurnOnInvPWM();	//ʹ������PWM
		cInvOn();
/********************************************************************/
//��Ϊ����һ����Ƶ���Ҳ�����
		for(lubPhase = cPhaseR;lubPhase < cPhaseNum;lubPhase++)
		{
			p->lf_angle[lubPhase] += c_50us_step;		//�Ƕ��ۼ�
			if(p->lf_angle[lubPhase] >= c2Pi)
			{
				p->lf_angle[lubPhase] -= c2Pi;
			}

//			lf_sine_ref[lubPhase] = arm_sin_f32(p->lf_angle[lubPhase]);					//��������
			lf_sine_ref[lubPhase] = gfSinTabArray[(INT16U)(p->lf_angle[lubPhase] * 114.5f)];//��Ϊ����Խ�ʡ�ڴ�
			lf_voltage_ref[lubPhase] = 0.9f * cINVPWMPeriodf * lf_sine_ref[lubPhase];	//��ѹ����
			p->lf_current_ref[lubPhase] = lf_sine_ref[lubPhase];						//��װ�ǵ���

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
 * �������ƣ�s_lib_discharge_init
 * �������룺ͷ�ļ��еĽṹ�������ַ
 * ��������ֵ����
 * ������������������ֱ��ĸ�߷ŵ�ĳ�ʼ��
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
		p->lf_angle[lubPhase] = (float)lubPhase * c2PiDivide3;	//�����ֵ�ֱ�Ϊ 0, 2pi/3, 4pi/3

		p->lf_current_ref[lubPhase] = 0;
		p->lsw_voltage_ref[lubPhase] = 0;
	}
	p->lub_discharge_relayoff_all = 1;	//��ʼֵӦ��Ϊ1����û�Ͽ��Ӵ�����������Ὣ�����㡣

	StateFlg.bit.ub_discharge_volt_low = 1;
	StateFlg.bit.ub_discharge_flag = 0;
	lsw_Volt_Comp = 0;
}

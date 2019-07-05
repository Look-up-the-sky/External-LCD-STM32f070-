/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�Lib_PowerCtrlByMeter.c
 * �޸��ˣ��²�
 * �޸����ڣ�2014.11.21
 * ���������ݵ����ֵ�͵�ǰ�Ĺ���ֵ�����趨����
 * �޸����ݣ�
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"
#include "Lib_Communication.h"

/********************************************************************
 * Local Types
 ********************************************************************/
INT8U lubMeterLoss;

#ifdef FloatEnable
float lfPowerByMeter;
#else
INT16S lswPowerByMeter;
#endif

INT16S lswMeterPowerPre;

PQDDATA PQDDataPre;

/********************************************************************
 * Extern Global Types
 ********************************************************************/
#ifdef FloatEnable
extern float gfRatedPower;
#else
extern INT16U guwRatedPower;
#endif

#ifdef FloatEnable
/*****************************************************************************
 * �������ܣ����������ܺ�����
 * �������룺����ܹ��ʣ�������ܹ��ʡ�
 * ��������������״̬��1s����һ��
 ****************************************************************************/
float sSetPowerByMeter(PQDDATA *PQDDataTemp, float lfInvPower)
{
	INT16U i;
	
	for(i = 0;i < cPQDDataSize;i++)
	{
		if(PQDDataTemp->all[i] != PQDDataPre.all[i])
		{
			break;											//�����ݸ�ǰһ֡���ݲ�һ����˵���и���
		}
	}
	
	if(i != cPQDDataSize)
	{
		lfPowerByMeter = lfInvPower + (float)(PQDDataTemp->data.swActivePower[cPhaseR] + PQDDataTemp->data.swActivePower[cPhaseS] + PQDDataTemp->data.swActivePower[cPhaseT]) / 2;
		lubMeterLoss = 0;
	}
	else if(++lubMeterLoss >= 10)							//����10�����ڵò����µ����ݣ���ΪPQD�ѶϿ�
	{
		lubMeterLoss = 10;
		lfPowerByMeter = gfRatedPower;
	}
	
	for(i = 0;i < cPQDDataSize;i++)
	{
		PQDDataPre.all[i] = PQDDataTemp->all[i];
	}
	
	if(lfPowerByMeter > gfRatedPower)
	{
		lfPowerByMeter = gfRatedPower;
	}
	else if(lfPowerByMeter < c0W)
	{
		lfPowerByMeter = c0W;
	}
	
	return(lfPowerByMeter);
}

/*****************************************************************************
 * �������ܣ��������������ݳ�ʼ��������
 * �������룺��
 * ����������������״̬�µ���
 ****************************************************************************/
void sPowerByMeterInit(void)
{
	INT16U i;

	lfPowerByMeter = gfRatedPower;
	for(i = 0;i < cPQDDataSize;i++)
	{
		PQDDataPre.all[i] = c0W;
	}
	lubMeterLoss = 0;
}
#else
INT16U sSetPowerByMeter(PQDDATA *PQDDataTemp, INT16U luwInvPower)
{
	INT16U i;
	
	for(i = 0;i < cPQDDataSize;i++)
	{
		if(PQDDataTemp->all[i] != PQDDataPre.all[i])
		{
			break;											//�����ݸ�ǰһ֡���ݲ�һ����˵���и���
		}
	}
	
	if(i != cPQDDataSize)
	{
		lswPowerByMeter = (INT16S)luwInvPower + (PQDDataTemp->data.swActivePower[cPhaseR] + PQDDataTemp->data.swActivePower[cPhaseS] + PQDDataTemp->data.swActivePower[cPhaseT]) / 2;
		lubMeterLoss = 0;
	}
	else if(++lubMeterLoss >= 10)							//����10�����ڵò����µ����ݣ���ΪPQD�ѶϿ�
	{
		lubMeterLoss = 10;
		lswPowerByMeter = guwRatedPower;
	}
	
	for(i = 0;i < cPQDDataSize;i++)
	{
		PQDDataPre.all[i] = PQDDataTemp->all[i];
	}
	
	if(lswPowerByMeter > (INT16S)guwRatedPower)
	{
		lswPowerByMeter = (INT16S)guwRatedPower;
	}
	else if(lswPowerByMeter < c0W)
	{
		lswPowerByMeter = c0W;
	}
	
	return(lswPowerByMeter);
}

/*****************************************************************************
 * �������ܣ��������������ݳ�ʼ��������
 * �������룺��
 * ����������������״̬�µ���
 ****************************************************************************/
void sPowerByMeterInit(void)
{
	INT16U i;

	lswPowerByMeter = (INT16S)guwRatedPower;
	for(i = 0;i < cPQDDataSize;i++)
	{
		PQDDataPre.all[i] = c0W;
	}
	lubMeterLoss = 0;
}
#endif

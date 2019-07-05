/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�Lib_PowerDecreaseByVolt.c
 * �޸��ˣ��²�
 * �޸����ڣ�2015.06.24
 * ��������ѹ����
 * �޸����ݣ�
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "app.h"

/********************************************************************
 * Local Types
 ********************************************************************/
#ifdef FloatEnable
float lfVoltPoint[2];
#else
INT16U luwDcrsVolt;
INT16U luwKeepVolt;
INT16U luwVoltPoint[2];
#endif

#ifdef FloatEnable
/*****************************************************************************
 * �������ƣ���ѹ���غ�����
 * �������룺������ѹ��
 * ��������ֵ����������ʰٷֱȡ�
 * ����������
 ****************************************************************************/
float sPowerByVolt(float lfGridVolt)
{
	if(lfGridVolt < lfVoltPoint[0])
	{
		return(c100Percent);
	}
	else if(lfGridVolt < lfVoltPoint[1])
	{
		return((lfVoltPoint[1] - lfGridVolt) / (lfVoltPoint[1] - lfVoltPoint[0]));
	}
	else
	{
		return(c0Percent);
	}
}

/*****************************************************************************
 * �������ƣ����ص�ѹ�趨������
 * �������룺���ص�ѹֵ��
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
void sSetPowerByVoltLimit(float lfVolt1,float lfVolt2)
{
	lfVoltPoint[0] = lfVolt1;
	lfVoltPoint[1] = lfVolt2;
	
	if(lfVoltPoint[0] < c230V)
	{
		lfVoltPoint[0] = c230V;
	}
	if(lfVoltPoint[1] < lfVoltPoint[0] + c2V)
	{
		lfVoltPoint[1] = lfVoltPoint[0] + c2V;
	}
}
#else
/*****************************************************************************
 * �������ƣ���ѹ���غ�����
 * �������룺������ѹ��
 * ��������ֵ�����������ֵ��
 * ����������
 ****************************************************************************/
INT8U sPowerByVolt(INT16U luwGridVolt)
{
	if(luwGridVolt > luwDcrsVolt)
	{
		return(2);
	}
	else if(luwGridVolt > luwKeepVolt)
	{
		return(1);
	}
	else
	{
		return(0);
	}
}

/*****************************************************************************
 * �������ƣ����ص�ѹ�趨������
 * �������룺���ص�ѹֵ��
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
void sSetPowerByVoltLimit(INT16U luwVolt1,INT16U luwVolt2)
{
	luwDcrsVolt = luwVolt1;
	luwKeepVolt = luwVolt2;

	if(luwDcrsVolt < c253VAC)
	{
		luwDcrsVolt = c253VAC;
	}
	else if(luwDcrsVolt > c276VAC)
	{
		luwDcrsVolt = c276VAC;
	}

	if(luwKeepVolt + c5VAC > luwDcrsVolt)
	{
		luwKeepVolt = luwDcrsVolt - c5VAC;
	}
	else if(luwKeepVolt + c10VAC < luwDcrsVolt)
	{
		luwKeepVolt = luwDcrsVolt - c10VAC;
	}
}
#endif

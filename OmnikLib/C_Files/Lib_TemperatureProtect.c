/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�TemperatureProtect.c
 * �޸��ˣ��²�
 * �޸����ڣ�2014.10.29
 * ���������½���
 * �޸����ݣ�
 * 1.1��--�¶ȹ��ϻָ�����ָ���������
 * 1.2��--�������͵ı������ƣ�
 * 1.3��--�¶Ȼָ�����Ϊ110%�Ķ���ʣ�
 * 1.4��--includeĿ¼��Ϊ����Ŀ���趨��
 * 1.5��--������ֵ��Ϊ����ʵİٷֱȣ�
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
 * �������ƣ����½����¶ȵ����ú�����
 * �������룺���½�����ʼ�¶ȣ����½��ؽ�ֹ�¶ȡ�
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
void sSetTPoint(float lfTemp1,float lfTemp2)
{
	lfTempS = lfTemp1;
	lfTempE = lfTemp2;
}

/*****************************************************************************
 * �������ܣ����±�����
 * �������룺������¶ȡ�
 * �����������޹��¹���ʱ���жϵ�ǰ�¶ȣ���������¶����ޣ����ù��¹��ϱ�־λ������
 * 			 �㹦�ʣ����ڽ��ط�Χ�ڣ��򷵻ؼ���Ľ��ع��ʣ������ڽ��ط�Χ���򷵻ض�
 * 			 �����ʣ����Ѿ��������¹���ʱ������ǰ�¶ȵ��ڽ��ط�Χ��������¹��ϱ�־
 * 			 λ�����ض���ʣ������ڽ��ط�Χ����������������ض���ʡ�
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
 * �������ܣ�
 * �������룺�ޡ�
 * ����������
 ****************************************************************************/
void sCalPowerByTemperatureInitial(void)
{
	lfPwrPercentByT = c100Percent;
	luwTempBackCnt  = 0;
}
#else
/*****************************************************************************
 * �������ƣ����½����¶ȵ����ú�����
 * �������룺���½�����ʼ�¶ȣ����½��ؽ�ֹ�¶ȡ�
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
void sSetTPoint(INT16U luwTemp1,INT16U luwTemp2)
{
	luwTempS = luwTemp1;
	luwTempE = luwTemp2;
}

/*****************************************************************************
 * �������ܣ����±�����
 * �������룺������¶ȡ�
 * �����������޹��¹���ʱ���жϵ�ǰ�¶ȣ���������¶����ޣ����ù��¹��ϱ�־λ������
 * 			 �㹦�ʣ����ڽ��ط�Χ�ڣ��򷵻ؼ���Ľ��ع��ʣ������ڽ��ط�Χ���򷵻ض�
 * 			 �����ʣ����Ѿ��������¹���ʱ������ǰ�¶ȵ��ڽ��ط�Χ��������¹��ϱ�־
 * 			 λ�����ض���ʣ������ڽ��ط�Χ����������������ض���ʡ�
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
 * �������ܣ�
 * �������룺�ޡ�
 * ����������
 ****************************************************************************/
void sCalPowerByTemperatureInitial(void)
{
	luwPwrPercentByT = 100;
	luwTempBackCnt  = 0;
}

#endif

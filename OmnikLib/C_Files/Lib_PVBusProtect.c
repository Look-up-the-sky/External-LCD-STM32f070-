/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�Lib_PVBusProtect.c
 * �޸��ˣ��²�
 * �޸����ڣ�2015.05.07
 * ������PV��Bus��ѹ����
 * �޸����ݣ�
 * 1.1��--�޸�״̬��־λ���ƣ�
 * 1.2��--�ӿ�PV��Bus�ı����ٶȣ�
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "app.h"

/********************************************************************
 * Macros
 ********************************************************************/


/********************************************************************
 * Macros
 ********************************************************************/


/********************************************************************
 * Local Types
 ********************************************************************/
INT8U lubPVVoltLowCnt;
INT8U lubPVVoltHighCnt;
INT8U lubPVVoltNormalCnt;
INT8U lubBusVoltLowCnt;
INT8U lubPVVoltLowCnt_SinglePV;
INT8U lubPVVoltNormalCnt_SinglePV;

/********************************************************************
 * Global Types
 ********************************************************************/

#ifdef FloatEnable
/*****************************************************************************
 * �������ƣ�PV��ѹ��⺯��
 * �������룺PV��ѹ������·����PV��ѹ����
 * ��������ֵ����
 * ����������
 ****************************************************************************/
void sPVVoltCheck(float *lfPVVolt,INT8U lubPVNum,float lfPVVoltLow)
{
	INT8U i;
	float lfMaxPVVolt;
	
	lfMaxPVVolt = c0V;
	
	for(i = 0;i < lubPVNum;i++)
	{
		if(lfMaxPVVolt < lfPVVolt[i])
		{
			lfMaxPVVolt = lfPVVolt[i];
		}
	}
	
	if(lfMaxPVVolt < lfPVVoltLow)
	{
		if(++lubPVVoltLowCnt >= 2)
		{
			Error.bit.mubPVVoltLow = cTrue;
			Error.bit.mubPVVoltHigh = cFalse;
			lubPVVoltLowCnt = 0;
		}
		lubPVVoltNormalCnt = 0;
	}
	else if(lfMaxPVVolt > cMaxPVVolt)
	{
		if(++lubPVVoltHighCnt >= 2)
		{
			Error.bit.mubPVVoltHigh = cTrue;
			Error.bit.mubPVVoltLow = cFalse;
			lubPVVoltHighCnt = 0;
		}
		lubPVVoltNormalCnt = 0;
	}
	else
	{
		if(Error.bit.mubPVVoltLow == cTrue || Error.bit.mubPVVoltHigh == cTrue)
		{
			if(++lubPVVoltNormalCnt >= 250)
			{
				lubPVVoltLowCnt = 0;
				lubPVVoltHighCnt = 0;
				Error.bit.mubPVVoltLow = cFalse;
				Error.bit.mubPVVoltHigh = cFalse;
			}
		}
	}
}

/*****************************************************************************
 * �������ƣ���·PV��ѹ�͹����жϺ���
 * �������룺ĳһ·PV��ѹ��PV��ѹ����
 * ��������ֵ����
 * ����������
 ****************************************************************************/
void sPVVoltLowCheck_SinglePV(float lfPVVolt,float lfPVVoltLow)
{
	if(lfPVVolt < lfPVVoltLow)
	{
		if(++lubPVVoltLowCnt_SinglePV >= 2)
		{
			Error.bit.mubPVVoltLow = cTrue;
			lubPVVoltLowCnt_SinglePV = 0;
		}
		lubPVVoltNormalCnt_SinglePV = 0;
	}
	else
	{
		if(Error.bit.mubPVVoltLow == cTrue)
		{
			if(++lubPVVoltNormalCnt_SinglePV >= 250)
			{
				lubPVVoltLowCnt_SinglePV = 0;
				Error.bit.mubPVVoltLow = cFalse;
			}
		}
	}
}

/*****************************************************************************
 * �������ƣ�Bus��ѹ�ͼ�⺯��
 * �������룺Bus��ѹ��Bus��ѹ����
 * ��������ֵ����
 * ����������
 ****************************************************************************/
void sBusVoltLowCheck(float lfBusVolt,float lfBusVoltLow)
{
	if(lfBusVolt < lfBusVoltLow)
	{
		if(++lubBusVoltLowCnt >= 100)
		{
			Error.bit.mubBusVoltLow = cTrue;
			lubBusVoltLowCnt = 0;
		}
	}
	else
	{
		lubBusVoltLowCnt = 0;
	}
}
#else
/*****************************************************************************
 * �������ƣ�PV��ѹ��⺯��
 * �������룺PV��ѹ������·����PV��ѹ����
 * ��������ֵ����
 * ����������
 ****************************************************************************/
void sPVVoltCheck(INT16U *luwPVVolt,INT8U lubPVNum,INT16U luwPVVoltLow)
{
	INT8U i;
	INT16U luwMaxPVVolt;
	
	luwMaxPVVolt = c0V;
	
	for(i = 0;i < lubPVNum;i++)
	{
		if(luwMaxPVVolt < luwPVVolt[i])
		{
			luwMaxPVVolt = luwPVVolt[i];
		}
	}
	
	if(luwMaxPVVolt < luwPVVoltLow)
	{
		if(++lubPVVoltLowCnt >= 2)
		{
			Error.bit.mubPVVoltLow = cTrue;
			Error.bit.mubPVVoltHigh = cFalse;
			lubPVVoltLowCnt = 0;
		}
		lubPVVoltNormalCnt = 0;
	}
	else if(luwMaxPVVolt > cMaxPVVolt)
	{
		if(++lubPVVoltHighCnt >= 2)
		{
			Error.bit.mubPVVoltHigh = cTrue;
			Error.bit.mubPVVoltLow = cFalse;
			lubPVVoltHighCnt = 0;
		}
		lubPVVoltNormalCnt = 0;
	}
	else
	{
		if(Error.bit.mubPVVoltLow == cTrue || Error.bit.mubPVVoltHigh == cTrue)
		{
			if(++lubPVVoltNormalCnt >= 250)
			{
				lubPVVoltLowCnt = 0;
				lubPVVoltHighCnt = 0;
				Error.bit.mubPVVoltLow = cFalse;
				Error.bit.mubPVVoltHigh = cFalse;
			}
		}
	}
}

/*****************************************************************************
 * �������ƣ�PV&Bus��ѹ��⺯��
 * �������룺Bus��ѹ��Bus��ѹ����
 * ��������ֵ����
 * ����������
 ****************************************************************************/
void sBusVoltLowCheck(INT16U luwBusVolt,INT16U luwBusVoltLow)
{
	if(luwBusVolt < luwBusVoltLow)
	{
		if(++lubBusVoltLowCnt >= 100)
		{
			Error.bit.mubBusVoltLow = cTrue;
			lubBusVoltLowCnt = 0;
		}
	}
	else
	{
		lubBusVoltLowCnt = 0;
	}
}
#endif

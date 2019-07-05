/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�DCCurrentAdj.c
 * �޸��ˣ��²�
 * �޸����ڣ�2015.05.18
 * ��������������ֱ����������
 * �޸����ݣ�
 * 1.1��--�޸�״̬��־λ���ƣ�
 * 1.2��--��Сֱ�������ɵ�����Χ��
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"

/********************************************************************
 * Local Types
 ********************************************************************/
INT16U luwDC0ACnt;

#ifdef FloatEnable
float lfDC0ASum[cPhaseNum];
#else

#endif

/********************************************************************
 * Global Types
 ********************************************************************/
#ifdef FloatEnable
float gfSinAdj[cPhaseNum];
float gfDC0A[cPhaseNum];
#else

#endif

/********************************************************************
 * Extern Global Types
 ********************************************************************/
#ifdef FloatEnable

#else

#endif

#ifdef FloatEnable
/*****************************************************************************
 * �������ƣ�ֱ���������ƺ�����
 * �������룺�ޡ�
 * �����������жϲ�������ֱ�������Ƿ񳬹�������ֵ��������ֵ�󣬵���ֱ���������ڱ�����
 ****************************************************************************/
void sCurrentDCAdj(const float *lfDCCurr, float lfDCCurrAdjMin, INT8U lubPhaseNum)
{
	INT8U i;
	float lfTemp;
	
	if(lfDCCurrAdjMin < c0A01)
	{
		for(i = cPhaseR;i < lubPhaseNum;i++)
		{
			gfSinAdj[i] = c0A;
		}
	}
	else
	{
		for(i = cPhaseR;i < lubPhaseNum;i++)
		{
			if(lfDCCurr[i] > lfDCCurrAdjMin || lfDCCurr[i] < -lfDCCurrAdjMin)
			{
				lfTemp = gfSinAdj[i];
				lfTemp -= lfDCCurr[i] * 0.5f;
				if(lfTemp > cMaxDCIAdj)
				{
					lfTemp = cMaxDCIAdj;
				}
				else if(lfTemp < -cMaxDCIAdj)
				{
					lfTemp = -cMaxDCIAdj;
				}
				gfSinAdj[i] = lfTemp;
			}
		}
	}
}

/*****************************************************************************
 * �������ƣ�ֱ������0ֵ���㺯����
 * �������룺�ޡ�
 * �������������㲢������ֱ��������0ֵ��
 ****************************************************************************/
void sCalDC0A(const float *lfDCCurr,const INT8U lubPhaseNum)
{
	INT8U i;
	float lfTemp;
	
	if(lubPhaseNum > cPhaseNum)
	{
		return;
	}
	
	if(StateFlg.bit.ubCalDC0AStep == 0)
	{
		for(i = cPhaseR;i < lubPhaseNum;i++)
		{
			gfDC0A[i] = c0A;
			lfDC0ASum[i] = c0A;
		}
		luwDC0ACnt = 0;
	}
	else if(StateFlg.bit.ubCalDC0AStep == 1)
	{
		for(i = cPhaseR;i < lubPhaseNum;i++)
		{
			lfDC0ASum[i] += lfDCCurr[i];
		}
		luwDC0ACnt++;
	}
	else if(StateFlg.bit.ubCalDC0AStep == 2)
	{
		if(luwDC0ACnt != 0)
		{
			for(i = cPhaseR;i < lubPhaseNum;i++)
			{
				lfTemp = lfDC0ASum[i] / luwDC0ACnt;
				if(lfTemp > cMaxDCIAdj)
				{
					lfTemp = cMaxDCIAdj;
				}
				else if(lfTemp < -cMaxDCIAdj)
				{
					lfTemp = -cMaxDCIAdj;
				}
				gfDC0A[i] = lfTemp;
			}
		}
		StateFlg.bit.ubCalDC0AStep = 3;
	}
}

#else
#endif

/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�CrossZeroRelayOn.c
 * �޸��ˣ����궬
 * �������̵�����������Ͽ���
 * �������ڣ�2017.06.22
 * �������ݣ�
 *******************************************************/
 
/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"
#include "Lib_CrossZeroRelayOn.h"
#include "Lib_Communication.h"

/********************************************************************
 * Local Types
 ********************************************************************/
INT16U luwCrossZeroDelayCnt;
INT16U luwRelayDelayOnCnt;
INT16U luwRelayDelayOnAdjCnt;

/*****************************************************************************
 * �������ƣ�����������
 * �������룺��
 * ��������ֵ����
 * �������������̵���������ʱ���ϱ�־λ���ڹ����ж��н������־λ��λ
 *****************************************************************************/
void sCrossZeroChk(void)
{
	if(StateFlg.bit.ubNRelayDelayOn)
	{
		StateFlg.bit.ubCrossZeroFlg = cTrue;
	}
}

/*****************************************************************************
 * �������ƣ�����̵�������������ʱʱ��
 * �������룺����Ƶ��(Hz)���̵�������ʱ��(us)����ʱ����(us)
 * ��������ֵ����
 * ����������
 *****************************************************************************/
void sCalCrossZeroRelayOn(float lfGridFreq, INT16U luwRelayAction, INT16S lswRelayDelayOnAdj)
{
	INT16U luwTemp;
	luwTemp = 1e+6 / (INT16U)lfGridFreq;
	luwTemp -= luwRelayAction;
	luwTemp -= lswRelayDelayOnAdj;
	luwTemp /= 1e+3 / c20KHz;
	luwRelayDelayOnCnt = luwTemp;
}

/*****************************************************************************
 * �������ƣ��̵�����������ִ��
 * �������룺����Ƶ��(Hz)���̵������˵�ѹ(V)
 * ��������ֵ��
 * ����������
 *****************************************************************************/
void sActCrossZeroRelayOn(float lfGridFreq, float lfRelayVolt)
{
	float lfTemp;
	/************��ʼ���������������󣬼̵�������************/
	if(StateFlg.bit.ubNRelayDelayOn == cTrue && StateFlg.bit.ubCrossZeroFlg == cTrue)
	{
		/************�ƴӹ���㿪ʼ�����ϼ̵�����ʱ��************/
		luwCrossZeroDelayCnt++;
		if(luwCrossZeroDelayCnt >= luwRelayDelayOnCnt)
		{
			/************���ϼ̵���************/
			cNRelayOn();
			/************���ݼ̵������˵�ѹ�õ��̵�������ʱ�䣬�ж��Ƿ���Ҫ����************/
			/************�жϼ̵�������ʱ�����Ƿ�ӽ�0V************/
			if(lfRelayVolt >= -c10V && lfRelayVolt <= c10V)
			{
				luwRelayDelayOnAdjCnt++;
				if(luwRelayDelayOnAdjCnt >= c300usCnt)//�������300us(6��)��0�㸽��
				{
					lfTemp = (float)luwCrossZeroDelayCnt;
					lfTemp -= c300usCnt / 2;
					lfTemp *= 1e+3 / c20KHz;//�����ʱ�䣬��λus
					lfTemp -= (float)1e+6 / lfGridFreq;//��ȥһ�����ڵ�ʱ�䣬Լ20ms
					if(lfTemp < -c200us || lfTemp > c200us)//������0.2ms���ϣ�������������Eeprom
					{
						lfTemp /= 2;
						Safty.data.swNRelayDelayOnAdj += (INT16S)lfTemp;
						sNRelayDelayOnAdj(cTrue);
					}
					sNRelayDelayOn(cFalse);
				}
			}
			else
			{
				luwRelayDelayOnAdjCnt = 0;
			}
		}
	}
	else
	{
		luwCrossZeroDelayCnt = 0;
		StateFlg.bit.ubCrossZeroFlg = cFalse;
	}	
}

/*****************************************************************************
 * �������ƣ��̵�����ʱ����ʹ��
 * �������룺��־λ
 * ��������ֵ����
 * ����������
 *****************************************************************************/
void sNRelayDelayOn(INT8U lubEnFlg)
{
	StateFlg.bit.ubNRelayDelayOn = lubEnFlg;
}

/*****************************************************************************
 * �������ƣ��̵�����ʱ����ʹ��
 * �������룺��־λ
 * ��������ֵ����
 * ����������
 *****************************************************************************/
void sNRelayDelayOnAdj(INT8U lubEnFlg)
{
	StateFlg.bit.ubNRelayDelayOnAdj = lubEnFlg;
}

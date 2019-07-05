/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�Lib_DCCurrentCheck.c
 * �޸��ˣ��²�
 * �޸����ڣ�2014.10.29
 * ��������������ֱ���������
 * �޸����ݣ�
 * 1.1��--������ȫ����Ϊ�ֲ�������
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"

/********************************************************************
 * Local Types
 ********************************************************************/
#ifdef FloatEnable
float lfDCAvg;
float lfDCIZeroCrossAvg;
float lfDCISum;
float lfDCLimit1;
float lfDCLimit2;
float lfDCBack;
#else
INT16S lswDCAvg;
INT16S lswDCIZeroCrossAvg;
INT32S lslDCISum;
INT16S lswDCLimit1;
INT16S lswDCLimit2;
INT16S lswDCBack;
#endif

INT16U luwDCFault1Cnt[cPhaseNum];
INT16U luwDCFault2Cnt[cPhaseNum];
INT16U luwDCFaultBackCnt;
INT16U luwDCFaultCntLimit1;
INT16U luwDCFaultCntLimit2;
INT16U luwDCFaultBackCntLimit;

/*****************************************************************************
 * �������ƣ�����ֱ����������ʱ�亯����
 * �������룺ֱ����������ʱ��1��ֱ����������ʱ��2��ֱ�������ָ�ʱ�䡣
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
void sSetDCLimitTime(INT16U luwTime1,INT16U luwTime2,INT16U luwBackTime)
{
	luwDCFaultCntLimit1 = luwTime1;
	luwDCFaultCntLimit2 = luwTime2;
	luwDCFaultBackCntLimit = luwBackTime;
}

#ifdef FloatEnable
/*****************************************************************************
 * �������ƣ�����ֱ������������ֵ������
 * �������룺ֱ������������ֵ1��ֱ������������ֵ2��ֱ�������ָ���ֵ��
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
void sSetDCLimit(float lfLimit1,float lflimit2,float lfBackLimit)
{
	lfDCLimit1 = lfLimit1;
	lfDCLimit2 = lflimit2;
	lfDCBack = lfBackLimit;
}

/*****************************************************************************
 * �������ܣ���������ֱ������������
 * �������룺�ޡ�
 * �����������жϲ�������ֱ�������Ƿ񳬹�������ֵ������������ֵ����ֱ���������ϱ�
 * 			 ־λ��
 ****************************************************************************/
INT8U sCurrentDCCheck(const float *lfDCCurr, INT8U lubPhaseNum,INT8U lubError)
{
	INT8U lubPhase;
	
	if(lubError == 0)
	{
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			if(lfDCCurr[lubPhase] > lfDCLimit2 || lfDCCurr[lubPhase] < -lfDCLimit2)
			{
				luwDCFault2Cnt[lubPhase]++;
				luwDCFault1Cnt[lubPhase]++;
				if(luwDCFault2Cnt[lubPhase] >= luwDCFaultCntLimit2)
				{
					luwDCFault2Cnt[lubPhase] = 0;
					luwDCFault1Cnt[lubPhase] = 0;
					return(1);
				}
			}
			else if(lfDCCurr[lubPhase] > lfDCLimit1 || lfDCCurr[lubPhase] < -lfDCLimit1)
			{
				luwDCFault2Cnt[lubPhase] = 0;
				luwDCFault1Cnt[lubPhase]++;
				if(luwDCFault1Cnt[lubPhase] >= luwDCFaultCntLimit1)
				{
					luwDCFault1Cnt[lubPhase] = 0;
					return(1);
				}
			}
			else
			{
				luwDCFault2Cnt[lubPhase] = 0;
				luwDCFault1Cnt[lubPhase] = 0;
			}
		}
		return(0);
	}
	else
	{
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			if(lfDCCurr[lubPhase] > lfDCBack || lfDCCurr[lubPhase] < -lfDCBack)
			{
				luwDCFaultBackCnt = 0;
			}
		}
		
		if(++luwDCFaultBackCnt >= luwDCFaultBackCntLimit)
		{
			luwDCFaultBackCnt = 0;
			return(0);
		}
		return(1);
	}
}
#else
/*****************************************************************************
 * �������ƣ�����ֱ������������ֵ������
 * �������룺ֱ������������ֵ1��ֱ������������ֵ2��ֱ�������ָ���ֵ��
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
void sSetDCLimit(INT16S lswLimit1,INT16S lswlimit2,INT16S lswBackLimit)
{
	lswDCLimit1 = lswLimit1;
	lswDCLimit2 = lswlimit2;
	lswDCBack = lswBackLimit;
}

/*****************************************************************************
 * �������ܣ���������ֱ������������
 * �������룺�ޡ�
 * �����������жϲ�������ֱ�������Ƿ񳬹�������ֵ������������ֵ����ֱ���������ϱ�
 * 			 ־λ��
 ****************************************************************************/
INT8U sCurrentDCCheck(const INT16S *lswDCCurr, INT8U lubPhaseNum,INT8U lubError)
{
	INT8U lubPhase;
	
	if(lubError == 0)
	{
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			if(lswDCCurr[lubPhase] > lswDCLimit2 || lswDCCurr[lubPhase] < -lswDCLimit2)
			{
				luwDCFault2Cnt[lubPhase]++;
				luwDCFault1Cnt[lubPhase]++;
				if(luwDCFault2Cnt[lubPhase] >= luwDCFaultCntLimit2)
				{
					luwDCFault2Cnt[lubPhase] = 0;
					luwDCFault1Cnt[lubPhase] = 0;
					return(1);
				}
			}
			else if(lswDCCurr[lubPhase] > lswDCLimit1 || lswDCCurr[lubPhase] < -lswDCLimit1)
			{
				luwDCFault2Cnt[lubPhase] = 0;
				luwDCFault1Cnt[lubPhase]++;
				if(luwDCFault1Cnt[lubPhase] >= luwDCFaultCntLimit1)
				{
					luwDCFault1Cnt[lubPhase] = 0;
					return(1);
				}
			}
			else
			{
				luwDCFault2Cnt[lubPhase] = 0;
				luwDCFault1Cnt[lubPhase] = 0;
			}
		}
		return(0);
	}
	else
	{
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			if(lswDCCurr[lubPhase] > lswDCBack || lswDCCurr[lubPhase] < -lswDCBack)
			{
				luwDCFaultBackCnt = 0;
			}
		}
		
		if(++luwDCFaultBackCnt >= luwDCFaultBackCntLimit)
		{
			luwDCFaultBackCnt = 0;
			return(0);
		}
		return(1);
	}
}
#endif

/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�OverVolt10minProtect.c
 * �޸��ˣ��²�
 * ����������10���ӹ�ѹ����
 * �������ڣ�2014.10.29
 * �������ݣ�
 * 1.1��--�����߼��븡���߼�����һ��     �漪��20140305
 * 1.2��--c0VӦΪc0VAC
 * 1.3��--includeĿ¼��Ϊ����Ŀ���趨��
 * 1.4��--�Ż��㷨��
 *******************************************************/

#include "App.h"

#ifdef FloatEnable
#ifndef DebugEnable						//10���ӹ�ѹ������800���ֽڵ�ram�����ͬʱ����Debugģʽ��ram������
#define cMaxVoltNum	200

INT16U luw3sGridVoltCnt;
INT16U luwGridVoltSumCount;

float lfGridVoltBuffer[cPhaseNum][cMaxVoltNum];
float lfGridVolt3sAvg[cPhaseNum];
float lfGridVolt3sSum[cPhaseNum];
float lf10MinGridHigh;
#endif
#else
#define cMaxVoltNum	16

INT16U luw30sGridVoltCnt;
INT16U luwGridVoltSumCount;

INT16U luwGridVoltBuffer[cPhaseNum][cMaxVoltNum];
INT32U lulGridVolt30sAvg[cPhaseNum];
INT32U lulGridVolt30sSum[cPhaseNum];
INT16U luw10MinGridHigh;
#endif

#ifdef FloatEnable
/*****************************************************************************
 * �������ƣ�10���ӹ�ѹ����ֵ���ú�����
 * �������룺10���ӹ�ѹ������ֵ��
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
void sSet10MinLimit(float lfLimit)
{
#ifndef DebugEnable
	lf10MinGridHigh = lfLimit;
#endif
}

/*****************************************************************************
 * �������ܣ���ʼ��10���ӹ�ѹ�����ı�����
 * �������룺�ޡ�
 * ������������10���ӹ�ѹ�����ļ�ʱ����0��30sƽ��ֵ��������ACֵ��
 ****************************************************************************/
void s10minVoltInitial(INT8U lubPhaseNum)
{
#ifndef DebugEnable
	INT8U lubPhase;

	for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
	{
		for(luw3sGridVoltCnt = 0;luw3sGridVoltCnt < cMaxVoltNum;luw3sGridVoltCnt++)
		{
			lfGridVoltBuffer[lubPhase][luw3sGridVoltCnt] = cNormalACVolt;
		}
		lfGridVolt3sSum[lubPhase] = c0V;
	}
	luw3sGridVoltCnt = 0;
	luwGridVoltSumCount = 0;
#endif
}

/*****************************************************************************
 * �������ܣ�10���ӹ�ѹ������
 * �������룺������ѹ����������������λ��
 * ��������������30s������ѹƽ��ֵ����λ�洢�ڳ���ΪcMaxVoltNum������������ƽ��
 * 			 ֵ������������趨ֵ�����õ�����ѹ���ϱ�־λ�����30s��ƽ��ֵ�Ѿ��ָ���
 * 			 ������Χ�����������ѹ���ϱ�־λ��
 ****************************************************************************/
INT8U s10minVoltCheck(const float *lfGridVolt, INT8U lubPhaseNum,INT8U lubError)
{
#ifndef DebugEnable
	INT8U i;
	INT8U lubPhase;
	INT8U lubPhaseCount;
	
	if(luwGridVoltSumCount < cT3s)
	{
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			lfGridVolt3sSum[lubPhase] += lfGridVolt[lubPhase];
		}
		luwGridVoltSumCount++;
		return(lubError);
	}
	else
	{
		if(luw3sGridVoltCnt >= cMaxVoltNum)
		{
			luw3sGridVoltCnt =0;
		}
		lubPhaseCount = 0;
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			lfGridVoltBuffer[lubPhase][luw3sGridVoltCnt] = lfGridVolt3sSum[lubPhase] / luwGridVoltSumCount;
			lfGridVolt3sSum[lubPhase] = 0;
			lfGridVolt3sAvg[lubPhase] = 0;
			
			for(i = 0;i < cMaxVoltNum;i++)
			{
				lfGridVolt3sAvg[lubPhase] += lfGridVoltBuffer[lubPhase][i];
			}
			lfGridVolt3sAvg[lubPhase] = lfGridVolt3sAvg[lubPhase] / cMaxVoltNum;
			if((lfGridVoltBuffer[lubPhase][luw3sGridVoltCnt] <= lf10MinGridHigh) && (lubError == 1))
			{
				lubPhaseCount++;
			}
			if(lubPhaseCount >= lubPhaseNum)
			{
				s10minVoltInitial(lubPhaseNum);
				return(0);				
			}		
		}
		
		luwGridVoltSumCount = 0;		
		luw3sGridVoltCnt++;
		
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			if(lfGridVolt3sAvg[lubPhase] > lf10MinGridHigh)
			{
				return(1);
			}
		}
		return(0);
	}
#else
	return(0);
#endif
}
#else
/*****************************************************************************
 * �������ƣ�10���ӹ�ѹ����ֵ���ú�����
 * �������룺10���ӹ�ѹ������ֵ��
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
void sSet10MinLimit(INT16U luwLimit)
{
	luw10MinGridHigh = luwLimit;
}

/*****************************************************************************
 * �������ܣ���ʼ��10���ӹ�ѹ�����ı�����
 * �������룺�ޡ�
 * ������������10���ӹ�ѹ�����ļ�ʱ����0��30sƽ��ֵ��������ACֵ��
 ****************************************************************************/
void s10minVoltInitial(INT8U lubPhaseNum)
{
	INT8U lubPhase;

	for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
	{
		for(luw30sGridVoltCnt = 0;luw30sGridVoltCnt < cMaxVoltNum;luw30sGridVoltCnt++)
		{
			luwGridVoltBuffer[lubPhase][luw30sGridVoltCnt] = cNormalACVolt;
		}
		lulGridVolt30sSum[lubPhase] = c0V;
	}
	luw30sGridVoltCnt = 0;
	luwGridVoltSumCount = 0;
}

/*****************************************************************************
 * �������ܣ�10���ӹ�ѹ������
 * �������룺������ѹ����������������λ��
 * ��������������30s������ѹƽ��ֵ����λ�洢�ڳ���ΪcMaxVoltNum������������ƽ��
 * 			 ֵ������������趨ֵ�����õ�����ѹ���ϱ�־λ�����30s��ƽ��ֵ�Ѿ��ָ���
 * 			 ������ Χ�����������ѹ���ϱ�־λ��
 ****************************************************************************/
INT8U s10minVoltCheck(const INT16U *luwGridVolt, INT8U lubPhaseNum,INT8U lubError)
{
	INT8U i;
	INT8U lubPhase;
	INT8U lubPhaseCount;
	
	if(luwGridVoltSumCount < cT30s)
	{
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			lulGridVolt30sSum[lubPhase] += luwGridVolt[lubPhase];
		}
		luwGridVoltSumCount++;
		return(lubError);
	}
	else
	{
		if(luw30sGridVoltCnt >= cMaxVoltNum)
		{
			luw30sGridVoltCnt = 0;
		}
		lubPhaseCount = 0;		
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			luwGridVoltBuffer[lubPhase][luw30sGridVoltCnt] = (INT16U)(lulGridVolt30sSum[lubPhase] / luwGridVoltSumCount);
			lulGridVolt30sSum[lubPhase] = 0;
			lulGridVolt30sAvg[lubPhase] = 0;
			for(i = 0;i < cMaxVoltNum;i++)
			{
				lulGridVolt30sAvg[lubPhase] += luwGridVoltBuffer[lubPhase][i];
			}
			lulGridVolt30sAvg[lubPhase] = lulGridVolt30sAvg[lubPhase] / cMaxVoltNum;
			if((lfGridVoltBuffer[lubPhase][luw30sGridVoltCnt] <= luw10MinGridHigh ) && (lubError == 1))
			{
				lubPhaseCount++;
			}
			if(lubPhaseCount >= lubPhaseNum)
			{
				s10minVoltInitial(lubPhaseNum);
				return(0);				
			}			
		}
		luwGridVoltSumCount =0;
		luw30sGridVoltCnt++;		
		
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			if(lulGridVolt30sAvg[lubPhase] > luw10MinGridHigh)
			{
				return(1);
			}
		}
		
		return(0);
	}
}
#endif

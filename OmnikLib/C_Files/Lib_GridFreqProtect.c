/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�GridFreqProtect.c
 * �޸��ˣ��²�
 * �޸����ڣ�2014.10.29
 * ��������ǷƵ����
 * �޸����ݣ�
 * 1.1��--������ȫ����Ϊ�ֲ�������
 * 1.2��--ȡ��1sʱ���׼��
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"
#include "Lib_VoltFreq.h"
#include "Lib_SelfTest.h"

/********************************************************************
 * Local Types
 ********************************************************************/
INT16U luwOverFreqCnt;
INT16U luwOverFreqCnt2;
INT16U luwUnderFreqCnt;
INT16U luwUnderFreqCnt2;
INT16U luwFreqBackCnt;

#ifdef FloatEnable
float lfGridFreqValueLimit[6];
#else
INT16U luwGridFreqHigh1;
INT16U luwGridFreqHigh2;
INT16U luwGridFreqLow1;
INT16U luwGridFreqLow2;
INT16U luwGridFreqBackLow;
INT16U luwGridFreqBackHigh;
#endif

INT16U luwGridFreqTimeLimit[5];

/*****************************************************************************
 * �������ƣ����Ƶ�ʱ���ʱ�亯����
 * �������룺����ѡ��
 * ��������ֵ������ʱ�䡣
 * ����������
 ****************************************************************************/
INT16U sGetFreqTime(INT8U lubTimeNum)
{
	if(lubTimeNum <= cBack)
	{
		return(luwGridFreqTimeLimit[lubTimeNum]);
	}
	else
	{
		return(0);
	}
}

/*****************************************************************************
 * �������ƣ�����Ƶ�ʱ���ʱ�亯����
 * �������룺����ѡ��ʱ�䡣
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
void sSetFreqTime(INT8U lubTimeNum,INT16U luwTime)
{
	if(lubTimeNum <= cBack)
	{
		luwGridFreqTimeLimit[lubTimeNum] = luwTime;
	}
}
#ifdef FloatEnable
/*****************************************************************************
 * �������ܣ�����Ƶ�ʱ�����
 * �������룺����Ƶ�ʣ�����ʱ�����ͣ�����λ��
 * ����������������Ƶ�ʲ���������Χ��ʱ����ʱ��������ʱ�䳬���涨ֵ���õ���Ƶ�ʹ�
 * 			 �ϱ�־λ���ڼ�ʱ�������������Ƶ�ʻָ��������ʱ��������Ƶ�ʹ��Ϻ���
 * 			 Ƶ�ʻָ���������Χ�����������Ƶ�ʹ��ϱ�־λ��
 ****************************************************************************/
INT8U sFreqCheck(float lfGridFreq,INT8U lubError,INT8U lubEnableFlag)
{
	if(lubError == 0)
	{
		if(lfGridFreq < lfGridFreqValueLimit[cLow2] && (lubEnableFlag & 0x02))
		{
			luwUnderFreqCnt++;
			luwUnderFreqCnt2++;
			luwOverFreqCnt = 0;
			luwOverFreqCnt2 = 0;
			if(luwUnderFreqCnt2 >= luwGridFreqTimeLimit[cLow2])
			{
				luwUnderFreqCnt2 = 0;
				luwUnderFreqCnt = 0;
				return(1);
			}
		}
		else if(lfGridFreq < lfGridFreqValueLimit[cLow1] && (lubEnableFlag & 0x01))
		{
			luwUnderFreqCnt++;
			luwUnderFreqCnt2 = 0;
			luwOverFreqCnt = 0;
			luwOverFreqCnt2 = 0;
			if(luwUnderFreqCnt >= luwGridFreqTimeLimit[cLow1])
			{
				luwUnderFreqCnt = 0;
				return(1);
			}
		}
		else if(lfGridFreq > lfGridFreqValueLimit[cHigh2] && (lubEnableFlag & 0x02))
		{
			luwUnderFreqCnt = 0;
			luwUnderFreqCnt2 = 0;
			luwOverFreqCnt++;
			luwOverFreqCnt2++;
			if(luwOverFreqCnt2 >= luwGridFreqTimeLimit[cHigh2])
			{
				luwOverFreqCnt = 0;
				luwOverFreqCnt2 = 0;
				return(1);
			}
		}
		else if(lfGridFreq > lfGridFreqValueLimit[cHigh1] && (lubEnableFlag & 0x01))
		{
			luwUnderFreqCnt = 0;
			luwUnderFreqCnt2 = 0;
			luwOverFreqCnt++;
			luwOverFreqCnt2 = 0;
			if(luwOverFreqCnt >= luwGridFreqTimeLimit[cHigh1])
			{
				luwOverFreqCnt = 0;
				return(1);
			}
		}
		else
		{
			sClearSelfTestFlag(0);
			luwUnderFreqCnt = 0;
			luwUnderFreqCnt2 = 0;
			luwOverFreqCnt = 0;
			luwOverFreqCnt2  = 0;
		}
		return(0);
	}
	else
	{
		if((lfGridFreq > lfGridFreqValueLimit[cBackLow]) && (lfGridFreq < lfGridFreqValueLimit[cBackHigh]))
		{
			luwFreqBackCnt++;
			if(luwFreqBackCnt >= luwGridFreqTimeLimit[cBack])
			{
				luwFreqBackCnt = 0;
				return(0);
			}
		}
		else
		{
			luwFreqBackCnt = 0;
		}
		return(1);
	}
}

/*****************************************************************************
 * �������ƣ����Ƶ����ֵ������
 * �������룺����ѡ��
 * ��������ֵ��������ֵ��
 * ����������
 ****************************************************************************/
float sGetFreqLimit(INT8U lubTimeNum)
{
	if(lubTimeNum <= cBackHigh)
	{
		return(lfGridFreqValueLimit[lubTimeNum]);
	}
	else
	{
		return(0.0f);
	}
}

/*****************************************************************************
 * �������ƣ�����Ƶ����ֵ������
 * �������룺����ѡ��Ƶ����ֵ��
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
void sSetFreqLimit(INT8U lubTimeNum,float lfFreq)
{
	if(lubTimeNum <= cBackHigh)
	{
		lfGridFreqValueLimit[lubTimeNum] = lfFreq;
	}
}

#else
/*****************************************************************************
 * �������ܣ�����Ƶ�ʱ�����
 * �������룺����Ƶ�ʣ�����ʱ�����ͣ�����λ��
 * ����������������Ƶ�ʲ���������Χ��ʱ����ʱ��������ʱ�䳬���涨ֵ���õ���Ƶ�ʹ�
 * 			 �ϱ�־λ���ڼ�ʱ�������������Ƶ�ʻָ��������ʱ��������Ƶ�ʹ��Ϻ���
 * 			 Ƶ�ʻָ���������Χ�����������Ƶ�ʹ��ϱ�־λ��
 ****************************************************************************/
INT8U sFreqCheck(INT16U luwGridFreq,INT8U lubError,INT8U lubEnableFlag)
{
	if(lubError == 0)
	{
		if(luwGridFreq < luwGridFreqLow2)
		{
			luwUnderFreqCnt++;
			luwUnderFreqCnt2++;
			luwOverFreqCnt = 0;
			luwOverFreqCnt2 = 0;
			if(luwUnderFreqCnt2 >= luwGridFreqTimeLimit[cLow2])
			{
				luwUnderFreqCnt = 0;
				luwUnderFreqCnt2 = 0;
				return(1);
			}
		}
		else if(luwGridFreq < luwGridFreqLow1)
		{
			luwUnderFreqCnt++;
			luwUnderFreqCnt2 = 0;
			luwOverFreqCnt = 0;
			luwOverFreqCnt2 = 0;
			if(luwUnderFreqCnt >= luwGridFreqTimeLimit[cLow1])
			{
				luwUnderFreqCnt = 0;
				return(1);
			}
		}
		else if(luwGridFreq > luwGridFreqHigh2)
		{
			luwUnderFreqCnt = 0;
			luwUnderFreqCnt2 = 0;
			luwOverFreqCnt++;
			luwOverFreqCnt2++;
			if(luwOverFreqCnt2 >= luwGridFreqTimeLimit[cHigh2])
			{
				luwOverFreqCnt = 0;
				luwOverFreqCnt2 = 0;
				return(1);
			}
		}
		else if(luwGridFreq > luwGridFreqHigh1)
		{
			luwUnderFreqCnt = 0;
			luwUnderFreqCnt2 = 0;
			luwOverFreqCnt++;
			luwOverFreqCnt2 = 0;
			if(luwOverFreqCnt >= luwGridFreqTimeLimit[cHigh1])
			{
				luwOverFreqCnt = 0;
				return(1);
			}
		}
		else
		{
			luwOverFreqCnt   = 0;
			luwOverFreqCnt2  = 0;
			luwUnderFreqCnt  = 0;
			luwUnderFreqCnt2 = 0;
		}
		return(0);
	}
	else
	{
		if((luwGridFreq > luwGridFreqBackLow) && (luwGridFreq < luwGridFreqBackHigh))
		{
			luwFreqBackCnt++;
			if(luwFreqBackCnt >= luwGridFreqTimeLimit[cBack])
			{
				luwFreqBackCnt = 0;
				return(0);
			}
		}
		else
		{
			luwFreqBackCnt = 0;
		}

		return(1);
	}
}

/*****************************************************************************
 * �������ƣ����ù�Ƶ��ֵ������
 * �������룺��Ƶ��ֵ��
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
void sSetFreqHigh(INT16U luwFreq1,INT16U luwFreq2)
{
	luwGridFreqHigh1 = luwFreq1;
	luwGridFreqHigh2 = luwFreq2;
}

/*****************************************************************************
 * �������ƣ�����ǷƵ��ֵ������
 * �������룺ǷƵ��ֵ��
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
void sSetFreqLow(INT16U luwFreq1,INT16U luwFreq2)
{
	luwGridFreqHigh1 = luwFreq1;
	luwGridFreqHigh2 = luwFreq2;
}

/*****************************************************************************
 * �������ƣ����ûָ�Ƶ����ֵ������
 * �������룺�ָ�Ƶ����ֵ��
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
void sSetFreqBack(INT16U luwFreqLow,INT16U luwFreqHigh)
{
	luwGridFreqBackLow = luwFreqLow;
	luwGridFreqBackHigh = luwFreqHigh;
}
#endif

/*****************************************************************************
 * �������ܣ�����Ƶ�ʱ���������ʼ����
 * �������룺�ޡ�
 * ������������ʼ������Ƶ�ʱ��������еľֲ�������
 ****************************************************************************/
void sFreqCheckInitial(void)
{
}

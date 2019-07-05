/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�Lib_GridVoltProtect.c
 * �޸��ˣ��²�
 * ��������Ƿѹ����
 * �������ڣ�2014.10.29
 * �޸����ݣ�
 * 1.1��--������ȫ����Ϊ�ֲ�������
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"
#include "Lib_VoltFreq.h"
#include "Lib_SelfTest.h"
#include "Lib_Communication.h"

/********************************************************************
 * Global Types
 ********************************************************************/


/********************************************************************
 * Local Types
 ********************************************************************/
INT8U  lub_t;         // ͻ��ʱ��t
INT16U luwOverVoltCnt[cPhaseNum];
INT16U luwOverVoltCnt2[cPhaseNum];
INT16U luwUnderVoltCnt[cPhaseNum];
INT16U luwUnderVoltCnt2[cPhaseNum];
INT16U luwVoltBackCnt;
INT16U luwProtectCnt; // ����ʱ�� 


#ifdef FloatEnable
float lfGridVoltValueLimit[6];
float lfGridVolt_A[cPhaseNum];
float lfGridVolt_U[cPhaseNum];
float lfGridVolt_B[cPhaseNum];
#else
INT16U luwGridVoltHigh1;
INT16U luwGridVoltHigh2;
INT16U luwGridVoltLow1;
INT16U luwGridVoltLow2;
INT16U luwGridVoltBackLow;
INT16U luwGridVoltBackHigh;
#endif

INT16U luwGridVoltTimeLimit[5];

/*****************************************************************************
 * �������ܣ�������ѹ����������ʼ����
 * �������룺�ޡ�
 * ������������ʼ��������ѹ���������еľֲ�������
 ****************************************************************************/
void sVoltCheckInitial(INT16U lubPhaseNum)
{
	INT8U lubPhase;
	for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
	{
		luwOverVoltCnt[lubPhase] = 0;
		luwOverVoltCnt2[lubPhase] = 0;
		luwUnderVoltCnt[lubPhase] = 0;
		luwUnderVoltCnt2[lubPhase] = 0;
	}
	luwVoltBackCnt = 0;
	luwProtectCnt = 0;
}

/*****************************************************************************
 * �������ƣ���õ�ѹ����ʱ�亯����
 * �������룺����ѡ��
 * ��������ֵ������ʱ�䡣
 * ����������
 ****************************************************************************/
INT16U sGetVoltTime(INT8U lubTimeNum)
{
	if(lubTimeNum <= cBack)
	{
		return(luwGridVoltTimeLimit[lubTimeNum]);
	}
	else
	{
		return(0);
	}
}

/*****************************************************************************
 * �������ƣ����õ�ѹ����ʱ�亯����
 * �������룺����ѡ��ʱ�䡣
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
void sSetVoltTime(INT8U lubTimeNum,INT16U luwTime)
{
	if(lubTimeNum <= cBack)
	{
		luwGridVoltTimeLimit[lubTimeNum] = luwTime*10;
	}
}

#ifdef FloatEnable
/*****************************************************************************
 * �������ܣ�������ѹ������
 * �������룺������ѹ���࣬����λ��
 * ������������������ѹ����������Χ��ʱ����ʱ��������ʱ�䳬���涨ֵ���õ�����ѹ��
 * 			 �ϱ�־λ���ڼ�ʱ���������������ѹ�ָ��������ʱ��������ѹ�ָ���������
 * 			 Χ�������������ѹ���ϱ�־λ��
 * �Ľ����򣺵�����ʱ��ϳ�ʱ��������������10s�����������������Ҫ������ڲ������
 * 			 ��ɵı����쳣�ṩ���������
 ****************************************************************************/
INT16U sVoltCheck(const float *lfGridVolt, INT8U lubPhaseNum, INT8U lubError, float lfGridFreq)
{
	INT8U lubPhase;
	INT8U lubSelfTestFlagTemp;
	INT16U luwtimedelta;
	luwtimedelta = 0;
	lubSelfTestFlagTemp = 0;

	if(lubError == 0)
	{
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			/********************************************************************
			* lfGridVolt_A,U,B ���������������ڵĵ�����ѹ��Чֵ;
			* lfGridVolt_AΪ������ѹͻ��ǰһ�������ڵ���Чֵ;
			* lfGridVolt_UΪ������ѹͻ����һ�������ڵ���Чֵ;
			* lfGridVolt_BΪ������ѹͻ���һ�������ڵ���Чֵ;
			* lub_tΪͻ��ʱ�䣬����㹫ʽΪ��t = ((B^2 - U^2)/(B^2 - A^2)) * T
			*********************************************************************/
			lfGridVolt_A[lubPhase] = lfGridVolt_U[lubPhase];
			lfGridVolt_U[lubPhase] = lfGridVolt_B[lubPhase];
			lfGridVolt_B[lubPhase] = lfGridVolt[lubPhase];
			lub_t =(((lfGridVolt_B[lubPhase]*lfGridVolt_B[lubPhase] - lfGridVolt_U[lubPhase]*lfGridVolt_U[lubPhase])/(lfGridVolt_B[lubPhase]*lfGridVolt_B[lubPhase] - lfGridVolt_A[lubPhase]*lfGridVolt_A[lubPhase]))/(lfGridFreq)) * 1000;
			
			if(lfGridVolt[lubPhase] > lfGridVoltValueLimit[cHigh1] && StateFlg.bit.ubVoltProtect!= 1) 	   //��ѹ1
			{
				luwProtectCnt = luwGridVoltTimeLimit[cHigh1];
				luwtimedelta = luwProtectCnt - lub_t;
				StateFlg.bit.ubVoltProtect = 1; 
			}
			else if(lfGridVolt[lubPhase] > lfGridVoltValueLimit[cHigh2] && StateFlg.bit.ubVoltProtect!= 1) //��ѹ2
			{
				luwProtectCnt = luwGridVoltTimeLimit[cHigh2];
				luwtimedelta = luwProtectCnt - lub_t;
				StateFlg.bit.ubVoltProtect = 1; 
			}
			else if(lfGridVolt[lubPhase] < lfGridVoltValueLimit[cLow1] && StateFlg.bit.ubVoltProtect!= 1) //Ƿѹ1
			{
				luwProtectCnt = luwGridVoltTimeLimit[cLow1];
				luwtimedelta = luwProtectCnt - lub_t;
				StateFlg.bit.ubVoltProtect = 1; 
			}
			else if(lfGridVolt[lubPhase] < lfGridVoltValueLimit[cLow2] && StateFlg.bit.ubVoltProtect!= 1)  //Ƿѹ2
			{
				luwProtectCnt = luwGridVoltTimeLimit[cLow2];
				luwtimedelta = luwProtectCnt - lub_t;
				StateFlg.bit.ubVoltProtect = 1; 
			}
			else if ((lfGridVolt[lubPhase]  < lfGridVoltValueLimit[cHigh1]) && (lfGridVolt[lubPhase]  > lfGridVoltValueLimit[cLow1]))
			{
				StateFlg.bit.ubVoltProtect = 0; //��ѹ��Чֵ��������Χ�ڣ������־λ��
				return (0);
			}
		}
		if(lubSelfTestFlagTemp == 0)
		{
			sClearSelfTestFlag(1);
		}
		
		return luwtimedelta;;
	}
	else
	{
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			if((lfGridVolt[lubPhase] < lfGridVoltValueLimit[cBackLow]) || (lfGridVolt[lubPhase] > lfGridVoltValueLimit[cBackHigh]))
			{
				luwVoltBackCnt = 0;
			}
		}

		if(++luwVoltBackCnt > luwGridVoltTimeLimit[cBack])
		{
			luwVoltBackCnt = 0;
			sVoltCheckInitial(lubPhaseNum);
			Error.bit.mubGridVoltFault = 0;
		}
		return(0);
	}
}

/*****************************************************************************
 * �������ƣ���õ�ѹ��ֵ������
 * �������룺����ѡ��
 * ��������ֵ��������ֵ��
 * ����������
 ****************************************************************************/
float sGetVoltLimit(INT8U lubTimeNum)
{
	if(lubTimeNum <= cBackHigh)
	{
		return(lfGridVoltValueLimit[lubTimeNum]);
	}
	else
	{
		return(0.0f);
	}
}

/*****************************************************************************
 * �������ƣ����õ�ѹ��ֵ������
 * �������룺����ѡ�񣬵�ѹ��ֵ��
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
void sSetVoltLimit(INT8U lubTimeNum,float lfVolt)
{
	if(lubTimeNum <= cBackHigh)
	{
		lfGridVoltValueLimit[lubTimeNum] = lfVolt;
	}
}

#else
/*****************************************************************************
 * �������ܣ�������ѹ������
 * �������룺������ѹ���࣬����λ��
 * ������������������ѹ����������Χ��ʱ����ʱ��������ʱ�䳬���涨ֵ���õ�����ѹ��
 * 			 �ϱ�־λ���ڼ�ʱ���������������ѹ�ָ��������ʱ��������ѹ�ָ���������
 * 			 Χ�������������ѹ���ϱ�־λ��
 * �Ľ����򣺵�����ʱ��ϳ�ʱ��������������10s�����������������Ҫ������ڲ������
 * 			 ��ɵı����쳣�ṩ���������
 ****************************************************************************/
INT8U sVoltCheck(const INT16U *luwGridVolt, INT8U lubPhaseNum,INT8U lubError)
{
	INT8U lubPhase;

	if(lubError == 0)
	{
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			if(luwGridVolt[lubPhase] < luwGridVoltLow2)
			{
				luwUnderVoltCnt2[lubPhase]++;
				luwUnderVoltCnt[lubPhase]++;
				luwOverVoltCnt[lubPhase] = 0;
				luwOverVoltCnt2[lubPhase] = 0;
				if(luwUnderVoltCnt2[lubPhase] >= luwGridVoltTimeLimit[cLow2])
				{
					luwUnderVoltCnt2[lubPhase] = 0;
					luwUnderVoltCnt[lubPhase] = 0;
					return(1);
				}
			}
			else if(luwGridVolt[lubPhase] < luwGridVoltLow1)
			{
				luwUnderVoltCnt2[lubPhase] = 0;
				luwUnderVoltCnt[lubPhase]++;
				luwOverVoltCnt[lubPhase] = 0;
				luwOverVoltCnt2[lubPhase] = 0;
				if(luwUnderVoltCnt[lubPhase] >= luwGridVoltTimeLimit[cLow1])
				{
					luwUnderVoltCnt[lubPhase] = 0;
					return(1);
				}
			}
			else if(luwGridVolt[lubPhase] > luwGridVoltHigh2)
			{
				luwUnderVoltCnt2[lubPhase] = 0;
				luwUnderVoltCnt[lubPhase] = 0;
				luwOverVoltCnt[lubPhase]++;
				luwOverVoltCnt2[lubPhase]++;
				if(luwOverVoltCnt2[lubPhase] >= luwGridVoltTimeLimit[cHigh2])
				{
					luwOverVoltCnt[lubPhase] = 0;
					luwOverVoltCnt2[lubPhase] = 0;
					return(1);
				}
			}
			else if(luwGridVolt[lubPhase] > luwGridVoltHigh1)
			{
				luwUnderVoltCnt2[lubPhase] = 0;
				luwUnderVoltCnt[lubPhase] = 0;
				luwOverVoltCnt[lubPhase]++;
				luwOverVoltCnt2[lubPhase] = 0;
				if(luwOverVoltCnt[lubPhase] >= luwGridVoltTimeLimit[cHigh1])
				{
					luwOverVoltCnt[lubPhase] = 0;
					return(1);
				}
			}
			else
			{
				luwOverVoltCnt[lubPhase] = 0;
				luwUnderVoltCnt[lubPhase] = 0;
				luwOverVoltCnt2[lubPhase] = 0;
				luwUnderVoltCnt2[lubPhase] = 0;
			}
		}
		return(0);
	}
	else
	{
		for(lubPhase = cPhaseR;lubPhase < lubPhaseNum;lubPhase++)
		{
			if((luwGridVolt[lubPhase] < luwGridVoltBackLow) || (luwGridVolt[lubPhase] > luwGridVoltBackHigh))
			{
				luwVoltBackCnt = 0;
			}
		}

		if(++luwVoltBackCnt > luwGridVoltTimeLimit[cBack])
		{
			luwVoltBackCnt = 0;
			sVoltCheckInitial(lubPhaseNum);
			return(0);
		}
		return(1);
	}
}

/*****************************************************************************
 * �������ƣ����ù�ѹ��ֵ������
 * �������룺��ѹ��ֵ��
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
void sSetVoltHigh(INT16U luwVolt1,INT16U luwVolt2)
{
	luwGridVoltHigh1 = luwVolt1;
	luwGridVoltHigh2 = luwVolt2;
}

/*****************************************************************************
 * �������ƣ�����Ƿѹ��ֵ������
 * �������룺Ƿѹ��ֵ��
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
void sSetVoltLow(INT16U luwVolt1,INT16U luwVolt2)
{
	luwGridVoltLow1 = luwVolt1;
	luwGridVoltLow2 = luwVolt2;
}

/*****************************************************************************
 * �������ƣ����ûָ���ѹ��ֵ������
 * �������룺�ָ���ѹ��ֵ��
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
void sSetVoltBack(INT16U luwVoltLow,INT16U luwVoltHigh)
{
	luwGridVoltBackLow = luwVoltLow;
	luwGridVoltBackHigh = luwVoltHigh;
}
#endif

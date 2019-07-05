/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�GroundCurrentProtect.c
 * �޸��ˣ��²�
 * ������©�����Լ졢©��������
 * �������ڣ�2014.10.29
 * �޸����ݣ�
 * 1.1��--��������ʱ������ֵ����ֵ����ֵ�ڳ�ʼ��������     �漪��
 * 1.2��--���������޸�    	    �漪��20140305
 * 1.3��--©����ƽ��ֵ���㴦����    �漪��20140324
 * 1.4��--������ʽ�������߼��������    �漪��20140325
 * 1.5��--��0mAƫ�õļ���ʱ��������100������
 * 1.6��--������©���������޷��ָ���bug��©���������ɷּ�������bug
 * 1.7��--�������͵Ŀ��ٱ�������
 * 1.8��--includeĿ¼��Ϊ����Ŀ���趨��
 * 1.9��--����һ������ָ�룬ָ�򿪷�������
 * 2.0��--�޸�״̬��־λ����,���������޸�Ϊ�궨��ֵ��
 * 2.1��-- cGround300msLimit��300ms�ļ�����ֵ
 * 		   cGround150msLimit��150ms�ļ�����ֵ
 * 		   cGroundCurrSaveNum��©������ʷ���ݳ���
 * 		   cGround30mAPreNum��30mA�Ƚ�ʱ�õ���ʷ����λ�ã�����ΪcGround300msLimit + 3(10msΪ3��20msΪ2)
 * 		   cGround60mAPreNum��60mA�Ƚ�ʱ�õ���ʷ����λ�ã�����ΪcGround150msLimit + 3(10msΪ3��20msΪ2)
 * 		   cGround150mAPreNum��150mA�Ƚ�ʱ�õ���ʷ����λ�ã�����Ϊ3(10msΪ3��20msΪ2)
 * 2.2��--�޸�30mA��60mA,150mA�����ж�����
 *			150mA���������жϷ���(30mA,60mAͬ��)
 *			�ж�ǰ©����ƽ��ֵx1����ǰ©����ƽ��ֵx1�� �ж�ǰ©������������y1����ǰ©������������y2��
 *			x2>0 && x1<0	x2+y2-x1-y1 > 150mA;
 *			x2>0 && x1>0	x2+y2-x1-y1 > 150mA && x2+y2>(-x1)+y2;
 *			x2<0 && x1<0    (-x2)+y2+x1-y1 > 150mA;
 *			x2<0 && x1>0	(-x2)+y2+x1-y1 > 150mA && (-x2)+y2>x1+y1;
 ********************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"

/********************************************************************
 * Local Types
 ********************************************************************/
#ifdef FloatEnable
float lfGroundIAC;
float lfGroundAvg;
float lfGroundIACOld[cGroundCurrSaveNum];
float lfGroundAvgOld[cGroundCurrSaveNum];
float lfGround0mASum;
float lfGround0mAAvg;
#else
INT16U luwGroundIAC;
INT16U luwGroundRmsPlusAvg;
INT16U luwGroundRmsPlusAvgOld[cGroundCurrSaveNum];
INT16S lswGround0mAAvg;
INT32S lslGround0mASum;
#endif

INT8U lubGround300msCnt;
INT8U lubGround150msCnt;
INT8U lubGroundAvg300msCnt;
INT8U lubGFCI0mAFaultCnt;
INT8U lubGFCI50mAFaultCnt;
INT8U lub0mACnt;

INT8S lsbGroundCnt;

INT16U luwGroundCurrBackCnt;

#ifdef FloatEnable
/********************************************************************
 * Global Types
 ********************************************************************/
pFuncf sSqrtFunc;
#else
INT16U swRoot(INT32U lulInput);
#endif

#ifdef FloatEnable

/*****************************************************************************
 * �������ܣ�0mA©�����Լ졣
 * �������룺©��������Чֵ��
 * �������������©��������Чֵ��������40mA������©�����豸���ϱ�־λ���豸���ϱ�־
 * 			 λ��
 ****************************************************************************/
void sGFCI0mAChk(float lfGroundIRms)
{
	if(lfGroundIRms > cGround0mATestLimit)
	{
		lubGFCI0mAFaultCnt++;
		if(lubGFCI0mAFaultCnt > 5)
		{
			Error.bit.mubGFCIDeviveFault = 1;
			lubGFCI0mAFaultCnt = 0;
		}
	}
	else
	{
		lubGFCI0mAFaultCnt = 0;
	}
}

/*****************************************************************************
 * �������ܣ�50mA©�����Լ졣
 * �������룺©��������Чֵ��
 * �������������©��������Чֵ��������70mA���ߵ���30mA������©�����豸���ϱ�־λ
 * 			 ���豸���ϱ�־λ��
 ****************************************************************************/
void sGFCI50mAChk(float lfGroundIRms)
{
	if((lfGroundIRms > cGround50mATestLimitU) || (lfGroundIRms < cGround50mATestLimitD))
	{
		lubGFCI50mAFaultCnt++;
		if(lubGFCI50mAFaultCnt > 5)
		{
			Error.bit.mubGFCIDeviveFault = 1;
			lubGFCI50mAFaultCnt = 0;
		}
	}
	else
	{
		lubGFCI50mAFaultCnt = 0;
	}
}

/*****************************************************************************
 * �������ܣ�����©����ֵ��
 * �������룺©��������Чֵ��©������ƽ��ֵ��
 * ��������������©��������Чֵ��ƽ��ֵ������©�����Ľ���������Чֵ������λ�洢©��
 * 			 ������������ֱ�������ĺ͡�
 ****************************************************************************/
void sCalGroundCurrent(float lfGroundIRms, float lfGroundIAvg)
{
	if(lfGroundIRms > lfGroundIAvg && lfGroundIRms > -lfGroundIAvg)
	{
		lfGroundIAC = lfGroundIRms * lfGroundIRms - lfGroundIAvg * lfGroundIAvg;
		lfGroundIAC = sSqrtFunc(lfGroundIAC);
	}
	else
	{
		lfGroundIAC = cGround0mA;
	}
	
	lfGroundAvg = lfGroundIAvg;

	for(lsbGroundCnt = cGround30mAPreNum;lsbGroundCnt > 0;lsbGroundCnt--)
	{
		lfGroundIACOld[lsbGroundCnt] = lfGroundIACOld[lsbGroundCnt - 1];
		lfGroundAvgOld[lsbGroundCnt] = lfGroundAvgOld[lsbGroundCnt - 1];
	}
	
	lfGroundIACOld[0] = lfGroundIAC;
	lfGroundAvgOld[0] = lfGroundAvg;
}

/*****************************************************************************
 * �������ܣ�150mA©����ͻ����������
 * �������룺�ޡ�
 * ������������ǰ©�����Ľ���������ֱ�������ĺ͵�ͻ��������120mAʱ����©�������ϱ�
 * 			 ־λ��
 ****************************************************************************/
void sGFCIProtectFast(void)
{
	INT8U lubStatus;
	float lfGround150mAJudge;
	float lfDeltIACPlusIAvg150mA;
	
	if(Error.bit.mubGroundCurrentFault == 0)
	{
		if(lfGroundAvg > cGround0mA)
		{	
			lubStatus = 1;
			lfGround150mAJudge = lfGroundIAC + lfGroundAvg - lfGroundAvgOld[cGround150mAPreNum] - lfGroundIACOld[cGround150mAPreNum];
			if(lfGroundAvgOld[cGround150mAPreNum] < cGround0mA)
			{
				lubStatus = 3;
				lfDeltIACPlusIAvg150mA = lfGroundIAC  + lfGroundAvg + lfGroundAvgOld[cGround150mAPreNum] - lfGroundIACOld[cGround150mAPreNum];
			}
		}
		else
		{
			lubStatus = 2;
			lfGround150mAJudge = lfGroundIAC - lfGroundAvg + lfGroundAvgOld[cGround150mAPreNum] - lfGroundIACOld[cGround150mAPreNum];
			if(lfGroundAvgOld[cGround150mAPreNum] > cGround0mA)
			{
				lubStatus = 4;
				lfDeltIACPlusIAvg150mA = lfGroundIAC  - lfGroundAvg - lfGroundAvgOld[cGround150mAPreNum] - lfGroundIACOld[cGround150mAPreNum];
			}
		}

		if((lubStatus <= 2 && lfGround150mAJudge > cGround150mALimit) || (lubStatus >= 3 && lfDeltIACPlusIAvg150mA > cGround0mA && lfGround150mAJudge > cGround150mALimit))
			//150mA  40ms
		{
			Error.bit.mubGroundCurrentFault = 1;
		}
	}
}

/*****************************************************************************
 * �������ܣ�300mA©����������30mA©����ͻ����������60mA©����ͻ����������
 * �������룺©��������Чֵ��
 * ������������©��������ʱ��©��������Чֵ����8���������ڳ���120mAʱ����©��������
 * 			 ��־λ��©�����Ľ���������ֱ�������ĺͱ仯������26mAʱ����©�������ϱ�
 * 			 ־λ��©�����Ľ���������ֱ�������ĺͱ仯������52mAʱ����©�������ϱ�־
 * 			 λ������©��������ʱ��©��������Чֵ��������20mAʱ����©�������ϱ�־λ��
 ****************************************************************************/
void sGFCIProtect(float lfGroundIRms)
{
	INT8U lubStatus;
	float lfGround30mAJudge;
	float lfGround60mAJudge;
	float lfDeltIACPlusIAvg30mA;
	float lfDeltIACPlusIAvg60mA;
	
	if(Error.bit.mubGroundCurrentFault == 0)
	{
		if(lfGroundIRms >= cGround300mALimit)
			//300mA  300ms
		{
			lubGroundAvg300msCnt++;
			if(lubGroundAvg300msCnt >= cGround300msLimit)
			{
				Error.bit.mubGroundCurrentFault = 1;
				lubGroundAvg300msCnt = 0;
			}
		}
		else
		{
			lubGroundAvg300msCnt = 0;
		}
		//30mA������������
		if(lfGroundAvg > cGround0mA)
		{	
			lubStatus = 1;
			lfGround30mAJudge = lfGroundIAC + lfGroundAvg - lfGroundAvgOld[cGround30mAPreNum] - lfGroundIACOld[cGround30mAPreNum];
			if(lfGroundAvgOld[cGround30mAPreNum] < cGround0mA)
			{
				lubStatus = 3;
				lfDeltIACPlusIAvg30mA = lfGroundIAC  + lfGroundAvg + lfGroundAvgOld[cGround30mAPreNum] - lfGroundIACOld[cGround30mAPreNum];
			}
		}
		else
		{
			lubStatus = 2;
			lfGround30mAJudge = lfGroundIAC - lfGroundAvg + lfGroundAvgOld[cGround30mAPreNum] - lfGroundIACOld[cGround30mAPreNum];
			if(lfGroundAvgOld[cGround30mAPreNum] > cGround0mA)
			{
				lubStatus = 4;
				lfDeltIACPlusIAvg30mA = lfGroundIAC  - lfGroundAvg - lfGroundAvgOld[cGround30mAPreNum] - lfGroundIACOld[cGround30mAPreNum];
			}
		}

		if((lubStatus <= 2 && lfGround30mAJudge > cGround30mALimit) || (lubStatus >= 3 && lfDeltIACPlusIAvg30mA > cGround0mA && lfGround30mAJudge > cGround30mALimit))
			//30mA  300ms
		{
			lubGround300msCnt++;
			if(lubGround300msCnt >= cGround300msLimit)
			{
				Error.bit.mubGroundCurrentFault = 1;
				lubGround300msCnt = 0;
			}
		}
		else if(lubGround300msCnt > 0)
		{
			lubGround300msCnt--;
		}
		
		//60mA������������
		if(lfGroundAvg > cGround0mA)
		{	
			lubStatus = 1;
			lfGround60mAJudge = lfGroundIAC + lfGroundAvg - lfGroundAvgOld[cGround60mAPreNum] - lfGroundIACOld[cGround60mAPreNum];
			if(lfGroundAvgOld[cGround60mAPreNum] < cGround0mA)
			{
				lubStatus = 3;
				lfDeltIACPlusIAvg60mA = lfGroundIAC  + lfGroundAvg + lfGroundAvgOld[cGround60mAPreNum] - lfGroundIACOld[cGround60mAPreNum];
			}
		}
		else
		{
			lubStatus = 2;
			lfGround60mAJudge = lfGroundIAC - lfGroundAvg + lfGroundAvgOld[cGround60mAPreNum] - lfGroundIACOld[cGround60mAPreNum];
			if(lfGroundAvgOld[cGround60mAPreNum] > cGround0mA)
			{
				lubStatus = 4;
				lfDeltIACPlusIAvg60mA = lfGroundIAC  - lfGroundAvg - lfGroundAvgOld[cGround60mAPreNum] - lfGroundIACOld[cGround60mAPreNum];
			} 
		}
		
		if((lubStatus <= 2 && lfGround60mAJudge > cGround60mALimit) || (lubStatus >= 3 && lfDeltIACPlusIAvg60mA > cGround0mA && lfGround60mAJudge > cGround60mALimit))
			//60mA  150ms
		{
			lubGround150msCnt++;
			if(lubGround150msCnt >= cGround150msLimit)
			{
				Error.bit.mubGroundCurrentFault = 1;
				lubGround150msCnt = 0;
			}
		}
		else if(lubGround150msCnt > 0)
		{
			lubGround150msCnt--;
		}
		luwGroundCurrBackCnt = 0;
	}
}

/*****************************************************************************
 * �������ܣ���ʼ��©��������ͱ����ı�����
 * �������룺�ޡ�
 * ������������©��������ͱ������õı�����������ȫ����0��
 ****************************************************************************/
void sInitialGroundCurrCal(void)
{
	for(lsbGroundCnt = 0;lsbGroundCnt < cGroundCurrSaveNum;lsbGroundCnt++)
	{
		lfGroundIACOld[lsbGroundCnt] = cGround0mA;
		lfGroundAvgOld[lsbGroundCnt] = cGround0mA;
	}
	lfGroundIAC = cGround0mA;
	lfGroundAvg = cGround0mA;
	lubGround300msCnt = 0;
	lubGround150msCnt = 0;
	lubGroundAvg300msCnt = 0;
	luwGroundCurrBackCnt = 0;
	lubGFCI0mAFaultCnt = 0;
	lubGFCI50mAFaultCnt = 0;
	lub0mACnt = 0;

	lfGround0mASum = cGround0mA;
	lfGround0mAAvg = cGround0mA;
}

void sGFCICheck(float lfGroundIRms, float lfGroundIAvg)
{
	if(StateFlg.bit.ubGFCIStep == 0)
	{
		sInitialGroundCurrCal();
		StateFlg.bit.ubGFCIStep = 1;
		cGFCI50mATestOff();
	}
	else if(StateFlg.bit.ubGFCIStep == 1)
	{
		if(Error.bit.mubGroundCurrentFault)
		{
			if(lfGroundIRms < cGround0mATestLimit)
			{
				luwGroundCurrBackCnt++;
				if(luwGroundCurrBackCnt > 275)
				{
					Error.bit.mubGroundCurrentFault = 0;
				}
			}
		}
		if(++lub0mACnt <= 100)
		{
			lfGround0mASum += lfGroundIAvg;
		}
		else
		{
			lfGround0mAAvg = lfGround0mASum / 100;
			lub0mACnt = 101;
		}
	}
	else if(StateFlg.bit.ubGFCIStep == 2)
	{
		sGFCI0mAChk(lfGroundIRms);
	}
	else if(StateFlg.bit.ubGFCIStep == 3)
	{
		cGFCI50mATestOn();
	}
	else if(StateFlg.bit.ubGFCIStep == 4)
	{
		sGFCI50mAChk(lfGroundIRms);
	}
	else if(StateFlg.bit.ubGFCIStep == 5)
	{
		cGFCI50mATestOff();
	}
	else if(StateFlg.bit.ubGFCIStep == 6)
	{
		sGFCI0mAChk(lfGroundIRms);
	}
	else if(StateFlg.bit.ubGFCIStep == 7)
	{
	}
	else
	{
		sGFCIProtect(lfGroundIRms);
		sGFCIProtectFast();
	}
}
#else
void sCalGroundCurrent(INT16U luwGroundIRms, INT16S lswGroundIAvg)
{
	INT32U lultemp;
	INT16S lswDeltGroundI;

	//Bug DS-239�����©��������������������bug
	if(lswGroundIAvg >= 0)
	{
		lswDeltGroundI = (INT16S)luwGroundIRms - lswGroundIAvg;
	}
	else
	{
		lswDeltGroundI = (INT16S)luwGroundIRms + lswGroundIAvg;
	}

	if(lswDeltGroundI > 0)
	{
		lultemp = (INT32S)luwGroundIRms * luwGroundIRms - (INT32S)lswGroundIAvg * lswGroundIAvg;
		luwGroundIAC = swRoot(lultemp);
	}
	else
	{
		luwGroundIAC = cGround0mA;
	}

	if(lswGroundIAvg > cGround0mA)
	{
		luwGroundRmsPlusAvg = luwGroundIAC + lswGroundIAvg;
	}
	else
	{
		luwGroundRmsPlusAvg = luwGroundIAC - lswGroundIAvg;
	}

	for(lsbGroundCnt = cGround30mAPreNum;lsbGroundCnt >= 0;lsbGroundCnt--)
	{
		luwGroundRmsPlusAvgOld[lsbGroundCnt + 1] = luwGroundRmsPlusAvgOld[lsbGroundCnt];
	}
	luwGroundRmsPlusAvgOld[0] = luwGroundRmsPlusAvg;
}

void sGFCIProtectFast(void)
{
	if(luwGroundRmsPlusAvg > luwGroundRmsPlusAvgOld[cGround150mAPreNum] + cGround150mALimit)
	{
		Error.bit.mubGroundCurrentFault = 1;
	}
}

void sGFCIProtect(INT16U luwGroundIRms)
{
	if(Error.bit.mubGroundCurrentFault == 0)
	{
		if(luwGroundIRms >= cGround300mALimit)
			//300mA  300ms
		{
			lubGroundAvg300msCnt++;
			if(lubGroundAvg300msCnt >= cGround300msLimit)
			{
				Error.bit.mubGroundCurrentFault = 1;
				lubGroundAvg300msCnt = 0;
			}
		}
		else
		{
			lubGroundAvg300msCnt = 0;
		}

		if(luwGroundRmsPlusAvg > luwGroundRmsPlusAvgOld[cGround30mAPreNum] + cGround30mALimit)
		{
			lubGround300msCnt++;
			if(lubGround300msCnt >= cGround300msLimit)
			{
				Error.bit.mubGroundCurrentFault = 1;
				lubGround300msCnt = 0;
			}
		}
		else
		{
			lubGround300msCnt = 0;
		}

		if(luwGroundRmsPlusAvg > luwGroundRmsPlusAvgOld[cGround60mAPreNum] + cGround60mALimit)
		{
			lubGround150msCnt++;
			if(lubGround150msCnt >= cGround150msLimit)
			{
				Error.bit.mubGroundCurrentFault = 1;
				lubGround150msCnt = 0;
			}
		}
		else
		{
			lubGround150msCnt = 0;
		}
		luwGroundCurrBackCnt = 0;
	}
}

void sInitialGroundCurrCal(void)
{
	for(lsbGroundCnt = 0;lsbGroundCnt < cGroundCurrSaveNum;lsbGroundCnt++)
	{
		luwGroundRmsPlusAvgOld[lsbGroundCnt] = cGround0mA;
	}
	luwGroundIAC = cGround0mA;
	luwGroundRmsPlusAvg = cGround0mA;
	lubGround300msCnt = 0;
	lubGround150msCnt = 0;
	lubGroundAvg300msCnt = 0;
	luwGroundCurrBackCnt = 0;
	lubGFCI0mAFaultCnt = 0;
	lubGFCI50mAFaultCnt = 0;
	lub0mACnt = 0;

	lslGround0mASum = cGround0mA;
	lswGround0mAAvg = cGround0mA;
}

void sGFCI0mAChk(INT16U luwGroundIRms)
{
	if(luwGroundIRms > cGround0mATestLimit)
	{
		lubGFCI0mAFaultCnt++;
		if(lubGFCI0mAFaultCnt > 5)
		{
			Error.bit.mubGFCIDeviveFault = 1;
			lubGFCI0mAFaultCnt = 0;
		}
	}
	else
	{
		lubGFCI0mAFaultCnt = 0;
	}
}

void sGFCI50mAChk(INT16U luwGroundIRms)
{
	if((luwGroundIRms > cGround50mATestLimitU) || (luwGroundIRms < cGround50mATestLimitD))
	{
		lubGFCI50mAFaultCnt++;
		if(lubGFCI50mAFaultCnt > 5)
		{
			Error.bit.mubGFCIDeviveFault = 1;
			lubGFCI50mAFaultCnt = 0;
		}
	}
	else
	{
		lubGFCI50mAFaultCnt = 0;
	}
}

void sGFCICheck(INT16U luwGroundIRms, INT16S lswGroundIAvg)
{
	if(StateFlg.bit.ubGFCIStep == 0)
	{
		sInitialGroundCurrCal();
		StateFlg.bit.ubGFCIStep = 1;
		cGFCI50mATestOff();
	}
	else if(StateFlg.bit.ubGFCIStep == 1)
	{
		//Bug DS-240�����©�������Ϻ��޷��ظ���bug
		if(Error.bit.mubGroundCurrentFault)
		{
			if(luwGroundIRms < cGround0mATestLimit)
			{
				luwGroundCurrBackCnt++;
				if(luwGroundCurrBackCnt > 275)
				{
					Error.bit.mubGroundCurrentFault = 0;
				}
			}
		}
		if(++lub0mACnt <= 100)
		{
			lslGround0mASum += lswGroundIAvg;
		}
		else
		{
			lswGround0mAAvg = lslGround0mASum / 100;
			lub0mACnt = 101;
		}
	}
	else if(StateFlg.bit.ubGFCIStep == 2)
	{
		sGFCI0mAChk(luwGroundIRms);
	}
	else if(StateFlg.bit.ubGFCIStep == 3)
	{
		cGFCI50mATestOn();
	}
	else if(StateFlg.bit.ubGFCIStep == 4)
	{
		sGFCI50mAChk(luwGroundIRms);
	}
	else if(StateFlg.bit.ubGFCIStep == 5)
	{
		cGFCI50mATestOff();
	}
	else if(StateFlg.bit.ubGFCIStep == 6)
	{
		sGFCI0mAChk(luwGroundIRms);
	}
	else if(StateFlg.bit.ubGFCIStep == 7)
	{
	}
	else
	{
		sGFCIProtect(luwGroundIRms);
		sGFCIProtectFast();
	}
}
#endif

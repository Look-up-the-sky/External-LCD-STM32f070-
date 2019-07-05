/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�InputCheck.c
 * �޸��ˣ��²�
 * ��������·����ģʽ�ж�
 * �������ڣ�2014.10.29
 * �������ݣ�
 * 1.1��--��ʽ΢���������б����޸�     �漪��20140305
 * 1.2��--��1������δ�����ֳ���·���ӷ�ʽ�����˳��ж������϶���·�ֿ���
 * 		  ��֮����ѡ����·�ֿ�������·����������Ϊ�ڿͻ����������������·�����������
 * 1.3��--includeĿ¼��Ϊ����Ŀ���趨��
 * 1.4��--��Bus��ѹ�Ĳο�ֵ�Է���ֵ�ķ�ʽ���ݣ�
 * 1.5��--����������ж�ʱ��������10���ӣ�
 *******************************************************/
#include "App.h"

/********************************************************************
 * Local Types
 ********************************************************************/
INT16U luwPVSeparateCnt;
INT16U luwInputCheckCnt;
INT16U luwStep2Cnt;

#ifdef FloatEnable
float lfBusRef;
float lfBusRefTemp;
#else
INT16U luwBusRef;
INT16U luwBusRefTemp;
#endif

/********************************************************************
 * Global Types
 ********************************************************************/
INT8U gubPVInModel;

/*****************************************************************************
 * �������ܣ���ʼ����·����ģʽ��
 * �������룺�ޡ�
 * ������������·����ģʽ���ļ��������㣬����ģʽ��ʼ��Ϊ�����롣
 ****************************************************************************/
void sPVInModelIni(void)
{
	luwPVSeparateCnt = 0;
	luwInputCheckCnt = 0;
	gubPVInModel = c0PVIN;
}

#ifdef FloatEnable
/*****************************************************************************
 * �������ܣ�������·����ģʽ��
 * �������룺�ޡ�
 * �����������ڵȴ�ģʽ���ж���·��ѹ��������10V���ڣ���·���е�ѹʱΪ��·����
 * 			 ֻ��һ·�е�ѹʱΪ��·���ӣ������10V���ڣ����ڽ�������ģʽ������·
 * 			 ��ѹ��·�������ѹ��Ȼ��10V���ڣ�������Ϊ��·�������������10V��Ϊ��·
 * 			 ������
 ****************************************************************************/
float sPVInModelChk(float *lfPVVolt,float lfBusRefFromGrid)
{
	float lfDeltPVVolt;

	lfBusRef = lfBusRefFromGrid;
	lfDeltPVVolt = lfPVVolt[cPVIn1] - lfPVVolt[cPVIn2];

	if(StateFlg.bit.ubPvMdelChkStep == 0)
	{
		if(lfDeltPVVolt < cDifLimitVoltInput && lfDeltPVVolt > -cDifLimitVoltInput)
		{
			if(luwPVSeparateCnt >= 2)
			{
				luwPVSeparateCnt -= 2;
			}

			if(lfDeltPVVolt > 0)
			{
				StateFlg.bit.ubPvSearchFlg = cPVIn2;
				gubPVInModel = c2PVIN;
				lfBusRefTemp = lfPVVolt[cPVIn1];
			}
			else
			{
				StateFlg.bit.ubPvSearchFlg = cPVIn1;
				gubPVInModel = c1PVIN;
				lfBusRefTemp = lfPVVolt[cPVIn2];
			}
		}
		else
		{
			if(++luwPVSeparateCnt > 250)
			{
				StateFlg.bit.ubPvMdelChkStep = 3;
				luwPVSeparateCnt = 0;

				if(lfPVVolt[cPVIn1] > cVoltLimitInput && lfPVVolt[cPVIn2] > cVoltLimitInput)
				{
					gubPVInModel = c1A2PVIN;
				}
				else if(lfPVVolt[cPVIn1] > cVoltLimitInput)
				{
					gubPVInModel = c1PVIN;
				}
				else if(lfPVVolt[cPVIn2] > cVoltLimitInput)
				{
					gubPVInModel = c2PVIN;
				}
				else
				{
					StateFlg.bit.ubPvMdelChkStep = 0;
					gubPVInModel = c0PVIN;
				}
			}
			else
			{
				if(lfPVVolt[cPVIn1] > cVoltLimitInput && lfPVVolt[cPVIn2] <= cVoltLimitInput)
				{
					gubPVInModel = c1PVIN;
				}
				else if(lfPVVolt[cPVIn2] > cVoltLimitInput && lfPVVolt[cPVIn1] <= cVoltLimitInput)
				{
					gubPVInModel = c2PVIN;
				}
			}
		}
	}
	else if(StateFlg.bit.ubPvMdelChkStep == 1)		//��������ģʽ�󽫴˱�־λ����Ϊ1
	{
		luwInputCheckCnt = 0;
		luwPVSeparateCnt = 0;
		StateFlg.bit.ubPvMdelChkStep = 2;
		luwStep2Cnt = 0;
	}
	else if(StateFlg.bit.ubPvMdelChkStep == 2)
	{
		if(lfBusRef < lfBusRefTemp)
		{
			lfBusRef = lfBusRefTemp;
		}

		if(lfDeltPVVolt < cDifLimitVoltInput && lfDeltPVVolt > -cDifLimitVoltInput)
		{
			if(luwPVSeparateCnt >= 2)
			{
				luwPVSeparateCnt -= 2;
			}
		}
		else
		{
			luwPVSeparateCnt++;
		}

		if(luwPVSeparateCnt > 200)
		{
			gubPVInModel = c1A2PVIN;
			StateFlg.bit.ubPvMdelChkStep = 3;
			luwPVSeparateCnt = 0;
			luwInputCheckCnt = 0;
		}
		else if(lfPVVolt[StateFlg.bit.ubPvSearchFlg] + cBusPVDifVoltInput < lfBusRef)
		{
			if(++luwInputCheckCnt > 250)
			{
				gubPVInModel = c1S2PVIN;
				StateFlg.bit.ubPvMdelChkStep = 3;
				luwPVSeparateCnt = 0;
				luwInputCheckCnt = 0;
			}
		}
		else
		{
			luwPVSeparateCnt = 0;
			luwInputCheckCnt = 0;
		}

		if(++luwStep2Cnt >= c10MinMppt)		//10������δ����ɼ������Ϊ��·�ֿ�
		{
			gubPVInModel = c1A2PVIN;
			StateFlg.bit.ubPvMdelChkStep = 3;
			luwStep2Cnt = 0;
			luwPVSeparateCnt = 0;
			luwInputCheckCnt = 0;
		}
	}
	else
	{
		if(lfPVVolt[cPVIn1] > cVoltLimitInput && lfPVVolt[cPVIn2] > cVoltLimitInput)
		{
			if(gubPVInModel < c1A2PVIN)
			{
				gubPVInModel = c1A2PVIN;
			}
		}
		else if(lfPVVolt[cPVIn1] > cVoltLimitInput)
		{
			if(gubPVInModel >= c1A2PVIN)
			{
				gubPVInModel = c1PVIN;
			}
		}
		else if(lfPVVolt[cPVIn2] > cVoltLimitInput)
		{
			if(gubPVInModel >= c1A2PVIN)
			{
				gubPVInModel = c2PVIN;
			}
		}
		else
		{
			StateFlg.bit.ubPvMdelChkStep = 0;
			gubPVInModel = c0PVIN;
		}
	}
	
	return(lfBusRef);
}
#else
/*****************************************************************************
 * �������ܣ�������·����ģʽ��
 * �������룺�ޡ�
 * �����������ڵȴ�ģʽ���ж���·��ѹ��������10V���ڣ���·���е�ѹʱΪ��·����
 * 			 ֻ��һ·�е�ѹʱΪ��·���ӣ������10V���ڣ����ڽ�������ģʽ������·
 * 			 ��ѹ��·�������ѹ��Ȼ��10V���ڣ�������Ϊ��·�������������10V��Ϊ��·
 * 			 ������
 ****************************************************************************/
INT16U sPVInModelChk(INT16U *luwPVVolt,INT16U luwBusRefFromGrid)
{
	INT16S lswDeltPVVolt;

	luwBusRef = luwBusRefFromGrid;
	lswDeltPVVolt = (INT16S)luwPVVolt[cPVIn1] - (INT16S)luwPVVolt[cPVIn2];

	if(StateFlg.bit.ubPvMdelChkStep == 0)
	{
		if(lswDeltPVVolt < cDifLimitVoltInput && lswDeltPVVolt > -cDifLimitVoltInput)
		{
			if(luwPVSeparateCnt >= 2)
			{
				luwPVSeparateCnt -= 2;
			}

			if(lswDeltPVVolt > 0)
			{
				StateFlg.bit.ubPvSearchFlg = cPVIn2;
				gubPVInModel = c2PVIN;
				luwBusRefTemp = luwPVVolt[cPVIn1];
			}
			else
			{
				StateFlg.bit.ubPvSearchFlg = cPVIn1;
				gubPVInModel = c1PVIN;
				luwBusRefTemp = luwPVVolt[cPVIn2];
			}
		}
		else
		{
			if(++luwPVSeparateCnt > 250)
			{
				StateFlg.bit.ubPvMdelChkStep = 3;
				luwPVSeparateCnt = 0;

				if(luwPVVolt[cPVIn1] > cVoltLimitInput && luwPVVolt[cPVIn2] > cVoltLimitInput)
				{
					gubPVInModel = c1A2PVIN;
				}
				else if(luwPVVolt[cPVIn1] > cVoltLimitInput)
				{
					gubPVInModel = c1PVIN;
				}
				else if(luwPVVolt[cPVIn2] > cVoltLimitInput)
				{
					gubPVInModel = c2PVIN;
				}
				else
				{
					StateFlg.bit.ubPvMdelChkStep = 0;
					gubPVInModel = c0PVIN;
				}
			}
		}
	}
	else if(StateFlg.bit.ubPvMdelChkStep == 1)		//��������ģʽ�󽫴˱�־λ����Ϊ1
	{
		luwInputCheckCnt = 0;
		luwPVSeparateCnt = 0;
		StateFlg.bit.ubPvMdelChkStep = 2;
		luwStep2Cnt = 0;
	}
	else if(StateFlg.bit.ubPvMdelChkStep == 2)
	{
		if(luwBusRef < luwBusRefTemp)
		{
			luwBusRef = luwBusRefTemp;
		}

		if(lswDeltPVVolt < cDifLimitVoltInput && lswDeltPVVolt > -cDifLimitVoltInput)
		{
			if(luwPVSeparateCnt >= 2)
			{
				luwPVSeparateCnt -= 2;
			}
		}
		else
		{
			luwPVSeparateCnt++;
		}

		if(luwPVSeparateCnt > 200)
		{
			gubPVInModel = c1A2PVIN;
			StateFlg.bit.ubPvMdelChkStep = 3;
			luwPVSeparateCnt = 0;
			luwInputCheckCnt = 0;
		}
		else if(luwPVVolt[StateFlg.bit.ubPvSearchFlg] + cBusPVDifVoltInput < guwStandBusVolt)
		{
			if(++luwInputCheckCnt > 250)
			{
				gubPVInModel = c1S2PVIN;
				StateFlg.bit.ubPvMdelChkStep = 3;
				luwPVSeparateCnt = 0;
				luwInputCheckCnt = 0;
			}
		}
		else
		{
			luwPVSeparateCnt = 0;
			luwInputCheckCnt = 0;
		}

		if(++luwStep2Cnt >= 3000)		//1������δ����ɼ������Ϊ��·�ֿ�
		{
			gubPVInModel = c1A2PVIN;
			StateFlg.bit.ubPvMdelChkStep = 3;
			luwStep2Cnt = 0;
			luwPVSeparateCnt = 0;
			luwInputCheckCnt = 0;
		}
	}
	else
	{
		if(luwPVVolt[cPVIn1] > cVoltLimitInput && luwPVVolt[cPVIn2] > cVoltLimitInput)
		{
			if(gubPVInModel < c1A2PVIN)
			{
				gubPVInModel = c1A2PVIN;
			}
		}
		else if(luwPVVolt[cPVIn1] > cVoltLimitInput)
		{
			if(gubPVInModel >= c1A2PVIN)
			{
				gubPVInModel = c1PVIN;
			}
		}
		else if(luwPVVolt[cPVIn2] > cVoltLimitInput)
		{
			if(gubPVInModel >= c1A2PVIN)
			{
				gubPVInModel = c2PVIN;
			}
		}
		else
		{
			StateFlg.bit.ubPvMdelChkStep = 0;
			gubPVInModel = c0PVIN;
		}
	}
	
	return(luwBusRef);
}
#endif

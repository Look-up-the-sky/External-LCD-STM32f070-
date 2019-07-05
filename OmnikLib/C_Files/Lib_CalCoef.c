/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�Lib_CalCoef.c
 * �޸��ˣ��²�
 * �޸����ڣ�2014.12.26
 * ������AD����ϵ���ĳ�ʼ���ͽ���
 * �޸����ݣ�
 * 		1.1--����ϵ��Ĭ��ֵ��Ϊ1000��
 *******************************************************/

#include "App.h"
#include "Lib_CalCoef.h"

/*****************************************************************************
 * �������ܣ�����ϵ������
 * �������룺
 * ������������������ϵ��
 ****************************************************************************/
INT16U sCalCoef(INT16U *luwCoef,INT16U *luwMeterData,INT16U *luwMachineData,INT16U luwLength)
{
	INT16U i;

	if(luwLength > cMaxATEDataNum)
	{
		return(cFalse);
	}

	for(i = 0;i < luwLength;i++)
	{
		if(luwMachineData[i] == 0)
		{
			continue;
		}

		luwCoef[i] = (INT32U)luwCoef[i] * luwMeterData[i] / luwMachineData[i];

		if(luwCoef[i] > cCoefMax || luwCoef[i] < cCoefMin)
		{
			luwCoef[i] = cCoefDef;
		}
	}

	return(cTrue);
}

/*****************************************************************************
 * �������ܣ�����ϵ����ʼ��
 * �������룺
 * ��������������ϵ����ʼ������ϵ������Χ����ָ�ΪĬ��ֵ
 ****************************************************************************/
void sCoefInit(INT16U *luwCoef,INT16U luwLength)
{
	INT16U i;

	if(luwLength > cMaxATEDataNum)
	{
		luwLength = cMaxATEDataNum;
	}

	for(i = 0;i < luwLength;i++)
	{
		if(luwCoef[i] > cCoefMax || luwCoef[i] < cCoefMin)
		{
			luwCoef[i] = cCoefDef;
		}
	}
}

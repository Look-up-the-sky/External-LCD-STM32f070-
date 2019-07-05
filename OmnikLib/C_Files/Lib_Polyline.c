/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�Lib_QByV.c
 * �޸��ˣ��²�
 * �޸����ڣ�2015.10.27
 * ������ȡ�����ϵ�ĳһ��
 * �޸����ݣ�
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"

/*****************************************************************************
 * �������ƣ����ߵ���ֵ������
 * �������룺X��ٷֱȵ㣬Y��ٷֱȵ㣬��ǰX��ٷֱȡ�
 * ��������ֵ����ǰY��ٷֱȡ�
 * �����������ٷֱ��趨�����ĸ��㣬�����һ������Ϊ�������ĸ�����׵�ַ������
 * 			 �ڶ�������Ϊ�������ĸ�����׵�ַ������ǰX��ٷֱ����ĸ���֮�⣬��
 *			 ����ֵȡ�˵�ֵ������ǰX��ٷֱ����ĸ���֮�ڣ��򷵻�ֵȡ���Բ�ֵ��
 *			 ֵ��
 ****************************************************************************/
float sGetPolylinePoint(INT8U *lubXPercentSet,INT8S *lsbYPercentSet,float lfXPercent)
{
	float lfTemp;
	float lfXPercentSet[4];
	float lfYPercentSet[4];
	INT8U i;

	if(lubXPercentSet[0] <= lubXPercentSet[1] && lubXPercentSet[1] <= lubXPercentSet[2] && lubXPercentSet[2] <= lubXPercentSet[3])
	{
		for(i = 0;i < 4;i++)
		{
			lfXPercentSet[i] = (float)lubXPercentSet[i] / 100.0f;
			lfYPercentSet[i] = (float)lsbYPercentSet[i] / 100.0f;
		}

		if(lfXPercent < lfXPercentSet[0])
		{
			lfTemp = lfYPercentSet[0];
		}
		else if(lfXPercent < lfXPercentSet[1])
		{
			/*********************************************
			 * y = y0 + (x - x0) * (y1 - y0) / (x1 - x0)
			 *********************************************/
			lfTemp = lfXPercent - lfXPercentSet[0];
			lfTemp *= lfYPercentSet[1] - lfYPercentSet[0];
			lfTemp /= lfXPercentSet[1] - lfXPercentSet[0];
			lfTemp += lfYPercentSet[0];
		}
		else if(lfXPercent < lfXPercentSet[2])
		{
			lfTemp = lfXPercent - lfXPercentSet[1];
			lfTemp *= lfYPercentSet[2] - lfYPercentSet[1];
			lfTemp /= lfXPercentSet[2] - lfXPercentSet[1];
			lfTemp += lfYPercentSet[1];
		}
		else if(lfXPercent < lfXPercentSet[3])
		{
			lfTemp = lfXPercent - lfXPercentSet[2];
			lfTemp *= lfYPercentSet[3] - lfYPercentSet[2];
			lfTemp /= lfXPercentSet[3] - lfXPercentSet[2];
			lfTemp += lfYPercentSet[2];
		}
		else
		{
			lfTemp = lfYPercentSet[3];
		}

		return(lfTemp);
	}
	else
	{
		return(0.0f);
	}
}

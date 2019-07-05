/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�PhaseOrderCheck.c
 * �޸��ˣ��²�
 * �޸����ڣ�2014.10.29
 * ����������ʶ��
 * �޸����ݣ�
 * 1.1��--����CLA���������ж�
 * 1.2��--includeĿ¼��Ϊ����Ŀ���趨��
 *******************************************************/

#include "App.h"

#define cOrderChkFilter	10

INT16U luwNegOrderCnt;
INT16U luwPosOrderCnt;

extern INT16U guwPhaseOrderCLA;

#ifdef FloatEnable
void sPhaseOrderChk(float lfSVoltAtRZero,float lfTVoltAtRZero)
{
	if(lfSVoltAtRZero > lfTVoltAtRZero)
	{
		luwPosOrderCnt = 0;
		luwNegOrderCnt++;
		if (luwNegOrderCnt > cOrderChkFilter)
		{
			StateFlg.bit.ubPhaseOrder = cNegOrder;
			guwPhaseOrderCLA = cNegOrder;
			luwNegOrderCnt = 0;
			StateFlg.bit.ubPhaseCheckOk = cTrue;
		}
	}
	else if(lfSVoltAtRZero < lfTVoltAtRZero)
	{
		luwNegOrderCnt = 0;
		luwPosOrderCnt++;
		if (luwPosOrderCnt > cOrderChkFilter)
		{
			StateFlg.bit.ubPhaseOrder = cPosOrder;
			guwPhaseOrderCLA = cPosOrder;
			luwPosOrderCnt = 0;
			StateFlg.bit.ubPhaseCheckOk = cTrue;
		}
	}
	else
	{
		luwPosOrderCnt = 0;
		luwNegOrderCnt = 0;
	}
}

#else

void sPhaseOrderChk(INT16S lswSVoltAtRZero,INT16S lswTVoltAtRZero)
{
	if(lswSVoltAtRZero > lswTVoltAtRZero)
	{
		luwPosOrderCnt = 0;
		luwNegOrderCnt++;
		if (luwNegOrderCnt > cOrderChkFilter)
		{
			StateFlg.bit.ubPhaseOrder = cNegOrder;
			guwPhaseOrderCLA = cNegOrder;
			luwNegOrderCnt = 0;
			StateFlg.bit.ubPhaseCheckOk = cTrue;
		}
	}
	else if(lswSVoltAtRZero < lswTVoltAtRZero)
	{
		luwNegOrderCnt = 0;
		luwPosOrderCnt++;
		if (luwPosOrderCnt > cOrderChkFilter)
		{
			StateFlg.bit.ubPhaseOrder = cPosOrder;
			guwPhaseOrderCLA = cPosOrder;
			luwPosOrderCnt = 0;
			StateFlg.bit.ubPhaseCheckOk = cTrue;
		}
	}
	else
	{
		luwPosOrderCnt = 0;
		luwNegOrderCnt = 0;
	}
}

#endif


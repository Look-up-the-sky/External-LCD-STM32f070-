/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：PhaseOrderCheck.c
 * 修改人：陈波
 * 修改日期：2014.10.29
 * 描述：相序识别
 * 修改内容：
 * 1.1版--增加CLA的正负序判断
 * 1.2版--include目录改为在项目中设定；
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


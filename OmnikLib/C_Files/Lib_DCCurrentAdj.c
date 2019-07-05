/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：DCCurrentAdj.c
 * 修改人：陈波
 * 修改日期：2015.05.18
 * 描述：并网电流直流分量矫正
 * 修改内容：
 * 1.1版--修改状态标志位名称；
 * 1.2版--缩小直流分量可调整范围；
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"

/********************************************************************
 * Local Types
 ********************************************************************/
INT16U luwDC0ACnt;

#ifdef FloatEnable
float lfDC0ASum[cPhaseNum];
#else

#endif

/********************************************************************
 * Global Types
 ********************************************************************/
#ifdef FloatEnable
float gfSinAdj[cPhaseNum];
float gfDC0A[cPhaseNum];
#else

#endif

/********************************************************************
 * Extern Global Types
 ********************************************************************/
#ifdef FloatEnable

#else

#endif

#ifdef FloatEnable
/*****************************************************************************
 * 函数名称：直流分量抑制函数。
 * 函数输入：无。
 * 函数描述：判断并网电流直流分量是否超过调整阈值，超过阈值后，调节直流分量调节变量。
 ****************************************************************************/
void sCurrentDCAdj(const float *lfDCCurr, float lfDCCurrAdjMin, INT8U lubPhaseNum)
{
	INT8U i;
	float lfTemp;
	
	if(lfDCCurrAdjMin < c0A01)
	{
		for(i = cPhaseR;i < lubPhaseNum;i++)
		{
			gfSinAdj[i] = c0A;
		}
	}
	else
	{
		for(i = cPhaseR;i < lubPhaseNum;i++)
		{
			if(lfDCCurr[i] > lfDCCurrAdjMin || lfDCCurr[i] < -lfDCCurrAdjMin)
			{
				lfTemp = gfSinAdj[i];
				lfTemp -= lfDCCurr[i] * 0.5f;
				if(lfTemp > cMaxDCIAdj)
				{
					lfTemp = cMaxDCIAdj;
				}
				else if(lfTemp < -cMaxDCIAdj)
				{
					lfTemp = -cMaxDCIAdj;
				}
				gfSinAdj[i] = lfTemp;
			}
		}
	}
}

/*****************************************************************************
 * 函数名称：直流分量0值计算函数。
 * 函数输入：无。
 * 函数描述：计算并网电流直流分量的0值。
 ****************************************************************************/
void sCalDC0A(const float *lfDCCurr,const INT8U lubPhaseNum)
{
	INT8U i;
	float lfTemp;
	
	if(lubPhaseNum > cPhaseNum)
	{
		return;
	}
	
	if(StateFlg.bit.ubCalDC0AStep == 0)
	{
		for(i = cPhaseR;i < lubPhaseNum;i++)
		{
			gfDC0A[i] = c0A;
			lfDC0ASum[i] = c0A;
		}
		luwDC0ACnt = 0;
	}
	else if(StateFlg.bit.ubCalDC0AStep == 1)
	{
		for(i = cPhaseR;i < lubPhaseNum;i++)
		{
			lfDC0ASum[i] += lfDCCurr[i];
		}
		luwDC0ACnt++;
	}
	else if(StateFlg.bit.ubCalDC0AStep == 2)
	{
		if(luwDC0ACnt != 0)
		{
			for(i = cPhaseR;i < lubPhaseNum;i++)
			{
				lfTemp = lfDC0ASum[i] / luwDC0ACnt;
				if(lfTemp > cMaxDCIAdj)
				{
					lfTemp = cMaxDCIAdj;
				}
				else if(lfTemp < -cMaxDCIAdj)
				{
					lfTemp = -cMaxDCIAdj;
				}
				gfDC0A[i] = lfTemp;
			}
		}
		StateFlg.bit.ubCalDC0AStep = 3;
	}
}

#else
#endif

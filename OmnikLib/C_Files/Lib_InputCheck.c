/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：InputCheck.c
 * 修改人：陈波
 * 描述：两路输入模式判断
 * 初版日期：2014.10.29
 * 更改内容：
 * 1.1版--格式微调及定点中变量修改     舒吉祥20140305
 * 1.2版--若1分钟内未能区分出两路连接方式，则退出判定程序，认定两路分开接
 * 		  （之所以选择两路分开而非两路并联，是因为在客户处几乎不会出现两路并联的情况）
 * 1.3版--include目录改为在项目中设定；
 * 1.4版--将Bus电压的参考值以返回值的方式传递；
 * 1.5版--将并网后的判断时间增加至10分钟；
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
 * 函数功能：初始化两路输入模式。
 * 函数输入：无。
 * 函数描述：两路输入模式检测的计数器清零，输入模式初始化为无输入。
 ****************************************************************************/
void sPVInModelIni(void)
{
	luwPVSeparateCnt = 0;
	luwInputCheckCnt = 0;
	gubPVInModel = c0PVIN;
}

#ifdef FloatEnable
/*****************************************************************************
 * 函数功能：区分两路输入模式。
 * 函数输入：无。
 * 函数描述：在等待模式下判断两路电压差，如果不在10V以内，两路都有电压时为两路独立
 * 			 只有一路有电压时为单路链接；如果在10V以内，则在进入运行模式后开启单路
 * 			 升压电路，如果电压依然在10V以内，则输入为两路并联，如果超过10V则为两路
 * 			 独立。
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
	else if(StateFlg.bit.ubPvMdelChkStep == 1)		//进入运行模式后将此标志位设置为1
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

		if(++luwStep2Cnt >= c10MinMppt)		//10分钟内未能完成检测则认为两路分开
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
 * 函数功能：区分两路输入模式。
 * 函数输入：无。
 * 函数描述：在等待模式下判断两路电压差，如果不在10V以内，两路都有电压时为两路独立
 * 			 只有一路有电压时为单路链接；如果在10V以内，则在进入运行模式后开启单路
 * 			 升压电路，如果电压依然在10V以内，则输入为两路并联，如果超过10V则为两路
 * 			 独立。
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
	else if(StateFlg.bit.ubPvMdelChkStep == 1)		//进入运行模式后将此标志位设置为1
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

		if(++luwStep2Cnt >= 3000)		//1分钟内未能完成检测则认为两路分开
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

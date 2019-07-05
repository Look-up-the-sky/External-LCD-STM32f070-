/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：IsolationCheck.c
 * 修改人：陈波
 * 描述：绝缘检测
 * 初版日期：2014.10.29
 * 更改内容：
 * 1.1版--定点处变量错误更改；代码格式微调
 * 1.2版--修正计算中的bug；
 * 1.3版--include目录改为在项目中设定；
 * 1.4版--修改状态标志位名称；
 * 1.5版--增加注释
 *******************************************************/

#include "App.h"
#include "Lib_Communication.h"

#ifdef FloatEnable
float lfPV1Only1;
float lfPV2Only1;
float lfPV1Only2;
float lfPV2Only2;
float lfIsoVoltOnly1;
float lfIsoVoltOnly2;
float lfVolt1[2];
float lfVolt2[2];
float lfVoltt[2];
float lfTempD[2];
float lfYfenzi;
float lfYfenmu;
float lfZfenzi;
float lfZfenmu;
float lfX_Y;
float lfY_X;
float lfZ_X;
float lfX_Z;
float lfY_Z;
float lfZ_Y;

INT16U sIsoCheckOK(float lfPV1Volt, float lfPV2Volt, float lfIsoRLimit);
#else
INT16U luwPV1Only1;
INT16U luwPV2Only1;
INT16U luwPV1Only2;
INT16U luwPV2Only2;
INT16U luwIsoVoltOnly1;
INT16U luwIsoVoltOnly2;
INT32S lswTempA[2];
INT32S lswTempB[2];
INT32S lswTempC[2];
INT32S lswTempD[2];
INT32S lswYfenzi;
INT32S lswYfenmu;
INT32S lswZfenzi;
INT32S lswZfenmu;
INT32S lswX1;
INT32S lswX2;
INT32S lswX3;
INT32S lswX1A;
INT32S lswX2A;
INT32S lswX3A;

INT16U sIsoCheckOK(INT16U luwPV1Volt, INT16U luwPV2Volt, INT16U luwIsoRLimit);
#endif

#ifdef FloatEnable
/*****************************************************************************
 * 函数功能：绝缘监测。
 * 函数输入：逆变器直流侧电压和绝缘监测电阻电压。
 * 函数描述：绝缘监测第一阶段，断开两个绝缘监测继电器；第二阶段合第一路绝缘监测继电
 * 			 器；第三阶段合第二路绝缘监测继电器，断开第一路绝缘监测继电器；第四阶段
 * 			 断开两个绝缘监测继电器，调用绝缘电阻计算函数，若绝缘值低于要求值，置绝
 * 			 缘监测故 障标志位。绝缘监测阶段由状态转换函数确定。
 ****************************************************************************/
INT8U sIsoCheck(float lfPV1Volt, float lfPV2Volt, float lfIsoVolt, float lfIsoRLimit)
{
	if(StateFlg.bit.ubIsoCheckStep == cIsoRelayOff)
	{
		cPVRelay1Off();
		cPVRelay2Off();
	}
	else if(StateFlg.bit.ubIsoCheckStep == cIsoOlnyRelay1)
	{
		cPVRelay1On();
		cPVRelay2Off();
		lfPV1Only1 = lfPV1Volt;
		lfPV2Only1 = lfPV2Volt;
		lfIsoVoltOnly1 = lfIsoVolt;
	}
	else if(StateFlg.bit.ubIsoCheckStep == cIsoOlnyRelay2)
	{
		cPVRelay1Off();
		cPVRelay2On();
		lfPV1Only2 = lfPV1Volt;
		lfPV2Only2 = lfPV2Volt;
		lfIsoVoltOnly2 = lfIsoVolt;
	}
	else if(StateFlg.bit.ubIsoCheckStep == cIsoChecking)
	{
		cPVRelay1Off();
		cPVRelay2Off();

		if(sIsoCheckOK(lfPV1Volt, lfPV2Volt, lfIsoRLimit) == cFalse)
		{
			return(cTrue);
		}
	}
	return(cFalse);
}

/*****************************************************************************
 * 函数功能：绝缘电阻计算。
 * 函数输入：逆变器直流侧电压。
 * 函数描述：根据绝缘监测继电器两种状态下的电压值，列出两个方程，但是由于未知电阻有
 * 			 三个，不能计算出准确值，因此，假设一个对地电阻电阻是无穷大，计算另外两
 * 			 个电阻值，如果计算出的电阻值不足要求值，则返回0，否则返回1。
 ****************************************************************************/
INT16U sIsoCheckOK(float lfPV1Volt, float lfPV2Volt, float lfIsoRLimit)
{
	lfVolt1[0] = lfPV1Only1 - lfIsoVoltOnly1;							//V11
	lfVolt2[0] = lfPV2Only1 - lfIsoVoltOnly1;							//V21
	lfVoltt[0] = lfIsoVoltOnly1;										//Vt1
	lfTempD[0] = lfVoltt[0] / cResistance3 - lfVolt1[0] / cResistance1;	//Vt1/R3-V11/R1

	//Bug DS-225：解决绝缘检测计算错误的问题
	lfVolt1[1] = lfPV1Only2 - lfIsoVoltOnly2;							//V12
	lfVolt2[1] = lfPV2Only2 - lfIsoVoltOnly2;							//V22
	lfVoltt[1] = lfIsoVoltOnly2;										//Vt2
	lfTempD[1] = lfVoltt[1] / cResistance3 - lfVolt2[1] / cResistance2;	//Vt2/R3-V22/R2

	lfYfenzi = lfTempD[0] * lfVoltt[1] - lfTempD[1] * lfVoltt[0];
	lfYfenmu = lfVolt2[0] * lfVoltt[1] - lfVolt2[1] * lfVoltt[0];
	if(lfYfenzi != 0)
	{
		lfY_X = lfYfenmu / lfYfenzi;										//Ry,Rx无穷大
	}
	if(lfY_X < 0.0f || lfY_X > c2M5Ohm)
	{
		lfY_X = c2M5Ohm;
	}

	lfZfenzi = lfTempD[0] * lfVolt2[1] - lfTempD[1] * lfVolt2[0];
	lfZfenmu = lfVolt2[0] * lfVoltt[1] - lfVolt2[1] * lfVoltt[0];
	if(lfZfenzi != 0)
	{
		lfZ_X = lfZfenmu / lfZfenzi;										//Rz,Rx无穷大
	}
	if(lfZ_X < 0.0f || lfZ_X > c2M5Ohm)
	{
		lfZ_X = c2M5Ohm;
	}

	lfYfenzi = lfTempD[0] * lfVoltt[1] - lfTempD[1] * lfVoltt[0];
	lfYfenmu = lfVolt1[0] * lfVoltt[1] - lfVolt1[1] * lfVoltt[0];
	if(lfYfenzi != 0)
	{
		lfX_Y = lfYfenmu / lfYfenzi;										//Rx,Ry无穷大
	}
	if(lfX_Y < 0.0f || lfX_Y > c2M5Ohm)
	{
		lfX_Y = c2M5Ohm;
	}

	lfZfenzi = lfTempD[0] * lfVolt1[1] - lfTempD[1] * lfVolt1[0];
	lfZfenmu = lfVolt1[0] * lfVoltt[1] - lfVolt1[1] * lfVoltt[0];
	if(lfZfenzi != 0)
	{
		lfZ_Y = lfZfenmu / lfZfenzi;										//Rz,Ry无穷大
	}
	if(lfZ_Y < 0.0f || lfZ_Y > c2M5Ohm)
	{
		lfZ_Y = c2M5Ohm;
	}

	lfYfenzi = lfTempD[0] * lfVolt2[1] - lfTempD[1] * lfVolt2[0];
	lfYfenmu = lfVolt1[0] * lfVolt2[1] - lfVolt1[1] * lfVolt2[0];
	if(lfYfenzi != 0)
	{
		lfX_Z = lfYfenmu / lfYfenzi;										//Rx,Rz无穷大
	}
	if(lfX_Z < 0.0f || lfX_Z > c2M5Ohm)
	{
		lfX_Z = c2M5Ohm;
	}

	lfZfenzi = lfTempD[0] * lfVolt1[1] - lfTempD[1] * lfVolt1[0];
	lfZfenmu = lfVolt2[0] * lfVolt1[1] - lfVolt2[1] * lfVolt1[0];
	if(lfZfenzi != 0)
	{
		lfY_Z = lfZfenmu / lfZfenzi;										//Ry,Rz无穷大
	}
	if(lfY_Z < 0.0f || lfY_Z > c2M5Ohm)
	{
		lfY_Z = c2M5Ohm;
	}

	if(lfPV1Volt < cNoInputVoltIso)			//Only connect PV2
	{
		MasterData.data.uwIsoResitance[0] = c2M5Ohm_k;
		MasterData.data.uwIsoResitance[1] = lfY_X / 1000.0f;
		MasterData.data.uwIsoResitance[2] = lfZ_X / 1000.0f;
		MasterData.data.uwIsoResitance[3] = c2M5Ohm_k;
		if(lfY_X < lfIsoRLimit || lfZ_X < lfIsoRLimit)
		{
			return (cFalse);
		}
		else
		{
			return (cTrue);
		}
	}
	else if(lfPV2Volt < cNoInputVoltIso)    //Only connect PV1
	{
		MasterData.data.uwIsoResitance[0] = lfX_Y / 1000.0f;
		MasterData.data.uwIsoResitance[1] = c2M5Ohm_k;
		MasterData.data.uwIsoResitance[2] = c2M5Ohm_k;
		MasterData.data.uwIsoResitance[3] = lfZ_Y / 1000.0f;
		if(lfX_Y < lfIsoRLimit || lfZ_Y < lfIsoRLimit)
		{
			return (cFalse);
		}
		else
		{
			return (cTrue);
		}
	}
	else if(lfPV1Volt - lfPV2Volt < cDifLimitVoltIso && lfPV1Volt - lfPV2Volt > -cDifLimitVoltIso)	//PV1 and PV2 is nearly the same
	{
		MasterData.data.uwIsoResitance[0] = lfX_Y / 1000.0f;
		MasterData.data.uwIsoResitance[1] = lfY_X / 1000.0f;
		MasterData.data.uwIsoResitance[2] = lfZ_X / 1000.0f;
		MasterData.data.uwIsoResitance[3] = lfZ_Y / 1000.0f;
		if((lfX_Y < lfIsoRLimit) || (lfY_X < lfIsoRLimit) || ((lfZ_Y < lfIsoRLimit) && (lfZ_X < lfIsoRLimit)))
		{
			return (cFalse);
		}
		else
		{
			return (cTrue);
		}
	}
	
	if(lfX_Y > lfX_Z)
	{
		MasterData.data.uwIsoResitance[0] = lfX_Y / 1000.0f;
	}
	else
	{
		MasterData.data.uwIsoResitance[0] = lfX_Z / 1000.0f;
	}
	
	if(lfY_X > lfY_Z)
	{
		MasterData.data.uwIsoResitance[1] = lfY_X / 1000.0f;
	}
	else
	{
		MasterData.data.uwIsoResitance[1] = lfY_Z / 1000.0f;
	}
	MasterData.data.uwIsoResitance[2] = lfZ_X / 1000.0f;
	MasterData.data.uwIsoResitance[3] = lfZ_Y / 1000.0f;

	if(((lfX_Y < lfIsoRLimit) && (lfX_Z < lfIsoRLimit)) ||
	   ((lfY_X < lfIsoRLimit) && (lfY_Z < lfIsoRLimit)) ||
	   ((lfZ_X < lfIsoRLimit) && (lfZ_Y < lfIsoRLimit)))
	{
		return (cFalse);
	}
	else
	{
		return (cTrue);
	}
}
#else
/*****************************************************************************
 * 函数功能：绝缘监测。
 * 函数输入：逆变器直流侧电压和绝缘监测电阻电压。
 * 函数描述：绝缘监测第一阶段，断开两个绝缘监测继电器；第二阶段合第一路绝缘监测继电
 * 			 器；第三阶段合第二路绝缘监测继电器，断开第一路绝缘监测继电器；第四阶段
 * 			 断开两个绝缘监测继电器，调用绝缘电阻计算函数，若绝缘值低于要求值，置绝
 * 			 缘监测故 障标志位。绝缘监测阶段由状态转换函数确定。
 ****************************************************************************/
INT8U sIsoCheck(INT16U luwPV1Volt, INT16U luwPV2Volt, INT16U luwIsoVolt, INT16U luwIsoRLimit)
{
	if(StateFlg.bit.ubIsoCheckStep == cIsoRelayOff)
	{
		cPVRelay1Off();
		cPVRelay2Off();
	}
	else if(StateFlg.bit.ubIsoCheckStep == cIsoOlnyRelay1)
	{
		cPVRelay1On();
		cPVRelay2Off();
		luwPV1Only1 = luwPV1Volt;
		luwPV2Only1 = luwPV2Volt;
		luwIsoVoltOnly1 = luwIsoVolt;
	}
	else if(StateFlg.bit.ubIsoCheckStep == cIsoOlnyRelay2)
	{
		cPVRelay1Off();
		cPVRelay2On();
		luwPV1Only2 = luwPV1Volt;
		luwPV2Only2 = luwPV2Volt;
		luwIsoVoltOnly2 = luwIsoVolt;
	}
	else if(StateFlg.bit.ubIsoCheckStep == cIsoChecking)
	{
		cPVRelay1Off();
		cPVRelay2Off();

		if(sIsoCheckOK(luwPV1Volt, luwPV2Volt, luwIsoRLimit) == cFalse)
		{
			return(cTrue);
		}
	}
	return(cFalse);
}
/*****************************************************************************
 * 函数功能：绝缘电阻计算。
 * 函数输入：逆变器直流侧电压。
 * 函数描述：根据绝缘监测继电器两种状态下的电压值，列出两个方程，但是由于未知电阻有
 * 			 三个，不能计算出准确值，因此，假设一个对地电阻电阻是无穷大，计算另外两
 * 			 个电阻值，如果计算出的电阻值不足要求值，则返回0，否则返回1。
 ****************************************************************************/
INT16U sIsoCheckOK(INT16U luwPV1Volt, INT16U luwPV2Volt, INT16U luwIsoRLimit)
{
	//Bug DS-225：解决绝缘检测计算错误的问题
	lswTempC[0] = ((INT32S)luwIsoVoltOnly1 * 13408) >> 10;
	lswTempA[0] = (INT32S)luwPV1Only1 * 20 - lswTempC[0];
	lswTempB[0] = (INT32S)luwPV2Only1 * 20 - lswTempC[0];
	lswTempD[0] = lswTempC[0] * 100 / cResistance3 - lswTempA[0] * 100 / cResistance1;

	lswTempC[1] = ((INT32S)luwIsoVoltOnly2 * 13408) >> 10;
	lswTempA[1] = (INT32S)luwPV1Only2 * 20 - lswTempC[1];
	lswTempB[1] = (INT32S)luwPV2Only2 * 20 - lswTempC[1];
	lswTempD[1] = lswTempC[1] * 100 / cResistance3 - lswTempB[1] * 100 / cResistance2;

	lswYfenzi = (lswTempD[0] * lswTempC[1] - lswTempD[1] * lswTempC[0])/10;
	lswYfenmu = lswTempB[0] * lswTempC[1] - lswTempB[1] * lswTempC[0];
	if(lswYfenzi != 0)
	{
		lswX2 = lswYfenmu / lswYfenzi;
	}
	if(lswX2 <= 0)
	{
		lswX2 = c2M5Ohm;
	}

	lswZfenzi = (lswTempD[0] * lswTempB[1] - lswTempD[1] * lswTempB[0])/10;
	lswZfenmu = lswTempB[0] * lswTempC[1] - lswTempB[1] * lswTempC[0];
	if(lswZfenzi != 0)
	{
		lswX3 = lswZfenmu / lswZfenzi;
	}
	if(lswX3 <= 0)
	{
		lswX3 = c2M5Ohm;
	}

	lswYfenzi = (lswTempD[0] * lswTempC[1] - lswTempD[1] * lswTempC[0])/10;
	lswYfenmu = lswTempA[0] * lswTempC[1] - lswTempA[1] * lswTempC[0];
	if(lswYfenzi != 0)
	{
		lswX1 = lswYfenmu / lswYfenzi;
	}
	if(lswX1 <= 0)
	{
		lswX1 = c2M5Ohm;
	}

	lswZfenzi = (lswTempD[0] * lswTempA[1] - lswTempD[1] * lswTempA[0])/10;
	lswZfenmu = lswTempA[0] * lswTempC[1] - lswTempA[1] * lswTempC[0];
	if(lswZfenzi != 0)
	{
		lswX3A = lswZfenmu / lswZfenzi;
	}
	if(lswX3A <= 0)
	{
		lswX3A = c2M5Ohm;
	}

	lswYfenzi = (lswTempD[0] * lswTempB[1] - lswTempD[1] * lswTempB[0])/10;
	lswYfenmu = lswTempA[0] * lswTempB[1] - lswTempA[1] * lswTempB[0];
	if(lswYfenzi != 0)
	{
		lswX1A = lswYfenmu / lswYfenzi;
	}
	if(lswX1A <= 0)
	{
		lswX1A = c2M5Ohm;
	}

	lswZfenzi = (lswTempD[0] * lswTempA[1] - lswTempD[1] * lswTempA[0])/10;
	lswZfenmu = lswTempB[0] * lswTempA[1] - lswTempB[1] * lswTempA[0];
	if(lswZfenzi != 0)
	{
		lswX2A = lswZfenmu / lswZfenzi;
	}
	if(lswX2A <= 0)
	{
		lswX2A = c2M5Ohm;
	}

	if(luwPV1Volt < cNoInputVoltIso)	//Only connect PV2
	{
		if(((lswX2 < luwIsoRLimit) && (lswX2A < luwIsoRLimit)) || ((lswX3 < luwIsoRLimit) && (lswX1A < luwIsoRLimit)))
		{
			return (cFalse);
		}
		else
		{
			return (cTrue);
		}
	}
	else if(luwPV2Volt < cNoInputVoltIso)    //Only connect PV1
	{
		if(((lswX1 < luwIsoRLimit) && (lswX1A < luwIsoRLimit)) || ((lswX3A < luwIsoRLimit) && (lswX2A < luwIsoRLimit)))
		{
			return (cFalse);
		}
		else
		{
			return (cTrue);
		}
	}
	else if((luwPV1Volt < (luwPV2Volt + cDifLimitVoltIso)) && (luwPV1Volt > (luwPV2Volt - cDifLimitVoltIso)))	//PV1 and PV2 is nearly the same
	{
		if((lswX1 < luwIsoRLimit) || (lswX2 < luwIsoRLimit) || ((lswX3A < luwIsoRLimit) && (lswX3 < luwIsoRLimit)))
		{
			return (cFalse);
		}
		else
		{
			return (cTrue);
		}
	}

	if(((lswX1 < luwIsoRLimit) && (lswX1A < luwIsoRLimit)) ||
	   ((lswX2 < luwIsoRLimit) && (lswX2A < luwIsoRLimit)) ||
	   ((lswX3 < luwIsoRLimit) && (lswX3A < luwIsoRLimit)))
	{
		return (cFalse);
	}
	else
	{
		return (cTrue);
	}
}
#endif

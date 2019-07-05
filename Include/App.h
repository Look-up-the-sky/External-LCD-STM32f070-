#ifndef _APP_H_
#define _APP_H_

#include "TypeDefine.h"

#define MonitorEnable

#define Default_Model   0
#define SUNNYBEE	1
#define HORNET  	2
extern INT8U PRJ_NUMBER;


/*******Logical bit********/
#define cTrue	1
#define cFalse	0

/*******Switch Model*********/
#define cOn		1
#define cOff	0

/*******Input Number********/
#define cPVIn1	0
#define cPVIn2	1

/************Phase************/
#define cPhaseR		0
#define cPhaseS		1
#define cPhaseT		2
#define cPhaseNum	3

/**********Task Number**********/

#define cModbusTask	      0
#define cVariableTask     1
#define cLedStateTask	    2
#define cDisplayTask		  3
#define cI2CTask			    4
#define cTaskNum			    5

/**********Inverter State**********/
#define cWaitState			0
#define cNormalState		1
#define cFaultState			2
#define cDataState			3
#define cFlashState			4

/***Frequency of Oscillator***/
#define cOscFreq	48000000

/*******Voltage********/
#define c10V	100
#define c20V	200
#define c50V	500
#define c80V	800
#define c110V	1100
#define c150V	1500
#define c180V	1800
#define c190V	1900
#define c210V	2100
#define c220V	2200
#define c230V	2300
#define c240V	2400
#define c250V	2500
#define c253V	2530
#define c280V	2800
#define c242V	2420

/************Frequency************/
#define c0Hz	0
#define c0Hz05	5
#define c0Hz8	80
#define c45Hz	4500
#define c49Hz9	4990
#define c50Hz	5000
#define c50Hz03	5003
#define c50Hz1	5010
#define c50Hz2	5020
#define c55Hz	5500
#define c59Hz9	5990
#define c60Hz1	6010
#define c65Hz	6500

/***********Current**********/
#define cMaxInvCurr	3000

/************Temperature************/
#define c10Degree	100
#define c55Degree	550
#define c62Degree	620
#define c80Degree	800
#define c85Degree	850
#define c90Degree	900
#define c95Degree	950
#define c100Degree	1000
#define c110Degree	1100

/**********ATE**********/
#define cMaxATEDataNum	10

typedef union
{
	INT16U all;
	struct
	{
		INT16U ubButtonState:2;
		INT16U ubDisplayLock:1;
		INT16U ubFlashFlg:2;
		INT16U ubGridZeroFlg:1;
		INT16U ubFlash:1;
		INT16U ubBufEmptyFlg:1;
		INT16U ubClearEtodayFlg:1;
		INT16U ubResetFlg:1;
		INT16U ubAdminFlg:1;
		INT16U ubReserve:5;
	}bit;
}STATEFLAG;

extern STATEFLAG StateFlg;		//״̬��־λ

typedef union
{
	INT32U all;
	struct
	{
		INT16U mubGFCIDeviveFault: 1;
		INT16U mubIslandFault:1;
		INT16U mubPVPowerLow:1;
		INT16U mubPVVoltLow:1;
		INT16U mubConsistencyFalut: 1;
	    INT16U mubBusVoltLow: 1;
	    INT16U mubBusVoltHigh: 1;
	    INT16U mubDeviceFault: 1;

	    INT16U mubBatOverVolt: 1;
	    INT16U mubNoUtility: 1;
	    INT16U mubGroundCurrentFault: 1;
	    INT16U mubBusUnbalance: 1;
	    INT16U mub10minOverVolt: 1;
	    INT16U mubOverTempFault: 1;
	    INT16U mubAutoTestfail: 1;
	    INT16U mubPVVoltHigh: 1;

	    INT16U mubFanFault: 1;
	    INT16U mubGridVoltFault: 1;
	    INT16U mubIsolationFault: 1;
	    INT16U mubDCOffset: 1;
	    INT16U mubBatReverse: 1;
	    INT16U mubPV2OverCurr: 1;
	    INT16U mubBatVoltLow: 1;
	    INT16U mubBatLoss: 1;

	    INT16U mubPV1OverCurr: 1;
	    INT16U mubRlyFault: 1;
	    INT16U mubOPCurrOver: 1;
	    INT16U mubInvOverCurr: 1;
	    INT16U mubGridVoltFaultTemp: 1;
	    INT16U mubGridFreqFault: 1;
	    INT16U mubEepromFault: 1;
	    INT16U mubCommLose: 1;
	}bit;
}ERRORFLAG;


#endif

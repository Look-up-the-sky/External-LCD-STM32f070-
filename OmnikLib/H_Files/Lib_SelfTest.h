#ifndef _LIB_SELF_TEST_
#define _LIB_SELF_TEST_

#define cSelfTestStart	0
#define cWaitVoltHigh1	1
#define cVoltHigh1Value	2
#define cWaitVoltHigh2	3
#define cVoltHigh2Value	4
#define cWaitVoltLow1	5
#define cVoltLow1Value	6
#define cWaitVoltLow2	7
#define cVoltLow2Value	8
#define cWaitFreqHigh1	9
#define cFreqHigh1Value	10
#define cWaitFreqHigh2	11
#define cFreqHigh2Value	12
#define cWaitFreqLow1	13
#define cFreqLow1Value	14
#define cWaitFreqLow2	15
#define cFreqLow2Value	16
#define cTimeFault		17
#define cOverTime		18
#define cSelfTestEnd	19

#define cSelfTestValue	0
#define cSelfTestTime	1

#ifdef FloatEnable
void sSelfTestControl(INT8U lubSelfTest,INT8U lubState);
void sSetSelfTestFlag(INT8U lubVoltorFreq);
void sClearSelfTestFlag(INT8U lubVoltorFreq);
INT16U sGetFaultDelay(INT8U lubSelfTest,INT8U lubState,float lfGridVolt,float lfGridFreq);
INT16U sGetFaultValue(void);
#else
void sSelfTestControl(INT16U luwSelfTest);
#endif
INT8U sGetSelfTestStep(void);
void sInitControl2Data(void);
INT16U sGetSelfTestData(INT8U i);
INT8U sSelfTestControl2(INT8U lubButtonState,INT16U luwValue,INT16U luwTime);

#endif

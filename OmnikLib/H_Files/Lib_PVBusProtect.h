#ifndef _LIB_PVBUSProtect_H_
#define _LIB_PVBUSProtect_H_

#ifdef FloatEnable
void sPVVoltCheck(float *lfPVVolt,INT8U lubPVNum,float lfPVVoltLow);
void sPVVoltLowCheck_SinglePV(float lfPVVolt,float lfPVVoltLow);
void sBusVoltLowCheck(float lfBusVolt,float lfBusVoltLow);
#else
void sPVVoltCheck(INT16U *luwPVVolt,INT8U lubPVNum,INT16U luwPVVoltLow);
void sBusVoltLowCheck(INT16U luwBusVolt,INT16U luwBusVoltLow);
#endif

#endif

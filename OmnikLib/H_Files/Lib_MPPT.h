#ifndef _LIB_MPPT_H_
#define _LIB_MPPT_H_

#ifdef FloatEnable
void sCalDeltVolt(INT8U lubInputType,float lfPowerAC,float *lfPowerDC,float lfPowerACMax,float *lfPVVolt,float *lfPVRef);
void sSetPVRatedPower(float lfPV1RatedPower,float lfPV2RatedPower);
void sSetMPPTPre(INT8U lubInputType,float lfPVVoltPreTemp,float lfPVPowerPreTemp);
#else
INT16S sCalDeltVolt(INT8U lubPVInNo,INT16U luwPowerAC,INT16U luwPowerDC,INT16U luwPVVolt,INT16U luwPowerACMax,INT16U luwPowerDCMax,INT16U luwPVRef);
void sSetPVRatedPower(float lfPV1RatedPower,float lfPV2RatedPower);
#endif

#endif


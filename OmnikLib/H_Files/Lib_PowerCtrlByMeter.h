#ifndef _LIB_POWERCTRLBYMETER_H_
#define _LIB_POWERCTRLBYMETER_H_

#ifdef FloatEnable
float sSetPowerByMeter(PQDDATA *PQDDataTemp, float lfInvPower);
void sPowerByMeterInit(void);
#else
INT16U sSetPowerByMeter(PQDDATA *PQDDataTemp, INT16U luwInvPower);
void sPowerByMeterInit(void);
#endif

#endif

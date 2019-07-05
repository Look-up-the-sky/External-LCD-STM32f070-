#ifndef _LIB_TEMPERATUREPROTECT_H_
#define _LIB_TEMPERATUREPROTECT_H_

#ifdef FloatEnable
float sCalPowerByTemperature(float lfTemperature,INT8U lubError);
void sCalPowerByTemperatureInitial(void);
void sSetTPoint(float lfTemp1,float lfTemp2);
#else
INT16U sCalPowerByTemperature(INT16U luwTemperature,INT8U lubError);
void sCalPowerByTemperatureInitial(void);
void sSetTPoint(INT16U luwTemp1,INT16U luwTemp2);
#endif

#endif

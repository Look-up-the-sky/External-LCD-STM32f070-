#ifndef _LIB_POWERDECREASEBYFREQ_H_
#define _LIB_POWERDECREASEBYFREQ_H_

#ifdef FloatEnable
void sSetPowerByFreqLimit(float lfStartFreqTemp, float lfRecoverFreqTemp, float lfDecreaseRatioTemp, float lfIncreaseRatioTemp, INT16U luwPowerDelayTimeTemp);
void sIniPowerByFreq(void);
float sPowerByFreq(float lfGridFreq, float lfACPower);
float sPowerByFreq2(float lfGridFreq, float lfStartFreq, float lfDecreaseRatio, float lfACPower);
#else
void sPowerByFreq(INT16U luwGridFreq, INT16U luwStartFreq, INT16U luwDecreaseRatio);
#endif

#endif

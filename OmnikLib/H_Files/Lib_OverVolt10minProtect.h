#ifndef _LIB_OVERVOLT10MINPROTECT_H_
#define _LIB_OVERVOLT10MINPROTECT_H_

#ifdef FloatEnable
void s10minVoltInitial(INT16U luwPhaseNum);
INT8U s10minVoltCheck(const float *lfGridVolt, INT8U luwPhaseNum,INT8U lubError);
void sSet10MinLimit(float lfLimit);
#else
void s10minVoltInitial(INT16U luwPhaseNum);
INT8U s10minVoltCheck(const INT16U *luwGridVolt, INT8U luwPhaseNum,INT8U lubError);
void sSet10MinLimit(INT16U luwLimit);
#endif

#endif

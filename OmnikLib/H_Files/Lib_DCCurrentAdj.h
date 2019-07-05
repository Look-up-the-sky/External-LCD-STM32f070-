#ifndef _LIB_DCCURRENTADJ_H_
#define _LIB_DCCURRENTADJ_H_

#ifdef FloatEnable
void sCurrentDCAdj(const float *lfDCCurr, float lfDCCurrAdjMin, INT8U lubPhaseNum);
void sCalDC0A(const float *lfDCCurr,const INT8U lubPhaseNum);
#else
#endif

#endif

#ifndef _LIB_GROUNDCURRENTPROTECT_H_
#define _LIB_GROUNDCURRENTPROTECT_H_

#ifdef FloatEnable
extern pFuncf sSqrtFunc;

void sCalGroundCurrent(float lfGroundIRms, float lfGroundIAvg);
void sGFCICheck(float lfGroundIRms, float lfGroundIAvg);
#else
void sCalGroundCurrent(INT16U luwGroundIRms, INT16S lswGroundIAvg);
void sGFCICheck(INT16U luwGroundIRms, INT16S lswGroundIAvg);
#endif

#endif

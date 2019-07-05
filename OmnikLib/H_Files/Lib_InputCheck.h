#ifndef _LIB_INPUT_CHECK_H_
#define _LIB_INPUT_CHECK_H_

void sPVInModelIni(void);
#ifdef FloatEnable
float sPVInModelChk(float *lfPVVolt,float lfBusRefFromGrid);
#else
INT16U sPVInModelChk(INT16U *luwPVVolt,INT16U luwBusRefFromGrid);
#endif

#endif

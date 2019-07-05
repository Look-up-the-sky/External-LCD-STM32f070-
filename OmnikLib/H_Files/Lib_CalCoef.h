#ifndef _LIB_CALCOEF_H_
#define _LIB_CALCOEF_H_

#define cCoefMax	1200
#define cCoefMin	800
#define cCoefDef	1000

INT16U sCalCoef(INT16U *luwCoef,INT16U *luwMeterData,INT16U *luwMachineData,INT16U luwLength);
void sCoefInit(INT16U *luwCoef,INT16U luwLength);

#endif

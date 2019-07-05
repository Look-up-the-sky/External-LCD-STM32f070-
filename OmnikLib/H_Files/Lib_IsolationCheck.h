#ifndef _LIB_ISOLATIONCHECK_H_
#define _LIB_ISOLATIONCHECK_H_

#ifdef FloatEnable
INT8U sIsoCheck(float lfPV1Volt, float lfPV2Volt, float lfIsoVolt, float lfIsoRLimit);
#else
INT8U sIsoCheck(INT16U luwPV1Volt, INT16U luwPV2Volt, INT16U luwIsoVolt, INT16U luwIsoRLimit);
#endif

#endif

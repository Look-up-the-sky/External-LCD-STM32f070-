#ifndef _LIB_DEBUG_H_
#define _LIB_DEBUG_H_

#ifdef DebugEnable
void sClearDebugRXCnt(void);
INT8U sAnalysisDebug(void);
INT8U sTXOKCheck(void);
INT16U sGetDebugTXBufData(void);
INT8U sSetDebugRXBufData(INT8U lubData);
#endif

#endif

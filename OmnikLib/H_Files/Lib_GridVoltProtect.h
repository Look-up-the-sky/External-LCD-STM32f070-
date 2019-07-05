/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�Lib_GridVoltProtect.h
 * �޸��ˣ��²�
 * ��������Ƿѹ����
 * �������ڣ�2015.06.24
 * �޸����ݣ�
 * 1.1��--������ȫ����Ϊ�ֲ�������
 *******************************************************/

#ifndef _LIB_GRIDVOLTPROTECT_H_
#define _LIB_GRIDVOLTPROTECT_H_

#include "Lib_VoltFreq.h"

#ifdef FloatEnable
INT16U sVoltCheck(const float *lfGridVolt, INT8U luwPhaseNum,INT8U lubError,float lfGridFreq);
void sSetVoltLimit(INT8U lubTimeNum,float lfVolt);
float sGetVoltLimit(INT8U lubTimeNum);
#else
INT8U sVoltCheck(const INT16U *luwGridVolt, INT16U luwPhaseNum,INT8U lubError);
void sSetVoltHigh(INT16U luwVolt1,INT16U luwVolt2);
void sSetVoltLow(INT16U luwVolt1,INT16U luwVolt2);
void sSetVoltBack(INT16U luwVoltLow,INT16U luwVoltHigh);
#endif

void sVoltCheckInitial(INT16U luwPhaseNum);
void sSetVoltTime(INT8U lubTimeNum,INT16U luwTime);
INT16U sGetVoltTime(INT8U lubTimeNum);

#endif

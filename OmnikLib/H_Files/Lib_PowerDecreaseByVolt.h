/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�Lib_PowerDecreaseByVolt.h
 * �޸��ˣ��²�
 * �޸����ڣ�2015.06.24
 * ��������ѹ����
 * �޸����ݣ�
 *******************************************************/

#ifndef _LIB_POWERDECREASEBYVOLT_H_
#define _LIB_POWERDECREASEBYVOLT_H_

#ifdef FloatEnable
float sPowerByVolt(float lfGridVolt);
void sSetPowerByVoltLimit(float lfVolt1,float lfVolt2);
#else
INT8U sPowerByVolt(INT16U luwGridVolt);
void sSetPowerByVoltLimit(INT16U luwVolt1,INT16U luwVolt2);
#endif

#endif

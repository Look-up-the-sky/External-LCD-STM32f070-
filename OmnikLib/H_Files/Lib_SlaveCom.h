/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�Lib_SlaveCom.h
 * �޸��ˣ��²�
 * ������ͨѶ
 * �������ڣ�2015.05.29
 * �������ݣ�
 * 1.1��--���Ӳ������趨���ܣ�
 *******************************************************/

#ifndef _LIB_SLAVECOM_H_
#define _LIB_SLAVECOM_H_

INT8U sAnalysisS(INT8U lubComNum);
INT8U sSetDelayTime(void);
void sSetSlaveCom(INT8U lubAddr,INT8U lubComNum);
INT8U sGetSlaveCom(INT8U lubComNum);
INT16U sGetEepromW(void);

#endif

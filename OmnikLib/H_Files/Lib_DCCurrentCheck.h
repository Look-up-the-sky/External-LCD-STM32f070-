/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�Lib_DCCurrentCheck.h
 * �޸��ˣ��²�
 * �޸����ڣ�2014.10.29
 * ��������������ֱ���������
 * �޸����ݣ�
 * 1.1��--������ȫ����Ϊ�ֲ�������
 *******************************************************/

#ifndef _LIB_DCCURRENTCHECK_H_
#define _LIB_DCCURRENTCHECK_H_

void sSetDCLimitTime(INT16U luwTime1,INT16U luwTime2,INT16U luwBackTime);

#ifdef FloatEnable
void sSetDCLimit(float lfLimit1,float lflimit2,float lfBackLimit);
INT8U sCurrentDCCheck(const float *lfDCCurr, INT8U lubPhaseNum,INT8U lubError);
#else
void sSetDCLimit(INT16S lswLimit1,INT16S lswlimit2,INT16S lswBackLimit);
INT8U sCurrentDCCheck(const INT16S *lswDCCurr, INT8U lubPhaseNum,INT8U lubError);
#endif

#endif

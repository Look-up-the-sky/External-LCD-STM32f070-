/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�GridFreqProtect.h
 * �޸��ˣ��²�
 * �޸����ڣ�2014.10.29
 * ��������ǷƵ����
 * �޸����ݣ�
 * 1.1��--������ȫ����Ϊ�ֲ�������
 *******************************************************/

#ifndef _LIB_GRIDFREQPROTECT_H_
#define _LIB_GRIDFREQPROTECT_H_

#include "Lib_VoltFreq.h"

#ifdef FloatEnable
float sGetFreqLimit(INT8U lubTimeNum);
void sSetFreqLimit(INT8U lubTimeNum,float lfFreq);
INT8U sFreqCheck(float lfGridFreq,INT8U lubError,INT8U lubEnableFlag);
#else
void sSetFreqHigh(INT16U luwFreq1,INT16U luwFreq2);
void sSetFreqLow(INT16U luwFreq1,INT16U luwFreq2);
void sSetFreqBack(INT16U luwFreqLow,INT16U luwFreqHigh);
INT8U sFreqCheck(INT16U luwGridFreq,INT8U lubError,INT8U lubEnableFlag);
#endif

INT16U sGetFreqTime(INT8U lubTimeNum);
void sSetFreqTime(INT8U lubTimeNum,INT16U luwTime);
void sFreqCheckInitial(void);

#endif

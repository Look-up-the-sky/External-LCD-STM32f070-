/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：GridFreqProtect.h
 * 修改人：陈波
 * 修改日期：2014.10.29
 * 描述：过欠频保护
 * 修改内容：
 * 1.1版--将变量全部设为局部变量；
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

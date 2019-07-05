/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Lib_GridVoltProtect.h
 * 修改人：陈波
 * 描述：过欠压保护
 * 初版日期：2015.06.24
 * 修改内容：
 * 1.1版--将变量全部设为局部变量；
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

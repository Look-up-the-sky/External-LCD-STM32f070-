/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Lib_DCCurrentCheck.h
 * 修改人：陈波
 * 修改日期：2014.10.29
 * 描述：并网电流直流分量检测
 * 修改内容：
 * 1.1版--将变量全部设为局部变量；
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

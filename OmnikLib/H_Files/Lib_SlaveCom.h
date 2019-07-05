/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Lib_SlaveCom.h
 * 修改人：陈波
 * 描述：通讯
 * 初版日期：2015.05.29
 * 更改内容：
 * 1.1版--增加波特率设定功能；
 *******************************************************/

#ifndef _LIB_SLAVECOM_H_
#define _LIB_SLAVECOM_H_

INT8U sAnalysisS(INT8U lubComNum);
INT8U sSetDelayTime(void);
void sSetSlaveCom(INT8U lubAddr,INT8U lubComNum);
INT8U sGetSlaveCom(INT8U lubComNum);
INT16U sGetEepromW(void);

#endif

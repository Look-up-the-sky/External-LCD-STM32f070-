/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Lib_PowerDecreaseByVolt.h
 * 修改人：陈波
 * 修改日期：2015.06.24
 * 描述：过压降载
 * 修改内容：
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

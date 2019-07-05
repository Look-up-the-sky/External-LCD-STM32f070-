/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Lib_CrossZeroRelayOn.h
 * 修改人：蒋骊冬
 * 描述：继电器过零吸合
 * 初版日期：2016.06.22
 * 更改内容：
 *******************************************************/

#ifndef _LIB_CROSSZERORELAYON_H_
#define _LIB_CROSSZERORELAYON_H_

void sCrossZeroChk(void);
void sCalCrossZeroRelayOn(float lfGridFreq, INT16U luwRelayAction, INT16S lswRelayDelayOnAdj);
void sActCrossZeroRelayOn(float lfGridFreq, float lfRelayVolt);
void sNRelayDelayOn(INT8U lubEnFlg);
void sNRelayDelayOnAdj(INT8U lubEnFlg);

#endif

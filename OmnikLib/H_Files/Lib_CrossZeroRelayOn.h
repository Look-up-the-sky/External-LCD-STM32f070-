/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�Lib_CrossZeroRelayOn.h
 * �޸��ˣ����궬
 * �������̵�����������
 * �������ڣ�2016.06.22
 * �������ݣ�
 *******************************************************/

#ifndef _LIB_CROSSZERORELAYON_H_
#define _LIB_CROSSZERORELAYON_H_

void sCrossZeroChk(void);
void sCalCrossZeroRelayOn(float lfGridFreq, INT16U luwRelayAction, INT16S lswRelayDelayOnAdj);
void sActCrossZeroRelayOn(float lfGridFreq, float lfRelayVolt);
void sNRelayDelayOn(INT8U lubEnFlg);
void sNRelayDelayOnAdj(INT8U lubEnFlg);

#endif

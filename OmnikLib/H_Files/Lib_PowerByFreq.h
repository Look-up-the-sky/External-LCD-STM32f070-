#ifndef _LIB_POWERBYFREQ_H_
#define _LIB_POWERBYFREQ_H_
typedef union
{
	INT8U all;
	struct
	{
		INT8U ubFreqDecreaseFlg:1;	//��Ƶ���ر�־λ
		INT8U ubPowerBackByFFlg:1;	//��Ƶ���ʻָ���־λ
		INT8U ubPowerFreezeFlg:1;	//���ʶ����־λ
		INT8U ubReserve:5;
	}bit;
}OVERFREQ;
extern OVERFREQ OverFreq;
#endif

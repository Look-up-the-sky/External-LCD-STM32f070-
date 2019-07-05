#ifndef _LIB_POWERBYFREQ_H_
#define _LIB_POWERBYFREQ_H_
typedef union
{
	INT8U all;
	struct
	{
		INT8U ubFreqDecreaseFlg:1;	//过频降载标志位
		INT8U ubPowerBackByFFlg:1;	//过频功率恢复标志位
		INT8U ubPowerFreezeFlg:1;	//功率冻结标志位
		INT8U ubReserve:5;
	}bit;
}OVERFREQ;
extern OVERFREQ OverFreq;
#endif

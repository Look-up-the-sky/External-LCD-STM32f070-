/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�Lib_PowerDecreaseByFreq.c
 * �޸��ˣ��²�
 * �޸����ڣ�2014.10.29
 * ��������Ƶ���ع���
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"
#include "Lib_Communication.h"

/********************************************************************
 * Local Types
 ********************************************************************/
typedef union
{
	INT8U all;
	struct
	{
		INT8U ubFreqDecreaseFlg:1;	//��Ƶ���ر�־λ
		INT8U ubPowerBackByFFlg:1;	//��Ƶ���ʻָ���־λ
		INT8U ubPowerFreezeFlg:1;	//���ʶ����־λ
		INT8U ubFreqDecrease2Flg:1;	//ǷƵ���ر�־λ
		INT8U ubPowerBackByF2Flg:1;	//Ƶ���ʻָ���־λ
		INT8U ubPowerFreeze2Flg:1;	//���ʶ����־λ
		INT8U ubReserve:2;
	}bit;
}OVERFREQ;
OVERFREQ OverFreq;

#ifdef FloatEnable
INT16U luwPowerDelayTime;
INT16U luwPowerDelayCnt;

float lfStartFreq;
float lfDecreaseRatio;
float lfIncreaseRatio;
float lfRecoverFreq;
float lfPowerByFreq;
float lfPowerByFreqPre;
float lfPowerFreeze;
float lfPowerFreeze2;
#else
#endif

INT8U lubFreqDecCnt1[2];
INT8U lubFreqDecCnt2[2];

INT16U luwPowerBackByFreqCnt[2];

/********************************************************************
 * Global Types
 ********************************************************************/


/********************************************************************
 * Extern Global Types
 ********************************************************************/


#ifdef FloatEnable
/*****************************************************************************
 * �������ܣ����ù�Ƶ���ص㡣
 * �������룺��Ƶ���ص㡢��Ƶ���ػָ��㡢�������ʡ��ָ����ʡ��ָ���ʱʱ�䡣
 * ����������
 ****************************************************************************/
void sSetPowerByFreqLimit(float lfStartFreqTemp, float lfRecoverFreqTemp, float lfDecreaseRatioTemp, float lfIncreaseRatioTemp, INT16U luwPowerDelayTimeTemp)
{
	lfStartFreq = lfStartFreqTemp;
	lfDecreaseRatio = lfDecreaseRatioTemp;
	lfIncreaseRatio = lfIncreaseRatioTemp;
	lfRecoverFreq = lfRecoverFreqTemp;
	luwPowerDelayTime = luwPowerDelayTimeTemp;
}

/*****************************************************************************
 * �������ܣ���Ƶ���س�ʼ����
 * �������룺��
 * ����������
 ****************************************************************************/
void sIniPowerByFreq(void)
{
	OverFreq.bit.ubFreqDecreaseFlg = cFalse;
	OverFreq.bit.ubPowerBackByFFlg = cFalse;
	luwPowerDelayCnt = 0;
}

/*****************************************************************************
 * �������ܣ���Ƶ���ء�
 * �������룺����Ƶ�ʡ���ǰ���ʰٷֱȡ�
 * ��������������Ƶ�ʳ�����Ƶ���ص�ʱ����¼��ǰ������Ϊ���Ṧ�ʣ�ͬʱ�������������
 * 			 Ƶ�ʵ����������������½�����Ƶ�ʻָ�����Ƶ���ص�����ʱ�����ʰ���һ����
 * 			 ����б��������
 ****************************************************************************/
float sPowerByFreq(float lfGridFreq, float lfACPower)
{
	if(lfGridFreq > lfStartFreq)
	{
		lubFreqDecCnt2[0] = 0;
		if(++lubFreqDecCnt1[0] >= 3)
		{
			lubFreqDecCnt1[0] = 3;
			OverFreq.bit.ubFreqDecreaseFlg = cTrue;
			OverFreq.bit.ubPowerBackByFFlg = cTrue;
			if(OverFreq.bit.ubPowerFreezeFlg == cFalse)
			{
				lfPowerFreeze = lfACPower;
				lfPowerByFreqPre = lfACPower;
				OverFreq.bit.ubPowerFreezeFlg = cTrue;
				if(lfPowerFreeze > c100Percent)
				{
					lfPowerFreeze = c100Percent;
				}
				lfPowerFreeze2 = lfPowerFreeze;
			}
			luwPowerDelayCnt = 0;
		}
	}
	else if(lfGridFreq < lfStartFreq - c0Hz03)
	{
		lubFreqDecCnt1[0] = 0;
		if(++lubFreqDecCnt2[0] >= 3)
		{
			lubFreqDecCnt2[0] = 3;
			OverFreq.bit.ubFreqDecreaseFlg = cFalse;
			OverFreq.bit.ubPowerFreezeFlg = cFalse;
		}
	}

	if(OverFreq.bit.ubFreqDecreaseFlg)		//��Ƶ���ؽ׶Σ���������Ƶ�����������½���
	{
		if(lfGridFreq > lfRecoverFreq)
		{
			lfPowerByFreq = lfPowerFreeze2 - (lfGridFreq - lfStartFreq) * lfDecreaseRatio * lfPowerFreeze2;
			if(lfPowerByFreq > lfPowerByFreqPre)
			{
				lfPowerByFreq = lfPowerByFreqPre;
			}
			lfPowerByFreqPre = lfPowerByFreq;
			lfPowerFreeze = lfPowerByFreq;
		}
		else
		{
			lfPowerByFreq = lfPowerFreeze - (lfGridFreq - lfStartFreq) * lfDecreaseRatio * lfPowerFreeze;
		}
		
		if(lfPowerByFreq > lfPowerFreeze)
		{
			lfPowerByFreq = lfPowerFreeze;
		}
		else if(lfPowerByFreq < c0Percent)
		{
			lfPowerByFreq = c0Percent;
		}
		
		return(lfPowerByFreq);
	}
	else if(OverFreq.bit.ubPowerBackByFFlg)	//��Ƶ�ָ��׶Σ����ʰ�һ��б������
	{
		if(lfGridFreq < lfRecoverFreq)
		{
			if(++luwPowerDelayCnt >= luwPowerDelayTime)
			{
				luwPowerDelayCnt = luwPowerDelayTime;
				lfPowerByFreq += lfIncreaseRatio / 6000.0f;
				if(lfPowerByFreq > c100Percent)
				{
					lfPowerByFreq = c100Percent;
					OverFreq.bit.ubPowerBackByFFlg = cFalse;
				}
				else if(lfPowerByFreq < c0Percent)
				{
					lfPowerByFreq = c0Percent;
				}
			}
		}
		return(lfPowerByFreq);
	}
	else
	{
		return(c100Percent);
	}
}

/*****************************************************************************
 * �������ܣ�ǷƵ���ء�
 * �������룺����Ƶ�ʡ�ǷƵ���ص㡢�������ʡ�
 * ��������������Ƶ�ʵ���Ƶ���ص�ʱ����¼��ǰ������Ϊ���Ṧ�ʣ�ͬʱ�������������
 * 			 Ƶ�ʵ��½������������½�����Ƶ�ʻָ�����Ƶ���ص�һ��ʱ�����ʰ���һ����
 * 			 ����б��������
 ****************************************************************************/
float sPowerByFreq2(float lfGridFreq, float lfStartFreq, float lfDecreaseRatio, float lfACPower)
{
	float lfPowerLimit;
	
	if(lfGridFreq < lfStartFreq)
	{
		lubFreqDecCnt2[1] = 0;
		if(++lubFreqDecCnt1[1] >= 3)
		{
			lubFreqDecCnt1[1] = 3;
			OverFreq.bit.ubFreqDecrease2Flg = cTrue;
			OverFreq.bit.ubPowerBackByF2Flg = cTrue;
			luwPowerBackByFreqCnt[1] = 0;
			if(OverFreq.bit.ubPowerFreeze2Flg == 0)
			{
				lfPowerFreeze = lfACPower;
				OverFreq.bit.ubPowerFreeze2Flg = 1;
			}
			if(lfPowerFreeze > c100Percent)
			{
				lfPowerFreeze = c100Percent;
			}
		}
	}
	else
	{
		lubFreqDecCnt1[1] = 0;
		if(++lubFreqDecCnt2[1] >= 3)
		{
			lubFreqDecCnt2[1] = 3;
			OverFreq.bit.ubFreqDecrease2Flg = cFalse;
			OverFreq.bit.ubPowerFreeze2Flg = 0;
		}
	}

	if(OverFreq.bit.ubFreqDecrease2Flg)		//ǷƵ���ؽ׶Σ���������Ƶ�����������½���
	{
		lfPowerByFreq = lfPowerFreeze - (lfStartFreq - lfGridFreq) * lfDecreaseRatio * lfPowerFreeze;
		if(lfPowerByFreq > c100Percent)
		{
			lfPowerByFreq = c100Percent;
		}
		else if(lfPowerByFreq < c0Percent)
		{
			lfPowerByFreq = c0Percent;
		}
		return(lfPowerByFreq);
	}
	else if(OverFreq.bit.ubPowerBackByF2Flg)	//��Ƶ�ָ��׶Σ����ʰ�һ��б������
	{
		lfPowerLimit = lfPowerFreeze + luwPowerBackByFreqCnt[1] / 60000.0f;

		if(MasterData.data.ubState == cNormalState)
		{
			luwPowerBackByFreqCnt[1]++;
		}
		else
		{
			luwPowerBackByFreqCnt[1] = 0;
		}
		if(luwPowerBackByFreqCnt[1] >= 60000)
		{
			luwPowerBackByFreqCnt[1] = 0;
			OverFreq.bit.ubPowerBackByF2Flg = cFalse;
		}
		
		if(lfPowerLimit > c100Percent)
		{
			lfPowerLimit = c100Percent;
		}
		else if(lfPowerLimit < c0Percent)
		{
			lfPowerLimit = c0Percent;
		}
		
		return(lfPowerLimit);
	}
	else
	{
		return(c100Percent);
	}
}

#else
#endif

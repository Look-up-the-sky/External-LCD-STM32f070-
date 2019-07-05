/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�DebugSCI.c
 * �޸��ˣ��²�
 * �޸����ڣ�2015.07.15
 * ������DebugͨѶ����
 * 1.1��--δ�õ���PV�͵�����ѹֵҲ�����ݱ��ж�ȡ��
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"
#include "Lib_Communication.h"

#ifdef DebugEnable
/********************************************************************
 * Macros
 ********************************************************************/
#define cMaxDebugRXLength	11
#define cMaxDebugTXLength	207

/********************************************************************
 * Local Types
 ********************************************************************/
INT8U lubDebugRXBuf[cMaxDebugRXLength];
INT8U lubDebugTXBuf[cMaxDebugTXLength];
INT8U lubDebugTXCnt;
INT8U gubDebugTXLength;
INT8U lubDebugRXCnt;

/********************************************************************
 * Extern Global Types
 ********************************************************************/
extern float gfACPowerCoef;
extern float gfGridVoltCoef;
extern float gfGridCurrCoef;
extern float gfPVVoltCoef;
extern float gfBusVoltCoef;

/********************************************************************
 * Functions
 *******************************************************************/
INT16U sCheckSum(INT8U* lubBufferAdr,INT8U lubLength);

/*****************************************************************************
 * �������ƣ�Debug���ݽ��յ�ַ��λ����
 * �������룺��
 * ��������ֵ����
 * ����������
 ****************************************************************************/
void sClearDebugRXCnt(void)
{
	lubDebugRXCnt = 0;
}

/*****************************************************************************
 * �������ƣ�Debug�������ݷ�������
 * �������룺��
 * ��������ֵ��������Ч��־λ��cTrue--��Ч��cFalse--��Ч
 * ����������
 ****************************************************************************/
INT8U sAnalysisDebug(void)
{
	INT8U i,j;
	INT16U luwAddr;
	INT16U luwCheckSum;
	
	if(lubDebugRXBuf[0] != 0xAA || lubDebugRXBuf[1] != 0x55)
	{
		return(cFalse);
	}
	
	for(i = 0;i < 4;i++)
	{
		lubDebugTXBuf[i] = lubDebugRXBuf[i];
	}
	
	if(lubDebugRXBuf[2] == 0xA0)
	{
		if(lubDebugRXBuf[3] == 0x01 && lubDebugRXBuf[4] == 0x00)
		{
			luwCheckSum = lubDebugRXBuf[5];
			luwCheckSum <<= 8;
			luwCheckSum += lubDebugRXBuf[6];
			if(luwCheckSum == sCheckSum(lubDebugRXBuf,lubDebugRXBuf[4] + 5))
			{
				j = 4;
				lubDebugTXBuf[j++] = lubDebugRXBuf[4];
			}
			else
			{
				return(cFalse);
			}
		}
		else
		{
			return(cFalse);
		}
	}
	else if(lubDebugRXBuf[2] == 0xA1)
	{
		if(lubDebugRXBuf[3] == 0x01 && lubDebugRXBuf[4] == 0x00)
		{
			luwCheckSum = lubDebugRXBuf[5];
			luwCheckSum <<= 8;
			luwCheckSum += lubDebugRXBuf[6];
			if(luwCheckSum == sCheckSum(lubDebugRXBuf,lubDebugRXBuf[4] + 5))
			{
				j = 4;
				lubDebugTXBuf[j++] = 0x32;
				lubDebugTXBuf[j++] = MasterData.data.uwReconnectTime >> 8;
				lubDebugTXBuf[j++] = MasterData.data.uwReconnectTime;
				lubDebugTXBuf[j++] = MasterData.data.uwPVVolt[cPVIn1] >> 8;
				lubDebugTXBuf[j++] = MasterData.data.uwPVVolt[cPVIn1];
				lubDebugTXBuf[j++] = MasterData.data.uwPVVolt[cPVIn2] >> 8;
				lubDebugTXBuf[j++] = MasterData.data.uwPVVolt[cPVIn2];
				lubDebugTXBuf[j++] = MasterData.data.uwPVVolt[cPVIn3] >> 8;
				lubDebugTXBuf[j++] = MasterData.data.uwPVVolt[cPVIn3];
				lubDebugTXBuf[j++] = MasterData.data.uwPVVolt[cPVIn4] >> 8;
				lubDebugTXBuf[j++] = MasterData.data.uwPVVolt[cPVIn4];

				lubDebugTXBuf[j++] = MasterData.data.uwPVCurr[cPVIn1] >> 8;
				lubDebugTXBuf[j++] = MasterData.data.uwPVCurr[cPVIn1];
				for(i = 0;i < 6;i++)
				{
					lubDebugTXBuf[j++] = 0;
				}
				lubDebugTXBuf[j++] = MasterData.data.uwGridVolt[cPhaseR] >> 8;
				lubDebugTXBuf[j++] = MasterData.data.uwGridVolt[cPhaseR];
				lubDebugTXBuf[j++] = MasterData.data.uwGridVolt[cPhaseS] >> 8;
				lubDebugTXBuf[j++] = MasterData.data.uwGridVolt[cPhaseS];
				lubDebugTXBuf[j++] = MasterData.data.uwGridVolt[cPhaseT] >> 8;
				lubDebugTXBuf[j++] = MasterData.data.uwGridVolt[cPhaseT];
				
				lubDebugTXBuf[j++] = MasterData.data.uwGridCurr[cPhaseR] >> 8;
				lubDebugTXBuf[j++] = MasterData.data.uwGridCurr[cPhaseR];
				for(i = 0;i < 4;i++)
				{
					lubDebugTXBuf[j++] = 0;
				}
				lubDebugTXBuf[j++] = MasterData.data.uwBusVolt >> 8;
				lubDebugTXBuf[j++] = MasterData.data.uwBusVolt;
				lubDebugTXBuf[j++] = 0;
				lubDebugTXBuf[j++] = 0;
				lubDebugTXBuf[j++] = MasterData.data.swDCCurr[cPhaseR] >> 8;
				lubDebugTXBuf[j++] = MasterData.data.swDCCurr[cPhaseR];
				lubDebugTXBuf[j++] = MasterData.data.swDCCurr[cPhaseS] >> 8;
				lubDebugTXBuf[j++] = MasterData.data.swDCCurr[cPhaseS];
				lubDebugTXBuf[j++] = MasterData.data.swDCCurr[cPhaseT] >> 8;
				lubDebugTXBuf[j++] = MasterData.data.swDCCurr[cPhaseT];
				lubDebugTXBuf[j++] = MasterData.data.uwGroundCurr >> 8;
				lubDebugTXBuf[j++] = MasterData.data.uwGroundCurr;
				lubDebugTXBuf[j++] = MasterData.data.swGroundCurr >> 8;
				lubDebugTXBuf[j++] = MasterData.data.swGroundCurr;
				lubDebugTXBuf[j++] = MasterData.data.uwGridFreq >> 8;
				lubDebugTXBuf[j++] = MasterData.data.uwGridFreq;
				lubDebugTXBuf[j++] = MasterData.data.error.all >> 24;
				lubDebugTXBuf[j++] = MasterData.data.error.all >> 16;
				lubDebugTXBuf[j++] = MasterData.data.error.all >> 8;
				lubDebugTXBuf[j++] = MasterData.data.error.all;
			}
			else
			{
				return(cFalse);
			}
		}
		else
		{
			return(cFalse);
		}
	}
	else if(lubDebugRXBuf[2] == 0xA2)
	{
		if(lubDebugRXBuf[3] == 0x00 && lubDebugRXBuf[4] == 0x04)
		{
			luwCheckSum = lubDebugRXBuf[9];
			luwCheckSum <<= 8;
			luwCheckSum += lubDebugRXBuf[10];
			if(luwCheckSum == sCheckSum(lubDebugRXBuf,lubDebugRXBuf[4] + 5))
			{
				for(i = 0;i < 4;i++)
				{
					DebugValue.data.ubDebugDataType[i] = lubDebugRXBuf[i + 5];
					if(DebugValue.data.ubDebugDataType[i] >= 20)
					{
						DebugValue.data.ubDebugDataType[i] = i;
					}
				}
				DebugValue.data.ubDebugFlag.bit.ub50usStart = 1;
				
				lubDebugTXBuf[4] = 0x01;
				lubDebugTXBuf[5] = 0x00;
				j = 6;
			}
			else
			{
				return(cFalse);
			}
		}
		else if(lubDebugRXBuf[3] <= 0x14 && lubDebugRXBuf[4] == 0x00)
		{
			luwCheckSum = lubDebugRXBuf[5];
			luwCheckSum <<= 8;
			luwCheckSum += lubDebugRXBuf[6];
			if(luwCheckSum == sCheckSum(lubDebugRXBuf,lubDebugRXBuf[4] + 5))
			{
				if(DebugValue.data.ubDebugFlag.bit.ub50usStart)
				{
					return(cFalse);
				}
				else
				{
					j = 4;
					lubDebugTXBuf[j++] = 0xC8;
					luwAddr = lubDebugRXBuf[3] - 1;
					luwAddr *= 50;
					for(i = 0;i < 25;i++)
					{
						lubDebugTXBuf[j++] = DebugValue.data.ubDebugBuf[0].all[luwAddr + 2 * i + 1];
						lubDebugTXBuf[j++] = DebugValue.data.ubDebugBuf[0].all[luwAddr + 2 * i];
						lubDebugTXBuf[j++] = DebugValue.data.ubDebugBuf[1].all[luwAddr + 2 * i + 1];
						lubDebugTXBuf[j++] = DebugValue.data.ubDebugBuf[1].all[luwAddr + 2 * i];
						lubDebugTXBuf[j++] = DebugValue.data.ubDebugBuf[2].all[luwAddr + 2 * i + 1];
						lubDebugTXBuf[j++] = DebugValue.data.ubDebugBuf[2].all[luwAddr + 2 * i];
						lubDebugTXBuf[j++] = DebugValue.data.ubDebugBuf[3].all[luwAddr + 2 * i + 1];
						lubDebugTXBuf[j++] = DebugValue.data.ubDebugBuf[3].all[luwAddr + 2 * i];
					}
				}
			}
			else
			{
				return(cFalse);
			}
		}
		else
		{
			return(cFalse);
		}
	}
	else if(lubDebugRXBuf[2] == 0xA3)
	{
		if(lubDebugRXBuf[3] == 0x01 && lubDebugRXBuf[4] == 0x00)
		{
			luwCheckSum = lubDebugRXBuf[5];
			luwCheckSum <<= 8;
			luwCheckSum += lubDebugRXBuf[6];
			if(luwCheckSum == sCheckSum(lubDebugRXBuf,lubDebugRXBuf[4] + 5))
			{
				j = 4;
				lubDebugTXBuf[j++] = 0x24;
				
				luwCheckSum = (INT16U)(gfACPowerCoef * 10000);
				lubDebugTXBuf[j++] = luwCheckSum >> 8;
				lubDebugTXBuf[j++] = luwCheckSum;
				for(i = 0;i < 4;i++)
				{
					lubDebugTXBuf[j++] = 0;
				}
				luwCheckSum = (INT16U)(gfGridVoltCoef * 10000);
				lubDebugTXBuf[j++] = luwCheckSum >> 8;
				lubDebugTXBuf[j++] = luwCheckSum;
				for(i = 0;i < 4;i++)
				{
					lubDebugTXBuf[j++] = 0;
				}
				luwCheckSum = (INT16U)(gfGridCurrCoef * 10000);
				lubDebugTXBuf[j++] = luwCheckSum >> 8;
				lubDebugTXBuf[j++] = luwCheckSum;
				for(i = 0;i < 4;i++)
				{
					lubDebugTXBuf[j++] = 0;
				}
				luwCheckSum = (INT16U)(gfPVVoltCoef * 10000);
				lubDebugTXBuf[j++] = luwCheckSum >> 8;
				lubDebugTXBuf[j++] = luwCheckSum;
				for(i = 0;i < 14;i++)
				{
					lubDebugTXBuf[j++] = 0;
				}
				luwCheckSum = (INT16U)(gfBusVoltCoef * 10000);
				lubDebugTXBuf[j++] = luwCheckSum >> 8;
				lubDebugTXBuf[j++] = luwCheckSum;
			}
			else
			{
				return(cFalse);
			}
		}
		else
		{
			return(cFalse);
		}
	}
	else if(lubDebugRXBuf[2] == 0xA4)
	{
		if(lubDebugRXBuf[3] < 0x24 && lubDebugRXBuf[4] == 0x02)
		{
			luwCheckSum = lubDebugRXBuf[7];
			luwCheckSum <<= 8;
			luwCheckSum += lubDebugRXBuf[8];
			if(luwCheckSum == sCheckSum(lubDebugRXBuf,lubDebugRXBuf[4] + 5))
			{
				luwCheckSum = lubDebugRXBuf[5];
				luwCheckSum <<= 8;
				luwCheckSum += lubDebugRXBuf[6];
				if(lubDebugRXBuf[3] == 0)
				{
					gfACPowerCoef = (float)luwCheckSum / 10000;
				}
				else if(lubDebugRXBuf[3] == 3)
				{
					gfGridVoltCoef = (float)luwCheckSum / 10000;
				}
				else if(lubDebugRXBuf[3] == 6)
				{
					gfGridCurrCoef = (float)luwCheckSum / 10000;
				}
				else if(lubDebugRXBuf[3] == 7)
				{
					gfPVVoltCoef = (float)luwCheckSum / 10000;
				}
				else if(lubDebugRXBuf[3] == 17)
				{
					gfBusVoltCoef = (float)luwCheckSum / 10000;
				}
				
				j = 4;
				lubDebugTXBuf[j++] = 0x01;
				lubDebugTXBuf[j++] = 0x00;
			}
			else
			{
				return(cFalse);
			}
		}
		else
		{
			return(cFalse);
		}
	}
	else
	{
		return(cFalse);
	}
	
	luwCheckSum = sCheckSum(lubDebugTXBuf,lubDebugTXBuf[4] + 5);
	lubDebugTXBuf[j++] = luwCheckSum >> 8;
	lubDebugTXBuf[j++] = luwCheckSum;
	if(j > cMaxDebugTXLength)
	{
		gubDebugTXLength = cMaxDebugTXLength;
	}
	else
	{
		gubDebugTXLength = j;
	}
	return(cTrue);
}

/*****************************************************************************
 * �������ƣ�Debug�������������⺯��
 * �������룺��
 * ��������ֵ�������־λ��cTrue--����cFalse--������
 * ����������
 ****************************************************************************/
INT8U sTXOKCheck(void)
{
	return(1);
}

/*****************************************************************************
 * �������ƣ�Debug�����ж����ú�����
 * �������룺�ޡ�
 * ��������ֵ����8λΪ������ɱ�־λ����8λΪ���������ݡ�
 * ����������
 ****************************************************************************/
INT16U sGetDebugTXBufData(void)
{
	INT8U lubValue;
	
	if(lubDebugTXCnt >= gubDebugTXLength)
	{
		lubDebugTXCnt = 0;
		return(0x100);
	}
	else
	{
		lubValue = lubDebugTXBuf[lubDebugTXCnt];
		lubDebugTXCnt++;
		return(lubValue);
	}
}

/*****************************************************************************
 * �������ƣ�Debug�����ж����ú�����
 * �������룺���յ������ݡ�
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
INT8U sSetDebugRXBufData(INT8U lubData)
{
	if(lubDebugRXCnt < cMaxDebugRXLength)
	{
		lubDebugRXBuf[lubDebugRXCnt] = lubData;
	}

	if(++lubDebugRXCnt >= cMaxRXLength)
	{
		lubDebugRXCnt = 0;
		return(cTrue);
	}

	return(cFalse);
}

/*****************************************************************************
 * �������ƣ�У��ͼ��㺯����
 * �������룺���ݵ�ַ�����ݳ��ȡ�
 * ��������ֵ��У��͡�
 * ����������
 ****************************************************************************/
INT16U sCheckSum(INT8U* lubBufferAdr,INT8U lubLength)
{
	INT8U i;
	INT16U luwCheckSum;
	
	luwCheckSum = 0;
	for(i = 2;i < lubLength;i++)
	{
		luwCheckSum += lubBufferAdr[i];
	}
	return(luwCheckSum);
}
#endif

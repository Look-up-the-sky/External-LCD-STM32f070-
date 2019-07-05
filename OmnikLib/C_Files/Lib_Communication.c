/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�Lib_Communication.c
 * �޸��ˣ�����ǿ
 * ������ͨѶ
 * �������ڣ�2015.05.29
 * �������ݣ�
 * 1.1��--�����й��ٷֱȳ�ʼ�����ܣ�
 * 1.2��--����ͨѶMCU���ݣ�
 * 1.3��--����debug����ʹ�ܣ�
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"
#include "Lib_Communication.h"

/********************************************************************
 * Const
 ********************************************************************/
const INT16U cMaxLength[cMaxBlockNum] =
{
	cCoefSize,cSaftySize,cInfoSize,cEnergySize,cErrorRecordSize,cSlaveDataSize,cPowerCtrlSize,cMasterDataSize,
	cPowerMeterSize,cSystemTimeSize,cPQDDataSize,cDebugSize,cComSize
};

/********************************************************************
 * Local Types
 ********************************************************************/
INT8U lubTXCnt[cComNum];
INT8U lubRXCnt[cComNum];

/********************************************************************
 * Global Types
 ********************************************************************/
INT8U *gubBlockPtr[cMaxBlockNum];
INT8U gubTXLength[cComNum];
INT8U lubTXBuf[cComNum][cMaxTXLength];
INT8U lubRXBuf[cComNum][cMaxRXLength];

COEF Coef;
SAFTY Safty;
INFO Info;
ENERGY Energy;
ERRORRECORD ErrorRecord;
SLAVEDATA SlaveData;
POWERCTRL PowerCtrl;
MASTERDATA MasterData;
POWERMETER PowerMeter;
SYSTEMTIME SystemTime;
PQDDATA PQDData;
#ifdef DebugEnable
DEBUGVALUE DebugValue;
#endif
COMDATA ComData;

/*****************************************************************************
 * �������ƣ���ʼ�������ַ������
 * �������룺�ޡ�
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
void sInitComData(void)
{
	gubBlockPtr[0] = Coef.all;
	gubBlockPtr[1] = Safty.all;
	gubBlockPtr[2] = Info.all;
	gubBlockPtr[3] = Energy.all;
	gubBlockPtr[4] = ErrorRecord.all;
	gubBlockPtr[5] = SlaveData.all;
	gubBlockPtr[6] = PowerCtrl.all;
	gubBlockPtr[7] = MasterData.all;
	gubBlockPtr[8] = PowerMeter.all;
	gubBlockPtr[9] = SystemTime.all;
	gubBlockPtr[10] = PQDData.all;
#ifdef DebugEnable
	gubBlockPtr[11] = DebugValue.all;
#else
	gubBlockPtr[11] = 0;
#endif
	gubBlockPtr[12] = ComData.all;
	
	PowerCtrl.data.ubPowerPercent = 100;
	PowerCtrl.data.ubCtrlType = 4;
	PowerCtrl.data.sbXPercent[0] = 0;
	PowerCtrl.data.sbXPercent[1] = 0;
	PowerCtrl.data.sbXPercent[2] = 0;
	PowerCtrl.data.sbXPercent[3] = 0;
	PowerCtrl.data.sbYPercent[0] = 0;
	PowerCtrl.data.sbYPercent[1] = 0;
	PowerCtrl.data.sbYPercent[2] = 0;
	PowerCtrl.data.sbYPercent[3] = 0;
}

/*****************************************************************************
 * �������ƣ������ж����ú�����
 * �������룺ͨѶ�ںš�
 * ��������ֵ����8λΪ������ɱ�־λ����8λΪ���������ݡ�
 * ����������
 ****************************************************************************/
INT16U sGetTXBufData(INT8U lubComNum)
{
	INT8U lubValue;

	lubComNum %= cComNum;

	if(lubTXCnt[lubComNum] >= gubTXLength[lubComNum])
	{
		lubTXCnt[lubComNum] = 0;
		return(0x100);
	}
	else
	{
		lubValue = lubTXBuf[lubComNum][lubTXCnt[lubComNum]];
		lubTXCnt[lubComNum]++;
		return(lubValue);
	}
}

/*****************************************************************************
 * �������ƣ������ж����ú�����
 * �������룺ͨѶ�ںţ����յ������ݡ�
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
INT8U sSetRXBufData(INT8U lubComNum,INT8U lubData)
{
	lubComNum %= cComNum;
	if(lubRXCnt[lubComNum] < cMaxRXLength)
	{
		lubRXBuf[lubComNum][lubRXCnt[lubComNum]] = lubData;
	}

	if(++lubRXCnt[lubComNum] >= cMaxRXLength)
	{
		lubRXCnt[lubComNum] = 0;
		return(cTrue);
	}

	return(cFalse);
}

/*****************************************************************************
 * �������ƣ����ݽ��յ�ַ��λ������
 * �������룺ͨѶ�ںš�
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
void sClearRXCnt(INT8U lubComNum)
{
	lubComNum %= cComNum;
	lubRXCnt[lubComNum] = 0;
}

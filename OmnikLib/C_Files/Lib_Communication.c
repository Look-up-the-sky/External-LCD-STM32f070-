/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Lib_Communication.c
 * 修改人：沈仕强
 * 描述：通讯
 * 初版日期：2015.05.29
 * 更改内容：
 * 1.1版--增加有功百分比初始化功能；
 * 1.2版--增加通讯MCU数据；
 * 1.3版--增加debug数据使能；
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
 * 函数名称：初始化区块地址函数。
 * 函数输入：无。
 * 函数返回值：无。
 * 函数描述：
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
 * 函数名称：发送中断设置函数。
 * 函数输入：通讯口号。
 * 函数返回值：高8位为发送完成标志位，低8位为待发送数据。
 * 函数描述：
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
 * 函数名称：接收中断设置函数。
 * 函数输入：通讯口号，接收到的数据。
 * 函数返回值：无。
 * 函数描述：
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
 * 函数名称：数据接收地址复位函数。
 * 函数输入：通讯口号。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sClearRXCnt(INT8U lubComNum)
{
	lubComNum %= cComNum;
	lubRXCnt[lubComNum] = 0;
}

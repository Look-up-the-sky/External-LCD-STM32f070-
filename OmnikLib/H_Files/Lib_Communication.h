/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�Lib_Communication.h
 * �޸��ˣ�����ǿ
 * ������ͨѶ
 * �������ڣ�2015.05.29
 * �������ݣ�
 * 1.1��--���Ӳ������趨���ܣ�
 * 1.2��--��debug���ܵĻ������ݸ�Ϊ�з�������
 * 1.3��--�����ջ��泤����Ϊ211���ֽڣ�������Info���ݣ�
 * 1.4��--����ͨѶMCU���ݣ�
 * 1.5��--����10���ӹ�ѹֵ������������ֵ�Լ�CEI 0-21��P(f)��Q(v)��ʱʱ�䣻
 * 1.6��--����debug����ʹ�ܣ�
 * 1.7��--����ST����ż���ֽڶ��룬���Խ�DEBUGFLAG�ĳ��ȸ�Ϊ2���ֽڣ�
 * 1.8��--ͨѶ���ݳ��ȸ�Ϊż�������������ȷ���8λ���󷢸�8λ��
 * 1.9��--�������������ͱ�����
 *******************************************************/

#ifndef _LIB_COMMUNICATION_H_
#define _LIB_COMMUNICATION_H_
#include "TypeDefine.h"
#include "App.h"
/********************************************************************
 * Macros
 ********************************************************************/
#define cMSSCICom	0
#ifndef MonitorEnable
#define cComNum		1			//��Ҫ���ģ�����ʹ�ô����趨
#else
#define cMonitorCom	1
#define cComNum		2
#endif

#define cMaxTXLength	212		//��Ҫ���ģ�����ָ��ȷ��
#define cMaxRXLength	172		//��Ҫ���ģ�����ָ��ȷ��

#define cHeadH		0
#define cHeadL		1
#define cMasterP	2
#define cSlaveP		3
#define cBlock		4
#define cFncCode	5
#define cStartPL	6
#define cStartPH	7
#define cLengthL	8
#define cLengthH	9
#define cDataP		10

#define cComWting	0
#define cComTXing	1
#define cComTXed	2
#define cComRXing	3
#define cComRXed	4

#define cHeadHData	0x59
#define cHeadLData	0x5A
#define cMasterAddr	0x00

#define cReadCmd		0x01
#define cWriteCmd		0x02
#define cResetCmd		0x03
#define cPrtctReadCmd	0x11
#define cPrtctWriteCmd	0x12
#define cPrtctResetCmd	0x13
#define cSetBDRCmd		0x7C
#define cRegisterCmd	0x7D
#define cSendAddrCmd	0x7E
#define cDeleteAddrCmd	0x7F

#define cACK	0x06
#define cNAK	0x15

#define cMaxSlaveAddr	16

#define cCoefBlock			0
#define cSaftyBlock			1
#define cInfoBlock			2
#define cEnergyBlock		3
#define cErrorRecordBlock	4
#define cSlaveDataBlock		5
#define cPowerCtrlBlock		6
#define cMasterDataBlock	7
#define cPowerMeterBlock	8
#define cSystemTimeBlock	9
#define cPQDDataBlock		10
#define cDebugBlock			11
#define cComDataBlock		12
#define cMaxBlockNum		13

#define cDebugChannelNum	4
#define cMaxDebugDataNum	500

#define cCoefSize			80
#define cSaftySize			96
#define cInfoSize			64
#define cEnergySize			16
#define cErrorRecordSize	160
#define cSlaveDataSize		34
#define cPowerCtrlSize		10
#define cMasterDataSize		92
#define cPowerMeterSize		34
#define cSystemTimeSize		6
#define cPQDDataSize		18
#ifdef DebugEnable
#define cDebugSize			((2 * cMaxDebugDataNum + 1) * 4 + 2)
#else
#define cDebugSize			0
#endif
#define cComSize			46

#define c9600Baudrate	0
#define c115200Baudrate	1

typedef union
{
	INT8U all;
	struct
	{
		INT8U ubStep:3;
		INT8U ubReserve:5;
	}bit;
}COMMUNICATIONFLAG;

typedef	union
{
	INT8U Byte[10];
	struct
    {
   		INT8U Minute;
   		INT8U Second;
   		INT8U Date;
   		INT8U Hour;
   		INT8U Year;
   		INT8U Month;
      INT16U ErrorCodeH;
      INT16U ErrorCodeL;
    }data;
}ERRORMESSAGE;

typedef union
{
	INT16U Word;							//ż���ֽڶ���
	struct
	{
		INT16U ub50usStart:1;
		INT16U ub50usReadEnable:1;
		INT16U ubReserve:14;
	}bit;
}DEBUGFLAG;

typedef union
{
	INT16U Word;							//ż���ֽڶ���
	struct
	{
		INT16U ubFreqHigh:1;
		INT16U ubFreqLow:1;
		INT16U ubVoltHigh:1;
		INT16U ubVoltLow:1;
		INT16U ubReserve:12;
	}bit;
}SOFTTYPE;

typedef union
{
	INT8U all[18];
	struct
	{
		/*****************bit0~bit7:��Ե����*****************/
		INT16U ubPV1P2GFault:1;				//PV1+�Եؾ�Ե����
		INT16U ubPV2P2GFault:1;				//PV2+�Եؾ�Ե����
		INT16U ubPV3P2GFault:1;				//PV3+�Եؾ�Ե����
		INT16U ubPV4P2GFault:1;				//PV4+�Եؾ�Ե����
		INT16U ubPV1N2GFault:1;				//PV1-�Եؾ�Ե����
		INT16U ubPV2N2GFault:1;				//PV1-�Եؾ�Ե����
		INT16U ubPV3N2GFault:1;				//PV1-�Եؾ�Ե����
		INT16U ubPV4N2GFault:1;				//PV1-�Եؾ�Ե����
		
		/*****************bit8~bit23:PV����*****************/
		INT16U ubPV1OverCurr_Hardware:1;	//PV1����Ӳ������
		INT16U ubPV2OverCurr_Hardware:1;	//PV2����Ӳ������
		INT16U ubPV3OverCurr_Hardware:1;	//PV3����Ӳ������
		INT16U ubPV4OverCurr_Hardware:1;	//PV4����Ӳ������
		INT16U ubPVPowerLow:1;				//���빦�ʵ͹���
		INT16U ubPVVoltLow:1;				//�����ѹ�͹���
		INT16U ubPVVoltHigh:1;				//�����ѹ�߹���
		INT16U ubReservePVBus:9;			//Ԥ��PV��Bus����
		
		/*****************bit24~bit31:Bus����*****************/
		INT16U ubBusPVoltHigh:1;			//��Bus��ѹ�߹���
		INT16U ubBusNVoltHigh:1;			//��Bus��ѹ�߹���
		INT16U ubBusPVoltHighAD:1;			//��Bus������ѹ�߹���
		INT16U ubBusNVoltHighAD:1;			//��Bus������ѹ�߹���
		INT16U ubBusPVoltHigh_Hardware:1;	//��Bus��ѹ��Ӳ������
		INT16U ubBusNVoltHigh_Hardware:1;	//��Bus��ѹ��Ӳ������
		INT16U ubBusUnbalance:1;			//Bus��ѹ��ƽ�����
		INT16U ubBusUnbalanceAD:1;			//Bus������ѹ��ƽ�����
		
		/*****************bit32~bit63:��������*****************/
		INT16U ubGridRVoltHigh1:1;			//R���ѹ����ֵ1����
		INT16U ubGridRVoltHigh2:1;			//R���ѹ����ֵ2����
		INT16U ubGridRVoltLow1:1;			//R���ѹ����ֵ1����
		INT16U ubGridRVoltLow2:1;			//R���ѹ����ֵ2����
		INT16U ubGridSVoltHigh1:1;			//S���ѹ����ֵ1����
		INT16U ubGridSVoltHigh2:1;			//S���ѹ����ֵ2����
		INT16U ubGridSVoltLow1:1;			//S���ѹ����ֵ1����
		INT16U ubGridSVoltLow2:1;			//S���ѹ����ֵ2����
		INT16U ubGridTVoltHigh1:1;			//T���ѹ����ֵ2����
		INT16U ubGridTVoltHigh2:1;			//T���ѹ����ֵ2����
		INT16U ubGridTVoltLow1:1;			//T���ѹ����ֵ1����
		INT16U ubGridTVoltLow2:1;			//T���ѹ����ֵ2����
		INT16U ubGritFreqHigh1:1;			//����Ƶ�ʸ���ֵ1����
		INT16U ubGridFreqHigh2:1;			//����Ƶ�ʸ���ֵ2����
		INT16U ubGridFreqLow1:1;			//����Ƶ�ʵ���ֵ1����
		INT16U ubGridFreqLow2:1;			//����Ƶ�ʵ���ֵ2����
		INT16U ubGridRVoltHighAD:1;			//R�������ѹ�߹���
		INT16U ubGridSVoltHighAD:1;			//S�������ѹ�߹���
		INT16U ubGridTVoltHighAD:1;			//T�������ѹ�߹���
		INT16U ubGridRVolt10minFault:1;		//R��10min��ѹ�߹���
		INT16U ubGridSVolt10minFault:1;		//S��10min��ѹ�߹���
		INT16U ubGridTVolt10minFault:1;		//T��10min��ѹ�߹���
		INT16U ubNoUtility:1;				//���е����
		INT16U ubIslandFault:1;				//�µ�����
		INT16U ubReserveGrid:8;				//Ԥ����������
		
		/*****************bit64~bit71:ֱ����������*****************/
		INT16U ubDCIRHigh1:1;				//R��ֱ��������ֵ1����
		INT16U ubDCIRHigh2:1;				//R��ֱ��������ֵ2����
		INT16U ubDCISHigh1:1;				//S��ֱ��������ֵ1����
		INT16U ubDCISHigh2:1;				//S��ֱ��������ֵ2����
		INT16U ubDCITHigh1:1;				//T��ֱ��������ֵ1����
		INT16U ubDCITHigh2:1;				//T��ֱ��������ֵ2����
		INT16U ubReserveDCI:2;
		
		/*****************bit72~bit79:©��������*****************/
		INT16U ubGroundCurr30mAFault:1;		//˲̬30mA©��������
		INT16U ubGroundCurr60mAFault:1;		//˲̬60mA©��������
		INT16U ubGroundCurr150mAFault:1;	//˲̬150mA©��������
		INT16U ubGroundCurr300mAFault:1;	//��̬300mA©��������
		INT16U ubGFCIDevive0mAFault:1;		//0mA©�����豸����
		INT16U ubGFCIDevive50mAFault:1;		//50mA©�����豸����
		INT16U ubReserveGFCI:2;				//Ԥ��©��������
		
		/*****************bit80~bit95:�̵�������*****************/
		INT16U ubMRRlyOnFault:1;			//��R��̵�����բ����
		INT16U ubMSRlyOnFault:1;			//��S��̵�����բ����
		INT16U ubMTRlyOnFault:1;			//��T��̵�����բ����
		INT16U ubMNRlyOnFault:1;			//��N��̵�����բ����
		INT16U ubSRRlyOnFault:1;			//��R��̵�����բ����
		INT16U ubSSRlyOnFault:1;			//��S��̵�����բ����
		INT16U ubSTRlyOnFault:1;			//��T��̵�����բ����
		INT16U ubSNRlyOnFault:1;			//��N��̵�����բ����
		INT16U ubMRRlyOffFault:1;			//��R��̵�����բ����
		INT16U ubMSRlyOffFault:1;			//��S��̵�����բ����
		INT16U ubMTRlyOffFault:1;			//��T��̵�����բ����
		INT16U ubMNRlyOffFault:1;			//��N��̵�����բ����
		INT16U ubSRRlyOffFault:1;			//��R��̵�����բ����
		INT16U ubSSRlyOffFault:1;			//��S��̵�����բ����
		INT16U ubSTRlyOffFault:1;			//��T��̵�����բ����
		INT16U ubSNRlyOffFault:1;			//��N��̵�����բ����
		
		/*****************bit96~bit111:�̵�������*****************/
		INT16U ubInnerOverT:1;				//ǻ����¹���
		INT16U ubInvROverT:1;				//R����¹���
		INT16U ubInvSOverT:1;				//S����¹���
		INT16U ubInvTOverT:1;				//T����¹���
		INT16U ubPV1OverT:1;				//PV1���¹���
		INT16U ubPV2OverT:1;				//PV2���¹���
		INT16U ubPV3OverT:1;				//PV3���¹���
		INT16U ubPV4OverT:1;				//PV4���¹���
		INT16U ubSinkOverT:1;				//ɢ�������¹���
		INT16U ubReserveT:7;				//Ԥ�����¹���
		
		/*****************bit112~bit119:�̵�������*****************/
		INT16U ubInnerFanFault:1;			//�ڲ����ȹ���
		INT16U ubOutFanFault:1;				//�ⲿ���ȹ���
		INT16U ubReserveFan:6;				//Ԥ�����ȹ���
		
		/*****************bit120~bit135:�Լ��һ���Թ���*****************/
		INT16U ubAutoTestfail:8;			//�Լ����
		INT16U ubConsistencyFalut:8;		//һ���Թ���
		
		/*****************bit136~bit143:��������*****************/
		INT16U ubReserve:8;					//Ԥ����������
	}bit;
}OMNIKFAULT;

typedef union
{
	INT8U all[2 * cMaxDebugDataNum];
	struct
	{
		INT16S swValue[cMaxDebugDataNum];
	}data;
}DEBUGBUF;

typedef union
{
	INT8U all[cCoefSize];
	struct
	{
		INT16U uwBusVoltCoef;
		INT16U uwGridVoltCoef[3];
		INT16U uwPVVoltCoef[4];
		INT16U uwGridCurrCoef[3];
		INT16U uwPVCurrCoef[4];
		INT16U uwWattCoef[3];
		INT16U uwInvVoltCoef[3];
		INT16U uwBusNVoltCoef;
		INT8U ubReserve[10];
		INT16U uwBusVoltCoefSlave;
		INT16U uwGridVoltCoefSlave[3];
		INT16U uwGridCurrCoefSlave[3];
		INT16U uwInvVoltCoefSlave[3];
		INT8U ubReserveSlave[4];
		INT16U uwCRC;
	}data;
}COEF;
extern COEF Coef;

typedef	union
{
	INT16U all;
	struct
    {
   	INT16U ubISO:1;
		INT16U ubGFCI:1;
		INT16U ubDCI:1;
		INT16U ub10minOverVolt:1;
		INT16U ubPByF:1;
		INT16U ubLVRT:1;
		INT16U ubIsland:1;
		INT16U ub60Hz:1;
		
		INT16U ubSoftStart:1;
		INT16U ubReserve1:1;
		INT16U ubMPPTScan:1;
		INT16U ubPByV:1;
		INT16U ubReserve2:4;
    }bit;
}PROTECTION;

typedef union
{
	INT8U all[cSaftySize];
	struct
	{
		INT16U uwCountry;
		INT16U uwStartDelayTime;
		INT16U uwGridVoltHigh[2];
		INT16U uwGridVoltHighCnt[2];
		INT16U uwGridVoltLow[2];
		INT16U uwGridVoltLowCnt[2];
		INT16U uwGridVoltBackHigh;
		INT16U uwGridVoltBackLow;
		INT16U uwGridVoltBackCnt;
		INT16U uwFreqHigh[2];
		INT16U uwFreqHighCnt[2];
		INT16U uwFreqLow[2];
		INT16U uwFreqLowCnt[2];
		INT16U uwFreqBackHigh;
		INT16U uwFreqBackLow;
		INT16U uwFreqBackCnt;
		INT16S swDCILimit[2];
		INT16S swDCICnt[2];
		INT16S swDCIBack;
		INT16U uwDCIBackCnt;
		PROTECTION Protection;
		INT16U uwPVStartVolt;
		INT8U ubConstCurrEnable;
		INT8U ubWorkEnable;
		INT16U uwScanMpptPeriod;
		INT16U uwTemperatureStart;
		INT16U uwTemperatureEnd;
		INT16U uw10minVoltHigh;
		SOFTTYPE uwSoftStartType;
		INT8U ubPowerPerMinute;
		INT8U ubPfDelay;				//CEI 0-21 P(f)��ʱʱ��
		INT8U ubQvDelay;				//CEI 0-21 Q(v)��ʱʱ��
		INT8U ubReserve[1];
    INT16S swNRelayDelayOnAdj;
    INT8U ubReserve1[12];
		INT16U uwCRC;
	}data;
}SAFTY;
extern SAFTY Safty;

typedef union
{
	INT8U all[cInfoSize];
	struct
	{
		INT8U ubManufacture[16];
		INT8U ubSerialNumber[16];
		INT16U uwMaxCurrent;
		INT16U uwRatedPower;
		INT8U ubInputNumber;
		INT8U ubGeneration;
		INT16U uwkWModel;
		INT8U ublanguage;
		INT8U ubFirstTurnOn;
		INT8U ubPasswordH;
		INT8U ubPasswordM;
		INT8U ubPasswordL;
		INT8U ubReserve[17];
		INT16U uwCRC;
	}data;
}INFO;
extern INFO Info;

typedef union
{
	INT8U all[cEnergySize];
	struct
	{
		INT32U ulTodayEnergy;
		INT32U ulTotalEnergy;
		INT8U ubReserve[6];
		INT16U uwCRC;
	}data;
}ENERGY;
extern ENERGY Energy;

typedef union
{
	INT8U all[cErrorRecordSize];
	struct
	{
		ERRORMESSAGE ErrorMessage[20];
		INT8U ubReserve[8];
		INT16U uwCRC;
	}data;
}ERRORRECORD;
extern ERRORRECORD ErrorRecord;

typedef	union
{
	INT16U all;
	struct
    {
   		INT16U ubResetWiFi:1;
   		INT16U ubMonitor:2;
   		INT16U ubSelfTestStep:8;
		INT16U ubConstPV:1;
		INT16U ubReserve:4;
    }bit;
}SLAVEFLAG;

typedef union
{
	INT8U all[cSlaveDataSize];
	struct
	{
		INT16U uwGridVolt[3];
		INT16U uwGridFreq;
		INT16S swDCCurr[3];
		INT16U uwGroundCurr;
		INT16U uwBoostTemperature;
		INT16U uwInnerTemperature;
		INT16U uwModuleTemperature[3];
		INT16U uwVersion;
		INT16U uwBuild;
		SLAVEFLAG SlaveFlag;
		INT16U uwLibBuild;
	}data;
}SLAVEDATA;
extern SLAVEDATA SlaveData;

typedef union
{
	INT8U all[cPowerCtrlSize];
	struct
	{
		INT8U ubPowerPercent;
		INT8U ubCtrlType;
		INT8S sbXPercent[4];
		INT8S sbYPercent[4];
	}data;
}POWERCTRL;
extern POWERCTRL PowerCtrl;

typedef union
{
	INT8U all[cMasterDataSize];
	struct
	{
		INT16U uwPVVolt[4];
		INT16U uwPVCurr[4];
		INT16U uwBusVolt;
		INT16U uwBusNVolt;
		INT16U uwGridVolt[3];
		INT16U uwGridCurr[3];
		INT16U uwGridFreq;
		INT16U uwInvVolt[3];
		INT16U uwPowerAC;
		INT16U uwPowerPhase[3];
		INT16U uwGroundCurr;
		INT16S swGroundCurr;
		INT16S swDCCurr[3];
		INT8U ubPowerFactor;
		INT8U ubState;
		INT16U uwReconnectTime;
		INT16U uwRelayCtrl;
		ERRORFLAG error;
		INT16U uwVersion;
		INT16U uwBuild;
		INT16U uwSelfTestTime;
		INT16U uwSelfTestValue;
		INT16U uwIsoResitance[4];
		INT16S swNRelayDelayOnAdj;
		INT16U uwNRelayDelayOnAdjSave;
		INT16U uwReserve;				//����999009C��Ŀ��һ���¶Ȳ�������MCU��ɣ������999009C��Ŀ�д��¶����������������ȿ��ƣ�������Ŀ�д˴���ȱ
		INT16U uwLibBuild;
	}data;
}MASTERDATA;
extern MASTERDATA MasterData;

typedef union
{
	INT8U all[cPowerMeterSize];
	struct
	{
		INT16U uwGridVolt[3];
		INT16U uwPVVolt[4];
		INT16U uwBusVolt;
		INT16U uwBusNVolt;
		INT16U uwGridCurr[3];
		INT16U uwPVCurr[4];
		INT16U uwACPower;
	}data;
}POWERMETER;
extern POWERMETER PowerMeter;

typedef union
{
	INT8U all[cSystemTimeSize];
	struct
	{
		INT8U ubYear;
		INT8U ubMonth;
		INT8U ubDate;
		INT8U ubHour;
		INT8U ubMinute;
		INT8U ubSecond;
	}data;
}SYSTEMTIME;
extern SYSTEMTIME SystemTime;

typedef union
{
	INT8U all[cPQDDataSize];
	struct
	{
		INT16U uwGridVolt[3];
		INT16S swActivePower[3];
		INT16S swReactivePower[3];
	}data;
}PQDDATA;
extern PQDDATA PQDData;

#ifdef DebugEnable
typedef union
{
	INT8U all[cDebugSize];
	struct
	{
		INT8U ubDebugDataType[cDebugChannelNum];
		DEBUGFLAG ubDebugFlag;
		DEBUGBUF ubDebugBuf[cDebugChannelNum];
	}data;
}DEBUGVALUE;
extern DEBUGVALUE DebugValue;
#endif

typedef union
{
	INT8U all;
	struct
	{
		INT8U ubComExist:1;
		INT8U ubMonitor:2;
		INT8U ubReserve:5;
	}bit;
}COMFLAG;

typedef union
{
	INT8U all[cComSize];
	struct
	{
		INT16U uwVersion;
		INT16U uwBuild;
		INT8U ubWiFiSN[16];
		INT8U ubWiFiIP[16];
		INT8U ubComUpdateStep;
		COMFLAG ubFlag;
		INT16U uwReserve;
		INT32U ulAddress;
		INT16U uwLibBuild;
	}data;
}COMDATA;
extern COMDATA ComData;

typedef union
{
	INT8U all;
	struct
	{
		INT8U ubSetFlag: 1;
		INT8U ubBaudrate:7;
	}bit;
}SLAVEBAUDRATE;
extern SLAVEBAUDRATE SlaveBaudrate;

/********************************************************************
 * Extern Global Types
 ********************************************************************/
extern INT8U gubTXLength[cComNum];
extern INT8U lubTXBuf[cComNum][cMaxTXLength];
extern INT8U lubRXBuf[cComNum][cMaxRXLength];
extern INT8U *gubBlockPtr[cMaxBlockNum];
extern const INT16U cMaxLength[cMaxBlockNum];

/********************************************************************
 * Functions
 ********************************************************************/
void sInitComData(void);
INT16U sGetTXBufData(INT8U lubComNum);
INT8U sSetRXBufData(INT8U lubComNum,INT8U lubData);
void sClearRXCnt(INT8U lubComNum);

#endif

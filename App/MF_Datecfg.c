/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：MF_Datecfg.c
 * 修改人：沈仕强
 * 修改日期：2019.06.20
 * 描述：蜜蜂寄存器函数
 *******************************************************/
#include "MF_Datecfg.h"

//设备描述
INT16U MF_ausFc04ProtocolID[2];	//协议标识
INT16U MF_ausDeviceStartIDTAG;  //设备开始标识TAG  	
INT16U MF_ausStartIDLength;  		//标识长度
INT16U MF_ausManufacturer[16];	//生产商
INT16U MF_ausProductModel[16];	//Model型号
INT16U MF_ausFacilityReserved[8];//保留
INT16U MF_ausFirmwareVer[16];	//软件版本
INT16U MF_ausProductSn[16];		//序列号
INT16U MF_ausProtocolNum[10];	//协议号
INT16U MF_ausProtocolVer[10];	//协议版本
//电网信息
INT16U MF_ausGridLogicModuleID;        //逻辑功能模块标识
INT16U MF_ausGridModuleLength;          //模块长度
INT16U MF_ausGridPhaseCur[PHASE_NBR];	//电网相电流
INT16U MF_ausGridLineVolt[PHASE_NBR];	//电网线电压
INT16U MF_ausGridPhaseVolt[PHASE_NBR];	//电网相电压
INT16U MF_ausGridFreq;					//电网频率
INT16U MF_outActivePower[2];	 	    			//输出有功功率
INT32U MF_outApparentPower;				//输出视在功率
INT32S MF_outReactivePower;				//输出无功功率
INT16S MF_sDailyElecGeneration;			//当天发电量
INT16U MF_ulTotalElecGeneration[2];			//总发电量
INT32U MF_ulTotalReactiveElecGen;		//总无功电量
INT16S MF_sPowerFactor;					//功率因数
INT16U MF_DCVol;         		 		//直流电压
INT16U MF_DCCur;            		//直流电流
INT16U MF_inActivePower;	    			//输入有功功率
INT16S MF_sEvirTmp1;						//箱体内部温度
INT16S MF_sHeatsinkTmp;				//散热片温度
INT16S MF_sChokeTmp;					//电感温度
INT16S MF_sResvTmp;					//其他温度
INT16U MF_usCurrentRunningState;			//主运行模式
INT16U MF_usSubCurrentRunningState;		//子运行模式
INT16U MF_sInvReserved1[5];				//保留
PRE_ALARM_TIME MF_PreAlarmTime;     //最近一次故障时间戳
INT16U MF_sInvReserved2[2];				//保留
INT16U MF_EventAlarmInfo;        //事件故障信息
INT16U MF_sResv;						//reserved
INT16U MF_YearElecGeneration;       //年发电量
INT16U MF_ausResvVal[7];					//reserved
//****************PV输入信息**************************
INT16U MF_ausPVLogicModuleID;        //逻辑功能模块标识
INT16U MF_ausPVModuleLength;          //模块长度
INT16U MF_usMpptNbr;						//MPPT数量
INT16U MF_usPvInputConnectionMode;		//PV输入模式
INT16U MF_usPvArrayFaultInfo;			//组件故障信息
PV_ARRAY_DISPLAY_INFO_STR MF_astrPvArrayDisplayInfo[3];   //模块事件故障信息
INT16U MF_ausDeviceEndIDTAG;  //设备开始标识TAG  	
INT16U MF_ausEndIDLength;  		//标识长度
//****************故障信息**************************
ALARM_DISPLAY_INFO_STR MF_astrAlarmDsiplayInfo[20];
//**********************意大利自检**************************
INT16U MF_SelfTestState;     //self-test State
ITALY_SELF_TEST_STR MF_SelfTest[8];
INT16U MF_TEST_TIME[3];
ITALY_RESULT_STR MF_ItalyResult[8];
//**********************通讯参数**************************
INT16U MF_ausFc03SetFlag[2];		//设置/控制标志  
INT16U MF_ausCommSetIDTAG;  //通讯设置标识TAG  	
INT16U MF_ausCommSetIDLength;  		//标识长度
INT16U MF_usFc03CommSetEn;		//通讯设置使能  resvered
INT16U MF_usModbusAddr;			//Modbus地址
INT16U MF_ausCommModelResv;		//resvered
INT16U MF_usBaudRate;			//波特率
INT16U MF_ausCommResv[4];		//resvered
//**********************关键参数设置**************************
INT16U MF_ausParamSetLogicModuleID;        //逻辑功能模块标识
INT16U MF_ausParamSetModuleLength;          //模块长度
INT16U MF_ausCountryLanguageSetEn[2];		//国家语言设置使能 resvered
INT16U MF_usCountry;             		//国家/电网法规
INT16U MF_usLanguage;							//语言
INT16U MF_InvResetParamEn[2];      //逆变器RESET参数使能
INT16U MF_InvResetParam;            //参数恢复默认值
INT16U MF_usExistingDataClearEn[2];  //历史记录数据清除使能
INT16U MF_usExistingDataClear;			//历史记录数据清除
INT16U MF_usPvInputConnectionModeEn[2];		//PV输入模式设置使能
INT16U MF_usPvInputConnectionModeSet;		//PV输入模式设置
INT16U MF_sTotalElecGenCompValEn[2];			//总发电量补偿值使能
INT16S MF_sTotalElecGenCompVal;			//总发电量补偿值
INT16U MF_InvFastControlSetEn[2];   //逆变器快速控制/设置使能
INT16U MF_ausSystemTime[3];				//系统时间
INT16U MF_usTimingOnControl;				//定时开机控制
INT16U MF_usTimingOffControl;				//定时关机控制
INT16U MF_usInvOnOffCmd;					//命令开\关机
INT16U MF_WaitStandupTime;					//开机延迟
INT16U MF_usActPwrModeEn;				//有功功率控制模式 EN/DIS
INT16U MF_usActPwrSetValue;				//有功功率设置值
INT16U MF_FirstConnectGridPwrAdjustRate;  //初次并网功率调整速率
INT16U MF_FaultRstReconnectPwrAdjustRate;  //故障恢复重连功率调整速率
INT16U MF_usActPwrControlAdjustRate;    //有功功率控制调节速率
INT16U MF_usReactPwrModeEn;				//无功功率控制模式 EN/DIS
INT16S MF_PowerFactorSetVal;				//功率因数设置值
INT16U MF_PFAdjustRate;         //PF调节速率
INT16S MF_usReactPwrSetValue;				//无功功率设置值
INT16U MF_ItalyOutsideCommFlag;    //意大利外部通信标识
INT16U MF_afGridVoltProtThresHigh[3];		//电网电压保护上限值
INT16U MF_afGridVoltProtThresLow[3];		//电网电压保护下限值
INT16U MF_ausGridVoltProtTimeHigh[3];	//电网电压保护上限时间
INT16U MF_ausGridVoltProtTimeLow[3];		//电网电压保护下限时间
INT16U MF_usGridVoltFltRecoverTime;		//电网电压恢复时间
INT16U MF_afGridFreqProtThresHigh[3];		//电网频率保护上限值
INT16U MF_afGridFreqProtThresLow[3];		//电网频率保护下限值
INT16U MF_ausGridFreqProtTimeHigh[3];	//电网频率保护上限时间
INT16U MF_ausGridFreqProtTimeLow[3];		//电网频率保护下限时间
INT16U MF_usGridFreqFltRecoverTime;		//电网频率恢复时间
INT16U MF_usWattFreqModeEn;						//Watt-Freq En/Dis
INT16U MF_usWattVoltModeEn;						//Watt-Volt En/Dis
INT16U MF_usPfWattModeEn;						//PF-Watt En/Dis
INT16U MF_usVarVoltModeEn;						//Var-Volt En/Dis
INT16U MF_usVarWattModeEn;						//Var-Watt En/Dis
INT16U MF_usResvModeEn;							//resvered Mode En/Dis
INT16U MF_usLVRTCurveEn;							//LVRT(曲线)使能
INT16U MF_usHVRTCurveEn;							//HVRT(曲线)使能
INT16U MF_usLFRTCurveEn;							//LFRT(曲线)使能
INT16U MF_usHFRTCurveEn;							//HFRT(曲线)使能
INT16U MF_usGridVoltProtThresAutoAdjEn;	//电网过压保护阈值自动调节使能/屏蔽
INT16U MF_usGridVoltHighPwrDeratedEn;	//电网过压降额使能/屏蔽
INT16U MF_usGlobalMpptScanEn;			//Global MPPT SCAN使能/屏蔽
INT16U MF_usPvVICurveEn;					//PV-VI曲线使能/屏蔽
INT16U MF_usItalySelfTestEn;				//ItalySelfTest使能
INT16U MF_usPidFuncEn;					//PID使能/屏蔽
INT16U MF_usDrmFuncEn;					//DRM使能/屏蔽
INT16U MF_usArcDetectEn;					//ARC检测使能/屏蔽
INT16U MF_usArcResetEn;					//ARC复位
INT16U MF_usAIFuncEn;					//AI使能/屏蔽
INT16U MF_usGFCIFuncEn;					//GFCI使能/屏蔽
INT16U MF_usISOFuncEn;					//ISO使能/屏蔽
INT16U MF_usDCIFuncEn;					//DCI使能/屏蔽
INT16U MF_ausPARAMResv1;				//Reserved
INT16S MF_wZeroExpPercent;	   //wZeroExpPercent
INT16U MF_ausProtEnResv[3];				//Reserved
INT16U MF_ausCommSetEndIDTAG;  //设备结束标识TAG  	
INT16U MF_ausCommSetEndIDLength;  		//标识长度	

const PARAM_CFG_TBL_STR g_astrParamSendTblAttr[PARAM_NUM] =
{
 //device info
 {0x0000,    0x0000,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFc04ProtocolID[0]},
 {0x0001,    0x0001,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFc04ProtocolID[1]},
 {0x0002,    0x0002,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausDeviceStartIDTAG},
 {0x0003,    0x0003,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausStartIDLength},
 {0x0004,    0x0004,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausManufacturer[0]},
 {0x0005,    0x0005,    0,    65535,			1,    SEND_TYPE_16BIT,    &MF_ausManufacturer[1]},
 {0x0006,    0x0006,    0,    65535,			1,    SEND_TYPE_16BIT,    &MF_ausManufacturer[2]},
 {0x0007,    0x0007,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausManufacturer[3]},
 {0x0008,    0x0008,    0,    65535,			1,    SEND_TYPE_16BIT,    &MF_ausManufacturer[4]},
 {0x0009,    0x0009,    0,    65535,			1,    SEND_TYPE_16BIT,    &MF_ausManufacturer[5]},
 {0x000A,    0x000A,    0,    65535,			1,    SEND_TYPE_16BIT,    &MF_ausManufacturer[6]},
 {0x000B,    0x000B,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausManufacturer[7]},
 {0x000C,    0x000C,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausManufacturer[8]},
 {0x000D,    0x000D,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausManufacturer[9]},
 {0x000E,    0x000E,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausManufacturer[10]},
 {0x000F,    0x000F,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausManufacturer[11]},
 {0x0010,    0x0010,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausManufacturer[12]},
 {0x0011,    0x0011,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausManufacturer[13]},
 {0x0012,    0x0012,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausManufacturer[14]},
 {0x0013,    0x0013,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausManufacturer[15]},
 
 {0x0014,    0x0014,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausProductModel[0]},
 {0x0015,    0x0015,    0,    65535,			1,    SEND_TYPE_16BIT,    &MF_ausProductModel[1]},
 {0x0016,    0x0016,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausProductModel[2]},
 {0x0017,    0x0017,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausProductModel[3]},
 {0x0018,    0x0018,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausProductModel[4]},
 {0x0019,    0x0019,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausProductModel[5]},
 {0x001A,    0x001A,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausProductModel[6]},
 {0x001B,    0x001B,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausProductModel[7]},
 {0x001C,    0x001C,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausProductModel[8]},
 {0x001D,    0x001D,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausProductModel[9]},
 {0x001E,    0x001E,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausProductModel[10]},
 {0x001F,    0x001F,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausProductModel[11]},
 {0x0020,    0x0020,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausProductModel[12]},
 {0x0021,    0x0021,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausProductModel[13]},
 {0x0022,    0x0022,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausProductModel[14]},
 {0x0023,    0x0023,    0,    65535,			1,    SEND_TYPE_16BIT,    &MF_ausProductModel[15]},
 
 {0x0024,    0x0024,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFacilityReserved[0]},
 {0x0025,    0x0025,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFacilityReserved[1]},
 {0x0026,    0x0026,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFacilityReserved[2]},
 {0x0027,    0x0027,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFacilityReserved[3]},
 {0x0028,    0x0028,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFacilityReserved[4]},
 {0x0029,    0x0029,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFacilityReserved[5]},
 {0x002A,    0x002A,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFacilityReserved[6]},
 {0x002B,    0x002B,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFacilityReserved[7]},
 
 {0x002C,    0x002C,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFirmwareVer[0]},
 {0x002D,    0x002D,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFirmwareVer[1]},
 {0x002E,    0x002E,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFirmwareVer[2]},
 {0x002F,    0x002F,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFirmwareVer[3]},
 {0x0030,    0x0030,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFirmwareVer[4]},
 {0x0031,    0x0031,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFirmwareVer[5]},
 {0x0032,    0x0032,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFirmwareVer[6]},
 {0x0033,    0x0033,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFirmwareVer[7]},
 {0x0034,    0x0034,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFirmwareVer[8]},
 {0x0035,    0x0035,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFirmwareVer[9]},
 {0x0036,    0x0036,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFirmwareVer[10]},
 {0x0037,    0x0037,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFirmwareVer[11]},
 {0x0038,    0x0038,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFirmwareVer[12]},
 {0x0039,    0x0039,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFirmwareVer[13]},
 {0x003A,    0x003A,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFirmwareVer[14]},
 {0x003B,    0x003B,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausFirmwareVer[15]},
 
 {0x003C,    0x003C,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausProductSn[0]},
 {0x003D,    0x003D,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausProductSn[1]},
 {0x003E,    0x003E,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausProductSn[2]},
 {0x003F,    0x003F,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausProductSn[3]},
 {0x0040,    0x0040,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausProductSn[4]},
 {0x0041,    0x0041,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausProductSn[5]},
 {0x0042,		 0x0042,		0,		65535,			1,	  SEND_TYPE_16BIT, 	  &MF_ausProductSn[6]},
 {0x0043,		 0x0043,		0,		65535,			1,	  SEND_TYPE_16BIT, 	  &MF_ausProductSn[7]},
 {0x0044,		 0x0044,		0,		65535,			1,	  SEND_TYPE_16BIT, 	  &MF_ausProductSn[8]},
 {0x0045,		 0x0045,		0,		65535,			1,	  SEND_TYPE_16BIT, 	  &MF_ausProductSn[9]},
 {0x0046,		 0x0046,		0,		65535,			1,	  SEND_TYPE_16BIT, 	  &MF_ausProductSn[10]},
 {0x0047,		 0x0047,		0,		65535,			1,	  SEND_TYPE_16BIT, 	  &MF_ausProductSn[11]},
 {0x0048,		 0x0048,		0,		65535,			1,	  SEND_TYPE_16BIT, 	  &MF_ausProductSn[12]},
 {0x0049,		 0x0049,		0,		65535,			1,	  SEND_TYPE_16BIT, 	  &MF_ausProductSn[13]},
 {0x004A,		 0x004A,		0,		65535,			1,	  SEND_TYPE_16BIT, 	  &MF_ausProductSn[14]},
 {0x004B,		 0x004B,		0,		65535,			1,	  SEND_TYPE_16BIT, 	  &MF_ausProductSn[15]},

 
 {0x004C,	 	 0x004C,		0,	  65535,			1,	  SEND_TYPE_16BIT,	  &MF_ausProtocolNum[0]},
 {0x004D,	   0x004D,		0,	  65535,			1,	  SEND_TYPE_16BIT,	  &MF_ausProtocolNum[1]},
 {0x004E,		 0x004E,		0,	  65535,			1,	  SEND_TYPE_16BIT,	  &MF_ausProtocolNum[2]},
 {0x004F,	 	 0x004F,		0,	  65535,			1,	  SEND_TYPE_16BIT,	  &MF_ausProtocolNum[3]},
 {0x0050,	   0x0050,		0,	  65535,			1,	  SEND_TYPE_16BIT,	  &MF_ausProtocolNum[4]},
 {0x0051,	 	 0x0051,		0,	  65535,			1,	  SEND_TYPE_16BIT,	  &MF_ausProtocolNum[5]},
 {0x0052,	   0x0052,		0,	  65535,			1,	  SEND_TYPE_16BIT,	  &MF_ausProtocolNum[6]},
 {0x0053,	   0x0053,		0,	  65535,			1,	  SEND_TYPE_16BIT,	  &MF_ausProtocolNum[7]},
 {0x0054,	   0x0054,		0,	  65535,			1,	  SEND_TYPE_16BIT,	  &MF_ausProtocolNum[8]},
 {0x0055,	   0x0055,		0,	  65535,			1,	  SEND_TYPE_16BIT,	  &MF_ausProtocolNum[9]},

 {0x0056,	 	 0x0056,		0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ausProtocolVer[0]},
 {0x0057,	 	 0x0057,		0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ausProtocolVer[1]},
 {0x0058,	 	 0x0058,		0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ausProtocolVer[2]},
 {0x0059,	   0x0059,		0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ausProtocolVer[3]},
 {0x005A,	   0x005A,		0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ausProtocolVer[4]},
 {0x005B,	   0x005B,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ausProtocolVer[5]},
 {0x005C,	   0x005C,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ausProtocolVer[6]},
 {0x005D,	   0x005D,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ausProtocolVer[7]},
 {0x005E,	   0x005E,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ausProtocolVer[8]},
 {0x005F,	   0x005F,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ausProtocolVer[9]},

 //grid info
 {0x0100,    0x0100,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausGridLogicModuleID},
 {0x0101,    0x0101,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausGridModuleLength},
 
 {0x0102,    0x0102,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausGridPhaseCur[PHASE_A]},
 {0x0103,    0x0103,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausGridPhaseCur[PHASE_B]},
 {0x0104,    0x0104,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausGridPhaseCur[PHASE_C]},
 {0x0105,    0x0105,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausGridLineVolt[PHASE_AB]},
 {0x0106,    0x0106,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausGridLineVolt[PHASE_BC]},
 {0x0107,    0x0107,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausGridLineVolt[PHASE_CA]},
 {0x0108,    0x0108,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausGridPhaseVolt[PHASE_A]},
 {0x0109,    0x0109,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausGridPhaseVolt[PHASE_B]},
 {0x010A,    0x010A,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausGridPhaseVolt[PHASE_C]},
 
 {0x010B,    0x010B,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausGridFreq},
 {0x010C,    0x010C,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_outActivePower[0]},
 {0x010D,    0x010D,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_outActivePower[1]},
 {0x010E,    0x010E,    0,    65535,   		1,    SEND_TYPE_32BIT,    &MF_outApparentPower},
 {0x0110,    0x0110,    0,    65535,   		1,    SEND_TYPE_32BIT,    &MF_outReactivePower},
 {0x0112,    0x0112,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_sDailyElecGeneration},
 {0x0113,    0x0113,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ulTotalElecGeneration[0]},
 {0x0114,    0x0113,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ulTotalElecGeneration[1]},
 {0x0115,    0x0115,    0,    65535,   		1,    SEND_TYPE_32BIT,    &MF_ulTotalReactiveElecGen},
 {0x0117,    0x0117,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_sPowerFactor},
 {0x0118,    0x0118,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_DCVol},
 {0x0119,    0x0119,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_DCCur},
 {0x011A,    0x011A,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_inActivePower},
 {0x011B,    0x011B,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_sEvirTmp1},
 {0x011C,    0x011C,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_sHeatsinkTmp},
 {0x011D,    0x011D,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_sChokeTmp},
 {0x011E,    0x011E,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_sResvTmp},
 {0x011F,    0x011F,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_usCurrentRunningState},
 {0x0120,    0x0120,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_usSubCurrentRunningState},
 {0x0121,    0x0121,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_sInvReserved1[0]},
 {0x0122,    0x0122,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_sInvReserved1[1]},
 {0x0123,    0x0123,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_sInvReserved1[2]},
 {0x0124,    0x0124,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_sInvReserved1[3]},
 {0x0125,    0x0125,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_sInvReserved1[4]},
 {0x0126,    0x0126,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_PreAlarmTime.YearMonth},
 {0x0127,    0x0127,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_PreAlarmTime.DateHour},
 {0x0128,    0x0128,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_PreAlarmTime.MinuteSecond},
 {0x0129,    0x0129,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_sInvReserved2[0]},
 {0x012A,    0x012A,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_sInvReserved2[1]},
 {0x012B,    0x012B,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_EventAlarmInfo},
 {0x012C,    0x012C,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_sResv},
 {0x012D,    0x012D,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_YearElecGeneration},
 {0x012E,    0x012E,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausResvVal[0]},
 {0x012F,    0x012F,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausResvVal[1]},
 {0x0130,    0x0130,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausResvVal[2]},
 {0x0131,    0x0131,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausResvVal[3]},
 {0x0132,    0x0132,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausResvVal[4]},
 {0x0133,    0x0133,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausResvVal[5]},
 {0x0134,    0x0134,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausResvVal[6]},


//****************PV输入信息**************************
 {0x0200,    0x0200,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausPVLogicModuleID},
 {0x0201,    0x0201,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_ausPVModuleLength},
 {0x0202,    0x0202,    0,    65535,		  1,    SEND_TYPE_16BIT,    &MF_usMpptNbr},
 {0x0203,    0x0203,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_usPvInputConnectionMode},
 {0x0204,    0x0204,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_usPvArrayFaultInfo},
 {0x0205,    0x0205,    0,    65535,   		1,	  SEND_TYPE_16BIT,    &MF_astrPvArrayDisplayInfo[0].INPUT_PV_ID},
 {0x0206,    0x0206,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_astrPvArrayDisplayInfo[0].PV_INPUT_VOL},
 {0x0207,    0x0207,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_astrPvArrayDisplayInfo[0].PV_INPUT_CUR},
 {0x0208,    0x0208,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_astrPvArrayDisplayInfo[0].PV_INPUT_POW},
 {0x0209,    0x0209,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_astrPvArrayDisplayInfo[0].RUN_MODLE},
 {0x020A,    0x020A,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_astrPvArrayDisplayInfo[0].MODLE_TEMP},
 {0x020B,    0x020B,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_astrPvArrayDisplayInfo[0].MODLE_ALARM_MESSAGE},
 
 {0x020C,	   0x020C,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrPvArrayDisplayInfo[1].INPUT_PV_ID},
 {0x020D,	   0x020D,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrPvArrayDisplayInfo[1].PV_INPUT_VOL},
 {0x020E,	   0x020E,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrPvArrayDisplayInfo[1].PV_INPUT_CUR},
 {0x020F,	   0x020F,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrPvArrayDisplayInfo[1].PV_INPUT_POW},
 {0x0210,	   0x0210,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrPvArrayDisplayInfo[1].RUN_MODLE},
 {0x0211,    0x0211,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_astrPvArrayDisplayInfo[1].MODLE_TEMP},
 {0x0212,    0x0212,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_astrPvArrayDisplayInfo[1].MODLE_ALARM_MESSAGE},
 
 {0x0213,    0x0213,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_astrPvArrayDisplayInfo[2].INPUT_PV_ID},
 {0x0214,    0x0214,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_astrPvArrayDisplayInfo[2].PV_INPUT_VOL},
 {0x0215,    0x0215,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_astrPvArrayDisplayInfo[2].PV_INPUT_CUR},
 {0x0216,    0x0216,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_astrPvArrayDisplayInfo[2].PV_INPUT_POW},
 {0x0217,    0x0217,    0,    65535,   		1,    SEND_TYPE_16BIT,    &MF_astrPvArrayDisplayInfo[2].RUN_MODLE},
 {0x0218,	   0x0218,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrPvArrayDisplayInfo[2].MODLE_TEMP},
 {0x0219,	   0x0219,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrPvArrayDisplayInfo[2].MODLE_ALARM_MESSAGE},
 
 {0x021A,	   0x021A,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ausDeviceEndIDTAG},
 {0x021B,	   0x021B,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ausEndIDLength},
 
//****************故障信息**************************
 {0x0BB8,	   0x0BB8,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[0].ALARM_NUM},
 {0x0BB9,	   0x0BB9,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[0].data.YearMonth},
 {0x0BBA,	   0x0BBA,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[0].data.DateHour},
 {0x0BBB,	   0x0BBB,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[0].data.MinuteSecond},
 {0x0BBC,	   0x0BBC,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[1].ALARM_NUM},
 {0x0BBD,	   0x0BBD,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[1].data.YearMonth},
 {0x0BBE,	   0x0BBE,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[1].data.DateHour},
 {0x0BBF,	   0x0BBF,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[1].data.MinuteSecond},
 {0x0BC0,	   0x0BC0,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[2].ALARM_NUM},
 {0x0BC1,	   0x0BC1,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[2].data.YearMonth},
 {0x0BC2,	   0x0BC2,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[2].data.DateHour},
 {0x0BC3,	   0x0BC3,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[2].data.MinuteSecond},
 {0x0BC4,	   0x0BC4,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[3].ALARM_NUM},
 {0x0BC5,	   0x0BC5,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[3].data.YearMonth},
 {0x0BC6,	   0x0BC6,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[3].data.DateHour},
 {0x0BC7,	   0x0BC7,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[3].data.MinuteSecond},
 {0x0BC8,	   0x0BC8,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[4].ALARM_NUM},
 {0x0BC9,	   0x0BC9,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[4].data.YearMonth},
 {0x0BCA,	   0x0BCA,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[4].data.DateHour},
 {0x0BCB,	   0x0BCB,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[4].data.MinuteSecond},
 {0x0BCC,	   0x0BCC,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[5].ALARM_NUM},
 {0x0BCD,	   0x0BCD,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[5].data.YearMonth},
 {0x0BCE,	   0x0BCE,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[5].data.DateHour},
 {0x0BCF,	   0x0BCF,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[5].data.MinuteSecond},
 {0x0BD0,	   0x0BD0,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[6].ALARM_NUM},
 {0x0BD1,	   0x0BD1,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[6].data.YearMonth},
 {0x0BD2,	   0x0BD2,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[6].data.DateHour},
 {0x0BD3,	   0x0BD3,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[6].data.MinuteSecond},
 {0x0BD4,	   0x0BD4,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[7].ALARM_NUM},
 {0x0BD5,	   0x0BD5,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[7].data.YearMonth},
 {0x0BD6,	   0x0BD6,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[7].data.DateHour},
 {0x0BD7,	   0x0BD7,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[7].data.MinuteSecond},
 {0x0BD8,	   0x0BD8,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[8].ALARM_NUM},
 {0x0BD9,	   0x0BD9,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[8].data.YearMonth},
 {0x0BDA,	   0x0BDA,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[8].data.DateHour},
 {0x0BDB,	   0x0BDB,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[8].data.MinuteSecond},
 {0x0BDC,	   0x0BDC,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[9].ALARM_NUM},
 {0x0BDD,	   0x0BDD,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[9].data.YearMonth},
 {0x0BDE,	   0x0BDE,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[9].data.DateHour},
 {0x0BDF,	   0x0BDF,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[9].data.MinuteSecond},
 {0x0BE0,	   0x0BE0,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[10].ALARM_NUM},
 {0x0BE1,	   0x0BE1,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[10].data.YearMonth},
 {0x0BE2,	   0x0BE2,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[10].data.DateHour},
 {0x0BE3,	   0x0BE3,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[10].data.MinuteSecond},
 {0x0BE4,	   0x0BE4,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[11].ALARM_NUM},
 {0x0BE5,	   0x0BE5,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[11].data.YearMonth},
 {0x0BE6,	   0x0BE6,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[11].data.DateHour},
 {0x0BE7,	   0x0BE7,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[11].data.MinuteSecond},
 {0x0BE8,	   0x0BE8,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[12].ALARM_NUM},
 {0x0BE9,	   0x0BE9,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[12].data.YearMonth},
 {0x0BEA,	   0x0BEA,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[12].data.DateHour},
 {0x0BEB,	   0x0BEB,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[12].data.MinuteSecond},
 {0x0BEC,	   0x0BEC,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[13].ALARM_NUM},
 {0x0BED,	   0x0BED,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[13].data.YearMonth},
 {0x0BEE,	   0x0BEE,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[13].data.DateHour},
 {0x0BEF,	   0x0BEF,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[13].data.MinuteSecond},
 {0x0BF0,	   0x0BF0,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[14].ALARM_NUM},
 {0x0BF1,	   0x0BF1,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[14].data.YearMonth},
 {0x0BF2,	   0x0BF2,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[14].data.DateHour},
 {0x0BF3,	   0x0BF3,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[14].data.MinuteSecond},
 {0x0BF4,	   0x0BF4,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[15].ALARM_NUM},
 {0x0BF5,	   0x0BF5,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[15].data.YearMonth},
 {0x0BF6,	   0x0BF6,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[15].data.DateHour},
 {0x0BF7,	   0x0BF7,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[15].data.MinuteSecond},
 {0x0BF8,	   0x0BF8,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[16].ALARM_NUM},
 {0x0BF9,	   0x0BF9,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[16].data.YearMonth},
 {0x0BFA,	   0x0BFA,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[16].data.DateHour},
 {0x0BFB,	   0x0BFB,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[16].data.MinuteSecond},
 {0x0BFC,	   0x0BFC,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[17].ALARM_NUM},
 {0x0BFD,	   0x0BFD,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[17].data.YearMonth},
 {0x0BFE,	   0x0BFE,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[17].data.DateHour},
 {0x0BFF,	   0x0BFF,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[17].data.MinuteSecond},
 {0x0C00,	   0x0C00,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[18].ALARM_NUM},
 {0x0C01,	   0x0C01,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[18].data.YearMonth},
 {0x0C02,	   0x0C02,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[18].data.DateHour},
 {0x0C03,	   0x0C03,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[18].data.MinuteSecond},
 {0x0C04,	   0x0C04,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[19].ALARM_NUM},
 {0x0C05,	   0x0C05,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[19].data.YearMonth},
 {0x0C06,	   0x0C06,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[19].data.DateHour},
 {0x0C07,	   0x0C07,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_astrAlarmDsiplayInfo[19].data.MinuteSecond},
 //**********************意大利自检**************************
 {0x0FA0,	   0x0FA0,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTestState},
 {0x0FA1,	   0x0FA1,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[0].Limit_Value},
 {0x0FA2,	   0x0FA2,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[0].Limit_Time},
 {0x0FA3,	   0x0FA3,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[0].Trip_Value},
 {0x0FA4,	   0x0FA4,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[0].Trip_Time},
 {0x0FA5,	   0x0FA5,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[1].Limit_Value},
 {0x0FA6,	   0x0FA6,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[1].Limit_Time},
 {0x0FA7,	   0x0FA7,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[1].Trip_Value},
 {0x0FA8,	   0x0FA8,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[1].Trip_Time},
 {0x0FA9,	   0x0FA9,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[2].Limit_Value},
 {0x0FAA,	   0x0FAA,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[2].Limit_Time},
 {0x0FAB,	   0x0FAB,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[2].Trip_Value},
 {0x0FAC,	   0x0FAC,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[2].Trip_Time},
 {0x0FAD,	   0x0FAD,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[3].Limit_Value},
 {0x0FAE,	   0x0FAE,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[3].Limit_Time},
 {0x0FAF,	   0x0FAF,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[3].Trip_Value},
 {0x0FB0,	   0x0FB0,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[3].Trip_Time},
 {0x0FB1,	   0x0FB1,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[4].Limit_Value},
 {0x0FB2,	   0x0FB2,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[4].Limit_Time},
 {0x0FB3,	   0x0FB3,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[4].Trip_Value},
 {0x0FB4,	   0x0FB4,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[4].Trip_Time},
 {0x0FB5,	   0x0FB5,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[5].Limit_Value},
 {0x0FB6,	   0x0FB6,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[5].Limit_Time},
 {0x0FB7,	   0x0FB7,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[5].Trip_Value},
 {0x0FB8,	   0x0FB8,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[5].Trip_Time},
 {0x0FB9,	   0x0FB9,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[6].Limit_Value},
 {0x0FBA,	   0x0FBA,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[6].Limit_Time},
 {0x0FBB,	   0x0FBB,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[6].Trip_Value},
 {0x0FBC,	   0x0FBC,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[6].Trip_Time},
 {0x0FBD,	   0x0FBD,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[7].Limit_Value},
 {0x0FBE,	   0x0FBE,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[7].Limit_Time},
 {0x0FBF,	   0x0FBF,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[7].Trip_Value},
 {0x0FC0,	   0x0FC0,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_SelfTest[7].Trip_Time},

 {0x1004,	   0x1004,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_TEST_TIME[0]},
 {0x1005,	   0x1005,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_TEST_TIME[1]},
 {0x1006,	   0x1006,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_TEST_TIME[2]},
 {0x1007,	   0x1007,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[0].Trip_Value},
 {0x1008,	   0x1008,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[0].Trip_Time},
 {0x1009,	   0x1009,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[0].Step_Result},
 {0x100A,	   0x100A,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[1].Trip_Value},
 {0x100B,	   0x100B,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[1].Trip_Time},
 {0x100C,	   0x100C,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[1].Step_Result},
 {0x100D,	   0x100D,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[2].Trip_Value},
 {0x100E,	   0x100E,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[2].Trip_Time},
 {0x100F,	   0x100F,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[2].Step_Result},
 {0x1010,	   0x1010,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[3].Trip_Value},
 {0x1011,	   0x1011,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[3].Trip_Time},
 {0x1012,	   0x1012,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[3].Step_Result},
 {0x1013,	   0x1013,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[4].Trip_Value},
 {0x1014,	   0x1014,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[4].Trip_Time},
 {0x1015,	   0x1015,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[4].Step_Result},
 {0x1016,	   0x1016,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[5].Trip_Value},
 {0x1017,	   0x1017,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[5].Trip_Time},
 {0x1018,	   0x1018,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[5].Step_Result},
 {0x1019,	   0x1019,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[6].Trip_Value},
 {0x101A,	   0x101A,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[6].Trip_Time},
 {0x101B,	   0x101B,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[6].Step_Result},
 {0x101C,	   0x101C,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[7].Trip_Value},
 {0x101D,	   0x101D,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[7].Trip_Time},
 {0x101E,	   0x101E,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &MF_ItalyResult[7].Step_Result}
};

/*********************************************************************************************
*********************************************************************************************
*********************************************************************************************
*********************************************************************************************
*********************************************************************************************
*********************************************************************************************/

const PARAM_CFG_TBL_STR g_astrParamCfgTblAttr[PARAM_NUM]=
{
//**********************通讯参数**************************
 {0x2000,    0x2000,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausFc03SetFlag[0]},
 {0x2001,    0x2001,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausFc03SetFlag[1]},
 {0x2002,    0x2002,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausCommSetIDTAG},
 {0x2003,    0x2003,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausCommSetIDLength},
 {0x2004,    0x2004,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usFc03CommSetEn},
 {0x2005,    0x2005,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usModbusAddr},
 {0x2006,    0x2006,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausCommModelResv},
 {0x2007,    0x2007,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usBaudRate},
 {0x2008,    0x2008,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausCommResv[0]},
 {0x2009,    0x2009,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausCommResv[1]},
 {0x200A,    0x200A,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausCommResv[2]},
 {0x200B,    0x200B,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausCommResv[3]},
 //**********************关键参数设置**************************
 {0x2100,    0x2100,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausParamSetLogicModuleID},
 {0x2101,    0x2101,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausParamSetModuleLength},
 {0x2102,    0x2102,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausCountryLanguageSetEn[0]},
 {0x2103,    0x2103,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausCountryLanguageSetEn[1]},
 {0x2104,    0x2104,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usCountry},
 {0x2105,    0x2105,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usLanguage},
 {0x2106,    0x2106,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_InvResetParamEn[0]},
 {0x2107,    0x2107,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_InvResetParamEn[1]},
 {0x2108,    0x2108,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_InvResetParam},
 {0x2109,    0x2109,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usExistingDataClearEn[0]},
 {0x210A,    0x210A,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usExistingDataClearEn[1]},
 {0x210B,    0x210B,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usExistingDataClear},
 {0x210C,    0x210C,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usPvInputConnectionModeEn[0]},
 {0x210D,    0x210D,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usPvInputConnectionModeEn[1]},
 {0x210E,    0x210E,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usPvInputConnectionModeSet},
 {0x210F,    0x210F,    0,	  65535,	10,   CFG_TYPE_U16,       &MF_sTotalElecGenCompValEn[0]},
 {0x2110,    0x2110,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_sTotalElecGenCompValEn[1]},
 {0x2111,    0x2111,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_sTotalElecGenCompVal},
 {0x2112,    0x2112,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_InvFastControlSetEn[0]},
 {0x2113,    0x2113,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_InvFastControlSetEn[1]},
 {0x2114,    0x2114,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausSystemTime[0]},
 {0x2115,    0x2115,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausSystemTime[1]},
 {0x2116,    0x2116,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausSystemTime[2]},
 {0x2117,    0x2117,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usTimingOnControl},
 {0x2118,    0x2118,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usTimingOffControl},
 {0x2119,    0x2119,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usInvOnOffCmd},
 {0x211A,    0x211A,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_WaitStandupTime},
 {0x211B,    0x211B,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usActPwrModeEn},
 {0x211C,    0x211C,    0,	  65535,	10,   CFG_TYPE_U16,       &MF_usActPwrSetValue},
 {0x211D,    0x211D,    0,	  65535,	10,   CFG_TYPE_U16,       &MF_FirstConnectGridPwrAdjustRate},
 {0x211E,    0x211E,    0,	  65535,	10,   CFG_TYPE_U16,       &MF_FaultRstReconnectPwrAdjustRate},
 {0x211F,    0x211F,    0,	  65535,	10,   CFG_TYPE_U16,       &MF_usActPwrControlAdjustRate},
 
 {0x2120,    0x2120,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usReactPwrModeEn},
 {0x2121,    0x2121,    0,	  65535,	1000, CFG_TYPE_S16,       &MF_PowerFactorSetVal},
 {0x2122,    0x2122,    0,	  65535,	1000, CFG_TYPE_U16,       &MF_PFAdjustRate},
 {0x2123,    0x2123,    0,	  65535,	10,   CFG_TYPE_S16,       &MF_usReactPwrSetValue},
 {0x2124,    0x2124,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ItalyOutsideCommFlag},
 {0x2125,    0x2125,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_afGridVoltProtThresHigh[0]},
 {0x2126,    0x2126,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_afGridVoltProtThresHigh[1]},
 {0x2127,    0x2127,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_afGridVoltProtThresHigh[2]},
 {0x2128,    0x2128,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_afGridVoltProtThresLow[0]},
 {0x2129,    0x2129,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_afGridVoltProtThresLow[1]},
 {0x212A,    0x212A,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_afGridVoltProtThresLow[2]},
 {0x212B,    0x212B,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausGridVoltProtTimeHigh[0]},
 {0x212C,    0x212C,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausGridVoltProtTimeHigh[1]},
 {0x212D,    0x212D,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausGridVoltProtTimeHigh[2]},
 {0x212E,    0x212E,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausGridVoltProtTimeLow[0]},
 {0x212F,    0x212F,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausGridVoltProtTimeLow[1]},
 
 {0x2130,    0x2130,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausGridVoltProtTimeLow[2]},
 {0x2131,    0x2131,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usGridVoltFltRecoverTime},
 {0x2132,    0x2132,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_afGridFreqProtThresHigh[0]},
 {0x2133,    0x2133,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_afGridFreqProtThresHigh[1]},
 {0x2134,    0x2134,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_afGridFreqProtThresHigh[2]},
 {0x2135,    0x2135,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_afGridFreqProtThresLow[0]},
 {0x2136,    0x2136,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_afGridFreqProtThresLow[1]},
 {0x2137,    0x2137,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_afGridFreqProtThresLow[2]},
 {0x2138,    0x2138,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausGridFreqProtTimeHigh[0]},
 {0x2139,    0x2139,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausGridFreqProtTimeHigh[1]},
 {0x213A,    0x213A,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausGridFreqProtTimeHigh[2]},
 {0x213B,    0x213B,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausGridFreqProtTimeLow[0]},
 {0x213C,    0x213C,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausGridFreqProtTimeLow[1]},
 {0x213D,    0x213D,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausGridFreqProtTimeLow[2]},
 {0x213E,    0x213E,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usGridFreqFltRecoverTime},
 {0x213F,    0x213F,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usWattFreqModeEn},
 
 {0x2140,    0x2140,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usWattVoltModeEn},
 {0x2141,    0x2141,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usPfWattModeEn},
 {0x2142,    0x2142,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usVarVoltModeEn},
 {0x2143,    0x2143,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usVarWattModeEn},
 {0x2144,    0x2144,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usResvModeEn},
 {0x2145,    0x2145,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usLVRTCurveEn},
 {0x2146,    0x2146,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usHVRTCurveEn},
 {0x2147,    0x2147,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usLFRTCurveEn},
 {0x2148,    0x2148,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usHFRTCurveEn},
 {0x2149,    0x2149,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usGridVoltProtThresAutoAdjEn},
 {0x214A,    0x214A,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usGridVoltHighPwrDeratedEn},
 {0x214B,    0x214B,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usGlobalMpptScanEn},
 {0x214C,    0x214C,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usPvVICurveEn},
 {0x214D,    0x214D,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usItalySelfTestEn},
 {0x214E,    0x214E,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usPidFuncEn},
 {0x214F,    0x214F,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usDrmFuncEn},
 
 {0x2150,    0x2150,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usArcDetectEn},
 {0x2151,    0x2151,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usArcResetEn},
 {0x2152,    0x2152,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usAIFuncEn},
 {0x2153,    0x2153,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usGFCIFuncEn},
 {0x2154,    0x2154,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usISOFuncEn},
 {0x2155,    0x2155,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_usDCIFuncEn},
 {0x2156,    0x2156,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausPARAMResv1},
 {0x2157,    0x2157,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_wZeroExpPercent},
 {0x2158,    0x2158,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausProtEnResv[0]},
 {0x2159,    0x2159,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausProtEnResv[1]},
 {0x215A,    0x215A,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausProtEnResv[2]},
 {0x215B,    0x215B,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausCommSetEndIDTAG},
 {0x215C,    0x215C,    0,	  65535,	1,    CFG_TYPE_U16,       &MF_ausCommSetEndIDLength}

};


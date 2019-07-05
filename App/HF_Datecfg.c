#include "HF_Datecfg.h"


//new param configure table
//Below is the protocol of 04 function code members.
//设备描述
INT16U HF_ausFc04ProtocolID[2];	//协议标识
INT16U HF_ausManufacturer[16];	//生产商
INT16U HF_ausProductModel[16];	//Model型号
INT16U HF_ausFirmwareVer[16];	//软件版本
INT16U HF_ausProductSn[16];		//序列号
INT16U HF_ausFacilityReserved[10];//保留
INT16U HF_ausProtocolNum[10];	//协议号
INT16U HF_ausProtocolVer[10];	//协议版本

//电网信息
INT16U HF_ausGridLineVolt[HF_PHASE_NBR];	//电网线电压
INT16U HF_ausGridPhaseVolt[HF_PHASE_NBR];	//电网相电压
INT16U HF_usGridFreq;					//电网频率

//发电信息
INT16U HF_ausGridCurr[HF_PHASE_NBR];		//电网相电流
INT16S HF_lActivePower[2];	    			//输出有功功率
INT16S HF_lReactivePower[2];				//输出无功功率
INT16U HF_ulApparentPower[2];				//输出视在功率
INT16S HF_sDailyElecGeneration;			//当天发电量
INT16U HF_ulTotalElecGeneration[2];			//总发电量
INT16S HF_sDailyReactiveElecGen;			//当天无功电量
INT16U HF_ulTotalReactiveElecGen[2];		//总无功电量
INT16S HF_sPowerFactor;					//功率因数

//逆变器信息
INT16U HF_usCurrentRunningState;			//主运行模式
INT16U HF_usSubCurrentRunningState;		//子运行模式
INT16S HF_sEvirTmp1;						//箱体内部温度1
INT16S HF_sEvirTmp2;						//箱体内部温度2
INT16S HF_sEvirTmp3;						//箱体内部温度2
INT16S HF_sBstHeatsinkTmp;				//BST散热片温度
INT16S HF_sInvHeatsinkTmp;				//INV散热片温度
INT16S HF_sResvHeatsinkTmp;				//其他散热片温度
INT16S HF_sBstChokeTmp;					//Bst电感温度
INT16S HF_sInvChokeTmp;					//Inv电感温度
INT16S HF_sResvChokeTmp;					//其他电感温度
INT16S HF_sOtherTmp;					//其他电感温度
INT16S HF_sInvReserved[4];				//保留
INT16S HF_sIsoValue;						//绝缘阻抗
INT16U HF_usFanStatus;					//风扇状态
INT16U HF_usPwrDeratedRunning;			//是否降额运行中
HF_ALARM_DISPLAY_INFO_STR HF_strLatestAlmDisplayInfo;		//故障告警显示信息
INT16U HF_usWarningCode;					//告警代码

//PV输入信息
INT16U HF_usMpptNbr;						//MPPT数量
INT16U HF_usPvInputConnectionMode;		//PV输入模式
INT16U HF_usPvArrayFaultInfo;			//组件故障信息
HF_PV_ARRAY_DISPLAY_INFO_STR HF_astrPvArrayDisplayInfo[8];

//故障信息
HF_ALARM_DISPLAY_INFO_STR HF_astrAlarmDsiplayInfo[20];

//发电量日志信息
INT16S HF_asDailyElecGenLog[31];		//日发电量日志
HF_Half_Power_STR HF_aulMonthlyElecGenLog[12];	//月发电量日志
HF_Half_Power_STR HF_aulYearlyElecGenLog[5];	//年发电量日志

//严重故障数据日志
INT16U HF_ausCriticalFltDataLog[20];	//严重故障日志

//调试录播信息
INT16S HF_sDcBusVolt;				//直流母线电压
INT16U HF_usStartUpCountDown;		//开机倒计时
INT16S HF_sInvDcCurr[HF_PHASE_NBR];		//逆变直流分量
INT16S HF_sLeakageCurr;				//对地漏电流
//INT16S HF_sDcNegBusVolt;
//04 function code end

//Below is function code 0x03\0x06\0x10
INT16U HF_ausFc03SetFlag[2];		//设置/控制标志  resvered

//通讯参数
INT16U HF_usFc03CommSetEn;		//通讯设置使能  resvered
INT16U HF_usModbusAddr;			//Modbus地址
INT16U HF_usBaudRate;			//波特率
INT16U HF_ausCommResv[6];		//resvered

//国家设置
INT16U HF_ausCountryCodeSetEn[2];		//国家码设置使能 resvered
INT16U HF_usCountry;             		//国家/电网法规
INT16U HF_usLanguage;					//语言
INT16U HF_afGridVoltProtThresHigh[3];		//电网电压保护上限值
INT16U HF_afGridVoltProtThresLow[3];		//电网电压保护下限值
INT16U HF_fGridVoltReconnectThresHigh;		//电网电压重连上限值
INT16U HF_fGridVoltReconnectThresLow;		//电网电压重连下限值
INT16U HF_ausGridVoltProtTimeHigh[3];	//电网电压保护上限时间
INT16U HF_ausGridVoltProtTimeLow[3];		//电网电压保护下限时间
INT16U HF_usGridVoltFltRecoverTime;		//电网电压恢复时间
INT16U HF_afGridFreqProtThresHigh[3];		//电网频率保护上限值
INT16U HF_afGridFreqProtThresLow[3];		//电网频率保护下限值
INT16U HF_fGridFreqReconnectThresHigh;		//电网频率重连上限值
INT16U HF_fGridFreqReconnectThresLow;		//电网频率重连下限值
INT16U HF_ausGridFreqProtTimeHigh[3];	//电网频率保护上限时间
INT16U HF_ausGridFreqProtTimeLow[3];		//电网频率保护下限时间
INT16U HF_usGridFreqFltRecoverTime;		//电网频率恢复时间
INT16U HF_afInvDcCurrProtThres[2];			//逆变直流分量保护值
INT16U HF_ausInvDcCurrChkTime[2];		//逆变直流分量检查时间
INT16U HF_usGridProtectLevel;			//电网保护等级/电网类型
INT16U HF_usPowerOnDelayTime;			//延迟时间
INT16U HF_fGrid10MinAvgOverVoltThres;		//电网10Min过压保护值
INT16U HF_ausCountryCodeResv[20];		//resvered

//关键数据设置
INT16U HF_ausKeyDataSetEn[2];			//关键数据设置使能 resvered
INT16U HF_usFcParamRecovery;				//恢复出厂默认值
INT16U HF_usExistingDataClear;			//历史记录数据清除
INT16U HF_usPvInputConnectionModeSet;		//PV输入模式设置
INT16S HF_sTotalElecGenCompVal;			//总发电量补偿值
INT16U HF_usGridConnectionMode;			//并网控制设置

//通用设置
INT16U HF_ausSystemTime[3];				//系统时间
INT16U HF_ausTimingOnOffEn[2];			//定时开关机使能 resvered
INT16U HF_usTimingOnTime;				//定时开机时间
INT16U HF_usTimingOffTime;				//定时关机时间
INT16U HF_usInvOnOffCmd;					//命令开\关机

//无功功率控制/电网支撑设置
INT16U HF_ausReactPowerSetEn[2];			//逆变器功率设置使能 resvered
INT16U HF_usReactPwrCtrlResv1;			//reserved
INT16U HF_fMaxOutputPwrPercent;			//最大输出功率百分比
INT16U HF_usReactPwrCtrlResv2;			//resvered
INT16U HF_usReactPwrCtrlResv3;			//resvered
INT16U HF_fPwrRampUpPercent;				//Reconnection power ramping up slop percent per minute
INT16U HF_usReactPwrModeEn;				//无功功率控制模式 EN/DIS
INT16S HF_fPowerFactorSetVal;				//功率因数PF设置值
INT16S HF_fReactPwrQPercent;				//无功功率Q百分比
INT16U HF_fReactPwrReguSpeed;				//无功调节速率
INT16S HF_asReactPwrCtrlResv[6];			//resvered
INT16U HF_usFreqWattModeEn;						//Freq-Watt En/Dis
INT16U HF_ausFreqWattCurve[HF_CURVE_NBR];			//Freq-Watt curve
INT16U HF_usVoltWattModeEn;						//Volt-Watt En/Dis
INT16U HF_ausVoltWattCurve[HF_CURVE_NBR];			//Volt-Watt curve
INT16U HF_usPfWattModeEn;						//PF-Watt En/Dis
INT16U HF_ausPfWattCurve[HF_CURVE_NBR];				//PF-Watt curve
INT16U HF_usVoltVarModeEn;						//Volt-Var En/Dis
INT16U HF_ausVoltVarCurve[HF_CURVE_NBR];			//Volt-Var curve
INT16U HF_usVarWattModeEn;						//Var-Watt En/Dis
INT16U HF_ausVarWattCurve[HF_CURVE_NBR];			//Var-Watt curve
INT16U HF_usResvModeEn;							//resvered Mode En/Dis
INT16U HF_ausResvCurve[HF_CURVE_NBR];				//resvered curve
INT16U HF_usLVRTCurveEn;							//LVRT(曲线)使能
INT16U HF_ausLVRTCurveSetResv[HF_CURVE_NBR];		//resvered
INT16U HF_usHVRTCurveEn;							//HVRT(曲线)使能
INT16U HF_ausHVRTCurveSetResv[HF_CURVE_NBR];		//resvered
INT16U HF_usLFRTCurveEn;							//LFRT(曲线)使能
INT16U HF_ausLFRTCurveSetResv[HF_CURVE_NBR];		//resvered
INT16U HF_usHFRTCurveEn;							//HFRT(曲线)使能
INT16U HF_ausHFRTCurveSetResv[HF_CURVE_NBR];		//resvered

//保护/监测功能使能
INT16U HF_ausProtSelfChkFuncEn[2];		//逆变器功率设置使能/屏蔽 resvered
INT16U HF_usActiveAntiIslandingEn;		//主动孤岛(AI)使能/屏蔽
INT16U HF_usGfciEn;						//GFCI保护使能/屏蔽
INT16U HF_usIsoCheckEn;					//ISO检测使能/屏蔽
INT16U HF_usDcInjectionEn;				//DCI保护使能/屏蔽
INT16U HF_usGridVoltProtThresAutoAdjEn;	//电网过压保护阈值自动调节使能/屏蔽
INT16U HF_usGridVoltHighPwrDeratedEn;	//电网过压降额使能/屏蔽
INT16U HF_usGlobalMpptScanEn;			//Global MPPT SCAN使能/屏蔽
INT16U HF_usPvVICurveEn;					//PV-VI曲线使能/屏蔽
INT16U HF_usFastStartupEn;				//自检使能/屏蔽(快速开机)
INT16U HF_usPidFuncEn;					//PID使能/屏蔽
INT16U HF_usDrmFuncEn;					//DRM使能/屏蔽
INT16U HF_usArcDetectEn;					//ARC检测使能/屏蔽
INT16U HF_usArcResetEn;					//ARC复位
INT16U HF_usReconnectPwrRampUpEn; 		//电网故障重连功率缓启
INT16U HF_usAntiRefluxPwrPercent; 		//防逆流功率百分比
INT16U HF_ausProtEnResv[8];				//Reserved


const PARAM_CFG_TBL_STR HF_astrParamSendTblAttr[PARAM_NUM] =
{
  //设备描述
 {0x0000,    0x0000,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausFc04ProtocolID[0]},
 {0x0001,    0x0001,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausFc04ProtocolID[1]},
 {0x0002,    0x0002,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausManufacturer[0]},
 {0x0003,    0x0003,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausManufacturer[1]},
 {0x0004,    0x0004,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausManufacturer[2]},
 {0x0005,    0x0005,    0,    65535,			1,    SEND_TYPE_16BIT,    &HF_ausManufacturer[3]},
 {0x0006,    0x0006,    0,    65535,			1,    SEND_TYPE_16BIT,    &HF_ausManufacturer[4]},
 {0x0007,    0x0007,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausManufacturer[5]},
 {0x0008,    0x0008,    0,    65535,			1,    SEND_TYPE_16BIT,    &HF_ausManufacturer[6]},
 {0x0009,    0x0009,    0,    65535,			1,    SEND_TYPE_16BIT,    &HF_ausManufacturer[7]},
 {0x000A,    0x000A,    0,    65535,			1,    SEND_TYPE_16BIT,    &HF_ausManufacturer[8]},
 {0x000B,    0x000B,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausManufacturer[9]},
 {0x000C,    0x000C,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausManufacturer[10]},
 {0x000D,    0x000D,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausManufacturer[11]},
 {0x000E,    0x000E,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausManufacturer[12]},
 {0x000F,    0x000F,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausManufacturer[13]},
 {0x0010,    0x0010,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausManufacturer[14]},
 {0x0011,    0x0011,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausManufacturer[15]},
 
 {0x0012,    0x0012,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductModel[0]},
 {0x0013,    0x0013,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductModel[1]},
 {0x0014,    0x0014,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductModel[2]},
 {0x0015,    0x0015,    0,    65535,			1,    SEND_TYPE_16BIT,    &HF_ausProductModel[3]},
 {0x0016,    0x0016,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductModel[4]},
 {0x0017,    0x0017,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductModel[5]},
 {0x0018,    0x0018,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductModel[6]},
 {0x0019,    0x0019,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductModel[7]},
 {0x001A,    0x001A,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductModel[8]},
 {0x001B,    0x001B,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductModel[9]},
 {0x001C,    0x001C,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductModel[10]},
 {0x001D,    0x001D,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductModel[11]},
 {0x001E,    0x001E,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductModel[12]},
 {0x001F,    0x001F,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductModel[13]},
 {0x0020,    0x0020,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductModel[14]},
 {0x0021,    0x0021,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductModel[15]},
 
 {0x0022,    0x0022,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausFirmwareVer[0]},
 {0x0023,    0x0023,    0,    65535,			1,    SEND_TYPE_16BIT,    &HF_ausFirmwareVer[1]},
 {0x0024,    0x0024,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausFirmwareVer[2]},
 {0x0025,    0x0025,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausFirmwareVer[3]},
 {0x0026,    0x0026,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausFirmwareVer[4]},
 {0x0027,    0x0027,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausFirmwareVer[5]},
 {0x0028,    0x0028,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausFirmwareVer[6]},
 {0x0029,    0x0029,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausFirmwareVer[7]},
 {0x002A,    0x002A,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausFirmwareVer[8]},
 {0x002B,    0x002B,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausFirmwareVer[9]},
 {0x002C,    0x002C,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausFirmwareVer[10]},
 {0x002D,    0x002D,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausFirmwareVer[11]},
 {0x002E,    0x002E,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausFirmwareVer[12]},
 {0x002F,    0x002F,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausFirmwareVer[13]},
 {0x0030,    0x0030,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausFirmwareVer[14]},
 {0x0031,    0x0031,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausFirmwareVer[15]},
 
 {0x0032,    0x0032,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductSn[0]},
 {0x0033,    0x0033,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductSn[1]},
 {0x0034,    0x0034,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductSn[2]},
 {0x0035,    0x0035,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductSn[3]},
 {0x0036,    0x0036,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductSn[4]},
 {0x0037,    0x0037,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductSn[5]},
 {0x0038,    0x0038,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductSn[6]},
 {0x0039,    0x0039,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductSn[7]},
 {0x003A,    0x003A,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductSn[8]},
 {0x003B,    0x003B,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductSn[9]},
 {0x003C,    0x003C,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductSn[10]},
 {0x003D,    0x003D,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductSn[11]},
 {0x003E,    0x003E,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductSn[12]},
 {0x003F,    0x003F,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductSn[13]},
 {0x0040,    0x0040,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductSn[14]},
 {0x0041,    0x0041,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausProductSn[15]},
 
 {0x0042,		 0x0042,		0,		65535,			1,	  SEND_TYPE_16BIT, 	  &HF_ausFacilityReserved[0]},
 {0x0043,		 0x0043,		0,		65535,			1,	  SEND_TYPE_16BIT, 	  &HF_ausFacilityReserved[1]},
 {0x0044,		 0x0044,		0,		65535,			1,	  SEND_TYPE_16BIT, 	  &HF_ausFacilityReserved[2]},
 {0x0045,		 0x0045,		0,		65535,			1,	  SEND_TYPE_16BIT, 	  &HF_ausFacilityReserved[3]},
 {0x0046,		 0x0046,		0,		65535,			1,	  SEND_TYPE_16BIT, 	  &HF_ausFacilityReserved[4]},
 {0x0047,		 0x0047,		0,		65535,			1,	  SEND_TYPE_16BIT, 	  &HF_ausFacilityReserved[5]},
 {0x0048,		 0x0048,		0,		65535,			1,	  SEND_TYPE_16BIT, 	  &HF_ausFacilityReserved[6]},
 {0x0049,		 0x0049,		0,		65535,			1,	  SEND_TYPE_16BIT, 	  &HF_ausFacilityReserved[7]},
 {0x004A,		 0x004A,		0,		65535,			1,	  SEND_TYPE_16BIT, 	  &HF_ausFacilityReserved[8]},
 {0x004B,		 0x004B,		0,		65535,			1,	  SEND_TYPE_16BIT, 	  &HF_ausFacilityReserved[9]},

 
 {0x004C,	 	 0x004C,		0,	  65535,			1,	  SEND_TYPE_16BIT,	  &HF_ausProtocolNum[0]},
 {0x004D,	   0x004D,		0,	  65535,			1,	  SEND_TYPE_16BIT,	  &HF_ausProtocolNum[1]},
 {0x004E,		 0x004E,		0,	  65535,			1,	  SEND_TYPE_16BIT,	  &HF_ausProtocolNum[2]},
 {0x004F,	 	 0x004F,		0,	  65535,			1,	  SEND_TYPE_16BIT,	  &HF_ausProtocolNum[3]},
 {0x0050,	   0x0050,		0,	  65535,			1,	  SEND_TYPE_16BIT,	  &HF_ausProtocolNum[4]},
 {0x0051,	 	 0x0051,		0,	  65535,			1,	  SEND_TYPE_16BIT,	  &HF_ausProtocolNum[5]},
 {0x0052,	   0x0052,		0,	  65535,			1,	  SEND_TYPE_16BIT,	  &HF_ausProtocolNum[6]},
 {0x0053,	   0x0053,		0,	  65535,			1,	  SEND_TYPE_16BIT,	  &HF_ausProtocolNum[7]},
 {0x0054,	   0x0054,		0,	  65535,			1,	  SEND_TYPE_16BIT,	  &HF_ausProtocolNum[8]},
 {0x0055,	   0x0055,		0,	  65535,			1,	  SEND_TYPE_16BIT,	  &HF_ausProtocolNum[9]},

 {0x0056,	 	 0x0056,		0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &HF_ausProtocolVer[0]},
 {0x0057,	 	 0x0057,		0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &HF_ausProtocolVer[1]},
 {0x0058,	 	 0x0058,		0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &HF_ausProtocolVer[2]},
 {0x0059,	   0x0059,		0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &HF_ausProtocolVer[3]},
 {0x005A,	   0x005A,		0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &HF_ausProtocolVer[4]},
 {0x005B,	   0x005B,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &HF_ausProtocolVer[5]},
 {0x005C,	   0x005C,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &HF_ausProtocolVer[6]},
 {0x005D,	   0x005D,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &HF_ausProtocolVer[7]},
 {0x005E,	   0x005E,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &HF_ausProtocolVer[8]},
 {0x005F,	   0x005F,	  0,	  65535,		  1,	  SEND_TYPE_16BIT,	  &HF_ausProtocolVer[9]},

 //电网信息
 {0x0100,    0x0100,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausGridLineVolt[HF_PHASE_AB]},
 {0x0101,    0x0101,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausGridLineVolt[HF_PHASE_BC]},
 {0x0102,    0x0102,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausGridLineVolt[HF_PHASE_CA]},
 {0x0103,    0x0103,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausGridPhaseVolt[HF_PHASE_A]},
 {0x0104,    0x0104,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausGridPhaseVolt[HF_PHASE_B]},
 {0x0105,    0x0105,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausGridPhaseVolt[HF_PHASE_C]},
 {0x0106,    0x0106,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_usGridFreq},
 //发电信息
 {0x0180,    0x0180,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausGridCurr[HF_PHASE_A]},
 {0x0181,    0x0181,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausGridCurr[HF_PHASE_B]},
 {0x0182,    0x0182,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ausGridCurr[HF_PHASE_C]},
 {0x0183,    0x0183,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_lActivePower[0]},
 {0x0184,    0x0184,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_lActivePower[1]},
 {0x0185,    0x0185,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_lReactivePower[0]},
 {0x0186,    0x0186,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_lReactivePower[1]},
 {0x0187,    0x0187,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ulApparentPower[0]},
 {0x0188,    0x0188,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ulApparentPower[1]},
 {0x0189,    0x0189,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_sDailyElecGeneration},
 {0x018A,    0x018A,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ulTotalElecGeneration[0]},
 {0x018B,    0x018B,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ulTotalElecGeneration[1]},
 {0x018C,    0x018C,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_sDailyReactiveElecGen},
 {0x018D,    0x018D,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ulTotalReactiveElecGen[0]},
 {0x018E,    0x018E,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_ulTotalReactiveElecGen[1]},
 {0x018F,    0x018F,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_sPowerFactor},
 
 //逆变器信息
 {0x0200,    0x0200,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_usCurrentRunningState},
 {0x0201,    0x0201,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_usSubCurrentRunningState},
 {0x0202,    0x0202,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_sEvirTmp1},
 {0x0203,    0x0203,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_sEvirTmp2},
 {0x0204,    0x0204,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_sEvirTmp3},
 {0x0205,    0x0205,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_sBstHeatsinkTmp},
 {0x0206,    0x0206,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_sInvHeatsinkTmp},
 {0x0207,    0x0207,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_sResvHeatsinkTmp},
 {0x0208,    0x0208,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_sBstChokeTmp},
 {0x0209,    0x0209,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_sInvChokeTmp},
 {0x020A,    0x020A,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_sResvChokeTmp},
 {0x020B,    0x020B,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_sOtherTmp},
 {0x020C,    0x020C,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_sInvReserved[0]},
 {0x020D,    0x020D,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_sInvReserved[1]},
 {0x020E,    0x020E,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_sInvReserved[2]},
 {0x020F,    0x020F,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_sInvReserved[3]},
 {0x0210,    0x0210,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_sIsoValue},
 {0x0211,    0x0211,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_usFanStatus},
 {0x0212,    0x0212,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_usPwrDeratedRunning},
 {0x0213,    0x0213,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_strLatestAlmDisplayInfo.data.YearMonth},
 {0x0214,    0x0214,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_strLatestAlmDisplayInfo.data.DateHour},
 {0x0215,    0x0215,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_strLatestAlmDisplayInfo.data.MinuteSecond},
 {0x0216,    0x0216,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_strLatestAlmDisplayInfo.ALARM_TYPE},
 {0x0217,    0x0217,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_strLatestAlmDisplayInfo.ALARM_NUM},
 {0x0218,    0x0218,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_usWarningCode},
 
 //pv输入信息(两个通道，后面保留，所以暂时不完善表格)
 {0x0280,    0x0280,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_usMpptNbr},
 {0x0281,    0x0281,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_usPvInputConnectionMode},
 {0x0282,    0x0282,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_usPvArrayFaultInfo},
 {0x0283,    0x0283,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrPvArrayDisplayInfo[0].INPUT_PV_ID},
 {0x0284,    0x0284,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrPvArrayDisplayInfo[0].PV_INPUT_VOL},
 {0x0285,    0x0285,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrPvArrayDisplayInfo[0].PV_INPUT_CUR},
 {0x0286,    0x0286,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrPvArrayDisplayInfo[0].PV_INPUT_POW[0]},
 {0x0287,    0x0287,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrPvArrayDisplayInfo[0].PV_INPUT_POW[1]},
 {0x0288,    0x0288,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrPvArrayDisplayInfo[0].MODLE_TEMP},
 {0x0289,    0x0289,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrPvArrayDisplayInfo[0].MODLE_ALARM_MESSAGE},
 {0x028A,    0x028A,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrPvArrayDisplayInfo[1].INPUT_PV_ID},
 {0x028B,    0x028B,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrPvArrayDisplayInfo[1].PV_INPUT_VOL},
 {0x028C,    0x028C,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrPvArrayDisplayInfo[1].PV_INPUT_CUR},
 {0x028D,    0x028D,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrPvArrayDisplayInfo[1].PV_INPUT_POW[0]},
 {0x028E,    0x028E,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrPvArrayDisplayInfo[1].PV_INPUT_POW[1]},
 {0x028F,    0x028F,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrPvArrayDisplayInfo[1].MODLE_TEMP},
 {0x0290,    0x0290,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrPvArrayDisplayInfo[1].MODLE_ALARM_MESSAGE},
 
 //故障信息
 {0x0300,    0x0300,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[0].data.YearMonth},
 {0x0301,    0x0301,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[0].data.DateHour},
 {0x0302,    0x0302,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[0].data.MinuteSecond},
 {0x0303,    0x0303,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[0].ALARM_TYPE},
 {0x0304,    0x0304,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[0].ALARM_NUM},
 {0x0305,    0x0305,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[1].data.YearMonth},
 {0x0306,    0x0306,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[1].data.DateHour},
 {0x0307,    0x0307,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[1].data.MinuteSecond},
 {0x0308,    0x0308,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[1].ALARM_TYPE},
 {0x0309,    0x0309,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[1].ALARM_NUM},
 {0x030A,    0x030A,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[2].data.YearMonth},
 {0x030B,    0x030B,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[2].data.DateHour},
 {0x030C,    0x030C,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[2].data.MinuteSecond},
 {0x030D,    0x030D,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[2].ALARM_TYPE},
 {0x030E,    0x030E,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[2].ALARM_NUM},
 {0x030F,    0x030F,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[3].data.YearMonth},
 {0x0310,    0x0310,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[3].data.DateHour},
 {0x0311,    0x0311,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[3].data.MinuteSecond},
 {0x0312,    0x0312,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[3].ALARM_TYPE},
 {0x0313,    0x0313,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[3].ALARM_NUM},
 {0x0314,    0x0314,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[4].data.YearMonth},
 {0x0315,    0x0315,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[4].data.DateHour},
 {0x0316,    0x0316,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[4].data.MinuteSecond},
 {0x0317,    0x0317,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[4].ALARM_TYPE},
 {0x0318,    0x0318,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[4].ALARM_NUM},
 {0x0319,    0x0319,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[5].data.YearMonth},
 {0x031A,    0x031A,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[5].data.DateHour},
 {0x031B,    0x031B,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[5].data.MinuteSecond},
 {0x031C,    0x031C,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[5].ALARM_TYPE},
 {0x031D,    0x031D,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[5].ALARM_NUM},
 {0x031E,    0x031E,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[6].data.YearMonth},
 {0x031F,    0x031F,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[6].data.DateHour},
 {0x0320,    0x0320,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[6].data.MinuteSecond},
 {0x0321,    0x0321,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[6].ALARM_TYPE},
 {0x0322,    0x0322,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[6].ALARM_NUM},
 {0x0323,    0x0323,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[7].data.YearMonth},
 {0x0324,    0x0324,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[7].data.DateHour},
 {0x0325,    0x0325,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[7].data.MinuteSecond},
 {0x0326,    0x0326,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[7].ALARM_TYPE},
 {0x0327,    0x0327,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[7].ALARM_NUM},
 {0x0328,    0x0328,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[8].data.YearMonth},
 {0x0329,    0x0329,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[8].data.DateHour},
 {0x032A,    0x032A,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[8].data.MinuteSecond},
 {0x032B,    0x032B,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[8].ALARM_TYPE},
 {0x032C,    0x032C,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[8].ALARM_NUM},
 {0x032D,    0x032D,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[9].data.YearMonth},
 {0x032E,    0x032E,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[9].data.DateHour},
 {0x032F,    0x032F,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[9].data.MinuteSecond},
 {0x0330,    0x0330,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[9].ALARM_TYPE},
 {0x0331,    0x0331,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[9].ALARM_NUM},
 {0x0332,    0x0332,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[10].data.YearMonth},
 {0x0333,    0x0333,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[10].data.DateHour},
 {0x0334,    0x0334,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[10].data.MinuteSecond},
 {0x0335,    0x0335,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[10].ALARM_TYPE},
 {0x0336,    0x0336,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[10].ALARM_NUM},
 {0x0337,    0x0337,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[11].data.YearMonth},
 {0x0338,    0x0338,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[11].data.DateHour},
 {0x0339,    0x0339,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[11].data.MinuteSecond},
 {0x033A,    0x033A,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[11].ALARM_TYPE},
 {0x033B,    0x033B,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[11].ALARM_NUM},
 {0x033C,    0x033C,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[12].data.YearMonth},
 {0x033D,    0x033D,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[12].data.DateHour},
 {0x033E,    0x033E,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[12].data.MinuteSecond},
 {0x033F,    0x033F,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[12].ALARM_TYPE},
 {0x0340,    0x0340,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[12].ALARM_NUM},
 {0x0341,    0x0341,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[13].data.YearMonth},
 {0x0342,    0x0342,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[13].data.DateHour},
 {0x0343,    0x0343,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[13].data.MinuteSecond},
 {0x0344,    0x0344,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[13].ALARM_TYPE},
 {0x0345,    0x0345,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[13].ALARM_NUM},
 {0x0346,    0x0346,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[14].data.YearMonth},
 {0x0347,    0x0347,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[14].data.DateHour},
 {0x0348,    0x0348,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[14].data.MinuteSecond},
 {0x0349,    0x0349,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[14].ALARM_TYPE},
 {0x034A,    0x034A,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[14].ALARM_NUM},
 {0x034B,    0x034B,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[15].data.YearMonth},
 {0x034C,    0x034C,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[15].data.DateHour},
 {0x034D,    0x034D,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[15].data.MinuteSecond},
 {0x034E,    0x034E,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[15].ALARM_TYPE},
 {0x034F,    0x034F,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[15].ALARM_NUM},
 {0x0350,    0x0350,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[16].data.YearMonth},
 {0x0351,    0x0351,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[16].data.DateHour},
 {0x0352,    0x0352,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[16].data.MinuteSecond},
 {0x0353,    0x0353,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[16].ALARM_TYPE},
 {0x0354,    0x0354,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[16].ALARM_NUM},
 {0x0355,    0x0355,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[17].data.YearMonth},
 {0x0356,    0x0356,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[17].data.DateHour},
 {0x0357,    0x0357,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[17].data.MinuteSecond},
 {0x0358,    0x0358,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[17].ALARM_TYPE},
 {0x0359,    0x0359,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[17].ALARM_NUM},
 {0x035A,    0x035A,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[18].data.YearMonth},
 {0x035B,    0x035B,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[18].data.DateHour},
 {0x035C,    0x035C,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[18].data.MinuteSecond},
 {0x035D,    0x035D,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[18].ALARM_TYPE},
 {0x035E,    0x035E,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[18].ALARM_NUM},
 {0x035F,    0x035F,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[19].data.YearMonth},
 {0x0360,    0x0360,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[19].data.DateHour},
 {0x0361,    0x0361,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[19].data.MinuteSecond},
 {0x0362,    0x0362,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[19].ALARM_TYPE},
 {0x0363,    0x0363,    0,    65535,   		1,    SEND_TYPE_16BIT,    &HF_astrAlarmDsiplayInfo[19].ALARM_NUM}
 
 //发电量日志信息（暂时不显示）
 
};
/*********************************************************************************************
*********************************************************************************************
*********************************************************************************************
*********************************************************************************************
*********************************************************************************************
*********************************************************************************************/

const PARAM_CFG_TBL_STR HF_astrParamCfgTblAttr[PARAM_NUM]=
{
//**********************通讯参数**************************
 {0x2080,    0x2080,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usFc03CommSetEn},
 {0x2081,    0x2081,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usModbusAddr},
 {0x2082,    0x2082,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usBaudRate},
 
 //国家设置
 {0x2100,    0x2100,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausCountryCodeSetEn[0]},
 {0x2101,    0x2101,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausCountryCodeSetEn[1]},
 {0x2102,    0x2102,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usCountry},
 {0x2103,    0x2103,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usLanguage},
 {0x2104,    0x2104,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_afGridVoltProtThresHigh[0]},
 {0x2105,    0x2105,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_afGridVoltProtThresHigh[1]},
 {0x2106,    0x2106,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_afGridVoltProtThresHigh[2]},
 {0x2107,    0x2107,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_afGridVoltProtThresLow[0]},
 {0x2108,    0x2108,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_afGridVoltProtThresLow[1]},
 {0x2109,    0x2109,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_afGridVoltProtThresLow[2]},
 {0x210A,    0x210A,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_fGridVoltReconnectThresHigh},
 {0x210B,    0x210B,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_fGridVoltReconnectThresLow},
 {0x210C,    0x210C,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausGridVoltProtTimeHigh[0]},
 {0x210D,    0x210D,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausGridVoltProtTimeHigh[1]},
 {0x210E,    0x210E,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausGridVoltProtTimeHigh[2]},
 {0x210F,    0x210F,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausGridVoltProtTimeLow[0]},
 {0x2110,    0x2110,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausGridVoltProtTimeLow[1]},
 {0x2111,    0x2111,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausGridVoltProtTimeLow[2]},
 {0x2112,    0x2112,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usGridVoltFltRecoverTime},
 {0x2113,    0x2113,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_afGridFreqProtThresHigh[0]},
 {0x2114,    0x2114,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_afGridFreqProtThresHigh[1]},
 {0x2115,    0x2115,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_afGridFreqProtThresHigh[2]},
 {0x2116,    0x2116,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_afGridFreqProtThresLow[0]},
 {0x2117,    0x2117,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_afGridFreqProtThresLow[1]},
 {0x2118,    0x2118,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_afGridFreqProtThresLow[2]},
 {0x2119,    0x2119,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_fGridFreqReconnectThresHigh},
 {0x211A,    0x211A,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_fGridFreqReconnectThresLow},
 {0x211B,    0x211B,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausGridFreqProtTimeHigh[0]},
 {0x211C,    0x211C,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausGridFreqProtTimeHigh[1]},
 {0x211D,    0x211D,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausGridFreqProtTimeHigh[2]},
 {0x211E,    0x211E,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausGridFreqProtTimeLow[0]},
 {0x211F,    0x211F,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausGridFreqProtTimeLow[1]},
 {0x2120,    0x2120,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausGridFreqProtTimeLow[2]},
 {0x2121,    0x2121,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usGridFreqFltRecoverTime},
 {0x2122,    0x2122,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_afInvDcCurrProtThres[0]},
 {0x2123,    0x2123,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_afInvDcCurrProtThres[1]},
 {0x2124,    0x2124,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausInvDcCurrChkTime[0]},
 {0x2125,    0x2125,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausInvDcCurrChkTime[1]},
 {0x2126,    0x2126,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usGridProtectLevel},
 {0x2127,    0x2127,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usPowerOnDelayTime},
 {0x2128,    0x2128,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_fGrid10MinAvgOverVoltThres},
 {0x2129,    0x2129,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausCountryCodeResv[0]},//20组保留数据
 
 //关键数据设置
 {0x2200,    0x2200,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausKeyDataSetEn[0]},
 {0x2201,    0x2201,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausKeyDataSetEn[1]},
 {0x2202,    0x2202,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usFcParamRecovery},
 {0x2203,    0x2203,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usExistingDataClear},
 {0x2204,    0x2204,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usPvInputConnectionModeSet},
 {0x2205,    0x2205,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_sTotalElecGenCompVal},
 {0x2206,    0x2206,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usGridConnectionMode},
 
 //通用设置
 {0x2280,    0x2280,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausSystemTime[0]},
 {0x2281,    0x2281,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausSystemTime[1]},
 {0x2282,    0x2282,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausSystemTime[2]},
 {0x2283,    0x2283,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausTimingOnOffEn[0]},
 {0x2284,    0x2284,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausTimingOnOffEn[1]},
 {0x2285,    0x2285,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usTimingOnTime},
 {0x2286,    0x2286,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usTimingOffTime},
 {0x2287,    0x2287,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usInvOnOffCmd},

 //功率控制/电网支撑设置
 {0x2300,    0x2300,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausReactPowerSetEn[0]},
 {0x2301,    0x2301,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausReactPowerSetEn[1]},
 {0x2302,    0x2302,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usReactPwrCtrlResv1},
 {0x2303,    0x2303,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_fMaxOutputPwrPercent},
 {0x2304,    0x2304,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usReactPwrCtrlResv2},
 {0x2305,    0x2305,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usReactPwrCtrlResv3},
 {0x2306,    0x2306,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_fPwrRampUpPercent},
 {0x2307,    0x2307,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usReactPwrModeEn},
 {0x2308,    0x2308,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_fPowerFactorSetVal},
 {0x2309,    0x2309,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_fReactPwrQPercent},
 {0x230A,    0x230A,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_fReactPwrReguSpeed},
 {0x230B,    0x230B,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_asReactPwrCtrlResv[0]},
 {0x230C,    0x230C,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_asReactPwrCtrlResv[1]},
 {0x230D,    0x230D,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_asReactPwrCtrlResv[2]},
 {0x230E,    0x230E,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_asReactPwrCtrlResv[3]},
 {0x230F,    0x230F,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_asReactPwrCtrlResv[4]},
 {0x2310,    0x2310,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_asReactPwrCtrlResv[5]},
 {0x2311,    0x2311,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usFreqWattModeEn},
 {0x2312,    0x2312,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausFreqWattCurve[0]},
 {0x2313,    0x2313,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausFreqWattCurve[1]},
 {0x2314,    0x2314,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausFreqWattCurve[2]},
 {0x2315,    0x2315,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausFreqWattCurve[3]},
 {0x2316,    0x2316,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausFreqWattCurve[4]},
 {0x2317,    0x2317,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausFreqWattCurve[5]},
 {0x2318,    0x2318,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausFreqWattCurve[6]},
 {0x2319,    0x2319,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausFreqWattCurve[7]},
 {0x231A,    0x231A,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausFreqWattCurve[8]},
 {0x231B,    0x231B,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausFreqWattCurve[9]},
 {0x231C,    0x231C,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausFreqWattCurve[10]},
 {0x231D,    0x231D,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausFreqWattCurve[11]},
 {0x231E,    0x231E,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausFreqWattCurve[12]},
 {0x231F,    0x231F,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausFreqWattCurve[13]},
 {0x2320,    0x2320,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausFreqWattCurve[14]},
 {0x2321,    0x2321,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausFreqWattCurve[15]},
 {0x2322,    0x2322,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausFreqWattCurve[16]},
 {0x2323,    0x2323,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausFreqWattCurve[17]},
 {0x2324,    0x2324,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausFreqWattCurve[18]},
 {0x2325,    0x2325,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausFreqWattCurve[19]},
 {0x2326,    0x2326,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usVoltWattModeEn},
 {0x2327,    0x2327,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltWattCurve[0]},
 {0x2328,    0x2328,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltWattCurve[1]},
 {0x2329,    0x2329,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltWattCurve[2]},
 {0x232A,    0x232A,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltWattCurve[3]},
 {0x232B,    0x232B,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltWattCurve[4]},
 {0x232C,    0x232C,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltWattCurve[5]},
 {0x232D,    0x232D,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltWattCurve[6]},
 {0x232E,    0x232E,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltWattCurve[7]},
 {0x232F,    0x232F,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltWattCurve[8]},
 {0x2330,    0x2330,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltWattCurve[9]},
 {0x2331,    0x2331,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltWattCurve[10]},
 {0x2332,    0x2332,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltWattCurve[11]},
 {0x2333,    0x2333,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltWattCurve[12]},
 {0x2334,    0x2334,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltWattCurve[13]},
 {0x2335,    0x2335,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltWattCurve[14]},
 {0x2336,    0x2336,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltWattCurve[15]},
 {0x2337,    0x2337,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltWattCurve[16]},
 {0x2338,    0x2338,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltWattCurve[17]},
 {0x2339,    0x2339,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltWattCurve[18]},
 {0x233A,    0x233A,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltWattCurve[19]},
 {0x233B,    0x233B,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usPfWattModeEn},
 {0x233C,    0x233C,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausPfWattCurve[0]},
 {0x233D,    0x233D,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausPfWattCurve[1]},
 {0x233E,    0x233E,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausPfWattCurve[2]},
 {0x233F,    0x233F,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausPfWattCurve[3]},
 {0x2340,    0x2340,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausPfWattCurve[4]},
 {0x2341,    0x2341,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausPfWattCurve[5]},
 {0x2342,    0x2342,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausPfWattCurve[6]},
 {0x2343,    0x2343,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausPfWattCurve[7]},
 {0x2344,    0x2344,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausPfWattCurve[8]},
 {0x2345,    0x2345,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausPfWattCurve[9]},
 {0x2346,    0x2346,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausPfWattCurve[10]},
 {0x2347,    0x2347,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausPfWattCurve[11]},
 {0x2348,    0x2348,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausPfWattCurve[12]},
 {0x2349,    0x2349,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausPfWattCurve[13]},
 {0x234A,    0x234A,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausPfWattCurve[14]},
 {0x234B,    0x234B,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausPfWattCurve[15]},
 {0x234C,    0x234C,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausPfWattCurve[16]},
 {0x234D,    0x234D,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausPfWattCurve[17]},
 {0x234E,    0x234E,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausPfWattCurve[18]},
 {0x234F,    0x234F,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausPfWattCurve[19]},
 {0x2350,    0x2350,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usVoltVarModeEn},
 {0x2351,    0x2351,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltVarCurve[0]},
 {0x2352,    0x2352,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltVarCurve[1]},
 {0x2353,    0x2353,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltVarCurve[2]},
 {0x2354,    0x2354,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltVarCurve[3]},
 {0x2355,    0x2355,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltVarCurve[4]},
 {0x2356,    0x2356,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltVarCurve[5]},
 {0x2357,    0x2357,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltVarCurve[6]},
 {0x2358,    0x2358,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltVarCurve[7]},
 {0x2359,    0x2359,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltVarCurve[8]},
 {0x235A,    0x235A,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltVarCurve[9]},
 {0x235B,    0x235B,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltVarCurve[10]},
 {0x235C,    0x235C,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltVarCurve[11]},
 {0x235D,    0x235D,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltVarCurve[12]},
 {0x235E,    0x235E,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltVarCurve[13]},
 {0x235F,    0x235F,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltVarCurve[14]},
 {0x2360,    0x2360,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltVarCurve[15]},
 {0x2361,    0x2361,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltVarCurve[16]},
 {0x2362,    0x2362,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltVarCurve[17]},
 {0x2363,    0x2363,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltVarCurve[18]},
 {0x2364,    0x2364,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVoltVarCurve[19]},
 {0x2365,    0x2365,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usVarWattModeEn},
 {0x2366,    0x2366,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVarWattCurve[0]},
 {0x2367,    0x2367,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVarWattCurve[1]},
 {0x2368,    0x2368,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVarWattCurve[2]},
 {0x2369,    0x2369,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVarWattCurve[3]},
 {0x236A,    0x236A,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVarWattCurve[4]},
 {0x236B,    0x236B,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVarWattCurve[5]},
 {0x236C,    0x236C,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVarWattCurve[6]},
 {0x236D,    0x236D,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVarWattCurve[7]},
 {0x236E,    0x236E,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVarWattCurve[8]},
 {0x236F,    0x236F,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVarWattCurve[9]},
 {0x2370,    0x2370,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVarWattCurve[10]},
 {0x2371,    0x2371,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVarWattCurve[11]},
 {0x2372,    0x2372,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVarWattCurve[12]},
 {0x2373,    0x2373,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVarWattCurve[13]},
 {0x2374,    0x2374,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVarWattCurve[14]},
 {0x2375,    0x2375,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVarWattCurve[15]},
 {0x2376,    0x2376,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVarWattCurve[16]},
 {0x2377,    0x2377,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVarWattCurve[17]},
 {0x2378,    0x2378,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVarWattCurve[18]},
 {0x2379,    0x2379,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausVarWattCurve[19]},
 {0x237A,    0x237A,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usResvModeEn},
 {0x237B,    0x237B,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausResvCurve[0]},
 {0x237C,    0x237C,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausResvCurve[1]},
 {0x237D,    0x237D,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausResvCurve[2]},
 {0x237E,    0x237E,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausResvCurve[3]},
 {0x237F,    0x237F,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausResvCurve[4]},
 {0x2380,    0x2380,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausResvCurve[5]},
 {0x2381,    0x2381,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausResvCurve[6]},
 {0x2382,    0x2382,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausResvCurve[7]},
 {0x2383,    0x2383,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausResvCurve[8]},
 {0x2384,    0x2384,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausResvCurve[9]},
 {0x2385,    0x2385,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausResvCurve[10]},
 {0x2386,    0x2386,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausResvCurve[11]},
 {0x2387,    0x2387,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausResvCurve[12]},
 {0x2388,    0x2388,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausResvCurve[13]},
 {0x2389,    0x2389,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausResvCurve[14]},
 {0x238A,    0x238A,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausResvCurve[15]},
 {0x238B,    0x238B,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausResvCurve[16]},
 {0x238C,    0x238C,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausResvCurve[17]},
 {0x238D,    0x238D,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausResvCurve[18]},
 {0x238E,    0x238E,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausResvCurve[19]},
 {0x238F,    0x238F,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usLVRTCurveEn},
 {0x2390,    0x2390,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLVRTCurveSetResv[0]},
 {0x2391,    0x2391,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLVRTCurveSetResv[1]},
 {0x2392,    0x2392,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLVRTCurveSetResv[2]},
 {0x2393,    0x2393,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLVRTCurveSetResv[3]},
 {0x2394,    0x2394,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLVRTCurveSetResv[4]},
 {0x2395,    0x2395,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLVRTCurveSetResv[5]},
 {0x2396,    0x2396,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLVRTCurveSetResv[6]},
 {0x2397,    0x2397,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLVRTCurveSetResv[7]},
 {0x2398,    0x2398,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLVRTCurveSetResv[8]},
 {0x2399,    0x2399,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLVRTCurveSetResv[9]},
 {0x239A,    0x239A,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLVRTCurveSetResv[10]},
 {0x239B,    0x239B,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLVRTCurveSetResv[11]},
 {0x239C,    0x239C,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLVRTCurveSetResv[12]},
 {0x239D,    0x239D,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLVRTCurveSetResv[13]},
 {0x239E,    0x239E,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLVRTCurveSetResv[14]},
 {0x239F,    0x239F,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLVRTCurveSetResv[15]},
 {0x23A0,    0x23A0,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLVRTCurveSetResv[16]},
 {0x23A1,    0x23A1,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLVRTCurveSetResv[17]},
 {0x23A2,    0x23A2,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLVRTCurveSetResv[18]},
 {0x23A3,    0x23A3,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLVRTCurveSetResv[19]},
 {0x23A4,    0x23A4,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usHVRTCurveEn},
 {0x23A5,    0x23A5,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHVRTCurveSetResv[0]},
 {0x23A6,    0x23A6,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHVRTCurveSetResv[1]},
 {0x23A7,    0x23A7,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHVRTCurveSetResv[2]},
 {0x23A8,    0x23A8,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHVRTCurveSetResv[3]},
 {0x23A9,    0x23A9,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHVRTCurveSetResv[4]},
 {0x23AA,    0x23AA,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHVRTCurveSetResv[5]},
 {0x23AB,    0x23AB,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHVRTCurveSetResv[6]},
 {0x23AC,    0x23AC,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHVRTCurveSetResv[7]},
 {0x23AD,    0x23AD,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHVRTCurveSetResv[8]},
 {0x23AE,    0x23AE,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHVRTCurveSetResv[9]},
 {0x23AF,    0x23AF,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHVRTCurveSetResv[10]},
 {0x23B0,    0x23B0,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHVRTCurveSetResv[11]},
 {0x23B1,    0x23B1,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHVRTCurveSetResv[12]},
 {0x23B2,    0x23B2,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHVRTCurveSetResv[13]},
 {0x23B3,    0x23B3,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHVRTCurveSetResv[14]},
 {0x23B4,    0x23B4,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHVRTCurveSetResv[15]},
 {0x23B5,    0x23B5,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHVRTCurveSetResv[16]},
 {0x23B6,    0x23B6,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHVRTCurveSetResv[17]},
 {0x23B7,    0x23B7,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHVRTCurveSetResv[18]},
 {0x23B8,    0x23B8,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHVRTCurveSetResv[19]},
 {0x23B9,    0x23B9,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usLFRTCurveEn},
 {0x23BA,    0x23BA,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLFRTCurveSetResv[0]},
 {0x23BB,    0x23BB,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLFRTCurveSetResv[1]},
 {0x23BC,    0x23BC,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLFRTCurveSetResv[2]},
 {0x23BD,    0x23BD,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLFRTCurveSetResv[3]},
 {0x23BE,    0x23BE,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLFRTCurveSetResv[4]},
 {0x23BF,    0x23BF,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLFRTCurveSetResv[5]},
 {0x23C0,    0x23C0,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLFRTCurveSetResv[6]},
 {0x23C1,    0x23C1,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLFRTCurveSetResv[7]},
 {0x23C2,    0x23C2,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLFRTCurveSetResv[8]},
 {0x23C3,    0x23C3,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLFRTCurveSetResv[9]},
 {0x23C4,    0x23C4,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLFRTCurveSetResv[10]},
 {0x23C5,    0x23C5,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLFRTCurveSetResv[11]},
 {0x23C6,    0x23C6,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLFRTCurveSetResv[12]},
 {0x23C7,    0x23C7,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLFRTCurveSetResv[13]},
 {0x23C8,    0x23C8,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLFRTCurveSetResv[14]},
 {0x23C9,    0x23C9,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLFRTCurveSetResv[15]},
 {0x23CA,    0x23CA,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLFRTCurveSetResv[16]},
 {0x23CB,    0x23CB,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLFRTCurveSetResv[17]},
 {0x23CC,    0x23CC,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLFRTCurveSetResv[18]},
 {0x23CD,    0x23CD,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausLFRTCurveSetResv[19]},
 {0x23CE,    0x23CE,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usHFRTCurveEn},
 {0x23CF,    0x23CF,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHFRTCurveSetResv[0]},
 {0x23D0,    0x23D0,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHFRTCurveSetResv[1]},
 {0x23D1,    0x23D1,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHFRTCurveSetResv[2]},
 {0x23D2,    0x23D2,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHFRTCurveSetResv[3]},
 {0x23D3,    0x23D3,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHFRTCurveSetResv[4]},
 {0x23D4,    0x23D4,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHFRTCurveSetResv[5]},
 {0x23D5,    0x23D5,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHFRTCurveSetResv[6]},
 {0x23D6,    0x23D6,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHFRTCurveSetResv[7]},
 {0x23D7,    0x23D7,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHFRTCurveSetResv[8]},
 {0x23D8,    0x23D8,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHFRTCurveSetResv[9]},
 {0x23D9,    0x23D9,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHFRTCurveSetResv[10]},
 {0x23DA,    0x23DA,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHFRTCurveSetResv[11]},
 {0x23DB,    0x23DB,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHFRTCurveSetResv[12]},
 {0x23DC,    0x23DC,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHFRTCurveSetResv[13]},
 {0x23DD,    0x23DD,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHFRTCurveSetResv[14]},
 {0x23DE,    0x23DE,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHFRTCurveSetResv[15]},
 {0x23DF,    0x23DF,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHFRTCurveSetResv[16]},
 {0x23E0,    0x23E0,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHFRTCurveSetResv[17]},
 {0x23E1,    0x23E1,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHFRTCurveSetResv[18]},
 {0x23E2,    0x23E2,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausHFRTCurveSetResv[19]},
 
 //保护/检测功能使能
 {0x2480,    0x2480,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausProtSelfChkFuncEn[0]},
 {0x2481,    0x2481,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_ausProtSelfChkFuncEn[1]},
 {0x2482,    0x2482,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usActiveAntiIslandingEn},
 {0x2483,    0x2483,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usGfciEn},
 {0x2484,    0x2484,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usIsoCheckEn},
 {0x2485,    0x2485,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usDcInjectionEn},
 {0x2486,    0x2486,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usGridVoltProtThresAutoAdjEn},
 {0x2487,    0x2487,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usGridVoltHighPwrDeratedEn},
 {0x2488,    0x2488,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usGlobalMpptScanEn},
 {0x2489,    0x2489,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usPvVICurveEn},
 {0x248A,    0x248A,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usFastStartupEn},
 {0x248B,    0x248B,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usPidFuncEn},
 {0x248C,    0x248C,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usDrmFuncEn},
 {0x248D,    0x248D,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usArcDetectEn},
 {0x248E,    0x248E,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usArcResetEn},
 {0x248F,    0x248F,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usReconnectPwrRampUpEn},
 {0x2490,    0x2490,    0,	  65535,	1,    CFG_TYPE_U16,       &HF_usAntiRefluxPwrPercent}
};


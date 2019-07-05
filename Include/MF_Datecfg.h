#ifndef _MF_DATECFG_H_
#define _MF_DATECFG_H_
#include "config.h"
extern const PARAM_CFG_TBL_STR g_astrParamCfgTblAttr[PARAM_NUM];
extern const PARAM_CFG_TBL_STR g_astrParamSendTblAttr[PARAM_NUM];

#define PHASE_AB  0
#define PHASE_BC  1
#define PHASE_CA  2
#define PHASE_A  0
#define PHASE_B  1
#define PHASE_C  2
#define CURVE_NBR 20
#define PHASE_NBR 3
#define CALIB_PV_NBR 8
typedef struct
{
	INT16U ALARM_NUM;
	struct
	{
		INT16U YearMonth;
		INT16U DateHour;
		INT16U MinuteSecond;
	}data;	
}ALARM_DISPLAY_INFO_STR;
typedef struct
	{
		INT16U YearMonth;
		INT16U DateHour;
		INT16U MinuteSecond;
	}PRE_ALARM_TIME;	


typedef struct
{
	INT16U INPUT_PV_ID;
	INT16U PV_INPUT_VOL;
	INT16U PV_INPUT_CUR;
	INT16U PV_INPUT_POW;
	INT16U RUN_MODLE;
	INT16S MODLE_TEMP;
	INT16U MODLE_ALARM_MESSAGE;
}PV_ARRAY_DISPLAY_INFO_STR;

typedef struct
{
	INT16S PV_VOL_CAL_RATIO;
	INT16S PV_VOL_CAL_OFFSET;
	INT16S PV_CUR_CAL_RATIO;
	INT16S PV_CUR_CAL_OFFSET;
	
}CALIB_COEFF_PV_STR;

typedef struct
{
	INT16U Limit_Value;
	INT16U Limit_Time;
	INT16U Trip_Value;
	INT16U Trip_Time;
}ITALY_SELF_TEST_STR;

typedef struct
{
	INT16U Trip_Value;
	INT16U Trip_Time;
	INT16U Step_Result;
}ITALY_RESULT_STR;
//设备描述
extern INT16U MF_ausFc04ProtocolID[2];	//协议标识
extern INT16U MF_ausDeviceStartIDTAG;  //设备开始标识TAG  	
extern INT16U MF_ausStartIDLength;  		//标识长度
extern INT16U MF_ausManufacturer[16];	//生产商
extern INT16U MF_ausProductModel[16];	//Model型号
extern INT16U MF_ausFacilityReserved[8];//保留
extern INT16U MF_ausFirmwareVer[16];	//软件版本
extern INT16U MF_ausProductSn[16];		//序列号
extern INT16U MF_ausProtocolNum[10];	//协议号
extern INT16U MF_ausProtocolVer[10];	//协议版本
//电网信息
extern INT16U MF_ausGridLogicModuleID;        //逻辑功能模块标识
extern INT16U MF_ausGridModuleLength;          //模块长度
extern INT16U MF_ausGridPhaseCur[PHASE_NBR];	//电网相电流
extern INT16U MF_ausGridLineVolt[PHASE_NBR];	//电网线电压
extern INT16U MF_ausGridPhaseVolt[PHASE_NBR];	//电网相电压
extern INT16U MF_ausGridFreq;					//电网频率
extern INT16U MF_outActivePower[2];	    			//输出有功功率
extern INT32U MF_outApparentPower;				//输出视在功率
extern INT32S MF_outReactivePower;				//输出无功功率
extern INT16S MF_sDailyElecGeneration;			//当天发电量
extern INT16U MF_ulTotalElecGeneration[2];			//总发电量
extern INT32U MF_ulTotalReactiveElecGen;		//总无功电量
extern INT16S MF_sPowerFactor;					//功率因数
extern INT16U MF_DCVol;         		 		//直流电压
extern INT16U MF_DCCur;            		//直流电流
extern INT16U MF_inActivePower;	    			//输入有功功率
extern INT16S MF_sEvirTmp1;						//箱体内部温度
extern INT16S MF_sHeatsinkTmp;				//散热片温度
extern INT16S MF_sChokeTmp;					//电感温度
extern INT16S MF_sResvTmp;					//其他温度
extern INT16U MF_usCurrentRunningState;			//主运行模式
extern INT16U MF_usSubCurrentRunningState;		//子运行模式
extern INT16U MF_sInvReserved1[5];				//保留
extern PRE_ALARM_TIME MF_PreAlarmTime;     //最近一次故障时间戳
extern INT16U MF_sInvReserved2[2];				//保留
extern INT16U MF_EventAlarmInfo;        //事件故障信息
extern INT16U MF_sResv;						//reserved
extern INT16U MF_YearElecGeneration;       //年发电量
extern INT16U MF_ausResvVal[7];					//reserved
//****************PV输入信息**************************
extern INT16U MF_ausPVLogicModuleID;        //逻辑功能模块标识
extern INT16U MF_ausPVModuleLength;          //模块长度
extern INT16U MF_usMpptNbr;						//MPPT数量
extern INT16U MF_usPvInputConnectionMode;		//PV输入模式
extern INT16U MF_usPvArrayFaultInfo;			//组件故障信息
extern PV_ARRAY_DISPLAY_INFO_STR MF_astrPvArrayDisplayInfo[3];   //模块事件故障信息
extern INT16U MF_ausDeviceEndIDTAG;  //设备开始标识TAG  	
extern INT16U MF_ausEndIDLength;  		//标识长度
//****************故障信息**************************
extern ALARM_DISPLAY_INFO_STR MF_astrAlarmDsiplayInfo[20];
//**********************意大利自检**************************
extern INT16U MF_SelfTestState;     //self-test State
extern ITALY_SELF_TEST_STR MF_SelfTest[8];
extern INT16U MF_TEST_TIME[3];
extern ITALY_RESULT_STR MF_ItalyResult[8];
//**********************通讯参数**************************
extern INT16U MF_ausFc03SetFlag[2];		//设置/控制标志  
extern INT16U MF_ausCommSetIDTAG;  //通讯设置标识TAG  	
extern INT16U MF_ausCommSetIDLength;  		//标识长度
extern INT16U MF_usFc03CommSetEn;		//通讯设置使能  resvered
extern INT16U MF_usModbusAddr;			//Modbus地址
extern INT16U MF_ausCommModelResv;		//resvered
extern INT16U MF_usBaudRate;			//波特率
extern INT16U MF_ausCommResv[4];		//resvered
//**********************关键参数设置**************************
extern INT16U MF_ausParamSetLogicModuleID;        //逻辑功能模块标识
extern INT16U MF_ausParamSetModuleLength;          //模块长度
extern INT16U MF_ausCountryLanguageSetEn[2];		//国家语言设置使能 resvered
extern INT16U MF_usCountry;             		//国家/电网法规
extern INT16U MF_usLanguage;							//语言
extern INT16U MF_InvResetParamEn[2];      //逆变器RESET参数使能
extern INT16U MF_InvResetParam;            //参数恢复默认值
extern INT16U MF_usExistingDataClearEn[2];  //历史记录数据清除使能
extern INT16U MF_usExistingDataClear;			//历史记录数据清除
extern INT16U MF_usPvInputConnectionModeEn[2];		//PV输入模式设置使能
extern INT16U MF_usPvInputConnectionModeSet;		//PV输入模式设置
extern INT16U MF_sTotalElecGenCompValEn[2];			//总发电量补偿值使能
extern INT16S MF_sTotalElecGenCompVal;			//总发电量补偿值
extern INT16U MF_InvFastControlSetEn[2];   //逆变器快速控制/设置使能
extern INT16U MF_ausSystemTime[3];				//系统时间
extern INT16U MF_usTimingOnControl;				//定时开机控制
extern INT16U MF_usTimingOffControl;				//定时关机控制
extern INT16U MF_usInvOnOffCmd;					//命令开\关机
extern INT16U MF_WaitStandupTime;					//开机延迟
extern INT16U MF_usActPwrModeEn;				//有功功率控制模式 EN/DIS
extern INT16U MF_usActPwrSetValue;				//有功功率设置值
extern INT16U MF_FirstConnectGridPwrAdjustRate;  //初次并网功率调整速率
extern INT16U MF_FaultRstReconnectPwrAdjustRate;  //故障恢复重连功率调整速率
extern INT16U MF_usActPwrControlAdjustRate;    //有功功率控制调节速率
extern INT16U MF_usReactPwrModeEn;				//无功功率控制模式 EN/DIS
extern INT16S MF_PowerFactorSetVal;				//功率因数设置值
extern INT16U MF_PFAdjustRate;         //PF调节速率
extern INT16S MF_usReactPwrSetValue;				//无功功率设置值
extern INT16U MF_ItalyOutsideCommFlag;    //意大利外部通信标识
extern INT16U MF_afGridVoltProtThresHigh[3];		//电网电压保护上限值
extern INT16U MF_afGridVoltProtThresLow[3];		//电网电压保护下限值
extern INT16U MF_ausGridVoltProtTimeHigh[3];	//电网电压保护上限时间
extern INT16U MF_ausGridVoltProtTimeLow[3];		//电网电压保护下限时间
extern INT16U MF_usGridVoltFltRecoverTime;		//电网电压恢复时间
extern INT16U MF_afGridFreqProtThresHigh[3];		//电网频率保护上限值
extern INT16U MF_afGridFreqProtThresLow[3];		//电网频率保护下限值
extern INT16U MF_ausGridFreqProtTimeHigh[3];	//电网频率保护上限时间
extern INT16U MF_ausGridFreqProtTimeLow[3];		//电网频率保护下限时间
extern INT16U MF_usGridFreqFltRecoverTime;		//电网频率恢复时间
extern INT16U MF_usWattFreqModeEn;						//Watt-Freq En/Dis
extern INT16U MF_usWattVoltModeEn;						//Watt-Volt En/Dis
extern INT16U MF_usPfWattModeEn;						//PF-Watt En/Dis
extern INT16U MF_usVarVoltModeEn;						//Var-Volt En/Dis
extern INT16U MF_usVarWattModeEn;						//Var-Watt En/Dis
extern INT16U MF_usResvModeEn;							//resvered Mode En/Dis
extern INT16U MF_usLVRTCurveEn;							//LVRT(曲线)使能
extern INT16U MF_usHVRTCurveEn;							//HVRT(曲线)使能
extern INT16U MF_usLFRTCurveEn;							//LFRT(曲线)使能
extern INT16U MF_usHFRTCurveEn;							//HFRT(曲线)使能
extern INT16U MF_usGridVoltProtThresAutoAdjEn;	//电网过压保护阈值自动调节使能/屏蔽
extern INT16U MF_usGridVoltHighPwrDeratedEn;	//电网过压降额使能/屏蔽
extern INT16U MF_usGlobalMpptScanEn;			//Global MPPT SCAN使能/屏蔽
extern INT16U MF_usPvVICurveEn;					//PV-VI曲线使能/屏蔽
extern INT16U MF_usItalySelfTestEn;				//ItalySelfTest使能
extern INT16U MF_usPidFuncEn;					//PID使能/屏蔽
extern INT16U MF_usDrmFuncEn;					//DRM使能/屏蔽
extern INT16U MF_usArcDetectEn;					//ARC检测使能/屏蔽
extern INT16U MF_usArcResetEn;					//ARC复位
extern INT16U MF_usAIFuncEn;					//AI使能/屏蔽
extern INT16U MF_usGFCIFuncEn;					//GFCI使能/屏蔽
extern INT16U MF_usISOFuncEn;					//ISO使能/屏蔽
extern INT16U MF_usDCIFuncEn;					//DCI使能/屏蔽
extern INT16U MF_ausPARAMResv1;				//Reserved
extern INT16S MF_wZeroExpPercent;	   //wZeroExpPercent
extern INT16U MF_ausProtEnResv[3];				//Reserved
extern INT16U MF_ausCommSetEndIDTAG;  //设备结束标识TAG  	
extern INT16U MF_ausCommSetEndIDLength;  		//标识长度	

#endif

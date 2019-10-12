#ifndef _HF_DATECFG_H_
#define _HF_DATECFG_H_
#include "config.h"

extern const PARAM_CFG_TBL_STR HF_astrParamCfgTblAttr[PARAM_NUM];
extern const PARAM_CFG_TBL_STR HF_astrParamSendTblAttr[PARAM_NUM];

#define HF_PHASE_AB  0
#define HF_PHASE_BC  1
#define HF_PHASE_CA  2
#define HF_PHASE_A  0
#define HF_PHASE_B  1
#define HF_PHASE_C  2
#define HF_CURVE_NBR 20
#define HF_PHASE_NBR 3
#define HF_CALIB_PV_NBR 8

typedef struct
{
	struct
	{
		INT16U YearMonth;
		INT16U DateHour;
		INT16U MinuteSecond;
	}data;	
	INT16U ALARM_TYPE;
	INT16U ALARM_NUM;

}HF_ALARM_DISPLAY_INFO_STR;

typedef struct
{
	INT16U INPUT_PV_ID;
	INT16S PV_INPUT_VOL;
	INT16S PV_INPUT_CUR;
	INT16S PV_INPUT_POW[2];
	INT16S MODLE_TEMP;
	INT16U MODLE_ALARM_MESSAGE;
}HF_PV_ARRAY_DISPLAY_INFO_STR;

typedef struct
{
	INT16S PV_VOL_CAL_RATIO;
	INT16S PV_VOL_CAL_OFFSET;
	INT16S PV_CUR_CAL_RATIO;
	INT16S PV_CUR_CAL_OFFSET;
	
}HF_CALIB_COEFF_PV_STR;

typedef struct
{
	INT16U Power_H;
	INT16U Power_L;
	
}HF_Half_Power_STR;

typedef struct
{
	INT16U Limit_Value;
	INT16U Limit_Time;
	INT16U Trip_Value;
	INT16U Trip_Time;
}HF_ITALY_SELF_TEST_STR;

typedef struct
{
	INT16U Trip_Value;
	INT16U Trip_Time;
	INT16U Step_Result;
}HF_ITALY_RESULT_STR;

//设备描述
extern INT16U HF_ausFc04ProtocolID[2];	//协议标识
extern INT16U HF_ausManufacturer[16];	//生产商
extern INT16U HF_ausProductModel[16];	//Model型号
extern INT16U HF_ausFirmwareVer[16];	//软件版本
extern INT16U HF_ausProductSn[16];		//序列号
extern INT16U HF_ausFacilityReserved[10];//保留
extern INT16U HF_ausProtocolNum[10];	//协议号
extern INT16U HF_ausProtocolVer[10];	//协议版本

//电网信息
extern INT16U HF_ausGridLineVolt[HF_PHASE_NBR];	//电网线电压
extern INT16U HF_ausGridPhaseVolt[HF_PHASE_NBR];	//电网相电压
extern INT16U HF_usGridFreq;					//电网频率

//发电信息
extern INT16U HF_ausGridCurr[HF_PHASE_NBR];		//电网相电流
extern INT16S HF_lActivePower[2];	    			//输出有功功率
extern INT16S HF_lReactivePower[2];				//输出无功功率
extern INT16U HF_ulApparentPower[2];				//输出视在功率
extern INT16S HF_sDailyElecGeneration;			//当天发电量
extern INT16U HF_ulTotalElecGeneration[2];			//总发电量
extern INT16S HF_sDailyReactiveElecGen;			//当天无功电量
extern INT16U HF_ulTotalReactiveElecGen[2];		//总无功电量
extern INT16S HF_sPowerFactor;					//功率因数

//逆变器信息
extern INT16U HF_usCurrentRunningState;			//主运行模式
extern INT16U HF_usSubCurrentRunningState;		//子运行模式
extern INT16S HF_sEvirTmp1;						//箱体内部温度1
extern INT16S HF_sEvirTmp2;						//箱体内部温度2
extern INT16S HF_sEvirTmp3;						//箱体内部温度2
extern INT16S HF_sBstHeatsinkTmp;				//BST散热片温度
extern INT16S HF_sInvHeatsinkTmp;				//INV散热片温度
extern INT16S HF_sResvHeatsinkTmp;				//其他散热片温度
extern INT16S HF_sBstChokeTmp;					//Bst电感温度
extern INT16S HF_sInvChokeTmp;					//Inv电感温度
extern INT16S HF_sResvChokeTmp;					//其他电感温度
extern INT16S HF_sOtherTmp;					//其他电感温度
extern INT16S HF_sInvReserved[4];				//保留
extern INT16S HF_sIsoValue;						//绝缘阻抗
extern INT16U HF_usFanStatus;					//风扇状态
extern INT16U HF_usPwrDeratedRunning;			//是否降额运行中
extern HF_ALARM_DISPLAY_INFO_STR HF_strLatestAlmDisplayInfo;		//故障告警显示信息
extern INT16U HF_usWarningCode;					//告警代码

//PV输入信息
extern INT16U HF_usMpptNbr;						//MPPT数量
extern INT16U HF_usPvInputConnectionMode;		//PV输入模式
extern INT16U HF_usPvArrayFaultInfo;			//组件故障信息
extern HF_PV_ARRAY_DISPLAY_INFO_STR HF_astrPvArrayDisplayInfo[8];

//故障信息
extern HF_ALARM_DISPLAY_INFO_STR HF_astrAlarmDsiplayInfo[20];

//发电量日志信息
extern INT16S HF_asDailyElecGenLog[31];		//日发电量日志
extern HF_Half_Power_STR HF_aulMonthlyElecGenLog[12];	//月发电量日志
extern HF_Half_Power_STR HF_aulYearlyElecGenLog[5];	//年发电量日志

//严重故障数据日志
extern INT16U HF_ausCriticalFltDataLog[20];	//严重故障日志

//**********************意大利自检**************************
extern INT16U HF_SelfTestState;     //self-test State
extern HF_ITALY_SELF_TEST_STR HF_SelfTest[8];
extern INT16U HF_TEST_TIME[3];
extern HF_ITALY_RESULT_STR HF_ItalyResult[8];

//调试录播信息
extern INT16S HF_sDcBusVolt;				//直流母线电压
extern INT16U HF_usStartUpCountDown;		//开机倒计时
extern INT16S HF_sInvDcCurr[HF_PHASE_NBR];		//逆变直流分量
extern INT16S HF_sLeakageCurr;				//对地漏电流
//INT16S HF_sDcNegBusVolt;
//04 function code end

//Below is function code 0x03\0x06\0x10
extern INT16U HF_ausFc03SetFlag[2];		//设置/控制标志  resvered

//通讯参数
extern INT16U HF_usFc03CommSetEn;		//通讯设置使能  resvered
extern INT16U HF_usModbusAddr;			//Modbus地址
extern INT16U HF_usBaudRate;			//波特率
extern INT16U HF_ausCommResv[6];		//resvered

//国家设置
extern INT16U HF_ausCountryCodeSetEn[2];		//国家码设置使能 resvered
extern INT16U HF_usCountry;             		//国家/电网法规
extern INT16U HF_usLanguage;					//语言
extern INT16U HF_afGridVoltProtThresHigh[3];		//电网电压保护上限值
extern INT16U HF_afGridVoltProtThresLow[3];		//电网电压保护下限值
extern INT16U HF_fGridVoltReconnectThresHigh;		//电网电压重连上限值
extern INT16U HF_fGridVoltReconnectThresLow;		//电网电压重连下限值
extern INT16U HF_ausGridVoltProtTimeHigh[3];	//电网电压保护上限时间
extern INT16U HF_ausGridVoltProtTimeLow[3];		//电网电压保护下限时间
extern INT16U HF_usGridVoltFltRecoverTime;		//电网电压恢复时间
extern INT16U HF_afGridFreqProtThresHigh[3];		//电网频率保护上限值
extern INT16U HF_afGridFreqProtThresLow[3];		//电网频率保护下限值
extern INT16U HF_fGridFreqReconnectThresHigh;		//电网频率重连上限值
extern INT16U HF_fGridFreqReconnectThresLow;		//电网频率重连下限值
extern INT16U HF_ausGridFreqProtTimeHigh[3];	//电网频率保护上限时间
extern INT16U HF_ausGridFreqProtTimeLow[3];		//电网频率保护下限时间
extern INT16U HF_usGridFreqFltRecoverTime;		//电网频率恢复时间
extern INT16U HF_afInvDcCurrProtThres[2];			//逆变直流分量保护值
extern INT16U HF_ausInvDcCurrChkTime[2];		//逆变直流分量检查时间
extern INT16U HF_usGridProtectLevel;			//电网保护等级/电网类型
extern INT16U HF_usPowerOnDelayTime;			//延迟时间
extern INT16U HF_fGrid10MinAvgOverVoltThres;		//电网10Min过压保护值
extern INT16U HF_ausCountryCodeResv[20];		//resvered

//关键数据设置
extern INT16U HF_ausKeyDataSetEn[2];			//关键数据设置使能 resvered
extern INT16U HF_usFcParamRecovery;				//恢复出厂默认值
extern INT16U HF_usExistingDataClear;			//历史记录数据清除
extern INT16U HF_usPvInputConnectionModeSet;		//PV输入模式设置
extern INT16S HF_sTotalElecGenCompVal;			//总发电量补偿值
extern INT16U HF_usGridConnectionMode;			//并网控制设置

//通用设置
extern INT16U HF_ausSystemTime[3];				//系统时间
extern INT16U HF_ausTimingOnOffEn[2];			//定时开关机使能 resvered
extern INT16U HF_usTimingOnTime;				//定时开机时间
extern INT16U HF_usTimingOffTime;				//定时关机时间
extern INT16U HF_usInvOnOffCmd;					//命令开\关机

//无功功率控制/电网支撑设置
extern INT16U HF_ausReactPowerSetEn[2];			//逆变器功率设置使能 resvered
extern INT16U HF_usReactPwrCtrlResv1;			//reserved
extern INT16U HF_fMaxOutputPwrPercent;			//最大输出功率百分比
extern INT16U HF_usReactPwrCtrlResv2;			//resvered
extern INT16U HF_usReactPwrCtrlResv3;			//resvered
extern INT16U HF_fPwrRampUpPercent;				//Reconnection power ramping up slop percent per minute
extern INT16U HF_usReactPwrModeEn;				//无功功率控制模式 EN/DIS
extern INT16S HF_fPowerFactorSetVal;				//功率因数PF设置值
extern INT16S HF_fReactPwrQPercent;				//无功功率Q百分比
extern INT16U HF_fReactPwrReguSpeed;				//无功调节速率
extern INT16S HF_asReactPwrCtrlResv[6];			//resvered
extern INT16U HF_usFreqWattModeEn;						//Freq-Watt En/Dis
extern INT16U HF_ausFreqWattCurve[HF_CURVE_NBR];			//Freq-Watt curve
extern INT16U HF_usVoltWattModeEn;						//Volt-Watt En/Dis
extern INT16U HF_ausVoltWattCurve[HF_CURVE_NBR];			//Volt-Watt curve
extern INT16U HF_usPfWattModeEn;						//PF-Watt En/Dis
extern INT16U HF_ausPfWattCurve[HF_CURVE_NBR];				//PF-Watt curve
extern INT16U HF_usVoltVarModeEn;						//Volt-Var En/Dis
extern INT16U HF_ausVoltVarCurve[HF_CURVE_NBR];			//Volt-Var curve
extern INT16U HF_usVarWattModeEn;						//Var-Watt En/Dis
extern INT16U HF_ausVarWattCurve[HF_CURVE_NBR];			//Var-Watt curve
extern INT16U HF_usResvModeEn;							//resvered Mode En/Dis
extern INT16U HF_ausResvCurve[HF_CURVE_NBR];				//resvered curve
extern INT16U HF_usLVRTCurveEn;							//LVRT(曲线)使能
extern INT16U HF_ausLVRTCurveSetResv[HF_CURVE_NBR];		//resvered
extern INT16U HF_usHVRTCurveEn;							//HVRT(曲线)使能
extern INT16U HF_ausHVRTCurveSetResv[HF_CURVE_NBR];		//resvered
extern INT16U HF_usLFRTCurveEn;							//LFRT(曲线)使能
extern INT16U HF_ausLFRTCurveSetResv[HF_CURVE_NBR];		//resvered
extern INT16U HF_usHFRTCurveEn;							//HFRT(曲线)使能
extern INT16U HF_ausHFRTCurveSetResv[HF_CURVE_NBR];		//resvered

//保护/监测功能使能
extern INT16U HF_ausProtSelfChkFuncEn[2];		//逆变器功率设置使能/屏蔽 resvered
extern INT16U HF_usActiveAntiIslandingEn;		//主动孤岛(AI)使能/屏蔽
extern INT16U HF_usGfciEn;						//GFCI保护使能/屏蔽
extern INT16U HF_usIsoCheckEn;					//ISO检测使能/屏蔽
extern INT16U HF_usDcInjectionEn;				//DCI保护使能/屏蔽
extern INT16U HF_usGridVoltProtThresAutoAdjEn;	//电网过压保护阈值自动调节使能/屏蔽
extern INT16U HF_usGridVoltHighPwrDeratedEn;	//电网过压降额使能/屏蔽
extern INT16U HF_usGlobalMpptScanEn;			//Global MPPT SCAN使能/屏蔽
extern INT16U HF_usPvVICurveEn;					//PV-VI曲线使能/屏蔽
extern INT16U HF_usFastStartupEn;				//自检使能/屏蔽(快速开机)
extern INT16U HF_usPidFuncEn;					//PID使能/屏蔽
extern INT16U HF_usDrmFuncEn;					//DRM使能/屏蔽
extern INT16U HF_usArcDetectEn;					//ARC检测使能/屏蔽
extern INT16U HF_usArcResetEn;					//ARC复位
extern INT16U HF_usReconnectPwrRampUpEn; 		//电网故障重连功率缓启
extern INT16U HF_usAntiRefluxPwrPercent; 		//防逆流功率百分比
extern INT16U HF_ausProtEnResv[8];				//Reserved

extern INT16U HF_IAPState;  		//IAP状态


#endif


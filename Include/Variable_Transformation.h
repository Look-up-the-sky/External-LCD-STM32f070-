#ifndef _VARIABLE_TRANSFORMATION_H_
#define _VARIABLE_TRANSFORMATION_H_
#include "TypeDefine.h"
#include "MF_Datecfg.h"
#include "HF_Datecfg.h"
/*********************屏幕显示变量赋值****************/
typedef union
{
	INT8U all[1];
	struct
	{
	  INT8U ausProductModel[32];	//Model型号
		INT8U ausFirmwareVer[32];	//软件版本
		INT8U ausProductSn[32];		//序列号
		INT8U ausProtocolNum[20];	//协议号
		INT8U ausProtocolVer[20];	//协议版本
	}data;
}INFO_t;
typedef struct
	{
		INT16U ausGridPhaseCur[3];	//电网相电流
		INT16U ausGridLineVolt[3];	//电网线电压
		INT16U ausGridPhaseVolt[3];	//电网相电压
		INT16U ausGridFreq;					//电网频率
		INT32S outActivePower;	    			//输出有功功率
		INT32U outApparentPower;				//输出视在功率
		INT32S outReactivePower;				//输出无功功率
		INT16S sDailyElecGeneration;			//当天发电量
		INT32U ulTotalElecGeneration;			//总发电量
		INT32U ulTotalReactiveElecGen;		//总无功电量
		INT16S sPowerFactor;					//功率因数
		INT16U DCVol;         		 		//直流电压
		INT16U DCCur;            		//直流电流
		INT16U inActivePower;	    			//输入有功功率
		INT16S sEvirTmp1;						//箱体内部温度
		INT16S sHeatsinkTmp;				//散热片温度
		INT16S sChokeTmp;					//电感温度
		INT16S sResvTmp;					//其他温度
	}GRID_INFO;
	
typedef struct
	{
		INT16U usMpptNbr;						//MPPT数量
		INT16U usPvInputConnectionMode;		//PV输入模式
		HF_PV_ARRAY_DISPLAY_INFO_STR astrPvArrayDisplayInfo[3];   //模块事件故障信息	
	}PV_INFO;
	
typedef struct
	{
		INT16U Trip_Value;					
		INT16U Trip_Time;		
		INT16U State;		
	}AutoTest_INFO;
	
typedef struct
	{
		INT16U TEST_TIME[3];
		ITALY_RESULT_STR ItalyResult[8];	
	}AutoTestResult_INFO;
typedef struct
{
	INT16U* WattFreqModeEn;						//Watt-Freq En/Dis
	INT16U* WattVoltModeEn;						//Watt-Volt En/Dis
	INT16U* PfWattModeEn;						//PF-Watt En/Dis
	INT16U* VarVoltModeEn;						//Var-Volt En/Dis
	INT16U* VarWattModeEn;						//Var-Watt En/Dis
	INT16U* ResvModeEn;							//resvered Mode En/Dis
	INT16U* LVRTCurveEn;							//LVRT(曲线)使能
	INT16U* HVRTCurveEn;							//HVRT(曲线)使能
	INT16U* LFRTCurveEn;							//LFRT(曲线)使能
	INT16U* HFRTCurveEn;							//HFRT(曲线)使能
}Curve_Enable_INFO;

typedef struct
{
	//INT16U* ProtSelfChkFuncEn[2];		//逆变器功率设置使能/屏蔽 resvered
	INT16U* ActiveAntiIslandingEn;		//主动孤岛(AI)使能/屏蔽
	INT16U* GfciEn;						//GFCI保护使能/屏蔽
	INT16U* IsoCheckEn;					//ISO检测使能/屏蔽
	INT16U* DcInjectionEn;				//DCI保护使能/屏蔽
	INT16U* GridVoltProtThresAutoAdjEn;	//电网过压保护阈值自动调节使能/屏蔽
	INT16U* GridVoltHighPwrDeratedEn;	//电网过压降额使能/屏蔽
	INT16U* GlobalMpptScanEn;			//Global MPPT SCAN使能/屏蔽
	INT16U* PvVICurveEn;					//PV-VI曲线使能/屏蔽
	INT16U* ItalySelfTestEN;				//自检使能/屏蔽(快速开机)
	INT16U* PidFuncEn;					//PID使能/屏蔽
	INT16U* DrmFuncEn;					//DRM使能/屏蔽
	INT16U* ArcDetectEn;					//ARC检测使能/屏蔽
	INT16U* ArcResetEn;					//ARC复位
	INT16U* ReconnectPwrRampUpEn; 		//电网故障重连功率缓启,黄蜂特殊设置项
	INT16U* AntiRefluxPwrPercent; 		//防逆流功率百分比,黄蜂特殊设置项
	INT16S* ZeroExpPercent;					//蜜蜂特殊设置项
}Protection_Func_Enable_INFO;

typedef struct
{
	/*************蜜蜂参数****************/
	//INT16U* MActPwrModeEn;				//有功功率控制模式 EN/DIS
	INT16U* MActPwrSetValue;				//有功功率设置值
	//INT16U* MFirstConnectGridPwrAdjustRate;  //初次并网功率调整速率
	//INT16U* MFaultRstReconnectPwrAdjustRate;  //故障恢复重连功率调整速率
	//INT16U* MActPwrControlAdjustRate;    //有功功率控制调节速率
	INT16U* MReactPwrModeEn;				//无功功率控制模式 EN/DIS
	INT16S* MPowerFactorSetVal;				//功率因数设置值
	//INT16U* MPFAdjustRate;         //PF调节速率
	INT16S* MReactPwrSetValue;				//无功功率设置值
	INT16U* MItalyOutsideCommFlag;    //意大利外部通信标识
	/*************黄蜂参数****************/
	INT16U* HMaxOutputPwrPercent;			//最大输出功率百分比
	//INT16U* HPwrRampUpPercent;				//Reconnection power ramping up slop percent per minute
	INT16U* HReactPwrModeEn;				//无功功率控制模式 EN/DIS
	INT16S* HPowerFactorSetVal;				//功率因数PF设置值
	INT16S* HReactPwrQPercent;				//无功功率Q百分比
	//INT16U* HF_fReactPwrReguSpeed;				//无功调节速率
}Param_Set_INFO;

typedef struct
{
	INT16U* FcParamRecovery;				//恢复出厂默认值
  INT16U* ExistingDataClear;			//历史记录数据清除
  INT16U* PvInputConnectionMode;		//PV输入模式设置
  INT16S* TotalElecGenCompVal;			//总发电量补偿值
}Key_Data_INFO;

typedef struct
{
	INT16U* InvOnOffCmd;					//命令开\关机
	INT16U* WaitStandupTime;					//开机延迟
}INV_Set_INFO;

extern INFO_t Info_t;
extern GRID_INFO Grid_Info;
extern PV_INFO Pv_Info;
extern AutoTest_INFO AutoTest_Info;
extern AutoTestResult_INFO AutoTestResult_Info;
extern Curve_Enable_INFO Curve_Enable_Info;
extern Protection_Func_Enable_INFO Protection_Func_Enable_Info;
extern Param_Set_INFO Param_Set_Info;
extern Key_Data_INFO Key_Data_Info;
extern INV_Set_INFO INV_Set_Info;
extern INT16U IAPState;  		//IAP状态
extern INT16U InvOnOffCmd;  		//开关机状态
extern INT8U System_Time_Change_Flag;
extern INT8U Italy_SelfTest_Flag;
void Variable_Transformation(void);
void sVariableTask(void);

#endif

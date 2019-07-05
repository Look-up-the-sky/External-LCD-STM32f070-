/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Variable_Transformation.c
 * 修改人：沈仕强
 * 修改日期：2019.06.20
 * 描述：变量转换函数
 *******************************************************/
#include "Lib_Communication.h"
#include "Variable_Transformation.h"
#include "I2C.h"


INFO_t Info_t;
GRID_INFO Grid_Info;
PV_INFO Pv_Info;
AutoTest_INFO AutoTest_Info;
AutoTestResult_INFO AutoTestResult_Info;
Curve_Enable_INFO Curve_Enable_Info;
Param_Set_INFO Param_Set_Info;
Protection_Func_Enable_INFO Protection_Func_Enable_Info;
Key_Data_INFO Key_Data_Info;
INV_Set_INFO INV_Set_Info;
INT8U System_Time_Change_Flag = 0;
INT8U Italy_SelfTest_Flag = 0;

/*******************************************************************
 * 函数名称：变量赋值函数
 * 函数输入：
 * 函数返回值：无
 * 函数描述：用于屏幕显示
*********************************************************************/
void Variable_Transformation()
{
  INT8U i;
	if(PRJ_NUMBER == SUNNYBEE)
	{
		for(i = 0;i < 3;i++)
		{
			Grid_Info.ausGridPhaseCur[i] = __REV16(MF_ausGridPhaseCur[i]);
			Grid_Info.ausGridPhaseVolt[i] = __REV16(MF_ausGridPhaseVolt[i]);
		}
		memcpy(&Grid_Info.outActivePower,MF_outActivePower,4);
		Grid_Info.ausGridFreq = __REV16(MF_ausGridFreq);
		Grid_Info.sDailyElecGeneration = __REV16(MF_sDailyElecGeneration);
		memcpy(&Grid_Info.ulTotalElecGeneration,MF_ulTotalElecGeneration,4);
		Grid_Info.sPowerFactor = __REV16(MF_sPowerFactor);
		Grid_Info.sEvirTmp1 = __REV16(MF_sEvirTmp1);
		Grid_Info.sHeatsinkTmp = __REV16(MF_sHeatsinkTmp);
		memcpy(Info_t.data.ausProductModel,MF_ausProductModel,32); //Model型号
		memcpy(Info_t.data.ausFirmwareVer,MF_ausFirmwareVer,32); //软件版本
		memcpy(Info_t.data.ausProductSn,MF_ausProductSn,32); //序列号
		memcpy(Info_t.data.ausProtocolNum,MF_ausProtocolNum,20); //协议号
		memcpy(Info_t.data.ausProtocolVer,MF_ausProtocolVer,20); //协议版本
		Pv_Info.usMpptNbr = __REV16(MF_usMpptNbr);
		Pv_Info.usPvInputConnectionMode =__REV16(MF_usPvInputConnectionMode);
		Pv_Info.astrPvArrayDisplayInfo[0].PV_INPUT_VOL = __REV16(MF_astrPvArrayDisplayInfo[0].PV_INPUT_VOL);
		Pv_Info.astrPvArrayDisplayInfo[1].PV_INPUT_VOL = __REV16(MF_astrPvArrayDisplayInfo[1].PV_INPUT_VOL);
		Pv_Info.astrPvArrayDisplayInfo[0].PV_INPUT_CUR = __REV16(MF_astrPvArrayDisplayInfo[0].PV_INPUT_CUR);
		Pv_Info.astrPvArrayDisplayInfo[1].PV_INPUT_CUR = __REV16(MF_astrPvArrayDisplayInfo[1].PV_INPUT_CUR);
		for(i = 0;i < 20;i++)
		{
			ErrorRecord.data.ErrorMessage[i].data.Year =MF_astrAlarmDsiplayInfo[i].data.YearMonth;
			ErrorRecord.data.ErrorMessage[i].data.Month =MF_astrAlarmDsiplayInfo[i].data.YearMonth>>8;
			ErrorRecord.data.ErrorMessage[i].data.Date =MF_astrAlarmDsiplayInfo[i].data.DateHour;
			ErrorRecord.data.ErrorMessage[i].data.Hour =MF_astrAlarmDsiplayInfo[i].data.DateHour>>8;
			ErrorRecord.data.ErrorMessage[i].data.Minute =MF_astrAlarmDsiplayInfo[i].data.MinuteSecond;
			ErrorRecord.data.ErrorMessage[i].data.Second =MF_astrAlarmDsiplayInfo[i].data.MinuteSecond>>8;
			ErrorRecord.data.ErrorMessage[i].data.ErrorCodeL =MF_astrAlarmDsiplayInfo[i].ALARM_NUM;
		}
		if(System_Time_Change_Flag == 1)   //03变量赋值
		{
			memcpy(SystemTime.all,MF_ausSystemTime,6);    //系统时间
			Safty.data.uwCountry = __REV16(MF_usCountry);      //国家安规
			memcpy(Safty.data.uwFreqHigh,MF_afGridFreqProtThresHigh,4);   //频率上下限
			memcpy(Safty.data.uwFreqHighCnt,MF_ausGridFreqProtTimeHigh,4);
			memcpy(Safty.data.uwFreqLow,MF_afGridFreqProtThresLow,4);
			memcpy(Safty.data.uwFreqLowCnt,MF_ausGridFreqProtTimeLow,4);
			memcpy(Safty.data.uwGridVoltHigh,MF_afGridVoltProtThresHigh,4);   //电压上下限
			memcpy(Safty.data.uwGridVoltHighCnt,MF_ausGridVoltProtTimeHigh,4);
			memcpy(Safty.data.uwGridVoltLow,MF_afGridVoltProtThresLow,4);
			memcpy(Safty.data.uwGridVoltLowCnt,MF_ausGridVoltProtTimeLow,4);
			
			Curve_Enable_Info.WattFreqModeEn = &MF_usWattFreqModeEn; //曲线使能
			Curve_Enable_Info.WattVoltModeEn = &MF_usWattVoltModeEn;
			Curve_Enable_Info.PfWattModeEn = &MF_usPfWattModeEn;
			Curve_Enable_Info.VarVoltModeEn = &MF_usVarVoltModeEn;
			Curve_Enable_Info.VarWattModeEn = &MF_usVarWattModeEn;
			Curve_Enable_Info.ResvModeEn = &MF_usResvModeEn;
			Curve_Enable_Info.LVRTCurveEn = &MF_usLVRTCurveEn;
			Curve_Enable_Info.HVRTCurveEn = &MF_usHVRTCurveEn;
			Curve_Enable_Info.LFRTCurveEn = &MF_usLFRTCurveEn;
			Curve_Enable_Info.HFRTCurveEn = &MF_usHFRTCurveEn;
			Protection_Func_Enable_Info.ActiveAntiIslandingEn = &MF_usAIFuncEn; //保护使能
			Protection_Func_Enable_Info.GfciEn = &MF_usGFCIFuncEn;
			Protection_Func_Enable_Info.IsoCheckEn = &MF_usISOFuncEn;
			Protection_Func_Enable_Info.DcInjectionEn = &MF_usDCIFuncEn;
			Protection_Func_Enable_Info.GridVoltProtThresAutoAdjEn = &MF_usGridVoltProtThresAutoAdjEn;
			Protection_Func_Enable_Info.GridVoltHighPwrDeratedEn = &MF_usGridVoltHighPwrDeratedEn;
			Protection_Func_Enable_Info.GlobalMpptScanEn = &MF_usGlobalMpptScanEn;
			Protection_Func_Enable_Info.PvVICurveEn = &MF_usPvVICurveEn;
			Protection_Func_Enable_Info.ItalySelfTestEN = &MF_usItalySelfTestEn;//自检使能/屏蔽(快速开机)
			Protection_Func_Enable_Info.PidFuncEn = &MF_usPidFuncEn;
			Protection_Func_Enable_Info.DrmFuncEn = &MF_usDrmFuncEn;
			Protection_Func_Enable_Info.ArcDetectEn = &MF_usArcDetectEn;
			Protection_Func_Enable_Info.ArcResetEn = &MF_usArcResetEn;
			Protection_Func_Enable_Info.ZeroExpPercent = &MF_wZeroExpPercent; //蜜蜂特殊设置项，黄蜂无此项。
			Param_Set_Info.MActPwrSetValue = &MF_usActPwrSetValue;   //通用参数设置
			Param_Set_Info.MReactPwrModeEn = &MF_usReactPwrModeEn;
			Param_Set_Info.MPowerFactorSetVal = &MF_PowerFactorSetVal;
			Param_Set_Info.MReactPwrSetValue = &MF_usReactPwrSetValue;
			Param_Set_Info.MItalyOutsideCommFlag = &MF_ItalyOutsideCommFlag;
			Key_Data_Info.FcParamRecovery = &MF_InvResetParam;      //关键数据设置
			Key_Data_Info.ExistingDataClear = &MF_usExistingDataClear;
			Key_Data_Info.PvInputConnectionMode = &MF_usPvInputConnectionModeSet;
			Key_Data_Info.TotalElecGenCompVal = &MF_sTotalElecGenCompVal;
			INV_Set_Info.InvOnOffCmd = &MF_usInvOnOffCmd;
			INV_Set_Info.WaitStandupTime = &MF_WaitStandupTime;
			IicFlag.bit.mubTimeW = 1;
			System_Time_Change_Flag = 0;
		}
		memcpy(AutoTestResult_Info.TEST_TIME,MF_TEST_TIME,6);
		memcpy(AutoTestResult_Info.ItalyResult,MF_ItalyResult,48);
		

	}
	else if(PRJ_NUMBER == HORNET)
	{
		for(i = 0;i < 3;i++)
		{
			Grid_Info.ausGridPhaseCur[i] = __REV16(HF_ausGridCurr[i]);
			Grid_Info.ausGridPhaseVolt[i] = __REV16(HF_ausGridPhaseVolt[i]);
		}
		memcpy(&Grid_Info.outActivePower,HF_lActivePower,4);
		Grid_Info.ausGridFreq = __REV16(HF_usGridFreq);
		Grid_Info.sDailyElecGeneration = __REV16(HF_sDailyElecGeneration);
		memcpy(&Grid_Info.ulTotalElecGeneration,HF_ulTotalElecGeneration,4);
		Grid_Info.sPowerFactor = __REV16(HF_sPowerFactor);	
		Grid_Info.sEvirTmp1 = __REV16(HF_sEvirTmp1);
		Grid_Info.sHeatsinkTmp = __REV16(HF_sBstHeatsinkTmp);
		memcpy(Info_t.data.ausProductModel,HF_ausProductModel,32); //Model型号
		memcpy(Info_t.data.ausFirmwareVer,HF_ausFirmwareVer,32); //软件版本
		memcpy(Info_t.data.ausProductSn,HF_ausProductSn,32); //序列号
		memcpy(Info_t.data.ausProtocolNum,HF_ausProtocolNum,20); //协议号
		memcpy(Info_t.data.ausProtocolVer,HF_ausProtocolVer,20); //协议版本
		Pv_Info.usMpptNbr = __REV16(HF_usMpptNbr);
		Pv_Info.usPvInputConnectionMode =__REV16(HF_usPvInputConnectionMode);
		Pv_Info.astrPvArrayDisplayInfo[0].PV_INPUT_VOL = __REV16(HF_astrPvArrayDisplayInfo[0].PV_INPUT_VOL);
		Pv_Info.astrPvArrayDisplayInfo[1].PV_INPUT_VOL = __REV16(HF_astrPvArrayDisplayInfo[1].PV_INPUT_VOL);
		Pv_Info.astrPvArrayDisplayInfo[0].PV_INPUT_CUR = __REV16(HF_astrPvArrayDisplayInfo[0].PV_INPUT_CUR);
		Pv_Info.astrPvArrayDisplayInfo[1].PV_INPUT_CUR = __REV16(HF_astrPvArrayDisplayInfo[1].PV_INPUT_CUR);
		for(i = 0;i < 20;i++)
		{
			ErrorRecord.data.ErrorMessage[i].data.Year =HF_astrAlarmDsiplayInfo[i].data.YearMonth;
			ErrorRecord.data.ErrorMessage[i].data.Month =HF_astrAlarmDsiplayInfo[i].data.YearMonth>>8;
			ErrorRecord.data.ErrorMessage[i].data.Date =HF_astrAlarmDsiplayInfo[i].data.DateHour;
			ErrorRecord.data.ErrorMessage[i].data.Hour =HF_astrAlarmDsiplayInfo[i].data.DateHour>>8;
			ErrorRecord.data.ErrorMessage[i].data.Minute =HF_astrAlarmDsiplayInfo[i].data.MinuteSecond;
			ErrorRecord.data.ErrorMessage[i].data.Second =HF_astrAlarmDsiplayInfo[i].data.MinuteSecond>>8;
			ErrorRecord.data.ErrorMessage[i].data.ErrorCodeL =HF_astrAlarmDsiplayInfo[i].ALARM_NUM;
		}
		if(System_Time_Change_Flag == 1)  //03变量赋值
		{
			memcpy(SystemTime.all,HF_ausSystemTime,6);    //系统时间
			Safty.data.uwCountry = __REV16(HF_usCountry);      //国家安规
			memcpy(Safty.data.uwFreqHigh,HF_afGridFreqProtThresHigh,4);   //频率上下限
			memcpy(Safty.data.uwFreqHighCnt,HF_ausGridFreqProtTimeHigh,4);
			memcpy(Safty.data.uwFreqLow,HF_afGridFreqProtThresLow,4);
			memcpy(Safty.data.uwFreqLowCnt,HF_ausGridFreqProtTimeLow,4);
			memcpy(Safty.data.uwGridVoltHigh,HF_afGridVoltProtThresHigh,4);   //电压上下限
			memcpy(Safty.data.uwGridVoltHighCnt,HF_ausGridVoltProtTimeHigh,4);
			memcpy(Safty.data.uwGridVoltLow,HF_afGridVoltProtThresLow,4);
			memcpy(Safty.data.uwGridVoltLowCnt,HF_ausGridVoltProtTimeLow,4);
			
			Curve_Enable_Info.WattFreqModeEn = &HF_usFreqWattModeEn;  //曲线使能
			Curve_Enable_Info.WattVoltModeEn = &HF_usVoltWattModeEn;
			Curve_Enable_Info.PfWattModeEn = &HF_usPfWattModeEn;
			Curve_Enable_Info.VarVoltModeEn = &HF_usVoltVarModeEn;
			Curve_Enable_Info.VarWattModeEn = &HF_usVarWattModeEn;
			Curve_Enable_Info.ResvModeEn = &HF_usResvModeEn;
			Curve_Enable_Info.LVRTCurveEn = &HF_usLVRTCurveEn;
			Curve_Enable_Info.HVRTCurveEn = &HF_usHVRTCurveEn;
			Curve_Enable_Info.LFRTCurveEn = &HF_usLFRTCurveEn;
			Curve_Enable_Info.HFRTCurveEn = &HF_usHFRTCurveEn;
			Protection_Func_Enable_Info.ActiveAntiIslandingEn = &HF_usActiveAntiIslandingEn; //保护使能
			Protection_Func_Enable_Info.GfciEn = &HF_usGfciEn;
			Protection_Func_Enable_Info.IsoCheckEn = &HF_usIsoCheckEn;
			Protection_Func_Enable_Info.DcInjectionEn = &HF_usDcInjectionEn;
			Protection_Func_Enable_Info.GridVoltProtThresAutoAdjEn = &HF_usGridVoltProtThresAutoAdjEn;
			Protection_Func_Enable_Info.GridVoltHighPwrDeratedEn = &HF_usGridVoltHighPwrDeratedEn;
			Protection_Func_Enable_Info.GlobalMpptScanEn = &HF_usGlobalMpptScanEn;
			Protection_Func_Enable_Info.PvVICurveEn = &HF_usPvVICurveEn;
			Protection_Func_Enable_Info.ItalySelfTestEN = &HF_usFastStartupEn;//自检使能/屏蔽(快速开机)
			Protection_Func_Enable_Info.PidFuncEn = &HF_usPidFuncEn;
			Protection_Func_Enable_Info.DrmFuncEn = &HF_usDrmFuncEn;
			Protection_Func_Enable_Info.ArcDetectEn = &HF_usArcDetectEn;
			Protection_Func_Enable_Info.ArcResetEn = &HF_usArcResetEn;
			Protection_Func_Enable_Info.ReconnectPwrRampUpEn = &HF_usReconnectPwrRampUpEn;
			Protection_Func_Enable_Info.AntiRefluxPwrPercent = &HF_usAntiRefluxPwrPercent;
			Param_Set_Info.HMaxOutputPwrPercent = &HF_fMaxOutputPwrPercent;   //通用参数设置
			Param_Set_Info.HReactPwrModeEn = &HF_usReactPwrModeEn;
			Param_Set_Info.HPowerFactorSetVal = &HF_fPowerFactorSetVal;
			Param_Set_Info.HReactPwrQPercent = &HF_fReactPwrQPercent;
			Key_Data_Info.FcParamRecovery = &HF_usFcParamRecovery;      //关键数据设置
			Key_Data_Info.ExistingDataClear = &HF_usExistingDataClear;
			Key_Data_Info.PvInputConnectionMode = &HF_usPvInputConnectionModeSet;
			Key_Data_Info.TotalElecGenCompVal = &HF_sTotalElecGenCompVal;
			INV_Set_Info.InvOnOffCmd = &HF_usInvOnOffCmd;
			INV_Set_Info.WaitStandupTime = &HF_usPowerOnDelayTime;  	
			IicFlag.bit.mubTimeW = 1;
			System_Time_Change_Flag = 0;
		}	
	}	
}

void sVariableTask(void)
{
	Variable_Transformation();
}

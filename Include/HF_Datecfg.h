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

//�豸����
extern INT16U HF_ausFc04ProtocolID[2];	//Э���ʶ
extern INT16U HF_ausManufacturer[16];	//������
extern INT16U HF_ausProductModel[16];	//Model�ͺ�
extern INT16U HF_ausFirmwareVer[16];	//����汾
extern INT16U HF_ausProductSn[16];		//���к�
extern INT16U HF_ausFacilityReserved[10];//����
extern INT16U HF_ausProtocolNum[10];	//Э���
extern INT16U HF_ausProtocolVer[10];	//Э��汾

//������Ϣ
extern INT16U HF_ausGridLineVolt[HF_PHASE_NBR];	//�����ߵ�ѹ
extern INT16U HF_ausGridPhaseVolt[HF_PHASE_NBR];	//�������ѹ
extern INT16U HF_usGridFreq;					//����Ƶ��

//������Ϣ
extern INT16U HF_ausGridCurr[HF_PHASE_NBR];		//���������
extern INT16S HF_lActivePower[2];	    			//����й�����
extern INT16S HF_lReactivePower[2];				//����޹�����
extern INT16U HF_ulApparentPower[2];				//������ڹ���
extern INT16S HF_sDailyElecGeneration;			//���췢����
extern INT16U HF_ulTotalElecGeneration[2];			//�ܷ�����
extern INT16S HF_sDailyReactiveElecGen;			//�����޹�����
extern INT16U HF_ulTotalReactiveElecGen[2];		//���޹�����
extern INT16S HF_sPowerFactor;					//��������

//�������Ϣ
extern INT16U HF_usCurrentRunningState;			//������ģʽ
extern INT16U HF_usSubCurrentRunningState;		//������ģʽ
extern INT16S HF_sEvirTmp1;						//�����ڲ��¶�1
extern INT16S HF_sEvirTmp2;						//�����ڲ��¶�2
extern INT16S HF_sEvirTmp3;						//�����ڲ��¶�2
extern INT16S HF_sBstHeatsinkTmp;				//BSTɢ��Ƭ�¶�
extern INT16S HF_sInvHeatsinkTmp;				//INVɢ��Ƭ�¶�
extern INT16S HF_sResvHeatsinkTmp;				//����ɢ��Ƭ�¶�
extern INT16S HF_sBstChokeTmp;					//Bst����¶�
extern INT16S HF_sInvChokeTmp;					//Inv����¶�
extern INT16S HF_sResvChokeTmp;					//��������¶�
extern INT16S HF_sOtherTmp;					//��������¶�
extern INT16S HF_sInvReserved[4];				//����
extern INT16S HF_sIsoValue;						//��Ե�迹
extern INT16U HF_usFanStatus;					//����״̬
extern INT16U HF_usPwrDeratedRunning;			//�Ƿ񽵶�������
extern HF_ALARM_DISPLAY_INFO_STR HF_strLatestAlmDisplayInfo;		//���ϸ澯��ʾ��Ϣ
extern INT16U HF_usWarningCode;					//�澯����

//PV������Ϣ
extern INT16U HF_usMpptNbr;						//MPPT����
extern INT16U HF_usPvInputConnectionMode;		//PV����ģʽ
extern INT16U HF_usPvArrayFaultInfo;			//���������Ϣ
extern HF_PV_ARRAY_DISPLAY_INFO_STR HF_astrPvArrayDisplayInfo[8];

//������Ϣ
extern HF_ALARM_DISPLAY_INFO_STR HF_astrAlarmDsiplayInfo[20];

//��������־��Ϣ
extern INT16S HF_asDailyElecGenLog[31];		//�շ�������־
extern HF_Half_Power_STR HF_aulMonthlyElecGenLog[12];	//�·�������־
extern HF_Half_Power_STR HF_aulYearlyElecGenLog[5];	//�귢������־

//���ع���������־
extern INT16U HF_ausCriticalFltDataLog[20];	//���ع�����־

//**********************������Լ�**************************
extern INT16U HF_SelfTestState;     //self-test State
extern HF_ITALY_SELF_TEST_STR HF_SelfTest[8];
extern INT16U HF_TEST_TIME[3];
extern HF_ITALY_RESULT_STR HF_ItalyResult[8];

//����¼����Ϣ
extern INT16S HF_sDcBusVolt;				//ֱ��ĸ�ߵ�ѹ
extern INT16U HF_usStartUpCountDown;		//��������ʱ
extern INT16S HF_sInvDcCurr[HF_PHASE_NBR];		//���ֱ������
extern INT16S HF_sLeakageCurr;				//�Ե�©����
//INT16S HF_sDcNegBusVolt;
//04 function code end

//Below is function code 0x03\0x06\0x10
extern INT16U HF_ausFc03SetFlag[2];		//����/���Ʊ�־  resvered

//ͨѶ����
extern INT16U HF_usFc03CommSetEn;		//ͨѶ����ʹ��  resvered
extern INT16U HF_usModbusAddr;			//Modbus��ַ
extern INT16U HF_usBaudRate;			//������
extern INT16U HF_ausCommResv[6];		//resvered

//��������
extern INT16U HF_ausCountryCodeSetEn[2];		//����������ʹ�� resvered
extern INT16U HF_usCountry;             		//����/��������
extern INT16U HF_usLanguage;					//����
extern INT16U HF_afGridVoltProtThresHigh[3];		//������ѹ��������ֵ
extern INT16U HF_afGridVoltProtThresLow[3];		//������ѹ��������ֵ
extern INT16U HF_fGridVoltReconnectThresHigh;		//������ѹ��������ֵ
extern INT16U HF_fGridVoltReconnectThresLow;		//������ѹ��������ֵ
extern INT16U HF_ausGridVoltProtTimeHigh[3];	//������ѹ��������ʱ��
extern INT16U HF_ausGridVoltProtTimeLow[3];		//������ѹ��������ʱ��
extern INT16U HF_usGridVoltFltRecoverTime;		//������ѹ�ָ�ʱ��
extern INT16U HF_afGridFreqProtThresHigh[3];		//����Ƶ�ʱ�������ֵ
extern INT16U HF_afGridFreqProtThresLow[3];		//����Ƶ�ʱ�������ֵ
extern INT16U HF_fGridFreqReconnectThresHigh;		//����Ƶ����������ֵ
extern INT16U HF_fGridFreqReconnectThresLow;		//����Ƶ����������ֵ
extern INT16U HF_ausGridFreqProtTimeHigh[3];	//����Ƶ�ʱ�������ʱ��
extern INT16U HF_ausGridFreqProtTimeLow[3];		//����Ƶ�ʱ�������ʱ��
extern INT16U HF_usGridFreqFltRecoverTime;		//����Ƶ�ʻָ�ʱ��
extern INT16U HF_afInvDcCurrProtThres[2];			//���ֱ����������ֵ
extern INT16U HF_ausInvDcCurrChkTime[2];		//���ֱ���������ʱ��
extern INT16U HF_usGridProtectLevel;			//���������ȼ�/��������
extern INT16U HF_usPowerOnDelayTime;			//�ӳ�ʱ��
extern INT16U HF_fGrid10MinAvgOverVoltThres;		//����10Min��ѹ����ֵ
extern INT16U HF_ausCountryCodeResv[20];		//resvered

//�ؼ���������
extern INT16U HF_ausKeyDataSetEn[2];			//�ؼ���������ʹ�� resvered
extern INT16U HF_usFcParamRecovery;				//�ָ�����Ĭ��ֵ
extern INT16U HF_usExistingDataClear;			//��ʷ��¼�������
extern INT16U HF_usPvInputConnectionModeSet;		//PV����ģʽ����
extern INT16S HF_sTotalElecGenCompVal;			//�ܷ���������ֵ
extern INT16U HF_usGridConnectionMode;			//������������

//ͨ������
extern INT16U HF_ausSystemTime[3];				//ϵͳʱ��
extern INT16U HF_ausTimingOnOffEn[2];			//��ʱ���ػ�ʹ�� resvered
extern INT16U HF_usTimingOnTime;				//��ʱ����ʱ��
extern INT16U HF_usTimingOffTime;				//��ʱ�ػ�ʱ��
extern INT16U HF_usInvOnOffCmd;					//���\�ػ�

//�޹����ʿ���/����֧������
extern INT16U HF_ausReactPowerSetEn[2];			//�������������ʹ�� resvered
extern INT16U HF_usReactPwrCtrlResv1;			//reserved
extern INT16U HF_fMaxOutputPwrPercent;			//���������ʰٷֱ�
extern INT16U HF_usReactPwrCtrlResv2;			//resvered
extern INT16U HF_usReactPwrCtrlResv3;			//resvered
extern INT16U HF_fPwrRampUpPercent;				//Reconnection power ramping up slop percent per minute
extern INT16U HF_usReactPwrModeEn;				//�޹����ʿ���ģʽ EN/DIS
extern INT16S HF_fPowerFactorSetVal;				//��������PF����ֵ
extern INT16S HF_fReactPwrQPercent;				//�޹�����Q�ٷֱ�
extern INT16U HF_fReactPwrReguSpeed;				//�޹���������
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
extern INT16U HF_usLVRTCurveEn;							//LVRT(����)ʹ��
extern INT16U HF_ausLVRTCurveSetResv[HF_CURVE_NBR];		//resvered
extern INT16U HF_usHVRTCurveEn;							//HVRT(����)ʹ��
extern INT16U HF_ausHVRTCurveSetResv[HF_CURVE_NBR];		//resvered
extern INT16U HF_usLFRTCurveEn;							//LFRT(����)ʹ��
extern INT16U HF_ausLFRTCurveSetResv[HF_CURVE_NBR];		//resvered
extern INT16U HF_usHFRTCurveEn;							//HFRT(����)ʹ��
extern INT16U HF_ausHFRTCurveSetResv[HF_CURVE_NBR];		//resvered

//����/��⹦��ʹ��
extern INT16U HF_ausProtSelfChkFuncEn[2];		//�������������ʹ��/���� resvered
extern INT16U HF_usActiveAntiIslandingEn;		//�����µ�(AI)ʹ��/����
extern INT16U HF_usGfciEn;						//GFCI����ʹ��/����
extern INT16U HF_usIsoCheckEn;					//ISO���ʹ��/����
extern INT16U HF_usDcInjectionEn;				//DCI����ʹ��/����
extern INT16U HF_usGridVoltProtThresAutoAdjEn;	//������ѹ������ֵ�Զ�����ʹ��/����
extern INT16U HF_usGridVoltHighPwrDeratedEn;	//������ѹ����ʹ��/����
extern INT16U HF_usGlobalMpptScanEn;			//Global MPPT SCANʹ��/����
extern INT16U HF_usPvVICurveEn;					//PV-VI����ʹ��/����
extern INT16U HF_usFastStartupEn;				//�Լ�ʹ��/����(���ٿ���)
extern INT16U HF_usPidFuncEn;					//PIDʹ��/����
extern INT16U HF_usDrmFuncEn;					//DRMʹ��/����
extern INT16U HF_usArcDetectEn;					//ARC���ʹ��/����
extern INT16U HF_usArcResetEn;					//ARC��λ
extern INT16U HF_usReconnectPwrRampUpEn; 		//���������������ʻ���
extern INT16U HF_usAntiRefluxPwrPercent; 		//���������ʰٷֱ�
extern INT16U HF_ausProtEnResv[8];				//Reserved

extern INT16U HF_IAPState;  		//IAP״̬


#endif


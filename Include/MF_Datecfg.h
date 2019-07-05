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
//�豸����
extern INT16U MF_ausFc04ProtocolID[2];	//Э���ʶ
extern INT16U MF_ausDeviceStartIDTAG;  //�豸��ʼ��ʶTAG  	
extern INT16U MF_ausStartIDLength;  		//��ʶ����
extern INT16U MF_ausManufacturer[16];	//������
extern INT16U MF_ausProductModel[16];	//Model�ͺ�
extern INT16U MF_ausFacilityReserved[8];//����
extern INT16U MF_ausFirmwareVer[16];	//����汾
extern INT16U MF_ausProductSn[16];		//���к�
extern INT16U MF_ausProtocolNum[10];	//Э���
extern INT16U MF_ausProtocolVer[10];	//Э��汾
//������Ϣ
extern INT16U MF_ausGridLogicModuleID;        //�߼�����ģ���ʶ
extern INT16U MF_ausGridModuleLength;          //ģ�鳤��
extern INT16U MF_ausGridPhaseCur[PHASE_NBR];	//���������
extern INT16U MF_ausGridLineVolt[PHASE_NBR];	//�����ߵ�ѹ
extern INT16U MF_ausGridPhaseVolt[PHASE_NBR];	//�������ѹ
extern INT16U MF_ausGridFreq;					//����Ƶ��
extern INT16U MF_outActivePower[2];	    			//����й�����
extern INT32U MF_outApparentPower;				//������ڹ���
extern INT32S MF_outReactivePower;				//����޹�����
extern INT16S MF_sDailyElecGeneration;			//���췢����
extern INT16U MF_ulTotalElecGeneration[2];			//�ܷ�����
extern INT32U MF_ulTotalReactiveElecGen;		//���޹�����
extern INT16S MF_sPowerFactor;					//��������
extern INT16U MF_DCVol;         		 		//ֱ����ѹ
extern INT16U MF_DCCur;            		//ֱ������
extern INT16U MF_inActivePower;	    			//�����й�����
extern INT16S MF_sEvirTmp1;						//�����ڲ��¶�
extern INT16S MF_sHeatsinkTmp;				//ɢ��Ƭ�¶�
extern INT16S MF_sChokeTmp;					//����¶�
extern INT16S MF_sResvTmp;					//�����¶�
extern INT16U MF_usCurrentRunningState;			//������ģʽ
extern INT16U MF_usSubCurrentRunningState;		//������ģʽ
extern INT16U MF_sInvReserved1[5];				//����
extern PRE_ALARM_TIME MF_PreAlarmTime;     //���һ�ι���ʱ���
extern INT16U MF_sInvReserved2[2];				//����
extern INT16U MF_EventAlarmInfo;        //�¼�������Ϣ
extern INT16U MF_sResv;						//reserved
extern INT16U MF_YearElecGeneration;       //�귢����
extern INT16U MF_ausResvVal[7];					//reserved
//****************PV������Ϣ**************************
extern INT16U MF_ausPVLogicModuleID;        //�߼�����ģ���ʶ
extern INT16U MF_ausPVModuleLength;          //ģ�鳤��
extern INT16U MF_usMpptNbr;						//MPPT����
extern INT16U MF_usPvInputConnectionMode;		//PV����ģʽ
extern INT16U MF_usPvArrayFaultInfo;			//���������Ϣ
extern PV_ARRAY_DISPLAY_INFO_STR MF_astrPvArrayDisplayInfo[3];   //ģ���¼�������Ϣ
extern INT16U MF_ausDeviceEndIDTAG;  //�豸��ʼ��ʶTAG  	
extern INT16U MF_ausEndIDLength;  		//��ʶ����
//****************������Ϣ**************************
extern ALARM_DISPLAY_INFO_STR MF_astrAlarmDsiplayInfo[20];
//**********************������Լ�**************************
extern INT16U MF_SelfTestState;     //self-test State
extern ITALY_SELF_TEST_STR MF_SelfTest[8];
extern INT16U MF_TEST_TIME[3];
extern ITALY_RESULT_STR MF_ItalyResult[8];
//**********************ͨѶ����**************************
extern INT16U MF_ausFc03SetFlag[2];		//����/���Ʊ�־  
extern INT16U MF_ausCommSetIDTAG;  //ͨѶ���ñ�ʶTAG  	
extern INT16U MF_ausCommSetIDLength;  		//��ʶ����
extern INT16U MF_usFc03CommSetEn;		//ͨѶ����ʹ��  resvered
extern INT16U MF_usModbusAddr;			//Modbus��ַ
extern INT16U MF_ausCommModelResv;		//resvered
extern INT16U MF_usBaudRate;			//������
extern INT16U MF_ausCommResv[4];		//resvered
//**********************�ؼ���������**************************
extern INT16U MF_ausParamSetLogicModuleID;        //�߼�����ģ���ʶ
extern INT16U MF_ausParamSetModuleLength;          //ģ�鳤��
extern INT16U MF_ausCountryLanguageSetEn[2];		//������������ʹ�� resvered
extern INT16U MF_usCountry;             		//����/��������
extern INT16U MF_usLanguage;							//����
extern INT16U MF_InvResetParamEn[2];      //�����RESET����ʹ��
extern INT16U MF_InvResetParam;            //�����ָ�Ĭ��ֵ
extern INT16U MF_usExistingDataClearEn[2];  //��ʷ��¼�������ʹ��
extern INT16U MF_usExistingDataClear;			//��ʷ��¼�������
extern INT16U MF_usPvInputConnectionModeEn[2];		//PV����ģʽ����ʹ��
extern INT16U MF_usPvInputConnectionModeSet;		//PV����ģʽ����
extern INT16U MF_sTotalElecGenCompValEn[2];			//�ܷ���������ֵʹ��
extern INT16S MF_sTotalElecGenCompVal;			//�ܷ���������ֵ
extern INT16U MF_InvFastControlSetEn[2];   //��������ٿ���/����ʹ��
extern INT16U MF_ausSystemTime[3];				//ϵͳʱ��
extern INT16U MF_usTimingOnControl;				//��ʱ��������
extern INT16U MF_usTimingOffControl;				//��ʱ�ػ�����
extern INT16U MF_usInvOnOffCmd;					//���\�ػ�
extern INT16U MF_WaitStandupTime;					//�����ӳ�
extern INT16U MF_usActPwrModeEn;				//�й����ʿ���ģʽ EN/DIS
extern INT16U MF_usActPwrSetValue;				//�й���������ֵ
extern INT16U MF_FirstConnectGridPwrAdjustRate;  //���β������ʵ�������
extern INT16U MF_FaultRstReconnectPwrAdjustRate;  //���ϻָ��������ʵ�������
extern INT16U MF_usActPwrControlAdjustRate;    //�й����ʿ��Ƶ�������
extern INT16U MF_usReactPwrModeEn;				//�޹����ʿ���ģʽ EN/DIS
extern INT16S MF_PowerFactorSetVal;				//������������ֵ
extern INT16U MF_PFAdjustRate;         //PF��������
extern INT16S MF_usReactPwrSetValue;				//�޹���������ֵ
extern INT16U MF_ItalyOutsideCommFlag;    //������ⲿͨ�ű�ʶ
extern INT16U MF_afGridVoltProtThresHigh[3];		//������ѹ��������ֵ
extern INT16U MF_afGridVoltProtThresLow[3];		//������ѹ��������ֵ
extern INT16U MF_ausGridVoltProtTimeHigh[3];	//������ѹ��������ʱ��
extern INT16U MF_ausGridVoltProtTimeLow[3];		//������ѹ��������ʱ��
extern INT16U MF_usGridVoltFltRecoverTime;		//������ѹ�ָ�ʱ��
extern INT16U MF_afGridFreqProtThresHigh[3];		//����Ƶ�ʱ�������ֵ
extern INT16U MF_afGridFreqProtThresLow[3];		//����Ƶ�ʱ�������ֵ
extern INT16U MF_ausGridFreqProtTimeHigh[3];	//����Ƶ�ʱ�������ʱ��
extern INT16U MF_ausGridFreqProtTimeLow[3];		//����Ƶ�ʱ�������ʱ��
extern INT16U MF_usGridFreqFltRecoverTime;		//����Ƶ�ʻָ�ʱ��
extern INT16U MF_usWattFreqModeEn;						//Watt-Freq En/Dis
extern INT16U MF_usWattVoltModeEn;						//Watt-Volt En/Dis
extern INT16U MF_usPfWattModeEn;						//PF-Watt En/Dis
extern INT16U MF_usVarVoltModeEn;						//Var-Volt En/Dis
extern INT16U MF_usVarWattModeEn;						//Var-Watt En/Dis
extern INT16U MF_usResvModeEn;							//resvered Mode En/Dis
extern INT16U MF_usLVRTCurveEn;							//LVRT(����)ʹ��
extern INT16U MF_usHVRTCurveEn;							//HVRT(����)ʹ��
extern INT16U MF_usLFRTCurveEn;							//LFRT(����)ʹ��
extern INT16U MF_usHFRTCurveEn;							//HFRT(����)ʹ��
extern INT16U MF_usGridVoltProtThresAutoAdjEn;	//������ѹ������ֵ�Զ�����ʹ��/����
extern INT16U MF_usGridVoltHighPwrDeratedEn;	//������ѹ����ʹ��/����
extern INT16U MF_usGlobalMpptScanEn;			//Global MPPT SCANʹ��/����
extern INT16U MF_usPvVICurveEn;					//PV-VI����ʹ��/����
extern INT16U MF_usItalySelfTestEn;				//ItalySelfTestʹ��
extern INT16U MF_usPidFuncEn;					//PIDʹ��/����
extern INT16U MF_usDrmFuncEn;					//DRMʹ��/����
extern INT16U MF_usArcDetectEn;					//ARC���ʹ��/����
extern INT16U MF_usArcResetEn;					//ARC��λ
extern INT16U MF_usAIFuncEn;					//AIʹ��/����
extern INT16U MF_usGFCIFuncEn;					//GFCIʹ��/����
extern INT16U MF_usISOFuncEn;					//ISOʹ��/����
extern INT16U MF_usDCIFuncEn;					//DCIʹ��/����
extern INT16U MF_ausPARAMResv1;				//Reserved
extern INT16S MF_wZeroExpPercent;	   //wZeroExpPercent
extern INT16U MF_ausProtEnResv[3];				//Reserved
extern INT16U MF_ausCommSetEndIDTAG;  //�豸������ʶTAG  	
extern INT16U MF_ausCommSetEndIDLength;  		//��ʶ����	

#endif

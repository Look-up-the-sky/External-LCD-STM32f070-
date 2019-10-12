#ifndef _VARIABLE_TRANSFORMATION_H_
#define _VARIABLE_TRANSFORMATION_H_
#include "TypeDefine.h"
#include "MF_Datecfg.h"
#include "HF_Datecfg.h"
/*********************��Ļ��ʾ������ֵ****************/
typedef union
{
	INT8U all[1];
	struct
	{
	  INT8U ausProductModel[32];	//Model�ͺ�
		INT8U ausFirmwareVer[32];	//����汾
		INT8U ausProductSn[32];		//���к�
		INT8U ausProtocolNum[20];	//Э���
		INT8U ausProtocolVer[20];	//Э��汾
	}data;
}INFO_t;
typedef struct
	{
		INT16U ausGridPhaseCur[3];	//���������
		INT16U ausGridLineVolt[3];	//�����ߵ�ѹ
		INT16U ausGridPhaseVolt[3];	//�������ѹ
		INT16U ausGridFreq;					//����Ƶ��
		INT32S outActivePower;	    			//����й�����
		INT32U outApparentPower;				//������ڹ���
		INT32S outReactivePower;				//����޹�����
		INT16S sDailyElecGeneration;			//���췢����
		INT32U ulTotalElecGeneration;			//�ܷ�����
		INT32U ulTotalReactiveElecGen;		//���޹�����
		INT16S sPowerFactor;					//��������
		INT16U DCVol;         		 		//ֱ����ѹ
		INT16U DCCur;            		//ֱ������
		INT16U inActivePower;	    			//�����й�����
		INT16S sEvirTmp1;						//�����ڲ��¶�
		INT16S sHeatsinkTmp;				//ɢ��Ƭ�¶�
		INT16S sChokeTmp;					//����¶�
		INT16S sResvTmp;					//�����¶�
	}GRID_INFO;
	
typedef struct
	{
		INT16U usMpptNbr;						//MPPT����
		INT16U usPvInputConnectionMode;		//PV����ģʽ
		HF_PV_ARRAY_DISPLAY_INFO_STR astrPvArrayDisplayInfo[3];   //ģ���¼�������Ϣ	
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
	INT16U* LVRTCurveEn;							//LVRT(����)ʹ��
	INT16U* HVRTCurveEn;							//HVRT(����)ʹ��
	INT16U* LFRTCurveEn;							//LFRT(����)ʹ��
	INT16U* HFRTCurveEn;							//HFRT(����)ʹ��
}Curve_Enable_INFO;

typedef struct
{
	//INT16U* ProtSelfChkFuncEn[2];		//�������������ʹ��/���� resvered
	INT16U* ActiveAntiIslandingEn;		//�����µ�(AI)ʹ��/����
	INT16U* GfciEn;						//GFCI����ʹ��/����
	INT16U* IsoCheckEn;					//ISO���ʹ��/����
	INT16U* DcInjectionEn;				//DCI����ʹ��/����
	INT16U* GridVoltProtThresAutoAdjEn;	//������ѹ������ֵ�Զ�����ʹ��/����
	INT16U* GridVoltHighPwrDeratedEn;	//������ѹ����ʹ��/����
	INT16U* GlobalMpptScanEn;			//Global MPPT SCANʹ��/����
	INT16U* PvVICurveEn;					//PV-VI����ʹ��/����
	INT16U* ItalySelfTestEN;				//�Լ�ʹ��/����(���ٿ���)
	INT16U* PidFuncEn;					//PIDʹ��/����
	INT16U* DrmFuncEn;					//DRMʹ��/����
	INT16U* ArcDetectEn;					//ARC���ʹ��/����
	INT16U* ArcResetEn;					//ARC��λ
	INT16U* ReconnectPwrRampUpEn; 		//���������������ʻ���,�Ʒ�����������
	INT16U* AntiRefluxPwrPercent; 		//���������ʰٷֱ�,�Ʒ�����������
	INT16S* ZeroExpPercent;					//�۷�����������
}Protection_Func_Enable_INFO;

typedef struct
{
	/*************�۷����****************/
	//INT16U* MActPwrModeEn;				//�й����ʿ���ģʽ EN/DIS
	INT16U* MActPwrSetValue;				//�й���������ֵ
	//INT16U* MFirstConnectGridPwrAdjustRate;  //���β������ʵ�������
	//INT16U* MFaultRstReconnectPwrAdjustRate;  //���ϻָ��������ʵ�������
	//INT16U* MActPwrControlAdjustRate;    //�й����ʿ��Ƶ�������
	INT16U* MReactPwrModeEn;				//�޹����ʿ���ģʽ EN/DIS
	INT16S* MPowerFactorSetVal;				//������������ֵ
	//INT16U* MPFAdjustRate;         //PF��������
	INT16S* MReactPwrSetValue;				//�޹���������ֵ
	INT16U* MItalyOutsideCommFlag;    //������ⲿͨ�ű�ʶ
	/*************�Ʒ����****************/
	INT16U* HMaxOutputPwrPercent;			//���������ʰٷֱ�
	//INT16U* HPwrRampUpPercent;				//Reconnection power ramping up slop percent per minute
	INT16U* HReactPwrModeEn;				//�޹����ʿ���ģʽ EN/DIS
	INT16S* HPowerFactorSetVal;				//��������PF����ֵ
	INT16S* HReactPwrQPercent;				//�޹�����Q�ٷֱ�
	//INT16U* HF_fReactPwrReguSpeed;				//�޹���������
}Param_Set_INFO;

typedef struct
{
	INT16U* FcParamRecovery;				//�ָ�����Ĭ��ֵ
  INT16U* ExistingDataClear;			//��ʷ��¼�������
  INT16U* PvInputConnectionMode;		//PV����ģʽ����
  INT16S* TotalElecGenCompVal;			//�ܷ���������ֵ
}Key_Data_INFO;

typedef struct
{
	INT16U* InvOnOffCmd;					//���\�ػ�
	INT16U* WaitStandupTime;					//�����ӳ�
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
extern INT16U IAPState;  		//IAP״̬
extern INT16U InvOnOffCmd;  		//���ػ�״̬
extern INT8U System_Time_Change_Flag;
extern INT8U Italy_SelfTest_Flag;
void Variable_Transformation(void);
void sVariableTask(void);

#endif

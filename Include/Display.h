#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#ifdef SUNNYBEE
typedef union
{
	INT16U all;
	struct
	{
		INT8U ubEscState: 1;
		INT8U ubEnterState:1;
		INT8U ubUpState:1;
		INT8U ubDownState:1;
		INT8U ubEscPress: 1;
		INT8U ubEnterPress:1;
		INT8U ubUpPress:1;
		INT8U ubDownPress:1;
		INT8U ubEscLongPress: 1;
		INT8U ubEnterLongPress:1;
		INT8U ubUpLongPress:1;
		INT8U ubDownLongPress:1;
		INT8U ubReserve:4;
	}bit;
}BUTTON;
#else
/************½çÃæ×´Ì¬************/
#define cDefault		0
#define cEToday			1
#define cETotal			2
#define cPVVolt 		3
#define cPVCurr			4
#define cGridVolt		5
#define cGridCurr		6
#define cGridFreq		7
#define cModel			8
#define cSafty			9
#define cVersion	 	10
#define cTemperature	11
#define cSNAndIP		12
#define cLanguage		13
#define cDateTime		14
#define cUserLevel		15
#define cParameter		16
#define cCoef			17
#define cSelfTest		18
#define cPfQv			19
#define cErrorHistory	20
#define cProtection		21
#define cCountry		22

#define cCountrySet		100
#define cLanguageSet	101
#define cParameterSet	102
#define cProtectionSet	103
#define cSelfTestSet	104
#define cCoefSet		105
#define cVersionInf	    106
#define cTemperatureInf	107
#define cSNAndIPInf	    108
#define cPfQvSet		109
#define cDateTimeSet	110
#define cError			111
#define cUserLevelSet	112
#endif
typedef union
{
	INT8U ALL;
	struct
	{
		INT8U CRCFLAG :1;
		INT8U LASTFRAME :1;
		INT8U FINISH :1;
		INT8U DataFail:1;
		INT8U rsvd :4;
	} Bit;
} FRAME_FLAG_Typedef;

typedef struct
{
	INT8U FrameHead;
	INT8U FrameDataAdr[4];
	INT8U FrameData[128];
	INT8U CrcData[2];
	FRAME_FLAG_Typedef flag;
} FLASH_FRAME_Typedef;

extern FLASH_FRAME_Typedef m_frame_info;
void sDisplayTask(void);
void sInitDisplayTask(void);

#endif

#ifndef _I2C_H_
#define _I2C_H_

#define cCoefAdrEE	0
#define cSaftyAdrEE	cCoefAdrEE + cCoefSize
#define cInfoAdrEE	cSaftyAdrEE + cSaftySize
#define cEngyAdrEE	cInfoAdrEE + cInfoSize
#define cErrorAdrEE	cEngyAdrEE + cEnergySize

#define cBackUpEE	512
#define cCoefAdrBackUpEE	cCoefAdrEE + cBackUpEE
#define cSaftyAdrBackUpEE	cSaftyAdrEE + cBackUpEE
#define cInfoAdrBackUpEE	cInfoAdrEE + cBackUpEE
#define cEngyAdrBackUpEE	cEngyAdrEE + cBackUpEE
#define cErrorAdrBackUpEE	cErrorAdrEE + cBackUpEE

#define cTimeSize	6

/************”Ô—‘************/
#define cEnglish		0
#define cDeutsch		1
#define cDutch			2
#define cITALIANO		3
#define cMaxLanguage	4

typedef union
{
	INT32U all;
    struct{
        INT16U mubRatioR:1;
        INT16U mubSaftyR:1;
        INT16U mubInfoR:1;
        INT16U mubEngyR:1;
        INT16U mubErrorR:1;
        INT16U mubTimeR:1;
        INT16U mubReserveR:2;

        INT16U mubRatioW:1;
        INT16U mubSaftyW:1;
        INT16U mubInfoW:1;
        INT16U mubEngyW:1;
        INT16U mubErrorW:1;
        INT16U mubTimeW:1;
        INT16U mubReserveW:2;

        INT16U mubRatioRFail:1;
        INT16U mubSaftyRFail:1;
        INT16U mubInfoRFail:1;
        INT16U mubEngyRFail:1;
        INT16U mubErrorRFail:1;
        INT16U mubTimeRFail:1;
        INT16U mubReserveRFail:2;

        INT16U mubRatioWFail:1;
        INT16U mubSaftyWFail:1;
        INT16U mubInfoWFail:1;
        INT16U mubEngyWFail:1;
        INT16U mubErrorWFail:1;
        INT16U mubTimeWFail:1;
        INT16U mubReserveWFail:2;
    }bit;
}IICFLAG;
extern IICFLAG IicFlag;

void sI2CTask(void);
void sReadAllEepromData(void);
void sResetProtect(INT16U luwCountry);

#endif

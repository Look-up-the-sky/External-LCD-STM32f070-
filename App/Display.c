/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Display.c
 * 修改人：沈仕强
 * 修改日期：2019.06.20
 * 描述：显示控制
 *******************************************************/

#define version "V1.0.9"
/********************************************************************
 * Include Files
 ********************************************************************/
#include <stdio.h>
#include "App.h"
#include "stm32f0xx_gpio.h"
#include "I2C.h"
#include "Lib_Communication.h"
#include "Lib_Country.h"
#include "Display.h"
#include "InterfaceDef.h"
#include "string.h"
#include "Lib_SelfTest.h"
#include "Lib_ClearRam.h"
#include "MF_Datecfg.h"
#include "HF_Datecfg.h"
#include "Variable_Transformation.h"
#include "modbus.h"
#include "Led_State.h"
/********************************************************************
 * Macros
 ********************************************************************/
#define c5sDisplay	500  //屏幕刷新延时

/********************************************************************
 * Constant
 ********************************************************************/
const INT32U c10Time[9] = {1,10,100,1000,10000,100000,1000000,10000000,100000000};


/********************************************************************
 * Local Types
 ********************************************************************/
INT8U lubButtonOffCnt[4];
INT8U lubButtonOnCnt[4];
INT8U lubSpeakerflag;
INT8U lubSpeakerOnCnt;
INT8U lubResetCnt;
INT8U lubRefreshCnt;
INT8U lubScreenBuf[cMaxLine][cMaxRow];
INT8U lubIconSelected;
INT8U lubBlinkCnt;
INT8U lubLanguageSet;
INT8U lubPasswordSet[6] = {0x30,0x30,0x30,0x30,0x30,0x30};
INT8U lubPasswordIn[6] = {0x30,0x30,0x30,0x30,0x30,0x30};
INT16U lubInterface;
INT16U lubInterfacePre;
INT8U lubLongPressCnt[4];
INT8U lubString[21];
INT8U lubStringTemp[21];
INT8U lubPageFlg;
INT8U lubFreqOrPfFlg;
INT16U lubPowerFactor;
INT8U lubMaxSetListTemp;
INT16U luwVoltSet[9];
INT16U luwFreqSet[8];
INT16S luwParamSet[7];
INT16S luwParamSetMax[7];
INT16S luwParamSetMin[7];
INT16U luwProtectionSet[15];
INT16U luwProtectionSetMax[15];
INT16U luwProtectionSetMin[15];
INT16U luwCurveSet[10];
INT16U luwCurveSetMax[10];
INT16U luwCurveSetMin[10];
INT16S luwKeyDataSet[4];
INT16S luwKeyDataSetMax[4];
INT16S luwKeyDataSetMin[4];
INT16U luwInvSet[2];
INT16U luwInvSetMax[2];
INT16U luwInvSetMin[2];
INT16U luwNoOprateCnt;
INT16U luwBusyCheckCnt;
INT16U luwAutoTestCnt;
INT16U luwFreqSetMin[8];
INT16U luwFreqSetMax[8];
INT16U luwUpdateSuccessCnt;
BUTTON Button;
SYSTEMTIME TimeSet;
FLASH_FRAME_Typedef m_frame_info;

/********************************************************************
 * Extern Global Types
 ********************************************************************/
extern INT8U *gubBlockPtr[cMaxBlockNum];
extern const INT16U cMaxLength[cMaxBlockNum];
/********************************************************************
 * Functions
 *******************************************************************/
INT8U sGetButtonState(INT8U i);
void sButtonPressCheck(void);
void sChooseInterface(void);
void sUpdateLCD(void);
void sDisplayAsist(INT8U lubMin,INT8U lubMax,INT8U *lubStringAsist);
void sDelay(INT16U i);
void sWriteScreenBuf(INT8U lubLine,INT8U lubRow,INT8U lubLength,INT8U *lubDataAdr);
void sWriteLCDAdr(INT8U lubLCDAdr);
void sTransferData(INT8U lubDot,INT32U lulData,INT8U *lubDataP,INT8U lubLength);
void sAutoTestAsis(INT8U lubModel);

INT8U sGetLCDState(void);
INT16U sAutoTestAsis1(INT8U lubModel,INT8U lubItem,INT16U luwValue);
/*****************************************************************************
 * 函数名称：按键状态获得函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：
 ****************************************************************************/
INT8U sGetButtonState(INT8U i)
{
	if(i == 0)
	{
		return(cGetButtonEsc());
	}
	else if(i == 1)
	{
		return(cGetButtonEnter());
	}
	else if(i == 2)
	{
		return(cGetButtonUp());
	}
	else if(i == 3)
	{
		return(cGetButtonDown());
	}
	
	return(0);
}

/*****************************************************************************
 * 函数名称：按键状态获取函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：
 ****************************************************************************/
void sButtonPressCheck(void)
{
	INT8U i;
	
	for(i = 0;i < 4;i++)
	{
		if(Button.all & (1 << i))										//当前状态为按住
		{
			if(++lubButtonOnCnt[i]>=200)
			{
				lubButtonOnCnt[i] = 200;
			}
			if(sGetButtonState(i))										//判断按键当前是否被按下
			{
				cLCMAOn();
				luwNoOprateCnt = 0;
				lubButtonOffCnt[i] = 0;
				if(lubButtonOnCnt[i] >= cLongPressLimit)				//长按确认
					{
						Button.all &= ~(1 << (i + 4));
						Button.all |= (1 << (i + 8));
					}
			}
			else
			{
				Button.all &= ~(1 << (i + 8));
				if(++lubButtonOffCnt[i] >= cButtonOffMax)
				{							
					if(lubButtonOnCnt[i] >= cShortPressLimit)			//短按确认
					{
						Button.all |= (1 << (i + 4));	
						Button.all &= ~(1 << (i + 8));
						cSpeakerOn();
						lubSpeakerflag = 1;
					}
					else
					{
						Button.all &= ~(1 << (i + 4));
						Button.all &= ~(1 << (i + 8));
					}				
					lubButtonOnCnt[i] = 0;
					Button.all &= ~(1 << i);							//设为释放状态
				}
			}
		}
		else															//当前状态为释放
		{
			if(sGetButtonState(i))	
			{
				if(++lubButtonOnCnt[i] >= cButtonOnMax)
				{
					cLCMAOn();
					luwNoOprateCnt = 0;
					lubButtonOnCnt[i] = 0;
					Button.all |= (1 << i);						
				}
			}
			else
			{
				lubButtonOnCnt[i] = 0;				
			}
		}
	}
	
	if(lubSpeakerflag == 1)
	{
		if(++lubSpeakerOnCnt >= cSpeakerOnLimit)
		{
			lubSpeakerOnCnt = 0;
			cSpeakerOff();
			lubSpeakerflag = 0;
		}
	}

	if(Button.bit.ubUpLongPress && Button.bit.ubDownLongPress && Button.bit.ubEnterLongPress)
	{
		if(++lubResetCnt <= 10)
		{
			cSpeakerOn();
			Info.data.ubFirstTurnOn = cTrue;	
			sClearRamByte(gubBlockPtr[cEnergyBlock],cMaxLength[cEnergyBlock]);
			sClearRamByte(gubBlockPtr[cErrorRecordBlock],cMaxLength[cErrorRecordBlock]);
			IicFlag.all |= 0x1C00;
		}
		else
		{
			cSpeakerOff();
		}	
	}
	else
	{
		lubResetCnt = 0;
	}
}




/*****************************************************************************
 * 函数名称：显示任务
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：
 ****************************************************************************/
void sDisplayTask(void)
{
	sButtonPressCheck();															//按键控制
	if(++lubRefreshCnt >= cRefreshTime)
	{
		lubRefreshCnt = 0;
		sChooseInterface();															//设定显示界面
		sUpdateLCD();																//更新LCD显示
		Button.all &= 0xF0F;														//短按响应结束
	}
}



/*****************************************************************************
 * 函数名称：延时函数
 * 函数输入：循环次数
 * 函数返回值：无
 * 函数描述：无
 ****************************************************************************/
void sDelay(INT16U i)
{
	while(i)
	{
		i--;
	}
}

/*****************************************************************************
 * 函数名称：LCD空闲判断函数
 * 函数输入：无
 * 函数返回值：LCD忙标志位
 * 函数描述：返回1表示LCD忙，返回0表示LCD空闲
 ****************************************************************************/
INT8U sGetLCDState(void)
{
	INT8U lubBusyFlag;
	
	cLCMRSOff();
	cLCMRWOn();
	sDelay(3);
	LCDOUT(0xff);
	GPIOC->MODER = 0x00000000;													//设为输入
	cLCMENOn();
	sDelay(3);
	lubBusyFlag = GPIO_ReadInputData(GPIOC);									//读取状态
	GPIOC->MODER = 0x55550000;													//设为输出
	cLCMENOff();
	sDelay(3);
	return(lubBusyFlag & 0x8000);
}

/*****************************************************************************
 * 函数名称：写LCD数据函数
 * 函数输入：数据，指令
 * 函数返回值：无
 * 函数描述：
 ****************************************************************************/
void sWriteLCDData(INT8U lubData,INT8U lubCmd)
{
	if(lubCmd)
	{
		cLCMRSOn();
	}
	else
	{
		cLCMRSOff();
	}
	cLCMRWOff();
	sDelay(70);
	cLCMENOn();
	sDelay(90);
	LCDOUT(lubData);
	sDelay(90);
	cLCMENOff();
	sDelay(70);
}

/*****************************************************************************
 * 函数名称：显示初始化函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：
 ****************************************************************************/
void sInitDisplayTask(void)
{
	INT8U i,j;
	
	/***************初始化显示缓存***************/
	for(i = 0;i < 4;i++)
	{
		for(j = 0;j < 20;j++)
		{
			lubScreenBuf[i][j] = 0x20;
		}
	}
	
	/***************初始化显示界面***************/
	sWriteLCDData(0x38,0);
	sDelay(0x0F);
	sWriteLCDData(0x38,0);
	sDelay(0x0F);
	sWriteLCDData(0x38,0);
	sDelay(0x0F);
	for(i = 0;i < 4;i++)
	{
		while(sGetLCDState())
		{
			if(++luwBusyCheckCnt >= 9000)
			break;
		}
		luwBusyCheckCnt = 0;
		sWriteLCDData(lubLCMINTable[i],0);
	}
	lubInterface = cDefaultInterface;

	
	luwFreqSetMin[0] = 5000;
	luwFreqSetMin[1] = 5000;
	luwFreqSetMin[4] = 4500;
	luwFreqSetMin[5] = 4500;
	luwFreqSetMax[0] = 6500;
	luwFreqSetMax[1] = 6500;
	luwFreqSetMax[4] = 6000;
	luwFreqSetMax[5] = 6000;
	
	luwFreqSetMin[2] = 0;
	luwFreqSetMin[3] = 0;
	luwFreqSetMin[6] = 0;
	luwFreqSetMin[7] = 0;
	luwFreqSetMax[2] = 10000;
	luwFreqSetMax[3] = 10000;
	luwFreqSetMax[6] = 10000;
	luwFreqSetMax[7] = 10000;
	
}


/*****************************************************************************
 * 函数名称：显示缓存设置函数
 * 函数输入：行数，列数，长度，待设置数据
 * 函数返回值：无
 * 函数描述：
 ****************************************************************************/
void sWriteScreenBuf(INT8U lubLine,INT8U lubRow,INT8U lubLength,INT8U *lubDataAdr)
{
	INT8U i;	
	if(lubLine < cMaxLine)
	{
		for(i = lubRow;(i < cMaxRow) && (i < lubRow + lubLength) && (*lubDataAdr != '\0');i++)
		{
			lubScreenBuf[lubLine][i] = *lubDataAdr;
			lubDataAdr++;
		}
	}
}

/*****************************************************************************
 * 函数名称：界面选择函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：
 ****************************************************************************/
void sChooseInterface(void)
{
	INT8U i;
	INT16U luwKeyDataSetTemp;
	INT16U luwPVDCTemp;
	INT16U luwParamSetTemp;
	INT16U luwAutoTestTemp1;
	INT16U luwAutoTestTemp2;
	static INT16U luwAutoTestTemp1_pre;
	static INT16U luwAutoTestTemp2_pre;
	INT32U lulError,Error_Temp;
	static INT8U First_Enter_Flag = 1;
	
	if((IAPState > 1)&&(IAPState < 6))                  //显示逆变器升级状态
	{
		sWriteScreenBuf(0,0,20,"                    ");
		sWriteScreenBuf(1,0,20,"    INV UPDATING    ");
		sWriteScreenBuf(2,0,20,"                    ");
		sWriteScreenBuf(3,0,20,"                    ");
		return;
	}
	if((Button.all & 0x0FF0) == 0)						//60s无操作则返回默认界面
	{
		if(++luwNoOprateCnt >= 600)
		{
			cLCMAOff();
			luwNoOprateCnt = 600;
			lubInterface = cDefaultInterface;
			lubIconSelected = 0;
		}
	}
	else
	{
		luwNoOprateCnt = 0;
	}
	

 if(lubInterface == cDefaultInterface)										//默认首界面
	{
		if(INV_StateFlag == 4)
		{
			strcpy(lubString,"Normal   ");
		}
		else if(INV_StateFlag == 9)
		{
			strcpy(lubString,"OFF      ");
		}
		else
		{
			if(INV_StateFlag == 8)
			{
				strcpy(lubString,"Fault    ");
			}
			else
			{
				if(INV_StateFlag == 3)
				{
					strcpy(lubString,"Wait   s ");
					sTransferData(0,MasterData.data.uwReconnectTime,&lubString[4],3);	
					sDisplayAsist(4,6,lubString);
				}
				else
				{
					strcpy(lubString,"Waiting  ");
				}
			}
		
		}
		sWriteScreenBuf(0,0,9,lubString);	
		sWriteScreenBuf(0,9,11,(INT8U *)lubSafty[Safty.data.uwCountry]);
		
		strcpy(lubString,(INT8U *)lubPowerDisplay[Info.data.ublanguage]);
		strcpy(&lubString[8],"           W");
		sTransferData(0,__REV(Grid_Info.outActivePower),&lubString[10],8);
		sDisplayAsist(10,17,lubString);
		sWriteScreenBuf(1,0,20,lubString);	
		
		strcpy(lubString,(INT8U *)lubETodayDisplay[Info.data.ublanguage]);
		strcpy(&lubString[8],"         kWh");
		sTransferData(1,Grid_Info.sDailyElecGeneration,&lubString[12],4);
		if(Grid_Info.sDailyElecGeneration <  10)
		{
			sDisplayAsist(12,14,lubString);
		}
		else
		{
			sDisplayAsist(12,15,lubString);
		}
	
		sWriteScreenBuf(2,0,20,lubString);
		if(lubIconSelected >= 4)
		{
			lubIconSelected = 0;
		}
		if(++lubBlinkCnt >= 20)
		{
			lubBlinkCnt = 0;
			sWriteScreenBuf(3,0,20,(INT8U *)lubDefaultInterfaceIcon[Info.data.ublanguage][0]);	
		}
		else if(lubBlinkCnt >= 10)
		{
			sWriteScreenBuf(3,0,20,(INT8U *)lubDefaultInterfaceIcon[Info.data.ublanguage][lubIconSelected]);
		}
		
		if(Button.bit.ubEnterPress)													//单击Enter键
		{       
			lubInterface = (lubIconSelected + 1) << 8;
			lubIconSelected = 0;
		}
		else if(Button.bit.ubUpPress)												//单击Up键
		{       
			if(++lubIconSelected >= 4)
			{
				lubIconSelected = 0;
			}
			lubBlinkCnt = 9;
		}
		else if(Button.bit.ubDownPress)												//单击Down键
		{
			if(--lubIconSelected >= 4)
			{
				lubIconSelected = 3;
			}
			lubBlinkCnt = 9;
		}
		else if(Button.bit.ubEscPress)												//单击Esc键
		{
			lubInterface = cDefaultInterface;
			lubIconSelected = 0;
		}
		lubFreqOrPfFlg = 0;
	}

	else if(lubInterface == cETotalInterface)										//总发电量界面
	{
		strcpy(lubString,"        . kWh");
		sTransferData(1,__REV(Grid_Info.ulTotalElecGeneration),&lubString[1],8);					
		sDisplayAsist(1,7,lubString);
		sWriteScreenBuf(0,0,7,(INT8U *)lubETotalDisplay[Info.data.ublanguage]);
		sWriteScreenBuf(0,7,13,lubString);
		strcpy(lubString,"PV Input Mode:      ");
		sWriteScreenBuf(1,0,20,lubString);
		strcpy(lubString,"                    ");
		sWriteScreenBuf(2,0,20,lubString);
		if(Pv_Info.usPvInputConnectionMode == 0x01)
		{
			strcpy(lubString,"         Single MPPT");
			sWriteScreenBuf(2,0,20,lubString);
		}
		if(Pv_Info.usPvInputConnectionMode == 0x02)
		{
			strcpy(lubString,"      Multiple MPPTs");
			sWriteScreenBuf(2,0,20,lubString);
		}	
		strcpy(lubString,"                    ");
		sWriteScreenBuf(3,0,20,lubString);
		
		if(Button.bit.ubEnterPress)													//单击Enter键
		{       
			lubInterface = cDCInterface;
		}
		else if(Button.bit.ubEscPress)												//单击Esc键
		{       
			lubInterface = cDefaultInterface;
		}
	}
	else if(lubInterface == cDCInterface)											//PV显示界面
	{
		sWriteScreenBuf(0,0,20,"DC:                 ");
		sWriteScreenBuf(1,0,20,"   PV1      PV2     ");
		strcpy(lubString,"V:    . V      . V  ");
		if(PRJ_NUMBER == HORNET)
		{
			if((Pv_Info.astrPvArrayDisplayInfo[0].PV_INPUT_VOL&0x8000) == 0)
				{
					sTransferData(1,Pv_Info.astrPvArrayDisplayInfo[0].PV_INPUT_VOL,&lubString[3],4);
				}
				else
				{
					strcpy(&lubString[2],"-");
					luwPVDCTemp = 65536-Pv_Info.astrPvArrayDisplayInfo[0].PV_INPUT_VOL;
					sTransferData(1,luwPVDCTemp,&lubString[3],4);
				}
				if((Pv_Info.astrPvArrayDisplayInfo[1].PV_INPUT_VOL&0x8000) == 0)
				{
					sTransferData(1,Pv_Info.astrPvArrayDisplayInfo[1].PV_INPUT_VOL,&lubString[12],4);
				}
				else
				{
					strcpy(&lubString[11],"-");
					luwPVDCTemp = 65536-Pv_Info.astrPvArrayDisplayInfo[1].PV_INPUT_VOL;
					sTransferData(1,luwPVDCTemp,&lubString[12],4);
				}
		}
		else if(PRJ_NUMBER == SUNNYBEE)
		{
			sTransferData(1,Pv_Info.astrPvArrayDisplayInfo[0].PV_INPUT_VOL,&lubString[3],4);
			sTransferData(1,Pv_Info.astrPvArrayDisplayInfo[1].PV_INPUT_VOL,&lubString[12],4);
		}
		sDisplayAsist(3,5,lubString);
		sDisplayAsist(12,14,lubString);
		sWriteScreenBuf(2,0,20,lubString);
		
		strcpy(lubString,"I:    . A      . A  ");
		if(PRJ_NUMBER == HORNET)
		{
			if((Pv_Info.astrPvArrayDisplayInfo[0].PV_INPUT_CUR&0x8000) == 0)
				{
					sTransferData(1,Pv_Info.astrPvArrayDisplayInfo[0].PV_INPUT_CUR,&lubString[3],4);
				}
				else
				{
					strcpy(&lubString[2],"-");
					luwPVDCTemp = 65536-Pv_Info.astrPvArrayDisplayInfo[0].PV_INPUT_CUR;
					sTransferData(1,luwPVDCTemp,&lubString[3],4);
				}
			if((Pv_Info.astrPvArrayDisplayInfo[1].PV_INPUT_CUR&0x8000) == 0)
				{
					sTransferData(1,Pv_Info.astrPvArrayDisplayInfo[1].PV_INPUT_CUR,&lubString[12],4);
				}
				else
				{
					strcpy(&lubString[11],"-");
					luwPVDCTemp = 65536-Pv_Info.astrPvArrayDisplayInfo[1].PV_INPUT_CUR;
					sTransferData(1,luwPVDCTemp,&lubString[12],4);
				}
		}
		else if(PRJ_NUMBER == SUNNYBEE)
		{
			sTransferData(1,Pv_Info.astrPvArrayDisplayInfo[0].PV_INPUT_CUR,&lubString[3],4);
			sTransferData(1,Pv_Info.astrPvArrayDisplayInfo[1].PV_INPUT_CUR,&lubString[12],4);
		}
		sDisplayAsist(3,5,lubString);												//显示优化
		sDisplayAsist(12,14,lubString);
		sWriteScreenBuf(3,0,20,lubString);
		
		
		if(Button.bit.ubEnterPress)													//单击Enter键
		{        
			lubInterface = cACInterface;
		}
		else if(Button.bit.ubEscPress)												//单击Esc键
		{       
			lubInterface = cDefaultInterface;
		}
	}
	else if(lubInterface == cACInterface)											//AC显示界面
	{
		if(lubFreqOrPfFlg == 1)
		{
			if(INV_StateFlag == 4)
			{
				lubPowerFactor = Grid_Info.sPowerFactor;			
			}
			else
			{
				lubPowerFactor = 0;
			}
		
			strcpy(lubString,"AC:        PF:  .   ");		
			sTransferData(3,(INT16U)lubPowerFactor,&lubString[15],4);			
		}
		else
		{			
			strcpy(lubString,"AC:        F:  .  Hz");
			sTransferData(2,Grid_Info.ausGridFreq,&lubString[13],4);
			sDisplayAsist(13,14,lubString);
		}
		
		sWriteScreenBuf(0,0,20,lubString);
		 
		strcpy(lubString,"VR:   . V  IR:   . A");
		sTransferData(1,Grid_Info.ausGridPhaseVolt[cPhaseR],&lubString[3],4);
		sDisplayAsist(3,5,lubString);
		sTransferData(1,Grid_Info.ausGridPhaseCur[cPhaseR],&lubString[14],4);
		sDisplayAsist(14,16,lubString);
		sWriteScreenBuf(1,0,20,lubString);
		
		sWriteScreenBuf(2,0,20,"                    ");
		sWriteScreenBuf(3,0,20,"                    ");
		if(PRJ_NUMBER == HORNET)
		{
			strcpy(lubString,"VS:   . V  IS:   . A");
			sTransferData(1,Grid_Info.ausGridPhaseVolt[cPhaseS],&lubString[3],4);
			sDisplayAsist(3,5,lubString);
			sTransferData(1,Grid_Info.ausGridPhaseCur[cPhaseS],&lubString[14],4);
			sDisplayAsist(14,16,lubString);
			sWriteScreenBuf(2,0,20,lubString);
			
			strcpy(lubString,"VT:   . V  IT:   . A");
			sTransferData(1,Grid_Info.ausGridPhaseVolt[cPhaseT],&lubString[3],4);
			sDisplayAsist(3,5,lubString);
			sTransferData(1,Grid_Info.ausGridPhaseCur[cPhaseT],&lubString[14],4);
			sDisplayAsist(14,16,lubString);
			sWriteScreenBuf(3,0,20,lubString);		
		}
		
		if(Button.bit.ubEnterPress)													//单击Enter键
		{    
			if(lubFreqOrPfFlg == 1)  
			{
				lubFreqOrPfFlg = 0;
				lubInterface = cTimeInterface;
			}
			else
			{
				lubFreqOrPfFlg = 1;
			}
		}
		else if(Button.bit.ubEscPress)												//单击Esc键
		{      
			lubFreqOrPfFlg = 0;
			lubInterface = cDefaultInterface;
		}

	}
	else if(lubInterface == cTimeInterface)											//时间显示界面
	{
		sWriteScreenBuf(0,0,20,"Date:               ");
		strcpy(lubString,"          20  -  -  ");
		lubString[12] = SystemTime.data.ubYear / 10 + 0x30;
		lubString[13] = (SystemTime.data.ubYear % 10) + 0x30;
		lubString[15] = SystemTime.data.ubMonth / 10 + 0x30;
		lubString[16] = (SystemTime.data.ubMonth % 10) + 0x30;
		lubString[18] = SystemTime.data.ubDate / 10 + 0x30;
		lubString[19] = (SystemTime.data.ubDate % 10) + 0x30;
		
		sWriteScreenBuf(1,0,20,lubString);
		strcpy(lubString,"Time:       :  :    ");
		lubString[10] = SystemTime.data.ubHour / 10 + 0x30;
		lubString[11] = (SystemTime.data.ubHour % 10) + 0x30;
		lubString[13] = SystemTime.data.ubMinute / 10 + 0x30;
		lubString[14] = (SystemTime.data.ubMinute % 10) + 0x30;
		lubString[16] = SystemTime.data.ubSecond / 10 + 0x30;
		lubString[17] = (SystemTime.data.ubSecond % 10) + 0x30;
		sWriteScreenBuf(2,0,20,lubString);
		sWriteScreenBuf(3,0,20,"                    ");
		
		if(Button.bit.ubEnterPress)						//单击Enter键
		{       
			lubInterface = cTempInterface;
		}
		else if(Button.bit.ubEscPress)
		{
			lubInterface = cDefaultInterface;
		}
	}
	else if(lubInterface == cTempInterface)     //温度显示界面
	{	
		sWriteScreenBuf(0,0,20,"Temperature:        ");
		strcpy(lubString,"Inside:   .         ");
		sTransferData(1,Grid_Info.sEvirTmp1,&lubString[7],4);
		lubString[12] = 0xB2;
		lubString[13] = 0x43;
		sDisplayAsist(7,9,lubString);
		sWriteScreenBuf(1,0,20,lubString);
		
		strcpy(lubString,"Heat sink:   .      ");
		sTransferData(1,Grid_Info.sHeatsinkTmp,&lubString[10],4);
		lubString[15] = 0xB2;
		lubString[16] = 0x43;
		sDisplayAsist(10,12,lubString);
		sWriteScreenBuf(2,0,20,lubString);
			
			
		if(Button.bit.ubEnterPress || Button.bit.ubEscPress)						//单击Enter或Esc键
		{       
			lubInterface = cDefaultInterface;
		}		
	}
	/*********************************************************************
	*
	*													info
	*
	*
	**********************************************************************/
	else if(lubInterface == cInfoSN)											
	{
		sWriteScreenBuf(0,0,20,"SN:                 ");
		sWriteScreenBuf(0,4,16,Info_t.data.ausProductSn);
		sWriteScreenBuf(1,0,20,"                    ");
		sWriteScreenBuf(1,4,16,(&Info_t.data.ausProductSn[16]));
		sWriteScreenBuf(2,0,20,"Model:              ");
		sWriteScreenBuf(2,6,14,Info_t.data.ausProductModel);
		sWriteScreenBuf(3,0,20,"                    ");
		
		if(Button.bit.ubEscPress)													//单击Esc键
		{        
			lubInterface = cDefaultInterface;
		}
		else if(Button.bit.ubUpPress)												//单击Up键
		{       
			lubInterface = cInfoLCDVersion;
		}
		else if(Button.bit.ubDownPress)												//单击Down键
		{        
			lubInterface = cInfoVersion;
		}
		else if(Button.bit.ubEnterLongPress && INV_StateFlag != 4)
		{
			SlaveData.data.SlaveFlag.bit.ubConstPV = 1;
			lubInterface = cDefaultInterface;
		}
	}

	else if(lubInterface == cInfoVersion)												
	{
		sWriteScreenBuf(0,0,20,"Software Version:   ");
		sWriteScreenBuf(1,0,20,"                    ");
		sWriteScreenBuf(1,4,16,Info_t.data.ausFirmwareVer);
		sWriteScreenBuf(2,0,20,"Protocol Version:   ");
		sWriteScreenBuf(3,0,20,"                    ");
		sWriteScreenBuf(3,4,16,Info_t.data.ausProtocolNum);
		sWriteScreenBuf(3,12,8,Info_t.data.ausProtocolVer);
	
		if(Button.bit.ubEscPress)													//单击Esc键
		{        
			lubInterface = cDefaultInterface;
		}
		else if(Button.bit.ubUpPress)												//单击Up键
		{       
			lubInterface = cInfoSN;
		}
		else if(Button.bit.ubDownPress)												//单击Down键
		{        
			lubInterface = cInfoLCDVersion;
		}
	}
		else if(lubInterface == cInfoLCDVersion)												
		{
		sWriteScreenBuf(0,0,20,"                    ");
		sWriteScreenBuf(1,0,20,"LCD Soft Version:   ");
		sWriteScreenBuf(2,0,20,"                    ");
		sWriteScreenBuf(2,13,20,version);
		sWriteScreenBuf(3,0,20,"                    ");

		if(Button.bit.ubEscPress)													//单击Esc键
		{        
			lubInterface = cDefaultInterface;
		}
		else if(Button.bit.ubUpPress)												//单击Up键
		{       
			lubInterface = cInfoVersion;
		}
		else if(Button.bit.ubDownPress)												//单击Down键
		{        
			lubInterface = cInfoSN;
		}
	}
	/*********************************************************************
	*
	*													故障记录界面
	*
	*
	**********************************************************************/	
	
	else if(lubInterface >= cErrorPage1 && lubInterface <= cErrorPage20)			
	{
		lulError = ErrorRecord.data.ErrorMessage[lubInterface - cErrorPage1].data.ErrorCodeL;
		if(lulError)																//存在最新一条故障
		{
			strcpy(lubString,"Page      20  -  -  ");
			lubString[4] = (lubInterface - cErrorPage1 + 1) / 10 + 0x30;
			lubString[5] = ((lubInterface - cErrorPage1 + 1) % 10) + 0x30;
			
			if(ErrorRecord.data.ErrorMessage[lubInterface - cErrorPage1].data.Month == 0)
				ErrorRecord.data.ErrorMessage[lubInterface - cErrorPage1].data.Month  = 1;
			if(ErrorRecord.data.ErrorMessage[lubInterface - cErrorPage1].data.Date == 0)
				ErrorRecord.data.ErrorMessage[lubInterface - cErrorPage1].data.Date  = 1;
			
			lubString[12] = ErrorRecord.data.ErrorMessage[lubInterface - cErrorPage1].data.Year / 10 + 0x30;
			lubString[13] = (ErrorRecord.data.ErrorMessage[lubInterface - cErrorPage1].data.Year % 10) + 0x30;
			lubString[15] = ErrorRecord.data.ErrorMessage[lubInterface - cErrorPage1].data.Month / 10 + 0x30;
			lubString[16] = (ErrorRecord.data.ErrorMessage[lubInterface - cErrorPage1].data.Month % 10) + 0x30;
			lubString[18] = ErrorRecord.data.ErrorMessage[lubInterface - cErrorPage1].data.Date / 10 + 0x30;
			lubString[19] = (ErrorRecord.data.ErrorMessage[lubInterface - cErrorPage1].data.Date % 10) + 0x30;
			sWriteScreenBuf(0,0,20,lubString);
			strcpy(lubString,"E           :  :    ");
			Error_Temp = __REV16(lulError);
			if(PRJ_NUMBER == SUNNYBEE)
			{
				if((Error_Temp >= 0x0100)&&(Error_Temp <= 0x0106))
				{
					Error_Temp = Error_Temp&0x000F+1;
				}
				else
				{
					if((Error_Temp >= 0x0200)&&(Error_Temp <= 0x0208))
					{
						Error_Temp = (Error_Temp&0x000F)+8;
					}
					else
					{
						if((Error_Temp >= 0x0210)&&(Error_Temp <= 0x0218))
						{
							Error_Temp = (Error_Temp&0x000F)+16;
						}
						else
						{
							if((Error_Temp >= 0x0300)&&(Error_Temp <= 0x0307))
							{
								Error_Temp = (Error_Temp&0x000F)+23;
							}
							else
							{
								if((Error_Temp >= 0x0401)&&(Error_Temp <= 0x0409))
								{
									Error_Temp = (Error_Temp&0x000F)+30;
								}
								else
								{
									if((Error_Temp >= 0x0410)&&(Error_Temp <= 0x0416))
									{
										Error_Temp = (Error_Temp&0x000F)+40;
									}
									else
									{
										if((Error_Temp >= 0x0501)&&(Error_Temp <= 0x0509))
										{
											Error_Temp = (Error_Temp&0x000F)+46;
										}
										else
										{
											if((Error_Temp >= 0x0510)&&(Error_Temp <= 0x051A))
											{
												Error_Temp = (Error_Temp&0x000F)+56;
											}
											else
											{
												if((Error_Temp >= 0x0520)&&(Error_Temp <= 0x0529))
												{
													Error_Temp = (Error_Temp&0x000F)+67;
												}
												else
												{
													if(Error_Temp == 0x0530)
													{
														Error_Temp = (Error_Temp&0x000F)+77;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}		
			}
			else if(PRJ_NUMBER == HORNET)
			{
				if((Error_Temp >= 0x0100)&&(Error_Temp <= 0x0106))
				{
					Error_Temp = Error_Temp&0x000F+1;
				}
				else
				{
					if((Error_Temp >= 0x0200)&&(Error_Temp <= 0x0209))
					{
						Error_Temp = (Error_Temp&0x000F)+8;
					}
					else
					{
						if((Error_Temp >= 0x0210)&&(Error_Temp <= 0x0214))
						{
							Error_Temp = (Error_Temp&0x000F)+18;
						}
						else
						{
							if((Error_Temp >= 0x0300)&&(Error_Temp <= 0x0307))
							{
								Error_Temp = (Error_Temp&0x000F)+23;
							}
							else
							{
								if((Error_Temp >= 0x0401)&&(Error_Temp <= 0x0409))
								{
									Error_Temp = (Error_Temp&0x000F)+30;
								}
								else
								{
									if((Error_Temp >= 0x0410)&&(Error_Temp <= 0x0416))
									{
										Error_Temp = (Error_Temp&0x000F)+40;
									}
									else
									{
										if((Error_Temp >= 0x0501)&&(Error_Temp <= 0x0509))
										{
											Error_Temp = (Error_Temp&0x000F)+46;
										}
										else
										{
											if((Error_Temp >= 0x0510)&&(Error_Temp <= 0x0519))
											{
												Error_Temp = (Error_Temp&0x000F)+56;
											}
											else
											{
												if((Error_Temp >= 0x0520)&&(Error_Temp <= 0x0527))
												{
													Error_Temp = (Error_Temp&0x000F)+66;
												}
												else
												{
													if((Error_Temp >= 0x0601)&&(Error_Temp <= 0x0606))
													{
														Error_Temp = (Error_Temp&0x000F)+73;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}		
			}
			lubString[1] = Error_Temp / 10 + 0x30;
			lubString[2] = Error_Temp % 10 + 0x30;			
			lubString[10] = ErrorRecord.data.ErrorMessage[lubInterface - cErrorPage1].data.Hour / 10 + 0x30;
			lubString[11] = (ErrorRecord.data.ErrorMessage[lubInterface - cErrorPage1].data.Hour % 10) + 0x30;
			lubString[13] = ErrorRecord.data.ErrorMessage[lubInterface - cErrorPage1].data.Minute / 10 + 0x30;
			lubString[14] = (ErrorRecord.data.ErrorMessage[lubInterface - cErrorPage1].data.Minute % 10) + 0x30;
			lubString[16] = ErrorRecord.data.ErrorMessage[lubInterface - cErrorPage1].data.Second / 10 + 0x30;
			lubString[17] = (ErrorRecord.data.ErrorMessage[lubInterface - cErrorPage1].data.Second % 10) + 0x30;
			sWriteScreenBuf(1,0,20,lubString);
			if(PRJ_NUMBER == HORNET)
				sWriteScreenBuf(2,0,20,(INT8U *)HF_lubErrorInterpret[Error_Temp]);
			else if(PRJ_NUMBER == SUNNYBEE)
				sWriteScreenBuf(2,0,20,(INT8U *)MF_lubErrorInterpret[Error_Temp]);
			sWriteScreenBuf(3,0,20,"                    ");
			
			if(Button.bit.ubEscPress)												//单击Esc键
			{       
				lubInterface = cDefaultInterface;
			}
			else if(Button.bit.ubUpPress)											//单击Up键
			{       
				if(--lubInterface < cErrorPage1)
				{
					lubInterface = cErrorPage20;
				}
			}
			else if(Button.bit.ubDownPress)											//单击Down键
			{        
				if(++lubInterface > cErrorPage20)
				{
					lubInterface = cErrorPage1;
				}
			}
		}
		else                                      //无故障
		{
			if(lubInterface == cErrorPage1)
			{	
				sWriteScreenBuf(0,0,20,"                    ");
				sWriteScreenBuf(1,0,20,"  No Error Record   ");
				sWriteScreenBuf(2,0,20,"                    ");
				sWriteScreenBuf(3,0,20,"                    ");
						
				if(Button.bit.ubEscPress)												//??Esc?
				{       
					lubInterface = cDefaultInterface;
				}
			}
			else
			{
				lubInterface = cErrorPage1;				
			}
		}
	}
/*********************************************************************
	*
	*													设置界面
	*
	*
	**********************************************************************/	
	else if(lubInterface == cSetInterface)
	{
		sWriteScreenBuf(0,0,8,(INT8U *)lubSetMenuDisplay[Info.data.ublanguage]);
		if(++lubBlinkCnt >= 20)
		{
			lubBlinkCnt = 0;
			sWriteScreenBuf(0,8,12,(INT8U *)lubSetListDisplay[Info.data.ublanguage][lubIconSelected]);
		}
		else if(lubBlinkCnt >= 10)
		{
			sWriteScreenBuf(0,8,12,"            ");
		}
		sWriteScreenBuf(1,0,8,"        ");
		sWriteScreenBuf(2,0,8,"        ");
		sWriteScreenBuf(3,0,8,"        ");
		lubMaxSetListTemp = cMaxSetList;
		sWriteScreenBuf(1,8,12,(INT8U *)lubSetListDisplay[Info.data.ublanguage][(lubIconSelected + 1) % lubMaxSetListTemp]);
		sWriteScreenBuf(2,8,12,(INT8U *)lubSetListDisplay[Info.data.ublanguage][(lubIconSelected + 2) % lubMaxSetListTemp]);
		sWriteScreenBuf(3,8,12,(INT8U *)lubSetListDisplay[Info.data.ublanguage][(lubIconSelected + 3) % lubMaxSetListTemp]);
		
		if(Button.bit.ubEscPress)												//单击Esc键
		{        
			lubInterface = cDefaultInterface;
			lubIconSelected = 0;
		}
		else if(Button.bit.ubUpPress)											//单击Up键
		{      
			lubBlinkCnt = 10;  			
			if(--lubIconSelected >= lubMaxSetListTemp )
			{
				lubIconSelected = lubMaxSetListTemp - 1;
			}
		}
		else if(Button.bit.ubDownPress)											//单击Down键
		{        
			lubBlinkCnt = 10;  
			if(++lubIconSelected >= lubMaxSetListTemp)
			{
				lubIconSelected = 0;
			}	
		}
		else if(Button.bit.ubEnterPress)										//单击Enter键
		{   
			if((lubIconSelected > 1)&&(lubIconSelected <=12)) 
			{
				lubInterfacePre = lubInterface + lubIconSelected + 1;
				lubInterface = cPasswordIn;
			}
			else
			{
				lubInterfacePre = cSetInterface;
				lubInterface += lubIconSelected + 1;
			}
			
			lubIconSelected = 0;				
			TimeSet = SystemTime;
			if(PRJ_NUMBER == SUNNYBEE)
			{
				luwParamSet[0] = __REV16(*Param_Set_Info.MActPwrSetValue);     //通用设置
				luwParamSet[1] = __REV16(*Param_Set_Info.MReactPwrModeEn);
				luwParamSet[2] = __REV16(*Param_Set_Info.MPowerFactorSetVal);
				luwParamSet[3] = __REV16(*Param_Set_Info.MReactPwrSetValue);
				luwParamSet[4] = __REV16(*Param_Set_Info.MItalyOutsideCommFlag);
			}
			else if(PRJ_NUMBER == HORNET)
			{
				luwParamSet[0] = __REV16(*Param_Set_Info.HMaxOutputPwrPercent);     //通用设置
				luwParamSet[1] = __REV16(*Param_Set_Info.HReactPwrModeEn);
				luwParamSet[2] = __REV16(*Param_Set_Info.HPowerFactorSetVal);
				luwParamSet[3] = __REV16(*Param_Set_Info.HReactPwrQPercent);	
			}
			
			luwProtectionSet[0] = __REV16(*Protection_Func_Enable_Info.ActiveAntiIslandingEn);    //保护使能
			luwProtectionSet[1] = __REV16(*Protection_Func_Enable_Info.GfciEn);
			luwProtectionSet[2] = __REV16(*Protection_Func_Enable_Info.IsoCheckEn);
			luwProtectionSet[3] = __REV16(*Protection_Func_Enable_Info.DcInjectionEn);
			luwProtectionSet[4] = __REV16(*Protection_Func_Enable_Info.GridVoltProtThresAutoAdjEn);
			luwProtectionSet[5] = __REV16(*Protection_Func_Enable_Info.GridVoltHighPwrDeratedEn);
			luwProtectionSet[6] = __REV16(*Protection_Func_Enable_Info.GlobalMpptScanEn);
			luwProtectionSet[7] = __REV16(*Protection_Func_Enable_Info.PvVICurveEn);
			luwProtectionSet[8] = __REV16(*Protection_Func_Enable_Info.ItalySelfTestEN);
			luwProtectionSet[9] = __REV16(*Protection_Func_Enable_Info.PidFuncEn);
			luwProtectionSet[10] = __REV16(*Protection_Func_Enable_Info.DrmFuncEn);
			luwProtectionSet[11] = __REV16(*Protection_Func_Enable_Info.ArcDetectEn);
			luwProtectionSet[12] = __REV16(*Protection_Func_Enable_Info.ArcResetEn);
			if(PRJ_NUMBER == SUNNYBEE)
			{
				luwProtectionSet[13] = __REV16(*Protection_Func_Enable_Info.ZeroExpPercent);
			}
			else if(PRJ_NUMBER == HORNET)
			{
				luwProtectionSet[13] = __REV16(*Protection_Func_Enable_Info.ReconnectPwrRampUpEn);
				luwProtectionSet[14] = __REV16(*Protection_Func_Enable_Info.AntiRefluxPwrPercent);
			}
			luwCurveSet[0] = __REV16(*Curve_Enable_Info.WattFreqModeEn); //曲线使能
			luwCurveSet[1] = __REV16(*Curve_Enable_Info.WattVoltModeEn);
			luwCurveSet[2] = __REV16(*Curve_Enable_Info.PfWattModeEn);
			luwCurveSet[3] = __REV16(*Curve_Enable_Info.VarVoltModeEn);
			luwCurveSet[4] = __REV16(*Curve_Enable_Info.VarWattModeEn);
			luwCurveSet[5] = __REV16(*Curve_Enable_Info.ResvModeEn);
			luwCurveSet[6] = __REV16(*Curve_Enable_Info.LVRTCurveEn);
			luwCurveSet[7] = __REV16(*Curve_Enable_Info.HVRTCurveEn);
			luwCurveSet[8] = __REV16(*Curve_Enable_Info.LFRTCurveEn);
			luwCurveSet[9] = __REV16(*Curve_Enable_Info.HFRTCurveEn);
			
			luwKeyDataSet[0] = 0x55;   //关键数据设置
			luwKeyDataSet[1] = 0x55;
			luwKeyDataSet[2] = __REV16(*Key_Data_Info.PvInputConnectionMode);
			luwKeyDataSet[3] = __REV16(*Key_Data_Info.TotalElecGenCompVal);
			
			luwInvSet[0] = __REV16(*INV_Set_Info.InvOnOffCmd);  //逆变器设置
			luwInvSet[1] = __REV16(*INV_Set_Info.WaitStandupTime);

			
			luwFreqSet[0] = __REV16(Safty.data.uwFreqHigh[0]);
			luwFreqSet[1] = __REV16(Safty.data.uwFreqHigh[1]);			
			luwFreqSet[2] = __REV16(Safty.data.uwFreqHighCnt[0]);				
			luwFreqSet[3] = __REV16(Safty.data.uwFreqHighCnt[1]);	
			luwFreqSet[4] = __REV16(Safty.data.uwFreqLow[0]);
			luwFreqSet[5] = __REV16(Safty.data.uwFreqLow[1]);			
			luwFreqSet[6] = __REV16(Safty.data.uwFreqLowCnt[0]);
			luwFreqSet[7] = __REV16(Safty.data.uwFreqLowCnt[1]);
			
			luwVoltSet[0] = __REV16(Safty.data.uwGridVoltHigh[0]);
			luwVoltSet[1] = __REV16(Safty.data.uwGridVoltHigh[1]);
			luwVoltSet[2] = __REV16(Safty.data.uwGridVoltHighCnt[0]);
			luwVoltSet[3] = __REV16(Safty.data.uwGridVoltHighCnt[1]);
			luwVoltSet[4] = __REV16(Safty.data.uwGridVoltLow[0]);
			luwVoltSet[5] = __REV16(Safty.data.uwGridVoltLow[1]);
			luwVoltSet[6] = __REV16(Safty.data.uwGridVoltLowCnt[0]);
			luwVoltSet[7] = __REV16(Safty.data.uwGridVoltLowCnt[1]);
			
			if(PRJ_NUMBER == SUNNYBEE)
			{
				luwProtectionSetMax[0] = 0xAA;
				luwProtectionSetMax[1] = 0xAA;
				luwProtectionSetMax[2] = 0xAA;
				luwProtectionSetMax[3] = 0xAA;
				luwProtectionSetMax[4] = 0xAA;
				luwProtectionSetMax[5] = 0xAA;
				luwProtectionSetMax[6] = 0xAA;
				luwProtectionSetMax[7] = 0xAA;
				luwProtectionSetMax[8] = 0x08;
				luwProtectionSetMax[9] = 0xAA;
				luwProtectionSetMax[10] = 0xAA;
				luwProtectionSetMax[11] = 0xAA;
				luwProtectionSetMax[12] = 0xAA;
				luwProtectionSetMax[13] = 1000;
				luwProtectionSetMin[0] = 0x55;
				luwProtectionSetMin[1] = 0x55;
				luwProtectionSetMin[2] = 0x55;
				luwProtectionSetMin[3] = 0x55;
				luwProtectionSetMin[4] = 0x55;
				luwProtectionSetMin[5] = 0x55;
				luwProtectionSetMin[6] = 0x55;
				luwProtectionSetMin[7] = 0x55;
				luwProtectionSetMin[8] = 0x00;
				luwProtectionSetMin[9] = 0x55;
				luwProtectionSetMin[10] = 0x55;
				luwProtectionSetMin[11] = 0x55;
				luwProtectionSetMin[12] = 0x55;
				luwProtectionSetMin[13] = 1;
			}
			else if(PRJ_NUMBER == HORNET)
			{
				luwProtectionSetMax[0] = 1;
				luwProtectionSetMax[1] = 1;
				luwProtectionSetMax[2] = 1;
				luwProtectionSetMax[3] = 1;
				luwProtectionSetMax[4] = 1;
				luwProtectionSetMax[5] = 1;
				luwProtectionSetMax[6] = 1;
				luwProtectionSetMax[7] = 1;
				luwProtectionSetMax[8] = 0x08;
				luwProtectionSetMax[9] = 1;
				luwProtectionSetMax[10] = 1;
				luwProtectionSetMax[11] = 1;
				luwProtectionSetMax[12] = 1;
				luwProtectionSetMax[13] = 1;
				luwProtectionSetMax[14] = 100;
				luwProtectionSetMin[0] = 0;
				luwProtectionSetMin[1] = 0;
				luwProtectionSetMin[2] = 0;
				luwProtectionSetMin[3] = 0;
				luwProtectionSetMin[4] = 0;
				luwProtectionSetMin[5] = 0;
				luwProtectionSetMin[6] = 0;
				luwProtectionSetMin[7] = 0;
				luwProtectionSetMin[8] = 0x00;
				luwProtectionSetMin[9] = 0;
				luwProtectionSetMin[10] = 0;
				luwProtectionSetMin[11] = 0;
				luwProtectionSetMin[12] = 0;
				luwProtectionSetMin[13] = 0;
				luwProtectionSetMin[14] = 0;		
			}
			if(PRJ_NUMBER == SUNNYBEE)
			{
				luwParamSetMax[0] = 1100;
				luwParamSetMax[1] = 0xA4;
				luwParamSetMax[2] = 1000;
				luwParamSetMax[3] = 1000;
				luwParamSetMax[4] = 0xAA;
				luwParamSetMin[0] = 0;
				luwParamSetMin[1] = 0x55;
				luwParamSetMin[2] = -1000;
				luwParamSetMin[3] = -1000;
				luwParamSetMin[4] = 0x55;
			}
			else if(PRJ_NUMBER == HORNET)
			{
				luwParamSetMax[0] = 1100;
				luwParamSetMax[1] = 1;
				luwParamSetMax[2] = 1000;
				luwParamSetMax[3] = 1000;
				luwParamSetMin[0] = 0;
				luwParamSetMin[1] = 0;
				luwParamSetMin[2] = -1000;
				luwParamSetMin[3] = -1000;
			}
			
			if(PRJ_NUMBER == SUNNYBEE)
			{
				luwCurveSetMax[0] = 0xAA;
				luwCurveSetMax[1] = 0xAA;
				luwCurveSetMax[2] = 0xAA;
				luwCurveSetMax[3] = 0xAA;
				luwCurveSetMax[4] = 0xAA;
				luwCurveSetMax[5] = 0xAA;
				luwCurveSetMax[6] = 0xAA;
				luwCurveSetMax[7] = 0xAA;
				luwCurveSetMax[8] = 0xAA;
				luwCurveSetMax[9] = 0xAA;
				luwCurveSetMin[0] = 0x55;
				luwCurveSetMin[1] = 0x55;
				luwCurveSetMin[2] = 0x55;
				luwCurveSetMin[3] = 0x55;
				luwCurveSetMin[4] = 0x55;
				luwCurveSetMin[5] = 0x55;
				luwCurveSetMin[6] = 0x55;
				luwCurveSetMin[7] = 0x55;
				luwCurveSetMin[8] = 0x55;
				luwCurveSetMin[9] = 0x55;
			}
			else if(PRJ_NUMBER == HORNET)
			{
				luwCurveSetMax[0] = 1;
				luwCurveSetMax[1] = 1;
				luwCurveSetMax[2] = 1;
				luwCurveSetMax[3] = 1;
				luwCurveSetMax[4] = 1;
				luwCurveSetMax[5] = 1;
				luwCurveSetMax[6] = 1;
				luwCurveSetMax[7] = 1;
				luwCurveSetMax[8] = 1;
				luwCurveSetMax[9] = 1;
				luwCurveSetMin[0] = 0;
				luwCurveSetMin[1] = 0;
				luwCurveSetMin[2] = 0;
				luwCurveSetMin[3] = 0;
				luwCurveSetMin[4] = 0;
				luwCurveSetMin[5] = 0;
				luwCurveSetMin[6] = 0;
				luwCurveSetMin[7] = 0;
				luwCurveSetMin[8] = 0;
				luwCurveSetMin[9] = 0;
			}
			
				luwKeyDataSetMax[0] = 0xAA;
				luwKeyDataSetMax[1] = 0xAA;
				luwKeyDataSetMax[2] = 0x02;
				luwKeyDataSetMax[3] = 10000;
				luwKeyDataSetMin[0] = 0x55;
				luwKeyDataSetMin[1] = 0x55;
				luwKeyDataSetMin[2] = 0x01;
				luwKeyDataSetMin[3] = -10000;
			
				luwInvSetMax[0] = 0xAF;
				luwInvSetMax[1] = 255;
				luwInvSetMin[0] = 0xAE;
				luwInvSetMin[1] = 0;



		}
		
	}
	else if(lubInterface == cTimeSet)
	{
		if(++lubBlinkCnt >= 20)
		{
			lubBlinkCnt = 0;
		}		
		sWriteScreenBuf(0,0,20,"Set Time:           ");
		strcpy(lubString,"          20  -  -  ");
		if(lubIconSelected != 0 || lubBlinkCnt < 10)
		{
			lubString[12] = TimeSet.data.ubYear / 10 + 0x30;
			lubString[13] = (TimeSet.data.ubYear % 10) + 0x30;
		}
		if(lubIconSelected != 1 || lubBlinkCnt < 10)
		{
			lubString[15] = TimeSet.data.ubMonth / 10 + 0x30;
			lubString[16] = (TimeSet.data.ubMonth % 10) + 0x30;
		}
		if(lubIconSelected != 2 || lubBlinkCnt < 10)
		{
			lubString[18] = TimeSet.data.ubDate / 10 + 0x30;
			lubString[19] = (TimeSet.data.ubDate % 10) + 0x30;
		}
		sWriteScreenBuf(1,0,20,lubString);
		
		strcpy(lubString,"            :  :    ");
		if(lubIconSelected != 3 || lubBlinkCnt < 10)
		{
			lubString[10] = TimeSet.data.ubHour / 10 + 0x30;
			lubString[11] = (TimeSet.data.ubHour % 10) + 0x30;
		}
		if(lubIconSelected != 4 || lubBlinkCnt < 10)
		{
			lubString[13] = TimeSet.data.ubMinute / 10 + 0x30;
			lubString[14] = (TimeSet.data.ubMinute % 10) + 0x30;
		}
		if(lubIconSelected != 5 || lubBlinkCnt < 10)
		{
			lubString[16] = TimeSet.data.ubSecond / 10 + 0x30;
			lubString[17] = (TimeSet.data.ubSecond % 10) + 0x30;
		}
		sWriteScreenBuf(2,0,20,lubString);
		sWriteScreenBuf(3,0,20,"                    ");
		
		if(Button.bit.ubEscPress)												//单击Esc键
		{        
			lubInterface = cSetInterface;
			lubIconSelected = 0;
		}
		else if(Button.bit.ubUpPress || Button.bit.ubUpLongPress)				//单机或长按Up键
		{        
			if(lubIconSelected == 0)
			{
				if(++TimeSet.all[lubIconSelected] >= TimeRange.all[lubIconSelected])
				{
					TimeSet.all[lubIconSelected] = 17;							//年最小为17
				}
			}
			else if(lubIconSelected == 1 || lubIconSelected == 2)
			{
				if((TimeSet.all[1] == 4) || (TimeSet.all[1] == 6) || (TimeSet.all[1] == 9) || (TimeSet.all[1] == 11))
				{
					TimeRange.data.ubDate = 30;					
				}
				else
				{
					TimeRange.data.ubDate = 31;
				}
				if(TimeSet.all[1] == 2)
				{
					if(TimeSet.all[0] % 4 == 0)	
					{
						TimeRange.data.ubDate = 29;								//闰年
					}
					else
					{
						TimeRange.data.ubDate = 28;								//平年
					}
				}
				if(++TimeSet.all[lubIconSelected] > TimeRange.all[lubIconSelected])
				{
					TimeSet.all[lubIconSelected] = 1;							//月、日最小为1
				}	
			}
			else
			{
				if(++TimeSet.all[lubIconSelected] >= TimeRange.all[lubIconSelected])
				{
					TimeSet.all[lubIconSelected] = 0;							//时、分、秒最小为0
				}
			}
		}
		else if(Button.bit.ubDownPress || Button.bit.ubDownLongPress)			//单机或长按Down键
		{       
			if(lubIconSelected == 0)
			{
				if(--TimeSet.all[lubIconSelected] < 17)
				{
					TimeSet.all[lubIconSelected] = TimeRange.all[lubIconSelected] - 1;
				}
			}
			else if(lubIconSelected == 1 || lubIconSelected == 2)
			{
				if((--TimeSet.all[lubIconSelected] <= 0) || (TimeSet.all[lubIconSelected] > TimeRange.all[lubIconSelected]))
				{
					TimeSet.all[lubIconSelected] = TimeRange.all[lubIconSelected];
				}
			}
			else
			{
				if(--TimeSet.all[lubIconSelected] >= TimeRange.all[lubIconSelected])
				{
					TimeSet.all[lubIconSelected] = TimeRange.all[lubIconSelected] - 1;
				}
			}
		}
		else if(Button.bit.ubEnterPress)										//单击Enter键
		{        
			lubBlinkCnt = 10;
			if(++lubIconSelected >= 6)
			{
				lubInterface = cSetInterface;
				lubIconSelected = 0;
				SystemTime = TimeSet;			
				if(PRJ_NUMBER == SUNNYBEE)
				{
					memcpy(MF_ausSystemTime,SystemTime.all,6);
					write_10_date.usStartAddr = 0x2114;
					write_10_date.usLen = 3;
					FLAG_10_UPDATE = 1;
				}
				else if(PRJ_NUMBER == HORNET)
				{
					memcpy(HF_ausSystemTime,SystemTime.all,6);
					write_10_date.usStartAddr = 0x2280;
					write_10_date.usLen = 3;
					FLAG_10_UPDATE = 1;
				}
			}
		}
		
	}
	else if(lubInterface == cLanguageSet)										//语言设置界面
	{
		sWriteScreenBuf(0,0,20,"Language Select     ");
		if(++lubBlinkCnt >= 20)
		{
			lubBlinkCnt = 0;
			sWriteScreenBuf(1,0,20,(INT8U *)lubLanguage[lubIconSelected % 4]);
		}
		else if(lubBlinkCnt >= 10)
		{
			sWriteScreenBuf(1,0,20,"                    ");
		}
		sWriteScreenBuf(2,0,20,(INT8U *)lubLanguage[(1 + lubIconSelected) % 4]);
		sWriteScreenBuf(3,0,20,(INT8U *)lubLanguage[(2 + lubIconSelected) % 4]);
		
		if(Button.bit.ubEnterPress)													//单击Enter键
		{        
			lubInterface = cSetInterface;
			Info.data.ublanguage = lubIconSelected;
			lubIconSelected = 0;
			IicFlag.bit.mubInfoW = cTrue;
		}
		else if(Button.bit.ubEscPress)												//单击Esc键
		{        
				lubInterface = cSetInterface;
				lubIconSelected = 0;
		}
		else if(Button.bit.ubUpPress)												//单击Up键
		{       
			lubBlinkCnt = 10;			
			if(--lubIconSelected >= cMaxLanguage)
			{
				lubIconSelected = cITALIANO;
			}
		}
		else if(Button.bit.ubDownPress)												//单击Down键
		{        
			lubBlinkCnt = 10;
			if(++lubIconSelected >= cMaxLanguage)
			{
				lubIconSelected = cEnglish;
			}
		}
	}
	
	
	else if(lubInterface == cAutoTestSet)         //自检设置
	{
		luwNoOprateCnt = 0;
		if(Safty.data.uwCountry == cItaly )
		{
			sAutoTestAsis(1);
			lubInterfacePre = cAutoTestSet;
		}
		else
		{
			lubInterface = cSetInterface;
		}
	}
	else if(lubInterface == cAutoTestResult)     //自检结果
	{
	
		if(Safty.data.uwCountry == cItaly )
		{
			strcpy(lubString,"Test Time 20  -  -  ");
			lubString[12] = ((INT8U)AutoTestResult_Info.TEST_TIME[0] / 10) + 0x30;
			lubString[13] = ((INT8U)AutoTestResult_Info.TEST_TIME[0] % 10) + 0x30;
			lubString[15] = ((INT8U)(AutoTestResult_Info.TEST_TIME[0]>>8) / 10) + 0x30;
			lubString[16] = ((INT8U)(AutoTestResult_Info.TEST_TIME[0]>>8) % 10) + 0x30;
			lubString[18] = ((INT8U)AutoTestResult_Info.TEST_TIME[1]/ 10) + 0x30;
			lubString[19] = ((INT8U)AutoTestResult_Info.TEST_TIME[1] % 10) + 0x30;
			sWriteScreenBuf(0,0,20,lubString);
		
			strcpy(lubString,"            :  :    ");
			lubString[10] = ((INT8U)(AutoTestResult_Info.TEST_TIME[1]>>8) / 10) + 0x30;
			lubString[11] = ((INT8U)(AutoTestResult_Info.TEST_TIME[1]>>8) % 10) + 0x30;
			lubString[13] = ((INT8U)AutoTestResult_Info.TEST_TIME[2]/ 10) + 0x30;
			lubString[14] = ((INT8U)AutoTestResult_Info.TEST_TIME[2] % 10) + 0x30;
			lubString[16] = ((INT8U)(AutoTestResult_Info.TEST_TIME[2]>>8) / 10) + 0x30;
			lubString[17] = ((INT8U)(AutoTestResult_Info.TEST_TIME[2]>>8) % 10) + 0x30;
			sWriteScreenBuf(1,0,20,lubString);
			switch(lubIconSelected)
			{
				case 0:
				{
					strcpy(lubString,"27.S1       V     ms");
					sTransferData(1,__REV16(AutoTestResult_Info.ItalyResult[0].Trip_Value),&lubString[7],4);
					sDisplayAsist(7,9,lubString);	
					sTransferData(0,__REV16(AutoTestResult_Info.ItalyResult[0].Trip_Time),&lubString[14],4);
					sDisplayAsist(14,17,lubString);						
					sWriteScreenBuf(2,0,20,lubString);					
					strcpy(lubString,"27.S2       V     ms");
					sTransferData(1,__REV16(AutoTestResult_Info.ItalyResult[1].Trip_Value),&lubString[7],4);
					sDisplayAsist(7,9,lubString);	
					sTransferData(0,__REV16(AutoTestResult_Info.ItalyResult[1].Trip_Time),&lubString[14],4);
					sDisplayAsist(14,17,lubString);						
					sWriteScreenBuf(3,0,20,lubString);	
			
				}
				break;
				case 1:
				{
					strcpy(lubString,"59.S1       V     ms");
					sTransferData(1,__REV16(AutoTestResult_Info.ItalyResult[2].Trip_Value),&lubString[7],4);
					sDisplayAsist(7,9,lubString);	
					sTransferData(0,__REV16(AutoTestResult_Info.ItalyResult[2].Trip_Time),&lubString[14],4);
					sDisplayAsist(14,17,lubString);						
					sWriteScreenBuf(2,0,20,lubString);					
					strcpy(lubString,"59.S2       V     ms");
					sTransferData(1,__REV16(AutoTestResult_Info.ItalyResult[3].Trip_Value),&lubString[7],4);
					sDisplayAsist(7,9,lubString);	
					sTransferData(0,__REV16(AutoTestResult_Info.ItalyResult[3].Trip_Time),&lubString[14],4);
					sDisplayAsist(14,17,lubString);						
					sWriteScreenBuf(3,0,20,lubString);	
				}
				break;
				case 2:
				{
					strcpy(lubString,"81<S1      Hz     ms");
					sTransferData(2,(__REV16(AutoTestResult_Info.ItalyResult[4].Trip_Value)/10),&lubString[6],4);
					sDisplayAsist(6,7,lubString);	
					sTransferData(0,__REV16(AutoTestResult_Info.ItalyResult[4].Trip_Time),&lubString[14],4);
					sDisplayAsist(14,17,lubString);						
					sWriteScreenBuf(2,0,20,lubString);					
					strcpy(lubString,"81<S2      Hz     ms");
					sTransferData(2,(__REV16(AutoTestResult_Info.ItalyResult[5].Trip_Value)/10),&lubString[6],4);
					sDisplayAsist(6,7,lubString);	
					sTransferData(0,__REV16(AutoTestResult_Info.ItalyResult[5].Trip_Time),&lubString[14],4);
					sDisplayAsist(14,17,lubString);						
					sWriteScreenBuf(3,0,20,lubString);	
				}
				break;
				case 3:
				{
					strcpy(lubString,"81>S1      Hz     ms");
					sTransferData(2,(__REV16(AutoTestResult_Info.ItalyResult[6].Trip_Value)/10),&lubString[6],4);
					sDisplayAsist(6,7,lubString);	
					sTransferData(0,__REV16(AutoTestResult_Info.ItalyResult[6].Trip_Time),&lubString[14],4);
					sDisplayAsist(14,17,lubString);						
					sWriteScreenBuf(2,0,20,lubString);					
					strcpy(lubString,"81>S2      Hz     ms");
					sTransferData(2,(__REV16(AutoTestResult_Info.ItalyResult[7].Trip_Value)/10),&lubString[6],4);
					sDisplayAsist(6,7,lubString);	
					sTransferData(0,__REV16(AutoTestResult_Info.ItalyResult[7].Trip_Time),&lubString[14],4);
					sDisplayAsist(14,17,lubString);						
					sWriteScreenBuf(3,0,20,lubString);	
				}
				break;
				
			}
			if(Button.bit.ubEscPress)												//单击Esc键
			{        
				lubInterface = cSetInterface;
				lubIconSelected = 0;
			}
			else if(Button.bit.ubUpPress)											//单击Up键
			{        
				if(--lubIconSelected >= 4)
				{
					lubIconSelected = 3;
				}
			}
			else if(Button.bit.ubDownPress)											//单击Down键
			{        
				if(++lubIconSelected >= 4)
				{
					lubIconSelected = 0;
				}
			}
			else if(Button.bit.ubEnterPress)										//单击Enter键
			{        
				
			}	
		}
		else
		{
			lubInterface = cSetInterface;
		}
	}
	else if(lubInterface == cPasswordSet)         //密码设置
	{
		sWriteScreenBuf(0,0,20,"                    ");
		sWriteScreenBuf(1,0,20,"Set Password        ");
		strcpy(lubString,"                    ");
		
		if(++lubBlinkCnt >= 20)
		{
			lubBlinkCnt = 0;
		}	
		if(lubIconSelected != 0 || lubBlinkCnt < 10)
		{
			lubString[6] = lubPasswordSet[0];
		}
		if(lubIconSelected != 1 || lubBlinkCnt < 10)
		{
			lubString[7] = lubPasswordSet[1];
		}
		if(lubIconSelected != 2 || lubBlinkCnt <10)
		{
			lubString[8] = lubPasswordSet[2];
		}
		if(lubIconSelected != 3 || lubBlinkCnt < 10)
		{
			lubString[9] = lubPasswordSet[3];
		}
		if(lubIconSelected != 4 || lubBlinkCnt < 10)
		{
			lubString[10] = lubPasswordSet[4];
		}
		if(lubIconSelected != 5 || lubBlinkCnt < 10)
		{
			lubString[11] = lubPasswordSet[5];
		}
		sWriteScreenBuf(2,0,20,lubString);
		sWriteScreenBuf(3,0,20,"                    ");
		
		if(Button.bit.ubEscPress)												//单击Esc键
		{        
			lubInterface = cSetInterface;
			lubIconSelected = 0;
		}
		else if(Button.bit.ubUpPress)											//单击Up键
		{        
			if(++lubPasswordSet[lubIconSelected] > 0x39)
			{
				lubPasswordSet[lubIconSelected] = 0x30;
			}
		}
		else if(Button.bit.ubDownPress)											//单击Down键
		{        
			if(--lubPasswordSet[lubIconSelected] < 0x30)
			{
				lubPasswordSet[lubIconSelected] = 0x39;
			}
		}
		else if(Button.bit.ubEnterPress)										//单击Enter键
		{        
			if(++lubIconSelected >= 6)
			{
				lubIconSelected = 0;
				Info.data.ubPasswordH = (lubPasswordSet[0] - 0x30) * 16 + lubPasswordSet[1] - 0x30;
				Info.data.ubPasswordM = (lubPasswordSet[2] - 0x30) * 16 + lubPasswordSet[3] - 0x30;
				Info.data.ubPasswordL = (lubPasswordSet[4] - 0x30) * 16 + lubPasswordSet[5] - 0x30;
				lubInterface = cPasswordSaved;
				IicFlag.bit.mubInfoW = cTrue;
			
				for(i = 0;i < 6;i++)
				{
					lubPasswordSet[i] = 0x30;
				}
			}
			
		}
		
	}
	else if(lubInterface == cSaftySet)												//法规设置界面
	{		
		sWriteScreenBuf(0,0,9,"Country  ");
		sWriteScreenBuf(1,0,9,"Select   ");
		sWriteScreenBuf(2,0,9,"          ");
		sWriteScreenBuf(3,0,9,"          ");
		if(++lubBlinkCnt >= 20)
		{
			lubBlinkCnt = 0;
			sWriteScreenBuf(0,9,11,(INT8U *)lubSafty[lubSaftySort[lubIconSelected]]);
		}
		else if(lubBlinkCnt >= 10)
		{
			sWriteScreenBuf(0,9,11,"           ");
		}
		sWriteScreenBuf(1,9,11,(INT8U *)lubSafty[lubSaftySort[(lubIconSelected + 1) % cMaxSaftyCode]]);
		sWriteScreenBuf(2,9,11,(INT8U *)lubSafty[lubSaftySort[(lubIconSelected + 2) % cMaxSaftyCode]]);
		sWriteScreenBuf(3,9,11,(INT8U *)lubSafty[lubSaftySort[(lubIconSelected + 3) % cMaxSaftyCode]]);
		
		if(Button.bit.ubEnterPress)													//单击Enter键
		{  
			Safty.data.uwCountry = lubSaftySort[lubIconSelected];     
			if(PRJ_NUMBER == SUNNYBEE)
			{
				MF_usCountry = __REV16(Safty.data.uwCountry);      //国家安规
				write_10_date.usStartAddr = 0x2104;
				write_10_date.usLen = 1;
				FLAG_10_UPDATE = 1;
			}
			else if(PRJ_NUMBER == HORNET)
			{
				HF_usCountry = __REV16(Safty.data.uwCountry);      //国家安规
				write_10_date.usStartAddr = 0x2102;
				write_10_date.usLen = 1;
				FLAG_10_UPDATE = 1;
			}
			if(lubInterfacePre == cSaftyMessage)
			{
				lubInterface = cDefaultInterface;
				lubIconSelected = 0;
				Info.data.ubFirstTurnOn = cFalse;
				IicFlag.bit.mubInfoW = cTrue;
			}
			else
			{
				lubInterface = cSetInterface;
				lubIconSelected = 0;	
			}
		}
		else if(Button.bit.ubEscPress)												//单击Esc键
		{        
			if(lubInterfacePre != cSaftyMessage)
			{
				lubInterface = cSetInterface;
				lubIconSelected = 0;
			}
		}
		else if(Button.bit.ubDownPress || Button.bit.ubUpLongPress)												//单击Up键
		{        
			if(++lubIconSelected >= cMaxSaftyCode)
			{
				lubIconSelected = cUK;
			}
		}
		else if(Button.bit.ubUpPress || Button.bit.ubDownLongPress)												//单击Down键
		{        
			if(--lubIconSelected >= cMaxSaftyCode)
			{
				lubIconSelected = cLuxembourg;
			}
		}
		
	}
	else if(lubInterface == cProtectionSet)       //保护设置
	{
		sWriteScreenBuf(0,0,20,"Protection Set      ");
		if(++lubBlinkCnt >= 20)
		{
			lubBlinkCnt = 0;
			if(PRJ_NUMBER == SUNNYBEE)	
			{
				if(lubIconSelected <= 11)
				{
					sWriteScreenBuf(1,0,20,(INT8U *)MF_Protection_Set[lubIconSelected ]);
					sWriteScreenBuf(2,0,20,(INT8U *)MF_Protection_Set[(1 + lubIconSelected) ]);
					sWriteScreenBuf(3,0,20,(INT8U *)MF_Protection_Set[(2 + lubIconSelected) ]);
				}
				if(lubIconSelected == 12)
				{
					sWriteScreenBuf(1,0,20,(INT8U *)MF_Protection_Set[lubIconSelected ]);
					sWriteScreenBuf(2,0,20,(INT8U *)MF_Protection_Set[(1 + lubIconSelected) ]);
					sWriteScreenBuf(3,0,20,(INT8U *)MF_Protection_Set[0]);
				}
				if(lubIconSelected == 13)
				{
					sWriteScreenBuf(1,0,20,(INT8U *)MF_Protection_Set[lubIconSelected ]);
					sWriteScreenBuf(2,0,20,(INT8U *)MF_Protection_Set[0]);
					sWriteScreenBuf(3,0,20,(INT8U *)MF_Protection_Set[1]);
				}
			}
			else if(PRJ_NUMBER == HORNET)
			{
				if(lubIconSelected <= 10)
				{
					sWriteScreenBuf(1,0,20,(INT8U *)HF_Protection_Set[lubIconSelected ]);
					sWriteScreenBuf(2,0,20,(INT8U *)HF_Protection_Set[(1 + lubIconSelected) ]);
					sWriteScreenBuf(3,0,20,(INT8U *)HF_Protection_Set[(2 + lubIconSelected) ]);
				}
				if(lubIconSelected == 11)
				{
					sWriteScreenBuf(1,0,20,(INT8U *)HF_Protection_Set[lubIconSelected ]);
					sWriteScreenBuf(2,0,20,(INT8U *)HF_Protection_Set[(1 + lubIconSelected) ]);
					sWriteScreenBuf(3,0,20,(INT8U *)HF_Protection_Set[0]);
				}
				if(lubIconSelected == 12)
				{
					sWriteScreenBuf(1,0,20,(INT8U *)HF_Protection_Set[lubIconSelected ]);
					sWriteScreenBuf(2,0,20,(INT8U *)HF_Protection_Set[0]);
					sWriteScreenBuf(3,0,20,(INT8U *)HF_Protection_Set[1]);
				}
			}
			else
			{
				sWriteScreenBuf(1,0,20,"                    ");
				sWriteScreenBuf(2,0,20,"                    ");
				sWriteScreenBuf(3,0,20,"                    ");
			}
		}	
		else if(lubBlinkCnt >= 10)
		{
			sWriteScreenBuf(1,0,20,"                    ");
		}
		if(PRJ_NUMBER == SUNNYBEE)	
		{
			switch(lubIconSelected)
			{
				case 0:
				{
					switch(luwProtectionSet[0])
					{
						case 0xAA:
							sWriteScreenBuf(1,15,20,"<YES>");
						break;
						case 0x55:
							sWriteScreenBuf(1,15,20,"< N0>");
						break;
						default:
							sWriteScreenBuf(1,15,20,"<DEF>");
						break;
							
					}
					sWriteScreenBuf(2,15,20,"< NC>");
					sWriteScreenBuf(3,15,20,"< NC>");
				}
					break;
				case 1:
				{
					sWriteScreenBuf(1,15,20,"< NC>");
					sWriteScreenBuf(2,15,20,"< NC>");
					sWriteScreenBuf(3,15,20,"< NC>");
				}
					break;
				case 2:
				{
					sWriteScreenBuf(1,15,20,"< NC>");
					sWriteScreenBuf(2,15,20,"< NC>");
					sWriteScreenBuf(3,15,20,"< NC>");
				}
					break;
				case 3:
				{
					sWriteScreenBuf(1,15,20,"< NC>");
					sWriteScreenBuf(2,15,20,"< NC>");
					switch(luwProtectionSet[5])
					{
						case 0x55:
							sWriteScreenBuf(3,15,20,"<YES>");
						break;
						case 0xAA:
							sWriteScreenBuf(3,15,20,"< N0>");
						break;
						default:
							sWriteScreenBuf(3,15,20,"<DEF>");
						break;
							
					}
				}
					break;
				case 4:
				{
					sWriteScreenBuf(1,15,20,"< NC>");				
					switch(luwProtectionSet[5])
					{
						case 0x55:
							sWriteScreenBuf(2,15,20,"<YES>");
						break;
						case 0xAA:
							sWriteScreenBuf(2,15,20,"< N0>");
						break;
						default:
							sWriteScreenBuf(2,15,20,"<DEF>");
						break;		
					}
				sWriteScreenBuf(3,15,20,"< NC>");
				}
					break;
				case 5:
				{			
					switch(luwProtectionSet[5])
					{
						case 0x55:
							sWriteScreenBuf(1,15,20,"<YES>");
						break;
						case 0xAA:
							sWriteScreenBuf(1,15,20,"< N0>");
						break;
						default:
							sWriteScreenBuf(1,15,20,"<DEF>");
						break;		
					}
				sWriteScreenBuf(2,15,20,"< NC>");	
				sWriteScreenBuf(3,15,20,"< NC>");
				}
					break;
				case 6:
				{					
				 sWriteScreenBuf(1,15,20,"< NC>");	
				 sWriteScreenBuf(2,15,20,"< NC>");
					switch(luwProtectionSet[8])
					{
						case 0:
							sWriteScreenBuf(3,15,20,"CLOSE");
						break;
						case 1:
							sWriteScreenBuf(3,15,20,"27.S1");
						break;
						case 2:
							sWriteScreenBuf(3,15,20,"27.S2");
						break;
						case 3:
							sWriteScreenBuf(3,15,20,"59.S1");
						break;
						case 4:
							sWriteScreenBuf(3,15,20,"59.S2");
						break;
						case 5:
							sWriteScreenBuf(3,15,20,"81<S1");
						break;
						case 6:
							sWriteScreenBuf(3,15,20,"81<S2");
						break;
						case 7:
							sWriteScreenBuf(3,15,20,"81>S1");
						break;
						case 8:
							sWriteScreenBuf(3,15,20,"81>S2");
						break;
						default:
							sWriteScreenBuf(3,15,20,"<DEF>");
						break;		
					}
				}
					break;
				case 7:
				{					
				 sWriteScreenBuf(1,15,20,"< NC>");	
					switch(luwProtectionSet[8])
					{
						case 0:
							sWriteScreenBuf(2,15,20,"CLOSE");
						break;
						case 1:
							sWriteScreenBuf(2,15,20,"27.S1");
						break;
						case 2:
							sWriteScreenBuf(2,15,20,"27.S2");
						break;
						case 3:
							sWriteScreenBuf(2,15,20,"59.S1");
						break;
						case 4:
							sWriteScreenBuf(2,15,20,"59.S2");
						break;
						case 5:
							sWriteScreenBuf(2,15,20,"81<S1");
						break;
						case 6:
							sWriteScreenBuf(2,15,20,"81<S2");
						break;
						case 7:
							sWriteScreenBuf(2,15,20,"81>S1");
						break;
						case 8:
							sWriteScreenBuf(2,15,20,"81>S2");
						break;
						default:
							sWriteScreenBuf(2,15,20,"<DEF>");
						break;		
					}
				 sWriteScreenBuf(3,15,20,"< NC>");
				}
					break;
				case 8:
				{					
					switch(luwProtectionSet[8])
					{
						case 0:
							sWriteScreenBuf(1,15,20,"CLOSE");
						break;
						case 1:
							sWriteScreenBuf(1,15,20,"27.S1");
						break;
						case 2:
							sWriteScreenBuf(1,15,20,"27.S2");
						break;
						case 3:
							sWriteScreenBuf(1,15,20,"59.S1");
						break;
						case 4:
							sWriteScreenBuf(1,15,20,"59.S2");
						break;
						case 5:
							sWriteScreenBuf(1,15,20,"81<S1");
						break;
						case 6:
							sWriteScreenBuf(1,15,20,"81<S2");
						break;
						case 7:
							sWriteScreenBuf(1,15,20,"81>S1");
						break;
						case 8:
							sWriteScreenBuf(1,15,20,"81>S2");
						break;
						default:
							sWriteScreenBuf(1,15,20,"<DEF>");
						break;		
					}
				 sWriteScreenBuf(2,15,20,"< NC>");
				 switch(luwProtectionSet[10])
					{
						case 0x55:
							sWriteScreenBuf(3,15,20,"<YES>");
						break;
						case 0xAA:
							sWriteScreenBuf(3,15,20,"< N0>");
						break;
						default:
							sWriteScreenBuf(3,15,20,"<DEF>");
						break;		
					}
				}
					break;
				case 9:
				{
				 sWriteScreenBuf(1,15,20,"< NC>");
				 switch(luwProtectionSet[10])
					{
						case 0x55:
							sWriteScreenBuf(2,15,20,"<YES>");
						break;
						case 0xAA:
							sWriteScreenBuf(2,15,20,"< N0>");
						break;
						default:
							sWriteScreenBuf(2,15,20,"<DEF>");
						break;		
					}
				 sWriteScreenBuf(3,15,20,"< NC>");
				}
				 break;
				case 10:
				{
				 switch(luwProtectionSet[10])
					{
						case 0x55:
							sWriteScreenBuf(1,15,20,"<YES>");
						break;
						case 0xAA:
							sWriteScreenBuf(1,15,20,"< N0>");
						break;
						default:
							sWriteScreenBuf(1,15,20,"<DEF>");
						break;		
					}
				 sWriteScreenBuf(2,15,20,"< NC>");
				 sWriteScreenBuf(3,15,20,"< NC>");
				}
				 break;
				case 11:
				{
				 sWriteScreenBuf(1,15,20,"< NC>");
				 sWriteScreenBuf(2,15,20,"< NC>");
				 sTransferData(0,luwProtectionSet[13],&lubString[16],4);
				 sWriteScreenBuf(3,16,20,&lubString[16]);
				}
				 break;
				case 12:
				{
				 sWriteScreenBuf(1,15,20,"< NC>");
				 sTransferData(0,luwProtectionSet[13],&lubString[16],4);
				 sWriteScreenBuf(2,16,20,&lubString[16]);		 
				 switch(luwProtectionSet[0])
					{
						case 0xAA:
							sWriteScreenBuf(3,15,20,"<YES>");
						break;
						case 0x55:
							sWriteScreenBuf(3,15,20,"< N0>");
						break;
						default:
							sWriteScreenBuf(3,15,20,"<DEF>");
						break;		
					}
				}
				 break;
				case 13:
				{
				 sTransferData(0,luwProtectionSet[13],&lubString[16],4);
				 sWriteScreenBuf(1,16,20,&lubString[16]);		 
				 switch(luwProtectionSet[0])
					{
						case 0xAA:
							sWriteScreenBuf(2,15,20,"<YES>");
						break;
						case 0x55:
							sWriteScreenBuf(2,15,20,"< N0>");
						break;
						default:
							sWriteScreenBuf(2,15,20,"<DEF>");
						break;		
					}
			   sWriteScreenBuf(3,15,20,"< NC>");
				}
				 break;
			}			
		}
		else if(PRJ_NUMBER == HORNET)	
		{
			switch(lubIconSelected)
			{
				case 0:
				{
					switch(luwProtectionSet[0])
					{
						case 0:
							sWriteScreenBuf(1,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(1,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[1])
					{
						case 0:
							sWriteScreenBuf(2,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(2,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[2])
					{
						case 0:
							sWriteScreenBuf(3,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(3,15,20,"<DEF>");
						break;						
					}
				}
					break;
				case 1:
				{
					switch(luwProtectionSet[1])
					{
						case 0:
							sWriteScreenBuf(1,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(1,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[2])
					{
						case 0:
							sWriteScreenBuf(2,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(2,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[3])
					{
						case 0:
							sWriteScreenBuf(3,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(3,15,20,"<DEF>");
						break;						
					}
				}
					break;
				case 2:
				{
					switch(luwProtectionSet[2])
					{
						case 0:
							sWriteScreenBuf(1,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(1,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[3])
					{
						case 0:
							sWriteScreenBuf(2,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(2,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[4])
					{
						case 0:
							sWriteScreenBuf(3,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(3,15,20,"<DEF>");
						break;						
					}
				}
					break;
				case 3:
				{
					switch(luwProtectionSet[3])
					{
						case 0:
							sWriteScreenBuf(1,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(1,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[4])
					{
						case 0:
							sWriteScreenBuf(2,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(2,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[5])
					{
						case 0:
							sWriteScreenBuf(3,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(3,15,20,"<DEF>");
						break;						
					}
				}
					break;
				case 4:
				{
					switch(luwProtectionSet[4])
					{
						case 0:
							sWriteScreenBuf(1,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(1,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[5])
					{
						case 0:
							sWriteScreenBuf(2,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(2,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[6])
					{
						case 0:
							sWriteScreenBuf(3,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(3,15,20,"<DEF>");
						break;						
					}
				}
					break;
				case 5:
				{
					switch(luwProtectionSet[5])
					{
						case 0:
							sWriteScreenBuf(1,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(1,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[6])
					{
						case 0:
							sWriteScreenBuf(2,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(2,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[7])
					{
						case 0:
							sWriteScreenBuf(3,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(3,15,20,"<DEF>");
						break;						
					}
				}
					break;
				case 6:
				{
					switch(luwProtectionSet[6])
					{
						case 0:
							sWriteScreenBuf(1,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(1,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[7])
					{
						case 0:
							sWriteScreenBuf(2,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(2,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[8])
					{
						case 0:
							sWriteScreenBuf(3,15,20,"CLOSE");
						break;
						case 1:
							sWriteScreenBuf(3,15,20,"27.S1");
						break;
						case 2:
							sWriteScreenBuf(3,15,20,"27.S2");
						break;
						case 3:
							sWriteScreenBuf(3,15,20,"59.S1");
						break;
						case 4:
							sWriteScreenBuf(3,15,20,"59.S2");
						break;
						case 5:
							sWriteScreenBuf(3,15,20,"81<S1");
						break;
						case 6:
							sWriteScreenBuf(3,15,20,"81<S2");
						break;
						case 7:
							sWriteScreenBuf(3,15,20,"81>S1");
						break;
						case 8:
							sWriteScreenBuf(3,15,20,"81>S2");
						break;
						default:
							sWriteScreenBuf(3,15,20,"<DEF>");
						break;		
					}
				}
					break;
				case 7:
				{
					switch(luwProtectionSet[7])
					{
						case 0:
							sWriteScreenBuf(1,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(1,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[8])
					{
						case 0:
							sWriteScreenBuf(2,15,20,"CLOSE");
						break;
						case 1:
							sWriteScreenBuf(2,15,20,"27.S1");
						break;
						case 2:
							sWriteScreenBuf(2,15,20,"27.S2");
						break;
						case 3:
							sWriteScreenBuf(2,15,20,"59.S1");
						break;
						case 4:
							sWriteScreenBuf(2,15,20,"59.S2");
						break;
						case 5:
							sWriteScreenBuf(2,15,20,"81<S1");
						break;
						case 6:
							sWriteScreenBuf(2,15,20,"81<S2");
						break;
						case 7:
							sWriteScreenBuf(2,15,20,"81>S1");
						break;
						case 8:
							sWriteScreenBuf(2,15,20,"81>S2");
						break;
						default:
							sWriteScreenBuf(2,15,20,"<DEF>");
						break;		
					}
					switch(luwProtectionSet[9])
					{
						case 0:
							sWriteScreenBuf(3,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(3,15,20,"<DEF>");
						break;						
					}
				}
					break;
				case 8:
				{
					switch(luwProtectionSet[8])
					{
						case 0:
							sWriteScreenBuf(1,15,20,"CLOSE");
						break;
						case 1:
							sWriteScreenBuf(1,15,20,"27.S1");
						break;
						case 2:
							sWriteScreenBuf(1,15,20,"27.S2");
						break;
						case 3:
							sWriteScreenBuf(1,15,20,"59.S1");
						break;
						case 4:
							sWriteScreenBuf(1,15,20,"59.S2");
						break;
						case 5:
							sWriteScreenBuf(1,15,20,"81<S1");
						break;
						case 6:
							sWriteScreenBuf(1,15,20,"81<S2");
						break;
						case 7:
							sWriteScreenBuf(1,15,20,"81>S1");
						break;
						case 8:
							sWriteScreenBuf(1,15,20,"81>S2");
						break;
						default:
							sWriteScreenBuf(1,15,20,"<DEF>");
						break;		
					}
					switch(luwProtectionSet[9])
					{
						case 0:
							sWriteScreenBuf(2,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(2,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[10])
					{
						case 0:
							sWriteScreenBuf(3,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(3,15,20,"<DEF>");
						break;						
					}
				}
					break;
				case 9:
				{
					switch(luwProtectionSet[9])
					{
						case 0:
							sWriteScreenBuf(1,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(1,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[10])
					{
						case 0:
							sWriteScreenBuf(2,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(2,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[11])
					{
						case 0:
							sWriteScreenBuf(3,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(3,15,20,"<DEF>");
						break;						
					}
				}
					break;
				case 10:
				{
					switch(luwProtectionSet[10])
					{
						case 0:
							sWriteScreenBuf(1,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(1,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[11])
					{
						case 0:
							sWriteScreenBuf(2,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(2,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[12])
					{
						case 0:
							sWriteScreenBuf(3,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(3,15,20,"<DEF>");
						break;						
					}
				}
					break;
				case 11:
				{
					switch(luwProtectionSet[11])
					{
						case 0:
							sWriteScreenBuf(1,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(1,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[12])
					{
						case 0:
							sWriteScreenBuf(2,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(2,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[0])
					{
						case 0:
							sWriteScreenBuf(3,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(3,15,20,"<DEF>");
						break;						
					}
				}
					break;
				case 12:
				{
					switch(luwProtectionSet[12])
					{
						case 0:
							sWriteScreenBuf(1,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(1,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[0])
					{
						case 0:
							sWriteScreenBuf(2,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(2,15,20,"<DEF>");
						break;						
					}
					switch(luwProtectionSet[1])
					{
						case 0:
							sWriteScreenBuf(3,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(3,15,20,"<DEF>");
						break;						
					}
				}
					break;
			}			
		}
		if(Button.bit.ubEscPress)												//单击Esc键
		{        
			lubInterface = cSetInterface;
			lubIconSelected = 0;
		}
		else if(Button.bit.ubUpPress || Button.bit.ubUpLongPress)											//单击Up键
		{   
			if(++luwProtectionSet[lubIconSelected] > luwProtectionSetMax[lubIconSelected])
			{
				luwProtectionSet[lubIconSelected] = luwProtectionSetMin[lubIconSelected];
			}	
		}
		else if(Button.bit.ubDownPress || Button.bit.ubDownLongPress)											//单击Down键
		{        
			if(--luwProtectionSet[lubIconSelected] > luwProtectionSetMax[lubIconSelected])
			{
				luwProtectionSet[lubIconSelected] = luwProtectionSetMax[lubIconSelected];
			}
		}
		else if(Button.bit.ubEnterPress)										//单击Enter键
		{  
			lubBlinkCnt	= 10;		
			if(PRJ_NUMBER == SUNNYBEE)
			{
				if(++lubIconSelected > 13)
				{
					lubIconSelected = 0;
				}
			}
			else if(PRJ_NUMBER == HORNET)
			{
				if(++lubIconSelected > 12)
				{
					lubIconSelected = 0;
				}
			}
		}
		else if(Button.bit.ubEnterLongPress)									//长按Enter键
		{
			*Protection_Func_Enable_Info.ActiveAntiIslandingEn = __REV16(luwProtectionSet[0]);    //保护使能
			*Protection_Func_Enable_Info.GfciEn = __REV16(luwProtectionSet[1]);
			*Protection_Func_Enable_Info.IsoCheckEn = __REV16(luwProtectionSet[2]);
			*Protection_Func_Enable_Info.DcInjectionEn = __REV16(luwProtectionSet[3]);
			*Protection_Func_Enable_Info.GridVoltProtThresAutoAdjEn = __REV16(luwProtectionSet[4]);
			*Protection_Func_Enable_Info.GridVoltHighPwrDeratedEn = __REV16(luwProtectionSet[5]);
			*Protection_Func_Enable_Info.GlobalMpptScanEn = __REV16(luwProtectionSet[6]);
			*Protection_Func_Enable_Info.PvVICurveEn = __REV16(luwProtectionSet[7]);
			*Protection_Func_Enable_Info.ItalySelfTestEN = __REV16(luwProtectionSet[8]);
			*Protection_Func_Enable_Info.PidFuncEn = __REV16(luwProtectionSet[9]);
			*Protection_Func_Enable_Info.DrmFuncEn = __REV16(luwProtectionSet[10]);
			*Protection_Func_Enable_Info.ArcDetectEn = __REV16(luwProtectionSet[11]);
			*Protection_Func_Enable_Info.ArcResetEn = __REV16(luwProtectionSet[12]);
			if(PRJ_NUMBER == SUNNYBEE)
			{
				*Protection_Func_Enable_Info.ZeroExpPercent = __REV16(luwProtectionSet[13]);
				write_10_date.usStartAddr = 0x213F;
				write_10_date.usLen = 25;
				FLAG_10_UPDATE = 1;
			}
			else if(PRJ_NUMBER == HORNET)
			{
				*Protection_Func_Enable_Info.ReconnectPwrRampUpEn = __REV16(luwProtectionSet[13]);
				*Protection_Func_Enable_Info.AntiRefluxPwrPercent = __REV16(luwProtectionSet[14]);
				write_10_date.usStartAddr = 0x2482;
				write_10_date.usLen = 15;
				FLAG_10_UPDATE = 1;
			}
			
			while(cGetButtonEnter());
			Button.all &= 0x0000;
			lubIconSelected = 0;
			lubInterface = cSetInterface;
		}
		if(PRJ_NUMBER == SUNNYBEE)
		{
			for(i = 0;i < 8;i++)
			{
				if((luwProtectionSet[i] == 0xA9)||(luwProtectionSet[i] == 0xAB))
					luwProtectionSet[i] = luwProtectionSetMin[i];
				if((luwProtectionSet[i] == 0x54)||(luwProtectionSet[i] == 0x56))
					luwProtectionSet[i] = luwProtectionSetMax[i];
			}
			
			for(i = 9;i < 13;i++)
			{
				if((luwProtectionSet[i] == 0xA9)||(luwProtectionSet[i] == 0xAB))
					luwProtectionSet[i] = luwProtectionSetMin[i];
				if((luwProtectionSet[i] == 0x54)||(luwProtectionSet[i] == 0x56))
					luwProtectionSet[i] = luwProtectionSetMax[i];
			}
			
			if(luwProtectionSet[13] >luwProtectionSetMax[13])
				luwProtectionSet[13] = luwProtectionSetMin[13];
			if(luwProtectionSet[13] <luwProtectionSetMin[13])
				luwProtectionSet[13] = luwProtectionSetMax[13];
		}
		else if(PRJ_NUMBER == HORNET)
		{
		
			if(luwProtectionSet[14] >luwProtectionSetMax[14])
				luwProtectionSet[14] = luwProtectionSetMin[14];
			if(luwProtectionSet[14] <luwProtectionSetMin[14])
				luwProtectionSet[14] = luwProtectionSetMax[14];
		}
	}
	
	
	else if(lubInterface == cParamSet)              //通用参数设置
	{
		sWriteScreenBuf(0,0,20,"Parameter Set       ");
		if(++lubBlinkCnt >= 20)
		{
			lubBlinkCnt = 0;
			if(PRJ_NUMBER == SUNNYBEE)
			{
				if(lubIconSelected <= 2)
				{
					sWriteScreenBuf(1,0,20,(INT8U *)MF_Param_Set[lubIconSelected ]);
					sWriteScreenBuf(2,0,20,(INT8U *)MF_Param_Set[(1 + lubIconSelected) ]);
					sWriteScreenBuf(3,0,20,(INT8U *)MF_Param_Set[(2 + lubIconSelected) ]);
				}
				if(lubIconSelected == 3)
				{
					sWriteScreenBuf(1,0,20,(INT8U *)MF_Param_Set[lubIconSelected ]);
					sWriteScreenBuf(2,0,20,(INT8U *)MF_Param_Set[(1 + lubIconSelected) ]);
					sWriteScreenBuf(3,0,20,(INT8U *)MF_Param_Set[0]);
				}
				if(lubIconSelected == 4)
				{
					sWriteScreenBuf(1,0,20,(INT8U *)MF_Param_Set[lubIconSelected ]);
					sWriteScreenBuf(2,0,20,(INT8U *)MF_Param_Set[0]);
					sWriteScreenBuf(3,0,20,(INT8U *)MF_Param_Set[1]);
				}
			}
			else if(PRJ_NUMBER == HORNET)
			{
				if(lubIconSelected <= 1)
				{
					sWriteScreenBuf(1,0,20,(INT8U *)HF_Param_Set[lubIconSelected ]);
					sWriteScreenBuf(2,0,20,(INT8U *)HF_Param_Set[(1 + lubIconSelected) ]);
					sWriteScreenBuf(3,0,20,(INT8U *)HF_Param_Set[(2 + lubIconSelected) ]);
				}
				if(lubIconSelected == 2)
				{
					sWriteScreenBuf(1,0,20,(INT8U *)HF_Param_Set[lubIconSelected ]);
					sWriteScreenBuf(2,0,20,(INT8U *)HF_Param_Set[(1 + lubIconSelected) ]);
					sWriteScreenBuf(3,0,20,(INT8U *)HF_Param_Set[0]);
				}
				if(lubIconSelected == 3)
				{
					sWriteScreenBuf(1,0,20,(INT8U *)HF_Param_Set[lubIconSelected ]);
					sWriteScreenBuf(2,0,20,(INT8U *)HF_Param_Set[0]);
					sWriteScreenBuf(3,0,20,(INT8U *)HF_Param_Set[1]);
				}
			}
			else
			{
				sWriteScreenBuf(1,0,20,"                    ");
				sWriteScreenBuf(2,0,20,"                    ");
				sWriteScreenBuf(3,0,20,"                    ");
			}
		}
		else if(lubBlinkCnt >= 10)
		{
			sWriteScreenBuf(1,0,20,"                    ");
		}
			if(PRJ_NUMBER == SUNNYBEE)
			{
				switch(lubIconSelected)
				{
					case 0:
					{	
						sTransferData(0,luwParamSet[0],&lubString[16],4);
						sWriteScreenBuf(1,16,20,&lubString[16]);
						switch(luwParamSet[1])
						{
							case 0x55:
								sWriteScreenBuf(2,10,20,"       OFF");
							break;
							case 0xA1:
								sWriteScreenBuf(2,10,20,"    PF SET");
							break;
							case 0xA2:
								sWriteScreenBuf(2,10,20," Cosphi(P)");
							break;
							case 0xA3:
								sWriteScreenBuf(2,10,20,"        Qp");
							break;
							case 0xA4:
								sWriteScreenBuf(2,10,20,"        Qu");
							break;
							default:
								sWriteScreenBuf(2,10,20,"   Default");
							break;							
						}
						if((luwParamSet[2]&0x8000) == 0)
						{
							sTransferData(0,luwParamSet[2],&lubString[16],4);
							sWriteScreenBuf(3,16,20,&lubString[16]);
						}
						else
						{
							sWriteScreenBuf(3,15,16,"-");
							luwParamSetTemp = 65536-luwParamSet[2];
							sTransferData(0,luwParamSetTemp,&lubString[16],4);
							sWriteScreenBuf(3,16,20,&lubString[16]);
						}		
					}
					break;
					case 1:
					{	
						switch(luwParamSet[1])
						{
							case 0x55:
								sWriteScreenBuf(1,10,20,"       OFF");
							break;
							case 0xA1:
								sWriteScreenBuf(1,10,20,"    PF SET");
							break;
							case 0xA2:
								sWriteScreenBuf(1,10,20," Cosphi(P)");
							break;
							case 0xA3:
								sWriteScreenBuf(1,10,20,"        Qp");
							break;
							case 0xA4:
								sWriteScreenBuf(1,10,20,"        Qu");
							break;	
							default:
								sWriteScreenBuf(1,10,20,"   Default");
							break;
						}
						if((luwParamSet[2]&0x8000) == 0)
						{
							sTransferData(0,luwParamSet[2],&lubString[16],4);
							sWriteScreenBuf(2,16,20,&lubString[16]);
						}
						else
						{
							sWriteScreenBuf(2,15,16,"-");
							luwParamSetTemp = 65536-luwParamSet[2];
							sTransferData(0,luwParamSetTemp,&lubString[16],4);
							sWriteScreenBuf(2,16,20,&lubString[16]);
						}		
						if((luwParamSet[3]&0x8000) == 0)
						{
							sTransferData(0,luwParamSet[3],&lubString[16],4);
							sWriteScreenBuf(3,16,20,&lubString[16]);
						}
						else
						{
							sWriteScreenBuf(3,15,16,"-");
							luwParamSetTemp = 65536-luwParamSet[3];
							sTransferData(0,luwParamSetTemp,&lubString[16],4);
							sWriteScreenBuf(3,16,20,&lubString[16]);
						}		
					}
					break;
					case 2:
					{	
						if((luwParamSet[2]&0x8000) == 0)
						{
							sTransferData(0,luwParamSet[2],&lubString[16],4);
							sWriteScreenBuf(1,16,20,&lubString[16]);
						}
						else
						{
							sWriteScreenBuf(1,15,16,"-");
							luwParamSetTemp = 65536-luwParamSet[2];
							sTransferData(0,luwParamSetTemp,&lubString[16],4);
							sWriteScreenBuf(1,16,20,&lubString[16]);
						}		
						if((luwParamSet[3]&0x8000) == 0)
						{
							sTransferData(0,luwParamSet[3],&lubString[16],4);
							sWriteScreenBuf(2,16,20,&lubString[16]);
						}
						else
						{
							sWriteScreenBuf(2,15,16,"-");
							luwParamSetTemp = 65536-luwParamSet[3];
							sTransferData(0,luwParamSetTemp,&lubString[16],4);
							sWriteScreenBuf(2,16,20,&lubString[16]);
						}		
						switch(luwParamSet[4])
						{
							case 0x55:
								sWriteScreenBuf(3,10,20,"   Level 2");
							break;
							case 0xAA:
								sWriteScreenBuf(3,10,20,"   Level 1");
							break;
							default:
								sWriteScreenBuf(3,10,20,"   Default");
							break;
						}
					}
					break;
					case 3:
					{	
						if((luwParamSet[3]&0x8000) == 0)
						{
							sTransferData(0,luwParamSet[3],&lubString[16],4);
							sWriteScreenBuf(1,16,20,&lubString[16]);
						}
						else
						{
							sWriteScreenBuf(1,15,16,"-");
							luwParamSetTemp = 65536-luwParamSet[3];
							sTransferData(0,luwParamSetTemp,&lubString[16],4);
							sWriteScreenBuf(1,16,20,&lubString[16]);
						}		
						switch(luwParamSet[4])
						{
							case 0x55:
								sWriteScreenBuf(2,10,20,"   Level 2");
							break;
							case 0xAA:
								sWriteScreenBuf(2,10,20,"   Level 1");
							break;
							default:
								sWriteScreenBuf(2,10,20,"   Default");
							break;
						}
						sTransferData(0,luwParamSet[0],&lubString[16],4);
						sWriteScreenBuf(3,16,20,&lubString[16]);
					}
					break;
					case 4:
					{	
						switch(luwParamSet[4])
						{
							case 0x55:
								sWriteScreenBuf(1,10,20,"   Level 2");
							break;
							case 0xAA:
								sWriteScreenBuf(1,10,20,"   Level 1");
							break;
							default:
								sWriteScreenBuf(1,10,20,"   Default");
							break;
						}
						sTransferData(0,luwParamSet[0],&lubString[16],4);
						sWriteScreenBuf(2,16,20,&lubString[16]);
						switch(luwParamSet[1])
						{
							case 0x55:
								sWriteScreenBuf(3,10,20,"       OFF");
							break;
							case 0xA1:
								sWriteScreenBuf(3,10,20,"    PF SET");
							break;
							case 0xA2:
								sWriteScreenBuf(3,10,20," Cosphi(P)");
							break;
							case 0xA3:
								sWriteScreenBuf(3,10,20,"        Qp");
							break;
							case 0xA4:
								sWriteScreenBuf(3,10,20,"        Qu");
							break;	
							default:
								sWriteScreenBuf(3,10,20,"   Default");
							break;
						}
					}
					break;
				}
			}
			else if(PRJ_NUMBER == HORNET)
			{
				switch(lubIconSelected)
				{
				case 0:
				{	
					sTransferData(0,luwParamSet[0],&lubString[16],4);
					sWriteScreenBuf(1,16,20,&lubString[16]);
					switch(luwParamSet[1])
					{
						case 0:
							sWriteScreenBuf(2,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(2,15,20,"<DEF>");
						break;						
					}
					if((luwParamSet[2]&0x8000) == 0)
					{
						sTransferData(0,luwParamSet[2],&lubString[16],4);
						sWriteScreenBuf(3,16,20,&lubString[16]);
					}
					else
					{
						sWriteScreenBuf(3,15,16,"-");
						luwParamSetTemp = 65536-luwParamSet[2];
						sTransferData(0,luwParamSetTemp,&lubString[16],4);
						sWriteScreenBuf(3,16,20,&lubString[16]);
					}		
				}
				break;
				case 1:
				{	
					switch(luwParamSet[1])
					{
						case 0:
							sWriteScreenBuf(1,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(1,15,20,"<DEF>");
						break;						
					}
					if((luwParamSet[2]&0x8000) == 0)
					{
						sTransferData(0,luwParamSet[2],&lubString[16],4);
						sWriteScreenBuf(2,16,20,&lubString[16]);
					}
					else
					{
						sWriteScreenBuf(2,15,16,"-");
						luwParamSetTemp = 65536-luwParamSet[2];
						sTransferData(0,luwParamSetTemp,&lubString[16],4);
						sWriteScreenBuf(2,16,20,&lubString[16]);
					}		
					if((luwParamSet[3]&0x8000) == 0)
					{
						sTransferData(0,luwParamSet[3],&lubString[16],4);
						sWriteScreenBuf(3,16,20,&lubString[16]);
					}
					else
					{
						sWriteScreenBuf(3,15,16,"-");
						luwParamSetTemp = 65536-luwParamSet[3];
						sTransferData(0,luwParamSetTemp,&lubString[16],4);
						sWriteScreenBuf(3,16,20,&lubString[16]);
					}		
				}
				break;
				case 2:
				{
					if((luwParamSet[2]&0x8000) == 0)
					{
						sTransferData(0,luwParamSet[2],&lubString[16],4);
						sWriteScreenBuf(1,16,20,&lubString[16]);
					}
					else
					{
						sWriteScreenBuf(1,15,16,"-");
						luwParamSetTemp = 65536-luwParamSet[2];
						sTransferData(0,luwParamSetTemp,&lubString[16],4);
						sWriteScreenBuf(1,16,20,&lubString[16]);
					}		
					if((luwParamSet[3]&0x8000) == 0)
					{
						sTransferData(0,luwParamSet[3],&lubString[16],4);
						sWriteScreenBuf(2,16,20,&lubString[16]);
					}
					else
					{
						sWriteScreenBuf(2,15,16,"-");
						luwParamSetTemp = 65536-luwParamSet[3];
						sTransferData(0,luwParamSetTemp,&lubString[16],4);
						sWriteScreenBuf(2,16,20,&lubString[16]);
					}		
					sTransferData(0,luwParamSet[0],&lubString[16],4);
					sWriteScreenBuf(3,16,20,&lubString[16]);
				}
				break;
				case 3:
				{
					if((luwParamSet[3]&0x8000) == 0)
					{
						sTransferData(0,luwParamSet[3],&lubString[16],4);
						sWriteScreenBuf(1,16,20,&lubString[16]);
					}
					else
					{
						sWriteScreenBuf(1,15,16,"-");
						luwParamSetTemp = 65536-luwParamSet[3];
						sTransferData(0,luwParamSetTemp,&lubString[16],4);
						sWriteScreenBuf(1,16,20,&lubString[16]);
					}		
					sTransferData(0,luwParamSet[0],&lubString[16],4);
					sWriteScreenBuf(2,16,20,&lubString[16]);
					switch(luwParamSet[1])
					{
						case 0:
							sWriteScreenBuf(3,15,20,"< N0>");
						break;
						case 1:
							sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
							sWriteScreenBuf(3,15,20,"<DEF>");
						break;						
					}
				}
				break;
			}			
		}
			
		if(Button.bit.ubEnterPress)													//单击Enter键
		{     
			lubBlinkCnt = 10;
			if(PRJ_NUMBER == SUNNYBEE)
			{
				if(++lubIconSelected > 4)
				{
					lubIconSelected = 0;
				}
			}
			if(PRJ_NUMBER == HORNET)
			{
				if(++lubIconSelected > 3)
				{
					lubIconSelected = 0;
				}
			}
		}
		else if(Button.bit.ubEscPress)												//单击Esc键
		{        		
				lubInterface = cSetInterface;
				lubIconSelected = 0;
		}
		else if(Button.bit.ubUpPress || Button.bit.ubUpLongPress)												//单击Up键
		{       		
			if(++luwParamSet[lubIconSelected] > luwParamSetMax[lubIconSelected])
			{
				luwParamSet[lubIconSelected] = luwParamSetMin[lubIconSelected];
			}			
		}
		else if(Button.bit.ubDownPress || Button.bit.ubDownLongPress)												//单击Down键
		{        
			if(--luwParamSet[lubIconSelected] < luwParamSetMin[lubIconSelected])
			{
				luwParamSet[lubIconSelected] = luwParamSetMax[lubIconSelected];
			}
		}
		else if(Button.bit.ubEnterLongPress)									//长按Enter键
		{
			if(PRJ_NUMBER == SUNNYBEE)
			{
				*Param_Set_Info.MActPwrSetValue = __REV16(luwParamSet[0]);     //通用设置
				*Param_Set_Info.MReactPwrModeEn = __REV16(luwParamSet[1]);
				*Param_Set_Info.MPowerFactorSetVal = __REV16(luwParamSet[2]);
				*Param_Set_Info.MReactPwrSetValue = __REV16(luwParamSet[3]);
				*Param_Set_Info.MItalyOutsideCommFlag = __REV16(luwParamSet[4]);
				write_10_date.usStartAddr = 0x211C;
				write_10_date.usLen = 9;
				FLAG_10_UPDATE = 1;
			}
			else if(PRJ_NUMBER == HORNET)
			{
				*Param_Set_Info.HMaxOutputPwrPercent = __REV16(luwParamSet[0]);     //通用设置
				*Param_Set_Info.HReactPwrModeEn = __REV16(luwParamSet[1]);
				*Param_Set_Info.HPowerFactorSetVal = __REV16(luwParamSet[2]);
				*Param_Set_Info.HReactPwrQPercent = __REV16(luwParamSet[3]);	
				write_10_date.usStartAddr = 0x2303;
				write_10_date.usLen = 7;
				FLAG_10_UPDATE = 1;
			}
			while(cGetButtonEnter());
			Button.all &= 0x0000;
			lubIconSelected = 0;
			lubInterface = cSetInterface;
		}
		if(PRJ_NUMBER == SUNNYBEE)
		{
			if(luwParamSet[1] < 0xA1)
				luwParamSet[1] = luwParamSetMin[1];
			if(luwParamSet[1] == 0x56)
				luwParamSet[1] = 0xA1;
		}
		else if(PRJ_NUMBER == HORNET)
		{
			if(luwParamSet[1] > 1)
				luwParamSet[1] = luwParamSetMax[1];
		}
		
	}
	else if(lubInterface == cCurveSet)                    //曲线设置
	{
		sWriteScreenBuf(0,0,20,"Curve Set           ");
		if(++lubBlinkCnt >= 20)
		{
			lubBlinkCnt = 0;
			if(PRJ_NUMBER == SUNNYBEE)
			{
				sWriteScreenBuf(1,0,20,(INT8U *)Curve_Set[0]);
				sWriteScreenBuf(2,0,20,"                    ");
				sWriteScreenBuf(3,0,20,"                    ");
			}
			if(PRJ_NUMBER == HORNET)
			{
				if(lubIconSelected <= 7)
				{
					sWriteScreenBuf(1,0,20,(INT8U *)Curve_Set[lubIconSelected ]);
					sWriteScreenBuf(2,0,20,(INT8U *)Curve_Set[(1 + lubIconSelected) ]);
					sWriteScreenBuf(3,0,20,(INT8U *)Curve_Set[(2 + lubIconSelected) ]);
				}
				if(lubIconSelected == 8)
				{
					sWriteScreenBuf(1,0,20,(INT8U *)Curve_Set[lubIconSelected ]);
					sWriteScreenBuf(2,0,20,(INT8U *)Curve_Set[(1 + lubIconSelected) ]);
					sWriteScreenBuf(3,0,20,(INT8U *)Curve_Set[0]);
				}
				if(lubIconSelected == 9)
				{
					sWriteScreenBuf(1,0,20,(INT8U *)Curve_Set[lubIconSelected ]);
					sWriteScreenBuf(2,0,20,(INT8U *)Curve_Set[0]);
					sWriteScreenBuf(3,0,20,(INT8U *)Curve_Set[1]);
				}
			}	
			else
			{
				sWriteScreenBuf(1,0,20,"                    ");
				sWriteScreenBuf(2,0,20,"                    ");
				sWriteScreenBuf(3,0,20,"                    ");
			}			
		}
		else if(lubBlinkCnt >= 10)
		{
			sWriteScreenBuf(1,0,20,"                    ");
		}
		if(PRJ_NUMBER == SUNNYBEE)
		{
			switch(luwCurveSet[0])
			{
				case 0xAA:
						sWriteScreenBuf(1,15,20,"<YES>");
				break;
				case 0x55:
						sWriteScreenBuf(1,15,20,"< NO>");
				break;
				default:
						sWriteScreenBuf(1,15,20,"<DEF>");
				break;	
			}
		}
		else if(PRJ_NUMBER == HORNET)
		{
			switch(lubIconSelected)
			{
				case 0:
				{
					switch(luwCurveSet[0])
					{
						case 0:
						    sWriteScreenBuf(1,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(1,15,20,"<DEF>");
						break;	
					}
					switch(luwCurveSet[1])
					{
						case 0:
						    sWriteScreenBuf(2,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(2,15,20,"<DEF>");
						break;	
					}
					switch(luwCurveSet[2])
					{
						case 0:
						    sWriteScreenBuf(3,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(3,15,20,"<DEF>");
						break;	
					}
				}
				break;
				case 1:
				{
					switch(luwCurveSet[1])
					{
						case 0:
						    sWriteScreenBuf(1,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(1,15,20,"<DEF>");
						break;	
					}
					switch(luwCurveSet[2])
					{
						case 0:
						    sWriteScreenBuf(2,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(2,15,20,"<DEF>");
						break;	
					}
					switch(luwCurveSet[3])
					{
						case 0:
						    sWriteScreenBuf(3,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(3,15,20,"<DEF>");
						break;	
					}
				}
				break;
				case 2:
				{
					switch(luwCurveSet[2])
					{
						case 0:
						    sWriteScreenBuf(1,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(1,15,20,"<DEF>");
						break;	
					}
					switch(luwCurveSet[3])
					{
						case 0:
						    sWriteScreenBuf(2,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(2,15,20,"<DEF>");
						break;	
					}
					switch(luwCurveSet[4])
					{
						case 0:
						    sWriteScreenBuf(3,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(3,15,20,"<DEF>");
						break;	
					}
				}
				break;
				case 3:
				{
					switch(luwCurveSet[3])
					{
						case 0:
						    sWriteScreenBuf(1,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(1,15,20,"<DEF>");
						break;	
					}
					switch(luwCurveSet[4])
					{
						case 0:
						    sWriteScreenBuf(2,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(2,15,20,"<DEF>");
						break;	
					}
					switch(luwCurveSet[5])
					{
						case 0:
						    sWriteScreenBuf(3,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(3,15,20,"<DEF>");
						break;	
					}
				}
				break;
				case 4:
				{
					switch(luwCurveSet[4])
					{
						case 0:
						    sWriteScreenBuf(1,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(1,15,20,"<DEF>");
						break;	
					}
					switch(luwCurveSet[5])
					{
						case 0:
						    sWriteScreenBuf(2,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(2,15,20,"<DEF>");
						break;	
					}
					switch(luwCurveSet[6])
					{
						case 0:
						    sWriteScreenBuf(3,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(3,15,20,"<DEF>");
						break;	
					}
				}
				break;
				case 5:
				{
					switch(luwCurveSet[5])
					{
						case 0:
						    sWriteScreenBuf(1,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(1,15,20,"<DEF>");
						break;	
					}
					switch(luwCurveSet[6])
					{
						case 0:
						    sWriteScreenBuf(2,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(2,15,20,"<DEF>");
						break;	
					}
					switch(luwCurveSet[7])
					{
						case 0:
						    sWriteScreenBuf(3,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(3,15,20,"<DEF>");
						break;	
					}
				}
				break;
				case 6:
				{
					switch(luwCurveSet[6])
					{
						case 0:
						    sWriteScreenBuf(1,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(1,15,20,"<DEF>");
						break;	
					}
					switch(luwCurveSet[7])
					{
						case 0:
						    sWriteScreenBuf(2,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(2,15,20,"<DEF>");
						break;	
					}
					switch(luwCurveSet[8])
					{
						case 0:
						    sWriteScreenBuf(3,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(3,15,20,"<DEF>");
						break;	
					}
				}
				break;
				case 7:
				{
					switch(luwCurveSet[7])
					{
						case 0:
						    sWriteScreenBuf(1,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(1,15,20,"<DEF>");
						break;	
					}
					switch(luwCurveSet[8])
					{
						case 0:
						    sWriteScreenBuf(2,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(2,15,20,"<DEF>");
						break;	
					}
					switch(luwCurveSet[9])
					{
						case 0:
						    sWriteScreenBuf(3,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(3,15,20,"<DEF>");
						break;	
					}
				}
				break;
				case 8:
				{
					switch(luwCurveSet[8])
					{
						case 0:
						    sWriteScreenBuf(1,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(1,15,20,"<DEF>");
						break;	
					}
					switch(luwCurveSet[9])
					{
						case 0:
						    sWriteScreenBuf(2,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(2,15,20,"<DEF>");
						break;	
					}
					switch(luwCurveSet[0])
					{
						case 0:
						    sWriteScreenBuf(3,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(3,15,20,"<DEF>");
						break;	
					}
				}
				break;
				case 9:
				{
					switch(luwCurveSet[9])
					{
						case 0:
						    sWriteScreenBuf(1,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(1,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(1,15,20,"<DEF>");
						break;	
					}
					switch(luwCurveSet[0])
					{
						case 0:
						    sWriteScreenBuf(2,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(2,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(2,15,20,"<DEF>");
						break;	
					}
					switch(luwCurveSet[1])
					{
						case 0:
						    sWriteScreenBuf(3,15,20,"< NO>");
						break;
						case 1:
								sWriteScreenBuf(3,15,20,"<YES>");
						break;
						default:
								sWriteScreenBuf(3,15,20,"<DEF>");
						break;	
					}
				}
				break;
			}		
		}
		if(Button.bit.ubEnterPress)													//单击Enter键
		{     
			lubBlinkCnt = 10;
			if(PRJ_NUMBER == SUNNYBEE)
			{
				lubIconSelected = 0;
			}
			else if(PRJ_NUMBER == HORNET)
			{
				if(++lubIconSelected > 9)
				{
					lubIconSelected = 0;
				}
			}
		}
		else if(Button.bit.ubEscPress)												//单击Esc键
		{        		
				lubInterface = cSetInterface;
				lubIconSelected = 0;
		}
		else if(Button.bit.ubUpPress || Button.bit.ubUpLongPress)												//单击Up键
		{    
			if(PRJ_NUMBER == SUNNYBEE)
			{
				if(++luwCurveSet[lubIconSelected] > luwCurveSetMax[lubIconSelected])
				{
					luwCurveSet[lubIconSelected] = luwCurveSetMin[lubIconSelected];
				}		
			}			
			else if(PRJ_NUMBER == HORNET)
			{
				if(++luwCurveSet[lubIconSelected] > luwCurveSetMax[lubIconSelected])
				{
					luwCurveSet[lubIconSelected] = luwCurveSetMin[lubIconSelected];
				}	
			}
		}
		else if(Button.bit.ubDownPress || Button.bit.ubDownLongPress)												//单击Down键
		{    
			if(PRJ_NUMBER == SUNNYBEE)
			{
				if(--luwCurveSet[lubIconSelected] < luwCurveSetMin[lubIconSelected])
				{
					luwCurveSet[lubIconSelected] = luwCurveSetMax[lubIconSelected];
				}
			}
			else if(PRJ_NUMBER == HORNET)
			{
				if(--luwCurveSet[lubIconSelected] > luwCurveSetMax[lubIconSelected])
				{
					luwCurveSet[lubIconSelected] = luwCurveSetMax[lubIconSelected];
				}
			}
		}
		else if(Button.bit.ubEnterLongPress)									//长按Enter键
		{
			*Curve_Enable_Info.WattFreqModeEn = __REV16(luwCurveSet[0]); //曲线使能
			*Curve_Enable_Info.WattVoltModeEn = __REV16(luwCurveSet[1]);
			*Curve_Enable_Info.PfWattModeEn = __REV16(luwCurveSet[2]);
			*Curve_Enable_Info.VarVoltModeEn = __REV16(luwCurveSet[3]);
			*Curve_Enable_Info.VarWattModeEn = __REV16(luwCurveSet[4]);
			*Curve_Enable_Info.ResvModeEn = __REV16(luwCurveSet[5]);
			*Curve_Enable_Info.LVRTCurveEn = __REV16(luwCurveSet[6]);
			*Curve_Enable_Info.HVRTCurveEn = __REV16(luwCurveSet[7]);
			*Curve_Enable_Info.LFRTCurveEn = __REV16(luwCurveSet[8]);
			*Curve_Enable_Info.HFRTCurveEn = __REV16(luwCurveSet[9]);
			if(PRJ_NUMBER == SUNNYBEE)
			{
				write_10_date.usStartAddr = 0x213f;
				write_10_date.usLen = 10;
				FLAG_10_UPDATE = 1;
			}
			else if(PRJ_NUMBER == HORNET)
			{
				switch(lubIconSelected)
				{
					case 0:
					{
						write_10_date.usStartAddr = 0x2311;
						write_10_date.usLen = 1;
						FLAG_10_UPDATE = 1;
					}
					break;
					case 1:
					{
						write_10_date.usStartAddr = 0x2326;
						write_10_date.usLen = 1;
						FLAG_10_UPDATE = 1;
					}
					break;
					case 2:
					{
						write_10_date.usStartAddr = 0x233B;
						write_10_date.usLen = 1;
						FLAG_10_UPDATE = 1;
					}
					break;
					case 3:
					{
						write_10_date.usStartAddr = 0x2350;
						write_10_date.usLen = 1;
						FLAG_10_UPDATE = 1;
					}
					break;
					case 4:
					{
						write_10_date.usStartAddr = 0x2365;
						write_10_date.usLen = 1;
						FLAG_10_UPDATE = 1;
					}
					break;
					case 5:
					{
						write_10_date.usStartAddr = 0x237A;
						write_10_date.usLen = 1;
						FLAG_10_UPDATE = 1;
					}
					break;
					case 6:
					{
						write_10_date.usStartAddr = 0x238F;
						write_10_date.usLen = 1;
						FLAG_10_UPDATE = 1;
					}
					break;
					case 7:
					{
						write_10_date.usStartAddr = 0x23A4;
						write_10_date.usLen = 1;
						FLAG_10_UPDATE = 1;
					}
					break;
					case 8:
					{
						write_10_date.usStartAddr = 0x23B9;
						write_10_date.usLen = 1;
						FLAG_10_UPDATE = 1;
					}
					break;
					case 9:
					{
						write_10_date.usStartAddr = 0x23CE;
						write_10_date.usLen = 1;
						FLAG_10_UPDATE = 1;
					}
					break;
				}				
			}
			while(cGetButtonEnter());
			Button.all &= 0x0000;
			lubIconSelected = 0;
			lubInterface = cSetInterface;
		}
		if(PRJ_NUMBER == SUNNYBEE)
		{
			if((luwCurveSet[0] == 0xA9)||(luwCurveSet[0] == 0xAB))
				luwCurveSet[0] = luwCurveSetMin[0];
			if((luwCurveSet[0] == 0x54)||(luwCurveSet[0] == 0x56))
				luwCurveSet[0] = luwCurveSetMax[0];
		}	
	}
	else if(lubInterface == cKeyDataSet)                   //关键数据设置
	{
		sWriteScreenBuf(0,0,20,"KeyData Set         ");
		if(++lubBlinkCnt >= 20)
		{
			lubBlinkCnt = 0;
			if(lubIconSelected <= 1)
			{
				sWriteScreenBuf(1,0,20,(INT8U *)KeyData_Set[lubIconSelected ]);
				sWriteScreenBuf(2,0,20,(INT8U *)KeyData_Set[(1 + lubIconSelected) ]);
				sWriteScreenBuf(3,0,20,(INT8U *)KeyData_Set[(2 + lubIconSelected) ]);
			}
			if(lubIconSelected == 2)
			{
				sWriteScreenBuf(1,0,20,(INT8U *)KeyData_Set[lubIconSelected ]);
				sWriteScreenBuf(2,0,20,(INT8U *)KeyData_Set[(1 + lubIconSelected) ]);
				sWriteScreenBuf(3,0,20,(INT8U *)KeyData_Set[0]);
			}
			if(lubIconSelected == 3)
			{
				sWriteScreenBuf(1,0,20,(INT8U *)KeyData_Set[lubIconSelected ]);
				sWriteScreenBuf(2,0,20,(INT8U *)KeyData_Set[0]);
				sWriteScreenBuf(3,0,20,(INT8U *)KeyData_Set[1]);
			}
		}
		else if(lubBlinkCnt >= 10)
		{
			sWriteScreenBuf(1,0,20,"                    ");
		}
		switch(lubIconSelected)
		{
			case 0:
			{
				switch(luwKeyDataSet[0])
				{
					case 0xAA:
						sWriteScreenBuf(1,15,20,"<YES>");
					break;
					case 0x55:
						sWriteScreenBuf(1,15,20,"< N0>");
					break;
					default:
						sWriteScreenBuf(1,15,20,"<DEF>");
					break;		
				}
				switch(luwKeyDataSet[1])
				{
					case 0xAA:
						sWriteScreenBuf(2,15,20,"<YES>");
					break;
					case 0x55:
						sWriteScreenBuf(2,15,20,"< N0>");
					break;
					default:
						sWriteScreenBuf(2,15,20,"<DEF>");
					break;		
				}
				switch(luwKeyDataSet[2])
				{
					case 0x01:
						sWriteScreenBuf(3,14,20,"Single");
					break;
					case 0x02:
						sWriteScreenBuf(3,14,20,"Double");
					break;
					default:
						sWriteScreenBuf(3,14,20,"<DEF> ");
					break;		
				}
			}
			break;
			case 1:
			{
				switch(luwKeyDataSet[1])
				{
					case 0xAA:
						sWriteScreenBuf(1,15,20,"<YES>");
					break;
					case 0x55:
						sWriteScreenBuf(1,15,20,"< N0>");
					break;
					default:
						sWriteScreenBuf(1,15,20,"<DEF>");
					break;		
				}
				switch(luwKeyDataSet[2])
				{
					case 0x01:
						sWriteScreenBuf(2,14,20,"Single");
					break;
					case 0x02:
						sWriteScreenBuf(2,14,20,"Double");
					break;
					default:
						sWriteScreenBuf(2,14,20,"<DEF> ");
					break;		
				}
				if((luwKeyDataSet[3]&0x8000) == 0)
				{
					sTransferData(0,luwKeyDataSet[3],&lubString[14],5);
					sWriteScreenBuf(3,15,20,&lubString[14]);
				}
				else
				{
					sWriteScreenBuf(3,14,15,"-");
					luwKeyDataSetTemp = 65536-luwKeyDataSet[3];
					sTransferData(0,luwKeyDataSetTemp,&lubString[14],5);
					sWriteScreenBuf(3,15,20,&lubString[14]);
				}
			}
			break;
			case 2:
			{
				switch(luwKeyDataSet[2])
				{
					case 0x01:
						sWriteScreenBuf(1,14,20,"Single");
					break;
					case 0x02:
						sWriteScreenBuf(1,14,20,"Double");
					break;
					default:
						sWriteScreenBuf(1,14,20,"<DEF> ");
					break;		
				}
				if((luwKeyDataSet[3]&0x8000) == 0)
				{
					sTransferData(0,luwKeyDataSet[3],&lubString[14],5);
					sWriteScreenBuf(2,15,20,&lubString[14]);
				}
				else 
				{
					sWriteScreenBuf(2,14,15,"-");
					luwKeyDataSetTemp = 65536-luwKeyDataSet[3];
					sTransferData(0,luwKeyDataSetTemp,&lubString[14],5);
					sWriteScreenBuf(2,15,20,&lubString[14]);
				}
				switch(luwKeyDataSet[0])
				{
					case 0xAA:
						sWriteScreenBuf(3,15,20,"<YES>");
					break;
					case 0x55:
						sWriteScreenBuf(3,15,20,"< N0>");
					break;
					default:
						sWriteScreenBuf(3,15,20,"<DEF>");
					break;		
				}
			}
			break;
			case 3:
			{
				if((luwKeyDataSet[3]&0x8000) == 0)
				{
					sTransferData(0,luwKeyDataSet[3],&lubString[14],5);
					sWriteScreenBuf(1,15,20,&lubString[14]);
				}
				else 
				{
					sWriteScreenBuf(1,14,15,"-");
					luwKeyDataSetTemp = 65536-luwKeyDataSet[3];
					sTransferData(0,luwKeyDataSetTemp,&lubString[14],5);
					sWriteScreenBuf(1,15,20,&lubString[14]);
				}
				switch(luwKeyDataSet[0])
				{
					case 0xAA:
						sWriteScreenBuf(2,15,20,"<YES>");
					break;
					case 0x55:
						sWriteScreenBuf(2,15,20,"< N0>");
					break;
					default:
						sWriteScreenBuf(2,15,20,"<DEF>");
					break;		
				}
				switch(luwKeyDataSet[1])
				{
					case 0xAA:
						sWriteScreenBuf(3,15,20,"<YES>");
					break;
					case 0x55:
						sWriteScreenBuf(3,15,20,"< N0>");
					break;
					default:
						sWriteScreenBuf(3,15,20,"<DEF>");
					break;		
				}
			}
			break;
		}
		if(Button.bit.ubEscPress)												//单击Esc键
		{        
			lubInterface = cSetInterface;
			lubIconSelected = 0;
		}
		else if(Button.bit.ubUpPress || Button.bit.ubUpLongPress)				//单击或长按Up键
		{	        
			if(++luwKeyDataSet[lubIconSelected] > luwKeyDataSetMax[lubIconSelected])
			{
				luwKeyDataSet[lubIconSelected] = luwKeyDataSetMin[lubIconSelected];
			}	
		}
		else if(Button.bit.ubDownPress || Button.bit.ubDownLongPress)			//单击或长按Down键
		{        
			if(--luwKeyDataSet[lubIconSelected] < luwKeyDataSetMin[lubIconSelected])
			{
				luwKeyDataSet[lubIconSelected] = luwKeyDataSetMax[lubIconSelected];
			}
		}
		else if(Button.bit.ubEnterPress)										//单击Enter键
		{        
			lubBlinkCnt = 10;
			if(++lubIconSelected > 3)
			{
				lubIconSelected = 0;
			}
		}
		else if(Button.bit.ubEnterLongPress)									//长按Enter键
		{
			*Key_Data_Info.FcParamRecovery = __REV16(luwKeyDataSet[0]);   //关键数据设置
			*Key_Data_Info.ExistingDataClear = __REV16(luwKeyDataSet[1]);
			*Key_Data_Info.PvInputConnectionMode = __REV16(luwKeyDataSet[2]);
			*Key_Data_Info.TotalElecGenCompVal = __REV16(luwKeyDataSet[3]);
			switch(lubIconSelected)
			{
				case 0:
				{
					if(PRJ_NUMBER == SUNNYBEE)					
						write_10_date.usStartAddr = 0x2108;
					if(PRJ_NUMBER == HORNET)
						write_10_date.usStartAddr = 0x2202;
					write_10_date.usLen = 1;
					FLAG_10_UPDATE = 1;
				}
				break;
				case 1:
				{
					if(PRJ_NUMBER == SUNNYBEE)					
						write_10_date.usStartAddr = 0x210B;
					if(PRJ_NUMBER == HORNET)
						write_10_date.usStartAddr = 0x2203;
					write_10_date.usLen = 1;
					FLAG_10_UPDATE = 1;
				}
				break;
				case 2:
				{
					if(PRJ_NUMBER == SUNNYBEE)					
						write_10_date.usStartAddr = 0x210E;
					if(PRJ_NUMBER == HORNET)
						write_10_date.usStartAddr = 0x2204;
					write_10_date.usLen = 1;
					FLAG_10_UPDATE = 1;
				}
				break;
				case 3:
				{
					if(PRJ_NUMBER == SUNNYBEE)					
						write_10_date.usStartAddr = 0x2111;
					if(PRJ_NUMBER == HORNET)
						write_10_date.usStartAddr = 0x2205;
					write_10_date.usLen = 1;
					FLAG_10_UPDATE = 1;
				}
				break;
			}
			while(cGetButtonEnter());
			Button.all &= 0x0000;
			lubInterface = cSetInterface;
			lubIconSelected = 0;
			
		}
		if((luwKeyDataSet[0] == 0xA9)||(luwKeyDataSet[0] == 0xAB))
			luwKeyDataSet[0] = luwKeyDataSetMin[0];
		if((luwKeyDataSet[0] == 0x54)||(luwKeyDataSet[0] == 0x56))
			luwKeyDataSet[0] = luwKeyDataSetMax[0];
	
		if((luwKeyDataSet[1] == 0xA9)||(luwKeyDataSet[1] == 0xAB))
			luwKeyDataSet[1] = luwKeyDataSetMin[1];
		if((luwKeyDataSet[1] == 0x54)||(luwKeyDataSet[1] == 0x56))
			luwKeyDataSet[1] = luwKeyDataSetMax[1];		
		
		if(luwKeyDataSet[2] >luwKeyDataSetMax[2])
			luwKeyDataSet[2] = luwKeyDataSetMin[2];
		if(luwKeyDataSet[2] <luwKeyDataSetMin[2])
			luwKeyDataSet[2] = luwKeyDataSetMax[2];
		
		if(luwKeyDataSet[3] >luwKeyDataSetMax[3])
			luwKeyDataSet[3] = luwKeyDataSetMin[3];
		if(luwKeyDataSet[3] <luwKeyDataSetMin[3])
			luwKeyDataSet[3] = luwKeyDataSetMax[3];
	}
	else if(lubInterface == cFreqLimitSet)             //频率上下限设置
	{
		if(++lubBlinkCnt >= 20)
		{
			lubBlinkCnt = 0;
		}
		strcpy(lubString,"FH:     Hz        Hz");
		if(lubIconSelected != 0 || lubBlinkCnt < 10)
		{
			sTransferData(2,luwFreqSet[0],&lubString[3],4);
		}
		if(lubIconSelected != 1 || lubBlinkCnt < 10)
		{
			sTransferData(2,luwFreqSet[1],&lubString[13],4);
		}
		sWriteScreenBuf(0,0,20,lubString);

			strcpy(lubString,"T:      ms        ms");
			if(lubIconSelected != 2 || lubBlinkCnt < 10)
			{
				sTransferData(2,luwFreqSet[2],&lubString[2],5);
			}
			if(lubIconSelected != 3 || lubBlinkCnt < 10)
			{
				sTransferData(2,luwFreqSet[3],&lubString[12],5);
			}
		
		sDisplayAsist(2,4,lubString);
		sDisplayAsist(12,14,lubString);
		sWriteScreenBuf(1,0,20,lubString);
		
		strcpy(lubString,"FL:     Hz        Hz");
		if(lubIconSelected != 4 || lubBlinkCnt < 10)
		{
			sTransferData(2,luwFreqSet[4],&lubString[3],4);
		}
		if(lubIconSelected != 5 || lubBlinkCnt < 10)
		{
			sTransferData(2,luwFreqSet[5],&lubString[13],4);
		}
		sWriteScreenBuf(2,0,20,lubString);
		
			strcpy(lubString,"T:      ms        ms");
			if(lubIconSelected != 6 || lubBlinkCnt < 10)
			{
				sTransferData(2,luwFreqSet[6],&lubString[2],5);
			}
			if(lubIconSelected != 7 || lubBlinkCnt < 10)
			{
				sTransferData(2,luwFreqSet[7],&lubString[12],5);
			}
		

		sDisplayAsist(2,4,lubString);
		sDisplayAsist(12,14,lubString);
		sWriteScreenBuf(3,0,20,lubString);
		
		if(Button.bit.ubEscPress)												//单击Esc键
		{        
			lubInterface = cSetInterface;
			lubIconSelected = 0;
		}
		else if(Button.bit.ubUpPress || Button.bit.ubUpLongPress)				//单击或长按Up键
		{        
			if(++luwFreqSet[lubIconSelected] > luwFreqSetMax[lubIconSelected])
			{
				luwFreqSet[lubIconSelected] = luwFreqSetMin[lubIconSelected];
			}
		}
		else if(Button.bit.ubDownPress || Button.bit.ubDownLongPress)			//单击或长按Down键
		{	        
			if(--luwFreqSet[lubIconSelected] > luwFreqSetMax[lubIconSelected])
			{
				luwFreqSet[lubIconSelected] = luwFreqSetMax[lubIconSelected];
			}
		}
		else if(Button.bit.ubEnterPress)										//单击Enter键
		{        
			if(++lubIconSelected >= 8)
			{
				lubIconSelected = 0;
			}
		}
		else if(Button.bit.ubEnterLongPress)									//长按Enter键
		{
				
				Safty.data.uwFreqHigh[0] = __REV16(luwFreqSet[0]);
				Safty.data.uwFreqHigh[1] = __REV16(luwFreqSet[1]);
				Safty.data.uwFreqHighCnt[0] = __REV16(luwFreqSet[2]);	
				Safty.data.uwFreqHighCnt[1] = __REV16(luwFreqSet[3]);
				Safty.data.uwFreqLow[0] = __REV16(luwFreqSet[4]);
				Safty.data.uwFreqLow[1] = __REV16(luwFreqSet[5]);
				Safty.data.uwFreqLowCnt[0] = __REV16(luwFreqSet[6]);
				Safty.data.uwFreqLowCnt[1] = __REV16(luwFreqSet[7]);				
				if(PRJ_NUMBER == SUNNYBEE)
				{
					memcpy(MF_afGridFreqProtThresHigh,Safty.data.uwFreqHigh,4);   //频率上下限
					memcpy(MF_ausGridFreqProtTimeHigh,Safty.data.uwFreqHighCnt,4);
					memcpy(MF_afGridFreqProtThresLow,Safty.data.uwFreqLow,4);
					memcpy(MF_ausGridFreqProtTimeLow,Safty.data.uwFreqLowCnt,4);
					write_10_date.usStartAddr = 0x2132;
					write_10_date.usLen = 12;
					FLAG_10_UPDATE = 1;
				}
				else if(PRJ_NUMBER == HORNET)
				{
					memcpy(HF_afGridFreqProtThresHigh,Safty.data.uwFreqHigh,4);   //频率上下限
					memcpy(HF_ausGridFreqProtTimeHigh,Safty.data.uwFreqHighCnt,4);
					memcpy(HF_afGridFreqProtThresLow,Safty.data.uwFreqLow,4);
					memcpy(HF_ausGridFreqProtTimeLow,Safty.data.uwFreqLowCnt,4);
					write_10_date.usStartAddr = 0x2113;
					write_10_date.usLen = 14;
					FLAG_10_UPDATE = 1;
				}
				while(cGetButtonEnter());
				Button.all &= 0x0000;
				lubIconSelected = 0;
				lubInterface = cSetInterface;
		}
		
	}
	else if(lubInterface == cVoltLimitSet)       //电压上下限设置
	{
		if(++lubBlinkCnt >= 20)
		{
			lubBlinkCnt = 0;
		}
		strcpy(lubString,"VH:     V         V ");
		if(lubIconSelected != 0 || lubBlinkCnt < 10)
		{
			sTransferData(1,luwVoltSet[0],&lubString[3],4);
		}
		if(lubIconSelected != 1 || lubBlinkCnt < 10)
		{
			sTransferData(1,luwVoltSet[1],&lubString[13],4);
		}
		sWriteScreenBuf(0,0,20,lubString);
		
		strcpy(lubString,"T:      ms        ms");
		if(lubIconSelected != 2 || lubBlinkCnt < 10)
		{
			sTransferData(2,luwVoltSet[2],&lubString[2],5);
		}
		if(lubIconSelected != 3 || lubBlinkCnt < 10)
		{
			sTransferData(2,luwVoltSet[3],&lubString[12],5);
		}
		sDisplayAsist(2,4,lubString);
		sDisplayAsist(12,14,lubString);
		sWriteScreenBuf(1,0,20,lubString);
		
		strcpy(lubString,"VL:     V         V ");
		if(lubIconSelected != 4 || lubBlinkCnt < 10)
		{
			sTransferData(1,luwVoltSet[4],&lubString[3],4);
		}
		if(lubIconSelected != 5 || lubBlinkCnt < 10)
		{
			sTransferData(1,luwVoltSet[5],&lubString[13],4);		
		}
		sDisplayAsist(13,17,lubString);
		sWriteScreenBuf(2,0,20,lubString);
		
		strcpy(lubString,"T:      ms        ms");
		if(lubIconSelected != 6 || lubBlinkCnt < 10)
		{
			sTransferData(2,luwVoltSet[6],&lubString[2],5);
		}
		if(lubIconSelected != 7 || lubBlinkCnt < 10)
		{
			sTransferData(2,luwVoltSet[7],&lubString[12],5);
		}
		sDisplayAsist(2,4,lubString);
		sDisplayAsist(12,14,lubString);
		sWriteScreenBuf(3,0,20,lubString);
		
		if(Button.bit.ubEscPress)												//单击Esc键
		{        
			lubInterface = cSetInterface;
			lubIconSelected = 0;
		}
		else if(Button.bit.ubUpPress || Button.bit.ubUpLongPress)				//单击或长按Up键
		{	        
			if(++luwVoltSet[lubIconSelected] > luwVoltSetMax[lubIconSelected])
			{
				luwVoltSet[lubIconSelected] = luwVoltSetMin[lubIconSelected];
			}
		}
		else if(Button.bit.ubDownPress || Button.bit.ubDownLongPress)			//单击或长按Down键
		{        
			if(--luwVoltSet[lubIconSelected] > luwVoltSetMax[lubIconSelected])
			{
				luwVoltSet[lubIconSelected] = luwVoltSetMax[lubIconSelected];
			}
		}
		else if(Button.bit.ubEnterPress)										//单击Enter键
		{        
			if(++lubIconSelected >= 8)
			{
				lubIconSelected = 0;
			}
		}
		else if(Button.bit.ubEnterLongPress)									//长按Enter键
		{
				
				Safty.data.uwGridVoltHigh[0] = __REV16(luwVoltSet[0]);
				Safty.data.uwGridVoltHigh[1] =  __REV16(luwVoltSet[1]);
				Safty.data.uwGridVoltHighCnt[0] =  __REV16(luwVoltSet[2]);
				Safty.data.uwGridVoltHighCnt[1] =  __REV16(luwVoltSet[3]);
				Safty.data.uwGridVoltLow[0] =  __REV16(luwVoltSet[4]);
				Safty.data.uwGridVoltLow[1] =  __REV16(luwVoltSet[5]);
				Safty.data.uwGridVoltLowCnt[0] =  __REV16(luwVoltSet[6]);
				Safty.data.uwGridVoltLowCnt[1] =  __REV16(luwVoltSet[7]);
				
				if(PRJ_NUMBER == SUNNYBEE)
				{
					memcpy(MF_afGridVoltProtThresHigh,Safty.data.uwGridVoltHigh,4);   //电压上下限
					memcpy(MF_ausGridVoltProtTimeHigh,Safty.data.uwGridVoltHighCnt,4);
					memcpy(MF_afGridVoltProtThresLow,Safty.data.uwGridVoltLow,4);
					memcpy(MF_ausGridVoltProtTimeLow,Safty.data.uwGridVoltLowCnt,4);
					write_10_date.usStartAddr = 0x2125;
					write_10_date.usLen = 12;
					FLAG_10_UPDATE = 1;
				}
				else if(PRJ_NUMBER == HORNET)
				{
					memcpy(HF_afGridVoltProtThresHigh,Safty.data.uwGridVoltHigh,4);   //电压上下限
					memcpy(HF_ausGridVoltProtTimeHigh,Safty.data.uwGridVoltHighCnt,4);
					memcpy(HF_afGridVoltProtThresLow,Safty.data.uwGridVoltLow,4);
					memcpy(HF_ausGridVoltProtTimeLow,Safty.data.uwGridVoltLowCnt,4);
					write_10_date.usStartAddr = 0x2104;
					write_10_date.usLen = 14;
					FLAG_10_UPDATE = 1;
				}
				while(cGetButtonEnter());
				Button.all &= 0x0000;
				lubIconSelected = 0;
				lubInterface = cSetInterface;
		}
		
	}
	else if(lubInterface == cPasswordIn)
	{
		sWriteScreenBuf(0,0,20,"                    ");
		sWriteScreenBuf(1,0,11,(INT8U *)lubSetListDisplay[Info.data.ublanguage][4] + 1);
		sWriteScreenBuf(1,11,9,"         ");
		strcpy(lubString,"                    ");
				
		if(++lubBlinkCnt >= 20)
		{
			lubBlinkCnt = 0;
		}	
		if(lubIconSelected != 0 || lubBlinkCnt < 10)
		{
			lubString[6] = lubPasswordIn[0];
		}
		if(lubIconSelected != 1 || lubBlinkCnt < 10)
		{
			lubString[7] = lubPasswordIn[1];
		}
		if(lubIconSelected != 2 || lubBlinkCnt < 10)
		{
			lubString[8] = lubPasswordIn[2];
		}
		if(lubIconSelected != 3 || lubBlinkCnt < 10)
		{
			lubString[9] = lubPasswordIn[3];
		}
		if(lubIconSelected != 4 || lubBlinkCnt < 10)
		{
			lubString[10] = lubPasswordIn[4];
		}
		if(lubIconSelected != 5 || lubBlinkCnt < 10)
		{
			lubString[11] = lubPasswordIn[5];
		}
		sWriteScreenBuf(2,0,20,lubString);		
		sWriteScreenBuf(3,0,20,"                    ");
		
		if(Button.bit.ubEscPress)												//单击Esc键
		{
			for(i = 0;i < 6;i++)
			{
				lubPasswordIn[i] = 0x30;
			}
			lubInterface = cSetInterface;
			lubIconSelected = 0;
		}
		else if(Button.bit.ubUpPress)											//单击Up键
		{       
			if(++lubPasswordIn[lubIconSelected] > 0x39)
			{
				lubPasswordIn[lubIconSelected] = 0x30;
			}
		}
		else if(Button.bit.ubDownPress)											//单击Down键
		{	        
			if(--lubPasswordIn[lubIconSelected] < 0x30)
			{
				lubPasswordIn[lubIconSelected] = 0x39;
			}
		}
		else if(Button.bit.ubEnterPress)										//单击Enter键
		{			    
			lubBlinkCnt = 10;					
			if(++lubIconSelected >= 6)
			{
				lubIconSelected = 0;
				if(Info.data.ubPasswordH == (lubPasswordIn[0] - 0x30) * 16 + lubPasswordIn[1] - 0x30
				&& Info.data.ubPasswordM == (lubPasswordIn[2] - 0x30) * 16 + lubPasswordIn[3] - 0x30
				&& Info.data.ubPasswordL == (lubPasswordIn[4] - 0x30) * 16 + lubPasswordIn[5] - 0x30)
				{
					lubInterface = lubInterfacePre;
				}
				else
				{
					lubInterface = cPasswordIn;
				}
				for(i = 0;i < 6;i++)
				{
					lubPasswordIn[i] = 0x30;
				}
			}
		}
	}
	else if(lubInterface == cPasswordSaved)
	{
		sWriteScreenBuf(0,0,20,"                    ");
		sWriteScreenBuf(1,0,20,"Password is saved!  ");
		strcpy(lubString,"                    ");
		lubString[6] = ((Info.data.ubPasswordH >> 4)& 0x0f) + 0x30;
		lubString[7] = (Info.data.ubPasswordH & 0x0f) + 0x30;
		lubString[8] = ((Info.data.ubPasswordM >> 4)& 0x0f) + 0x30;
		lubString[9] = (Info.data.ubPasswordM & 0x0f) + 0x30;
		lubString[10] = ((Info.data.ubPasswordL >> 4)& 0x0f) + 0x30;
		lubString[11] = (Info.data.ubPasswordL & 0x0f) + 0x30;
		sWriteScreenBuf(2,0,20,lubString);
		sWriteScreenBuf(3,0,20,"                    ");
		
		if(Button.all & 0xF0)
		{
			lubInterface = cSetInterface;
			lubIconSelected = 0;
		}
		
	}
	else if(lubInterface == cAutoTestPage)                //意大利自检
	{
		if((Italy_SelfTest_Flag >= 1)&&(Italy_SelfTest_Flag <= 8))
		{
			if(PRJ_NUMBER == SUNNYBEE)
			{
				AutoTest_Info.Trip_Value = __REV16(MF_SelfTest[Italy_SelfTest_Flag-1].Trip_Value);    //意大利自检
				AutoTest_Info.Trip_Time = __REV16(MF_SelfTest[Italy_SelfTest_Flag-1].Trip_Time);    //意大利自检
				AutoTest_Info.State = __REV16(MF_SelfTestState);    //意大利自检
			}
			if(PRJ_NUMBER == HORNET)
			{
				AutoTest_Info.Trip_Value = __REV16(HF_SelfTest[Italy_SelfTest_Flag-1].Trip_Value);    //意大利自检
				AutoTest_Info.Trip_Time = __REV16(HF_SelfTest[Italy_SelfTest_Flag-1].Trip_Time);    //意大利自检
				AutoTest_Info.State = __REV16(HF_SelfTestState);    //意大利自检
			}
		}

		luwNoOprateCnt = 0;
		sWriteScreenBuf(0,0,20,"Auto Test:          ");
		sWriteScreenBuf(1,0,20,lubStringTemp);	
		
		luwAutoTestTemp1 = AutoTest_Info.Trip_Value;
		luwAutoTestTemp2 = AutoTest_Info.Trip_Time;
		
		if((lubIconSelected <= 4) && (lubIconSelected > 0))
		{
			strcpy(lubString,"        V       ms  ");	
			if(luwAutoTestCnt < 400)
			{
				sTransferData(1,luwAutoTestTemp1,&lubString[3],4);
				sDisplayAsist(3,5,lubString);			
			}
			else
			{
				luwAutoTestTemp1 = 0;
				luwAutoTestTemp2 = 0;
				sTransferData(1,luwAutoTestTemp1,&lubString[5],2);					
			}
		}	
		else if((lubIconSelected >= 5) && (lubIconSelected <= 8))
		{
			strcpy(lubString,"        Hz      ms  ");	
			if(luwAutoTestCnt < 400)
			{
				sTransferData(2,(luwAutoTestTemp1/10),&lubString[3],4);
				sDisplayAsist(3,4,lubString);
			}
			else
			{
				luwAutoTestTemp1 = 0;
				luwAutoTestTemp2 = 0;
				sTransferData(2,(luwAutoTestTemp1/10),&lubString[4],3);
			}
		}		
	
		sTransferData(0,luwAutoTestTemp2,&lubString[12],4);
		sDisplayAsist(12,15,lubString);
		sWriteScreenBuf(2,0,20,lubString);	
		sWriteScreenBuf(3,0,20,"Status:             ");
		if(First_Enter_Flag == 1)
		{
			luwAutoTestTemp1_pre = luwAutoTestTemp1;
			luwAutoTestTemp2_pre = luwAutoTestTemp2;
			First_Enter_Flag = 0;
		}
		if((luwAutoTestTemp1 == luwAutoTestTemp1_pre)&&(luwAutoTestTemp2 == luwAutoTestTemp2_pre))  //若跳脱值无变化，则不更新state状态，维持testing显示
		{
			AutoTest_Info.State = 0xA2;
			if((lubIconSelected <= 4) && (lubIconSelected > 0))
			{
				sWriteScreenBuf(2,0,20,"      --V     --ms  ");
			}
			else if((lubIconSelected >= 5) && (lubIconSelected <= 8))
			{
				sWriteScreenBuf(2,0,20,"      --Hz    --ms  ");
			}
		}	
		if(AutoTest_Info.State == 0xAA)	
		{
			luwAutoTestCnt = 0;
			sWriteScreenBuf(3,0,20,"Status:SelfTest  OK ");
		}
		else
		{
			if(AutoTest_Info.State == 0x55)	
			{
				luwAutoTestCnt = 0;
				sWriteScreenBuf(3,0,20,"Status:SelfTest Fail");
			}
			else
			{
				if(AutoTest_Info.State == 0xA2)	
				{
					luwAutoTestCnt = 0;
					sWriteScreenBuf(3,0,20,"Status:   Testing   ");
				}
			}
		
		}	 
		if((++luwAutoTestCnt >= 400) && (INV_StateFlag == 4))
		{
			luwAutoTestCnt = 400;
			sWriteScreenBuf(3,0,20,"Status:  Time Out   ");
		}
		if(Button.bit.ubEscPress)	
		{
			First_Enter_Flag = 1;
			SlaveData.data.SlaveFlag.bit.ubSelfTestStep = 1;
			luwAutoTestCnt = 0;
			lubInterface = lubInterfacePre;
			lubIconSelected = 0;
			*Protection_Func_Enable_Info.ItalySelfTestEN = 0x00;
			if(PRJ_NUMBER == SUNNYBEE)
				write_10_date.usStartAddr = 0x214D;
			else if(PRJ_NUMBER == HORNET)
				write_10_date.usStartAddr = 0x248A;
			write_10_date.usLen = 1;
			FLAG_10_UPDATE = 1;
		}
	} 
	else if(lubInterface == cInvSet)                    //逆变器设置
	{
		sWriteScreenBuf(0,0,20,"Inv Set             ");
		if(++lubBlinkCnt >= 20)
		{
			lubBlinkCnt = 0;
			sWriteScreenBuf(1,0,20,(INT8U *)Inv_Set[0]);
			sWriteScreenBuf(2,0,20,(INT8U *)Inv_Set[1]);
			sWriteScreenBuf(3,0,20,"                    ");
		}
		else if(lubBlinkCnt >= 10)
		{
			switch(lubIconSelected)
			{
				case 0:
					sWriteScreenBuf(1,0,20,"                    ");
				break;
				case 1:
					sWriteScreenBuf(2,0,20,"                    ");
				break;
			}
		}
		switch(luwInvSet[0])
		{
			case 0xAF:
				sWriteScreenBuf(1,15,20,"<YES>");
			break;
			case 0xAE:
				sWriteScreenBuf(1,15,20,"< N0>");
			break;
			default:
				sWriteScreenBuf(1,15,20,"<DEF>");
			break;		
		}
		sTransferData(0,luwInvSet[1],&lubString[16],4);
		sWriteScreenBuf(2,16,20,&lubString[16]);
		if(Button.bit.ubEscPress)												//单击Esc键
		{        
			lubInterface = cSetInterface;
			lubIconSelected = 0;
		}
		else if(Button.bit.ubUpPress || Button.bit.ubUpLongPress)				//单击或长按Up键
		{	        
			if(++luwInvSet[lubIconSelected] > luwInvSetMax[lubIconSelected])
			{
				luwInvSet[lubIconSelected] = luwInvSetMin[lubIconSelected];
			}	
		}
		else if(Button.bit.ubDownPress || Button.bit.ubDownLongPress)			//单击或长按Down键
		{        
			if(--luwInvSet[lubIconSelected] < luwInvSetMin[lubIconSelected])
			{
				luwInvSet[lubIconSelected] = luwInvSetMax[lubIconSelected];
			}
		}
		else if(Button.bit.ubEnterPress)										//单击Enter键
		{        
			lubBlinkCnt = 10;
			if(++lubIconSelected > 1)
			{
				lubIconSelected = 0;
			}
		}
		else if(Button.bit.ubEnterLongPress)									//长按Enter键
		{
			*INV_Set_Info.InvOnOffCmd = __REV16(luwInvSet[0]);  //逆变器设置
			*INV_Set_Info.WaitStandupTime = __REV16(luwInvSet[1]);
			switch(lubIconSelected)
			{
				case 0:
				{
					if(PRJ_NUMBER == SUNNYBEE)
					{
						write_10_date.usStartAddr = 0x2119;
						write_10_date.usLen = 1;
						FLAG_10_UPDATE = 1;
					}
					else if(PRJ_NUMBER == HORNET)
					{
						write_10_date.usStartAddr = 0x2287;
						write_10_date.usLen = 1;
						FLAG_10_UPDATE = 1;
					}
				}
				break;
				case 1:
				{
					if(PRJ_NUMBER == SUNNYBEE)
					{
						write_10_date.usStartAddr = 0x211A;
						write_10_date.usLen = 1;
						FLAG_10_UPDATE = 1;
					}
					else if(PRJ_NUMBER == HORNET)
					{
						write_10_date.usStartAddr = 0x2127;
						write_10_date.usLen = 1;
						FLAG_10_UPDATE = 1;
					}
				}
				break;
			}
			while(cGetButtonEnter());
			Button.all &= 0x0000;
			lubInterface = cSetInterface;
			lubIconSelected = 0;			
		}
		if(luwInvSet[1] > luwInvSetMax[1])
			luwInvSet[1] = luwInvSetMax[1];
	}
	else
	{
		lubInterface = cDefaultInterface;
		lubIconSelected = 0;
	}
}

/*****************************************************************************
 * 函数名称：自检辅助函数1
 * 函数输入：自检模式，自检项目，自检值
 * 函数返回值：自检值
 * 函数描述：
 ****************************************************************************/
INT16U sAutoTestAsis1(INT8U lubModel,INT8U lubItem,INT16U luwValue)
{
	INT16U luwAutoTestAsisTemp;

	if(luwValue > 0)
	{
		luwAutoTestAsisTemp = luwValue;
	}
	else
	{
		if(lubItem < 3)
		{
			if(lubModel == 1)
			{
				luwAutoTestAsisTemp = Safty.data.uwGridVoltHigh[lubItem - 1];
			}
			else if(lubModel == 2)
			{
				luwAutoTestAsisTemp = Safty.data.uwGridVoltHighCnt[lubItem - 1] * 10; 
			}
		}
		else if(lubItem < 5)
		{
			if(lubModel == 1)
			{
				luwAutoTestAsisTemp = Safty.data.uwGridVoltLow[lubItem - 3];
			}
			else if(lubModel == 2)
			{
				luwAutoTestAsisTemp = Safty.data.uwGridVoltLowCnt[lubItem - 3] * 10; 
			}
			
		}
		else if(lubItem < 7)
		{
			if(lubModel == 1)
			{
				luwAutoTestAsisTemp = Safty.data.uwFreqHigh[lubItem - 5]; 
			}
			else if(lubModel == 2)
			{
				luwAutoTestAsisTemp = Safty.data.uwFreqHighCnt[lubItem - 5] * 10; 
			}
		}
		else if(lubIconSelected < 9)
		{
			if(lubModel == 1)
			{
				luwAutoTestAsisTemp = Safty.data.uwFreqLow[lubItem - 7]; 
			}
			else if(lubModel == 2)
			{
				luwAutoTestAsisTemp = Safty.data.uwFreqLowCnt[lubItem - 7] * 10; 
			}
		}
	}
	
	return luwAutoTestAsisTemp;
}

/*****************************************************************************
 * 函数名称：自检辅助函数
 * 函数输入：自检模式
 * 函数返回值：无
 * 函数描述：
 ****************************************************************************/
void sAutoTestAsis(INT8U lubModel)
{
	if(++lubBlinkCnt >= 20)
	{
		lubBlinkCnt = 0;
	}
	
	sWriteScreenBuf(0,0,20,"Auto Test:          ");
	sWriteScreenBuf(1,0,20,"Select Item         ");
	
	if(lubPageFlg != 1)
	{
		strcpy(lubString,"   27.S1    27.S2   ");
		
		if(lubIconSelected == 1 && lubBlinkCnt > 10)
		{
			strcpy(lubString,"        ");
		}
		if(lubIconSelected == 2 && lubBlinkCnt > 10)
		{
			strcpy(&lubString[8],"            ");
		}
		sWriteScreenBuf(2,0,20,lubString);
		
		strcpy(lubString,"   59.S1    59.S2   ");
		if(lubIconSelected == 3 && lubBlinkCnt > 10)
		{
			strcpy(lubString,"        ");
		}
		if(lubIconSelected == 4 && lubBlinkCnt > 10)
		{
			strcpy(&lubString[8],"            ");
		}
		sWriteScreenBuf(3,0,20,lubString);
	}
	else
	{
		strcpy(lubString,"  81<.S1    81<.S2  ");
		if(lubIconSelected == 5 && lubBlinkCnt > 10)
		{
			strcpy(lubString,"        ");
		}
		if(lubIconSelected == 6 && lubBlinkCnt > 10)
		{
			strcpy(&lubString[8],"            ");
		}
		sWriteScreenBuf(2,0,20,lubString);
		
		strcpy(lubString,"  81>.S1    81>.S2  ");
		if(lubIconSelected == 7 && lubBlinkCnt > 10)
		{
			strcpy(lubString,"        ");
		}
		if(lubIconSelected == 8 && lubBlinkCnt > 10)
		{
			strcpy(&lubString[8],"            ");
		}
		sWriteScreenBuf(3,0,20,lubString);
	}
	
	
	if(Button.bit.ubEnterPress)
	{
		lubInterface = cAutoTestPage;
		if(lubIconSelected == 1)
		{
			strcpy(lubStringTemp,"  27.S1             ");
		}
		else if(lubIconSelected == 2)
		{
			strcpy(lubStringTemp,"  27.S2             ");
		}
		else if(lubIconSelected == 3)
		{
			strcpy(lubStringTemp,"  59.S1             ");
		}
		else if(lubIconSelected == 4)
		{
			strcpy(lubStringTemp,"  59.S2             ");
		}
		else if(lubIconSelected == 5)
		{
			strcpy(lubStringTemp,"  81<.S1            ");
		}
		else if(lubIconSelected == 6)
		{
			strcpy(lubStringTemp,"  81<.S2            ");
		}
		else if(lubIconSelected == 7)
		{
			strcpy(lubStringTemp,"  81>.S1            ");
		}
		else if(lubIconSelected == 8)
		{
			strcpy(lubStringTemp,"  81>.S2            ");
		}
		else
		{
			lubInterface = lubInterfacePre;
		}
		Italy_SelfTest_Flag = lubIconSelected;
		*Protection_Func_Enable_Info.ItalySelfTestEN = __REV16(lubIconSelected);
		if(PRJ_NUMBER == SUNNYBEE)
				write_10_date.usStartAddr = 0x214D;
		else if(PRJ_NUMBER == HORNET)
				write_10_date.usStartAddr = 0x248A;
		write_10_date.usLen = 1;
		FLAG_10_UPDATE = 1;
		
		lubPageFlg = 0;
	}
	else if(Button.bit.ubUpPress)
	{
		if(--lubIconSelected > 8)
		{
			lubPageFlg = 0;
			lubIconSelected = 8;
		}
	}
	else if(Button.bit.ubDownPress)
	{
		if(++lubIconSelected > 8)
		{
			lubPageFlg = 1;
			lubIconSelected = 1;
		}
	}
	else if(Button.bit.ubEscPress)
	{
		lubPageFlg = 0;
		lubIconSelected = 0;
		lubInterface = cSetInterface;
	}
	
	if(lubIconSelected > 4)
	{
		lubPageFlg = 1;
	}
	else
	{
		lubPageFlg = 0;
	}
}
/*****************************************************************************
 * 函数名称：写LCD地址函数
 * 函数输入：地址
 * 函数返回值：无
 * 函数描述：
 ****************************************************************************/
void sWriteLCDAdr(INT8U lubLCDAdr)
{
	while(sGetLCDState())
	{
		if(++luwBusyCheckCnt >= 9000)
		break;
	}
	luwBusyCheckCnt = 0;
	sWriteLCDData(0x80 | lubLCDAdr,0);
}

/*****************************************************************************
 * 函数名称：界面更新函数
 * 函数输入：无
 * 函数返回值：无
 * 函数描述：
 ****************************************************************************/
void sUpdateLCD(void)
{
	INT8U lubLine;
	INT8U lubRow;
	for(lubLine = 0;lubLine < cMaxLine;lubLine++)
	{
		for(lubRow = 0;lubRow < cMaxRow;lubRow++)
		{
			sWriteLCDAdr(lubLCDAdr[lubLine] + lubRow);
			while(sGetLCDState())
			{
				if(++luwBusyCheckCnt >= 9000)
				break;
			}
			luwBusyCheckCnt = 0;
			sWriteLCDData(lubScreenBuf[lubLine][lubRow],1);
		}
	}
}


/*****************************************************************************
 * 函数名称：数据转化函数函数
 * 函数输入：小数点位置，带转化数值，缓存首地址，可用缓存长度
 * 函数返回值：无
 * 函数描述：
 ****************************************************************************/
void sTransferData(INT8U lubDot,INT32U lulData,INT8U *lubDataP,INT8U lubLength)
{
	INT8U i;
	
	if(lubLength < 9)
	{
		for(i = lubLength;i > 0;i--)
		{
			lulData %= c10Time[i];
			lubDataP[lubLength - i] = lulData / c10Time[i - 1] + 0x30;
		}
		
		if(lubDot && lubDot < lubLength)
		{
			for(i = lubLength;i > lubLength - lubDot;i--)
			{
				lubDataP[i] = lubDataP[i - 1];
			}
			lubDataP[lubLength - lubDot] = '.';
		}
	}
}

/*****************************************************************************
 * 函数名称：辅助显示函数
 * 函数输入：辅助显示的起始列数，终止列数，字符串内容?
 * 函数返回值：无
 * 函数描述：
 ****************************************************************************/
void sDisplayAsist(INT8U lubMin,INT8U lubMax,INT8U *lubStringAsist)
{
	INT8U i,j;

	j = 0;
	for(i = lubMin;i < lubMax;i++)
	{
		if(lubStringAsist[i] == '0')
			j ++;
		else
			break;
	}
	
	for(i = lubMin;i < (lubMin + j);i++)
		lubStringAsist[i] = ' ';
}

/*****************************************************************************
 * 函数名称：故障代码计算函数
 * 函数输入：故障值
 * 函数返回值：故障位
 * 函数描述：
 ****************************************************************************/
INT8U sCalFaultValue(INT32U lulError)
{
	INT8U i;

	for(i = 0;i < 32;i++)
	{
		if(lulError & 0x01)
		{
			lulError = i;
			break;
		}
		lulError >>= 1;
	}
	return(i);
}

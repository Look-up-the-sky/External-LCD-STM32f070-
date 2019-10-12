#ifndef _INTERFACE_DEF_H_
#define _INTERFACE_DEF_H_


/********************************************************************
 * Macros
 ********************************************************************/
/************LCD刷新间隔时间************/
#define cRefreshTime	5

/************按键************/
#define cEnter	0
#define cEsc	1
#define cUp		2
#define cDown	3

/************按键确认时间************/
#define cButtonOnMax		5
#define cButtonOffMax		5
#define cShortPressLimit	10
#define cLongPressLimit		200
#define cSpeakerOnLimit		20

/************系数************/
#define luwCoefSetMin	800
#define luwCoefSetMax	1200

/************界面代号************/
#define cLanguageMessage	0x00
#define cSaftyMessage		0x01
#define cDefaultInterface	0x02
#define cETotalInterface	0x100
#define cDCInterface		0x101
#define cACInterface		0x102
#define cTimeInterface		0x103
#define cTempInterface 		0x104
#define cInfoSN					0x200
#define cInfoVersion		0x201
#define cInfoLCDVersion		0x202
#define cErrorPage1			0x300
#define cErrorPage2			0x301
#define cErrorPage3			0x302
#define cErrorPage4			0x303
#define cErrorPage5			0x304
#define cErrorPage6			0x305
#define cErrorPage7			0x306
#define cErrorPage8			0x307
#define cErrorPage9			0x308
#define cErrorPage10		0x309
#define cErrorPage11		0x30A
#define cErrorPage12		0x30B
#define cErrorPage13		0x30C
#define cErrorPage14		0x30D
#define cErrorPage15		0x30E
#define cErrorPage16		0x30F
#define cErrorPage17		0x310
#define cErrorPage18		0x311
#define cErrorPage19		0x312
#define cErrorPage20		0x313

#define cSetInterface		0x400
#define cTimeSet			  0x401
#define cLanguageSet		0x402
#define	cPasswordSet		0x403
#define	cSaftySet			  0x404
#define cProtectionSet	0x405
#define cParamSet		    0x406
#define cCurveSet		    0x407
#define cKeyDataSet		  0x408
#define cFreqLimitSet		0x409
#define cVoltLimitSet		0x40A
#define cAutoTestSet		0x40B
#define cAutoTestResult	0x40C
#define cInvSet      		0x40D
#define cPasswordIn			0x500
#define cPasswordSaved	0x501
#define cAutoTestPage		0x502

/************界面大小************/
#define cMaxLine	4
#define cMaxRow		20

/************可选设置数************/
#define	cMaxSetList	13

/************可选保护数************/
#define cMaxProtection	13

/************LCD控制***********/
#define cLCMENOn()	GPIO_SetBits(GPIOB,GPIO_Pin_2)
#define cLCMENOff()	GPIO_ResetBits(GPIOB,GPIO_Pin_2)
#define cLCMRWOn()	GPIO_SetBits(GPIOA,GPIO_Pin_0)
#define cLCMRWOff()	GPIO_ResetBits(GPIOA,GPIO_Pin_0)
#define cLCMRSOn()	GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define cLCMRSOff()	GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define cLCMAOn()		GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define cLCMAOff()	GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define	LCDOUT(x)		GPIOB->ODR = ((GPIOB->ODR & 0x00ff) | ((INT8U)(x) << 8))

/************按键状态***********/
#define cGetButtonEsc()			!(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4))
#define cGetButtonEnter()   !(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5))
#define cGetButtonUp()	    !(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7))
#define cGetButtonDown()		!(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6))

/************运行故障指示灯***********/


/************蜂鸣器控制***********/
#define cSpeakerOn()	GPIO_SetBits(GPIOA,GPIO_Pin_12)
#define cSpeakerOff()	GPIO_ResetBits(GPIOA,GPIO_Pin_12)

/********************************************************************
 * Constant
 ********************************************************************/
 const INT8U MF_Param_Set[5][21] = 
{									
	"MaxP(%)             ",			
	"Rea_P Mode          ",		
	"PF Val              ",
	"Rea_P Val           ",	
	"Italy ID            "

};
 const INT8U HF_Param_Set[4][21] = 
{
	"MaxP(%)             ",			
	"Rea_P Mode          ",		
	"PF Val              ",
	"Rea_P Val           ",	
};


 const INT8U Inv_Set[2][21] = 
{
	"Turn ON/OFF         ",											
	"Standup Time        ",											
};

 const INT8U KeyData_Set[4][21] = 
{
	"Inv Reset           ",											
	"History Clear       ",											
	"PV Mode             ",											
	"G compensate        "
};

const INT8U MF_Protection_Set[14][21] = 
{
	"AI                  ",											
	"GFCI                ",																					
	"ISO                 ",	
	"DCI                 ",
	"GridVAuto           ",
	"GridPDerated        ",
	"MpptScan            ",
	"PvVI                ",
	"ItalySelfTest       ",
	"PID                 ",
	"DRM                 ",
	"ARCDetect           ",
	"ARCReset            ",
	"ZeroExpPerent       ",	
};

const INT8U HF_Protection_Set[15][21] = 
{
	"AI                  ",											
	"GFCI                ",																					
	"ISO                 ",	
	"DCI                 ",
	"GridVAuto           ",
	"GridPDerated        ",
	"MpptScan            ",
	"PvVI                ",
	"ItalySelfTest       ",
	"PID                 ",
	"DRM                 ",
	"ARCDetect           ",
	"ARCReset            ",
	"ReCPwrLimit         ",	
	"AntiReflux(%)       ",	
};

const INT8U Curve_Set[10][21] = 
{
	"WattFreqModeEn      ",											
	"WattVoltModeEn      ",																					
	"PfWattModeEn        ",											
	"VarVoltModeEn       ",																					
	"VarWattModeEn       ",	
	"ResvModeEn          ",											
	"LVRTCurveEn         ",																					
	"HVRTCurveEn         ",	
	"LFRTCurveEn         ",																														
	"HFRTCurveEn         ",	
};

const INT8U lubLanguage[4][21] = 
{
	"          English   ",											// 英语
	"          Deutsch   ",											// 德语
	"          Dutch     ",											// 荷兰语
	"          ITALIANO  "											// 意大利语
};
const INT8U lubStateTable[5][9][17]=
{
	//////////////////////////////////////////////
	// English
	//////////////////////////////////////////////
	{
		" Waiting    s   \0",	//0
		"     Normal     \0",	//1
		"  F/W Updating  \0",	//2
		"      Lock      \0",	//3
		"   Master MCU   \0",	//4
		"   Slave MCU    \0",	//5
		"    Com MCU     \0",	//6
		"   Waiting ACK  \0",	//7
		"Address:0x      \0"	//8
	},

	//////////////////////////////////////////////
	// German
	//////////////////////////////////////////////
	{
		"     Warten     \0",	
		"  Normalbetrieb \0",	
		"     FLASH      \0",	
		"      Lock      \0",	
		"   Master MCU   \0",	
		"   Slave MCU    \0",	
		"    Com MCU     \0",	
		"   Waiting ACK  \0",	
		"Address:0x      \0"	
	},
	
	//////////////////////////////////////////////
	// Nederland 荷兰
	//////////////////////////////////////////////
	{
		"     Wachten    \0",	
		" 	 Normaal	 \0",	
		"     FLASH      \0",	
		"      Slot      \0",	
		"   Master MCU   \0",	
		"   Slave MCU    \0",	
		"    Com MCU     \0",	
		"   Waiting ACK  \0",	
		"Address:0x      \0"	
	},
	
	//////////////////////////////////////////////
	// Italian  意大利语
	//////////////////////////////////////////////
	{
		"  Attendere...  \0",	//0 "    Waiting     \0"
		" Stato Normale  \0",	//1  "  Normal State \0"
		"     FLASH      \0",	//2  "     FLASH     \0"
		"    Bolccato    \0",	//3 "      Lock      \0"
		"   Master MCU   \0",
		"   Slave MCU    \0",
		"    Com MCU     \0",
		"   Waiting ACK  \0",
		"Address:0x      \0"		
	}
};

const INT8U lubSaftySort[cMaxSaftyCode]={0,1,2,3,4,5,6,7,8,9,
										10,11,12,13,14,15,16,17,18,19,
										20,21,22,23,25,60,61,62,63,72};
const INT8U lubSafty[73][12] = 
{
	[0]= "         UK",
	[1]= "    Germany",
	[2]= "     France",
	[3]= "      Italy",
	[4]= "      Spain",
	[5]= "    Austria",
	[6]= "  Australia",
	[7]= "      India",
	[8]= "    Belgium",
	[9]= "     Danish",
	[10]= "Greece-land",
	[11]= " Greece-Isl",
	[12]= "    Holland",
  [13]= "   Portugal",
	[14]= "      China",
	[15]= "     Sweden",
	[16]= " Other 50Hz",	
	[17]= "    Romania",	
	[18]= "New Zealand",
	[19]= "     Brazil",
	[20]= "WestAustril",	
	[21]= "  Sri Lanka",
	[22]= "Philippines",
	[23]= "    Vietnam",
	[25]= "    Vrabelg",	
	[60]= "     Canada",
	[61]= " USA/Mexico",
	[62]= " Other 60Hz",
	[63]= "South Korea",
	[72]= " Luxembourg"
};

const INT8U lubDefaultInterfaceIcon[cMaxLanguage][4][21] = 
{
	// 英语
	{
		"  Info  Error  Set  ",
		"        Error  Set  ",
		"  Info         Set  ",
		"  Info  Error       "
	},
	// 德语
	{
		"  Info  Erreur Fixe ",
		"        Erreur Fixe ",
		"  Info         Fixe ",
		"  Info  Erreur      "
	},
	// 荷兰语
	{
		"  Info  Fout   Set  ",
		"        Fout   Set  ",
		"  Info         Set  ",
		"  Info  Fout        "
	},
	// 意大利语
	{
		"  INFO  ERRORE IMPS ",
		"        ERRORE IMPS ",
		"  INFO         IMPS ",
		"  INFO  ERRORE      "
	}
};

const INT8U MF_lubErrorInterpret[78][21] = 
{
	[1] = "PV Input Err        ",
	[2] = "PV1 Connection Err  ",
	[3] = "PV2 Connection Err  ",
	[4] = "PV3 Connection Err  ",
	[5] = "PV Over Volt Err    ",
	[6] = "PV2 Over Volt Err   ",
	[7] = "PV3 Over Volt Err   ",	
	[8] = "Grid Volt Loss Err  ",	
	[9] = "Grid Over Volt Err1 ",
	[10] = "Grid Over Volt Err2 ", 
	[11] = "Grid Under Volt Err1",
	[12] = "Grid Under Volt Err2",
	[13] = "Grid Over Volt 10min",
	[16] = "Grid Over Volt Trans",
	[17] = "Grid Over Freq Err1 ",
	[18] = "Grid Over Freq Err2 ",
	[19] = "Grid Under Freq Err1",
	[20] = "Grid Under Freq Err2",
	[24] = "Boost SelfChk Err1  ",
	[25] = "Boost SelfChk Err2  ",
	[27] = "PV Over Current Err1",
	[28] = "PV Over Current Err2",
	[30] = "PV Over Curr HW Err ",
	[31] = "PV BUS Volt High    ",
	[32] = "BUS Volt Low Err    ",
	[33] = "BUS Over Volt Trans ",
	[34] = "BUS Over Volt Err   ",
	[35] = "BUS Over Volt HW Err",
	[41] = "Inv Over Curr Trans ",
	[42] = "Inv Over Curr Err   ",
	[43] = "Inv Over Curr HW Err",
	[44] = "Inv SelfChk Err     ",
	[46] = "DCI High Err        ",
	[47] = "Leak Curr Err       ",
	[48] = "Ambient Temp Low Err",
	[49] = "Ambient Over Temp   ",
	[50] = "IGBT Over Temp Err  ",
	[52] = "Temp Sensor Loss Err",
	[58] = "Power Unmatch Err   ",
	[63] = "ISO Err             ",
	[64] = "Output Relay Err    ",
	[65] = "GFCI Sensor Err     ",
	[69] = "GFCI Channel Err    ",
	[70] = "Inv Curr Channel Err",
	[71] = "PV Curr Channel Err1",
	[72] = "PV Curr Channel Err2",
	[73] = "SMCU Grid Volt Err  ",
	[74] = "SMCU Grid Freq Err  ",
	[75] = "Firm Ver Incorrect  ",
	[76] = "Model Incorrect Err ",
	[77] = "Ground Wiring Err   ",
	
};

const INT8U HF_lubErrorInterpret[90][21] = 
{
	[1] = "PV Input Err        ",
	[2] = "PV1 Connection Err  ",
	[3] = "PV2 Connection Err  ",
	[4] = "PV3 Connection Err  ",
	[5] = "PV Over Volt Err    ",
	[6] = "PV2 Over Volt Err   ",
	[7] = "PV3 Over Volt Err   ",	
	[8] = "Grid Volt Loss Err  ",	
	[9] = "Grid Over Volt Err1 ",
	[10] = "Grid Over Volt Err2 ", 
	[11] = "Grid Under Volt Err1",
	[12] = "Grid Under Volt Err2",
	[13] = "Grid Over Volt 10min",
	[16] = "Grid Over Volt Trans",
	[17] = "Grid Over Freq Err1 ",
	[18] = "Grid Over Freq Err2 ",
	[19] = "Grid Under Freq Err1",
	[20] = "Grid Under Freq Err2",
	[24] = "Boost SelfChk Err1  ",
	[25] = "Boost SelfChk Err2  ",
	[27] = "PV Over Current Err1",
	[28] = "PV Over Current Err2",
	[30] = "PV Over Curr HW Err ",
	[31] = "PV BUS Volt High    ",
	[32] = "BUS Volt Low Err    ",
	[33] = "BUS Over Volt Trans ",
	[34] = "BUS Over Volt Err   ",
	[35] = "BUS Over Volt HW Err",
	[36] = "Bus Unbalance Err   ",
	[37] = "Inv Short Circuit   ",
	[38] = "Inv Soft Start Err  ",
	[41] = "Inv Over Curr Trans ",
	[42] = "Inv Over Curr Err   ",
	[43] = "Inv Over Curr HW Err",
	[44] = "Inv SelfChk Err     ",
	[46] = "DCI High Err        ",
	[47] = "Boost Over Temp Err ",
	[48] = "Inv Over Temp Err   ",
	[49] = "Temp Sensor Loss Err",
	[50] = "Communication Err   ",
	[51] = "SPI Comm Fail Err   ",
	[52] = "EEPROM RW Fail Err  ",
	[53] = "Shut Down Err       ",
	[54] = "Power Supply Err    ",
	[55] = "Power Unmatch Err   ",
	[56] = "DC capacitance Err  ",
	[57] = "AC lightning pro Err",
	[58] = "DC lightning pro Err",
	[59] = "Fuse Err            ",
	[60] = "Output Relay Err    ",
	[61] = "GFCI Sensor Err     ",
	[62] = "Fan Err             ",
	[63] = "Bootstrapping Err   ",
	[64] = "Bus SW Start Fail   ",
	[65] = "GFCI Channel Err    ",
	[66] = "Inv Curr Channel Err",
	[67] = "PV Curr Channel Err1",
	[68] = "PV Curr Channel Err2",
	[69] = "SMCU Grid Volt Err  ",
	[70] = "SMCU Grid Freq Err  ",
	[71] = "Firm Ver Incorrect  ",
	[72] = "Model Incorrect Err ",
	[73] = "Inv Peak Curr Limit ",
	[74] = "ISO Fail Err        ",
	[75] = "GFCI Protect Fail Er",
	[76] = "Ambient Temp Low Err",
	[77] = "Ambient Over Temp Er",
	[78] = "Ground Connection Er",
	[79] = "Remote Turn Off Err",	
};

const INT8U lubPowerDisplay[cMaxLanguage][9] = 
{
	"Power   ",															//ENGLISH
	"Leistung",															//DEUTSCH
	"Vermogen",															//DUTCH
	"POTENZA "															//ITALIANO
};

const INT8U lubETodayDisplay[cMaxLanguage][9] = 
{
	"EToday  ",															//ENGLISH
	"EToday  ",															//DEUTSCH
	"EVandaag",															//DUTCH
	"EGiorno "															//ITALIANO
};

const INT8U lubETotalDisplay[cMaxLanguage][8] = 
{
	"ETotal ",															//ENGLISH
	"ETotal ",															//DEUTSCH
	"ETotaa ",															//DUTCH
	"ETotale"															//ITALIANO
};

const INT8U lubPleaseDisplay[cMaxLanguage][21] = 
{
	"Please              ",												//ENGLISH
	"Bitte               ",												//DEUTSCH
	"                    ",												//DUTCH
	"                    "												//ITALIANO
};

const INT8U lubChooseDisplay[cMaxLanguage][21] = 
{
	"Choose Your Country ",												//ENGLISH
	{'W',0x84,'h','l','e','n',' ','S','i','e',' ','I','h','r',' ','L','a','n','d',' ','\0'},//DEUTSCH
	"Selecteer uw Land   ",												//DUTCH
	"scegliere il paese  "												//ITALIANO
};

const INT8U lubManufacturerDisplay[cMaxLanguage][20] = 
{
	"Manufacturer:       ",												//ENGLISH
	"hersteller:         ",												//DEUTSCH
	"De fabrikant:       ",												//DUTCH
	"produttore:         "												//ITALIANO
};

const INT8U lubSetMenuDisplay[cMaxLanguage][9] = 
{
	{"Set Menu"},														//ENGLISH
	{'M','e','n',0x9A,' ','E','i','n','\0'},							//DEUTSCH
	{"Set Menu"},														//DUTCH
	{"MENU    "}														//ITALIANO
};

const INT8U lubSetListDisplay[cMaxLanguage][cMaxSetList][13] = 
{
	// 英语
	{
		{" Time       "},
		{" Language   "},
		{" Password   "},
		{" Safty      "},
		{" Protection "},
		{" Param Set  "},
		{" Curve Set  "},
		{" KeyData Set"},
		{" Freq Limit "},
		{" Volt Limit "},
		{" Auto Test  "},
		{" Test Result"},
		{" Inv Set    "}			
	},
	// 德语
	{
		{" Zeit       "},
		{" Sprache    "},
		{" Kennwort   "},
		{" Securite   "},
		{" Protection "},
		{" Param Set  "},
		{" Curve Set  "},
		{" KeyData Set"},
		{" Freq Grenze"},
		{" Volt Grenze"},
		{" Auto Test  "},
		{" Test Result"},
		{" Inv Set    "}	
	},
	// 荷兰语
	{
		{" Tijd       "},
		{" Taal       "},
		{" Wachtwoo   "},
		{" Seguridad  "},
		{" Protection "},
		{" Param Set  "},
		{" Curve Set  "},
		{" KeyData Set"},
		{" Freq Limit "},
		{" Volt Limit "},
		{" Auto Test  "},
		{" Test Result"},
		{" Inv Set    "}	
	},
	// 意大利语
	{
		{" IMP.ORARIO "},
		{" LINGUA     "},	
		{" PASSWORD   "},
		{" SAFETY     "},
		{" PROTEZIONI "},
		{" Param Set  "},
		{" Curve Set  "},
		{" KeyData Set"},
		{" Freq Limit "},
		{" Volt Limit "},
		{" Auto Test  "},
		{" Test Result"},	
		{" Inv Set    "}	
	}
};

const INT8U lubProtectionDisplay[cMaxLanguage][cMaxProtection][14] = 
{
	// 英语
	{
		{"  ISO        "},
		{"  GFCI       "},
		{"  DCI        "},
		{"10MinOverVolt"},
		{"P-F Mode1    "},
		{"  LVRT       "},
		{"Islanding    "},
		{"60Hz Enable  "},
		{"Soft Start   "},
		{"P-F Mode2    "},
		{"Mppt Scan    "},
		{"P-V Mode     "},
		{"P-V Mode2    "}
	},
	// 德语
	{
		{"  ISO        "},
		{"  GFCI       "},
		{"  DCI        "},
		{"10MinOverVolt"},
		{"P-F Mode1    "},
		{"  LVRT       "},
		{"Islanding    "},
		{"60Hz Enable  "},
		{"Soft Start   "},
		{"P-F Mode2    "},
		{"Mppt Scan    "},
		{"P-V Mode     "},
		{"P-V Mode2    "}
	},
	// 荷兰语
	{
		{"  ISO        "},
		{"  GFCI       "},
		{"  DCI        "},
		{"10MinOverVolt"},
		{"P-F Mode1    "},
		{"  LVRT       "},
		{"Islanding    "},
		{"60Hz Enable  "},
		{"Soft Start   "},
		{"P-F Mode2    "},
		{"Mppt Scan    "},
		{"P-V Mode     "},
		{"P-V Mode2    "}
	},
	// 意大利语
	{                                       
		{"VERIFICA ISOL"},
		{"GUASTO IMPIAN"},
		{"DC Injection "},
		{"10MinOverVolt"},
		{"SOVRAFREQZ 1 "},
		{"  LVRT       "},
		{"PROT.ISOLA   "},
		{"60HzABILITATO"},
		{"Soft Start   "},
		{"SOVRAFREQZ 2 "},
		{"MPPT SCAN    "},
		{"P-V Mode     "},
		{"P-V Mode2    "}
	}
};

SYSTEMTIME TimeRange = {50,12,31,24,60,60};
const INT8U lubScanMpptPeriodMax[2] = {4,60};
const INT8U lubLCDAdr[4] = {0x00,0x40,0x14,0x54};
const INT8U lubLCMINTable[4] = {0x38,0x08,0x06,0x0C};
const INT16U luwVoltSetMin[8] = {2300,2300,0,0,1000,850,0,0};
const INT16U luwVoltSetMax[8] = {2800,3200,10000,10000,2300,2300,10000,10000};

#endif

/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Lib_SlaveCom.c
 * 修改人：沈仕强
 * 描述：通讯
 * 初版日期：2015.05.29
 * 更改内容：
 * 1.1版--增加波特率设定功能；
 * 1.2版--修正读写操作起始地址都从0开始的错误；
 * 1.3版--修正判断注册时的错误；
 * 1.4版--增加debug数据使能；
 * 1.5版--通讯数据长度改为偶数，所有数据先发低8位，后发高8位；
 * 1.6版--修正时间设置标志位；
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"
#include "Lib_Communication.h"
#include "Lib_CRC.h"
#include "Lib_ClearRam.h"

/********************************************************************
 * Local Types
 ********************************************************************/
INT8U lubDelayStep;
INT8U lubInverterAddr[cComNum];

INT16U luwEepromW;

INT32U lulDataFromSN;

/********************************************************************
 * Global Types
 ********************************************************************/
SLAVEBAUDRATE SlaveBaudrate;

/*****************************************************************************
 * 函数名称：接收数据分析函数。
 * 函数输入：通讯口号。
 * 函数返回值：数据有效标志位。cTrue--有效；cFalse--无效
 * 函数描述：
 ****************************************************************************/
INT8U sAnalysisS(INT8U lubComNum)
{
	INT8U i;
	INT8U lubPosition;
	INT16U luwCRC;
	INT16U luwStartP;
	INT16U luwMaxSize;
	
	luwStartP = lubRXBuf[lubComNum][cStartPH];
	luwStartP <<= 8;
	luwStartP += lubRXBuf[lubComNum][cStartPL];
	luwMaxSize = luwStartP + lubRXBuf[lubComNum][cLengthL];

	if(lubRXBuf[lubComNum][cHeadH] != cHeadHData || lubRXBuf[lubComNum][cHeadL] != cHeadLData	//帧头错误
	|| lubRXBuf[lubComNum][cMasterP] != cMasterAddr												//主地址错误
	|| (lubRXBuf[lubComNum][cSlaveP] != lubInverterAddr[lubComNum] 								//从地址不一致
		&& lubRXBuf[lubComNum][cSlaveP] != 0xFF)												//不为广播模式
#ifdef DebugEnable
	|| lubRXBuf[lubComNum][cBlock] >= cMaxBlockNum)												//区块号超范围
#else
	|| lubRXBuf[lubComNum][cBlock] >= cMaxBlockNum
	|| lubRXBuf[lubComNum][cBlock] == cDebugBlock)												//区块号超范围
#endif
	{
		return(cFalse);
	}

	if(lubRXBuf[lubComNum][cFncCode] == cReadCmd || lubRXBuf[lubComNum][cFncCode] == cPrtctReadCmd)
	{
		luwCRC = lubRXBuf[lubComNum][cDataP];
		luwCRC <<= 8;
		luwCRC += lubRXBuf[lubComNum][cDataP + 1];												//接收的CRC值

		if(luwCRC == fCrcCal8(lubRXBuf[lubComNum],cDataP)										//校验通过
		&& luwMaxSize <= cMaxLength[lubRXBuf[lubComNum][cBlock]])
		{
			for(i = 0;i < cDataP;i++)															//发送缓存指令部分赋值
			{
				lubTXBuf[lubComNum][i] = lubRXBuf[lubComNum][i];
			}

			lubTXBuf[lubComNum][cFncCode] += 0x80;												//回复功能码+0x80

			lubPosition = lubRXBuf[lubComNum][cBlock];											//区块号
			for(i = 0;i < lubRXBuf[lubComNum][cLengthL];i++)
			{
				lubTXBuf[lubComNum][cDataP + i] = *(gubBlockPtr[lubPosition] + luwStartP + i);
			}

			lubPosition = lubRXBuf[lubComNum][cLengthL] + cDataP;
		}
		else
		{
			return(cFalse);
		}
	}
	else if(lubRXBuf[lubComNum][cFncCode] == cWriteCmd)
	{
		lubPosition = lubRXBuf[lubComNum][cLengthL] + cDataP;

		luwCRC = lubRXBuf[lubComNum][lubPosition];
		luwCRC <<= 8;
		luwCRC += lubRXBuf[lubComNum][lubPosition + 1];											//接收的CRC值

		if(luwCRC == fCrcCal8(lubRXBuf[lubComNum],lubPosition)
		&& luwMaxSize <= cMaxLength[lubRXBuf[lubComNum][cBlock]])
		{
			for(i = 0;i < cStartPL;i++)															//发送缓存指令部分赋值
			{
				lubTXBuf[lubComNum][i] = lubRXBuf[lubComNum][i];
			}

			lubTXBuf[lubComNum][cFncCode] += 0x80;												//回复功能码+0x80

			lubPosition = lubRXBuf[lubComNum][cBlock];											//区块号
			for(i = 0;i < lubRXBuf[lubComNum][cLengthL];i++)
			{
				*(gubBlockPtr[lubPosition] + luwStartP + i) = lubRXBuf[lubComNum][cDataP + i];	//保存发送的数据
			}
			
			luwEepromW |= ((INT16U)1 << lubPosition);											//设置写Eeprom标志位

			lubTXBuf[lubComNum][cStartPH] = 0;
			lubTXBuf[lubComNum][cStartPL] = cACK;												//发送ACK
			lubPosition = cStartPH + 1;
		}
		else
		{
			return(cFalse);
		}
	}
	else if(lubRXBuf[lubComNum][cFncCode] == cResetCmd || lubRXBuf[lubComNum][cFncCode] == cPrtctResetCmd)
	{
		luwCRC = lubRXBuf[lubComNum][cStartPL];
		luwCRC <<= 8;
		luwCRC += lubRXBuf[lubComNum][cStartPH];												//接收的CRC值

		lubPosition = lubRXBuf[lubComNum][cBlock];
		if(luwCRC == fCrcCal8(lubRXBuf[lubComNum],cStartPL) && lubPosition < 5 && lubPosition != 1 && lubPosition!= 2)
		{
			for(i = 0;i < cStartPL;i++)															//发送缓存指令部分赋值
			{
				lubTXBuf[lubComNum][i] = lubRXBuf[lubComNum][i];
			}
			
			lubTXBuf[lubComNum][cFncCode] += 0x80;												//回复功能码+0x80
			
			sClearRamByte(gubBlockPtr[lubPosition],cMaxLength[lubPosition]);					//清数据
			
			luwEepromW |= ((INT16U)1 << lubPosition);											//设置写Eeprom标志位

			lubTXBuf[lubComNum][cStartPH] = 0;
			lubTXBuf[lubComNum][cStartPL] = cACK;												//发送ACK
			lubPosition = cStartPH + 1;
		}
		else
		{
			return(cFalse);
		}
	}
	else if(lubRXBuf[lubComNum][cFncCode] == cPrtctWriteCmd)
	{
		if(lubRXBuf[lubComNum][cBlock] == cErrorRecordBlock										//受保护写的区块
		|| lubRXBuf[lubComNum][cBlock] == cSlaveDataBlock
		|| lubRXBuf[lubComNum][cBlock] == cMasterDataBlock
		|| lubRXBuf[lubComNum][cBlock] == cPowerMeterBlock
		|| lubRXBuf[lubComNum][cBlock] == cDebugBlock
		|| lubRXBuf[lubComNum][cBlock] > cComDataBlock)
		{
			return(cFalse);
		}
		
		lubPosition = lubRXBuf[lubComNum][cLengthL] + cDataP;

		luwCRC = lubRXBuf[lubComNum][lubPosition];
		luwCRC <<= 8;
		luwCRC += lubRXBuf[lubComNum][lubPosition + 1];											//接收的CRC值
		
		if(luwCRC == fCrcCal8(lubRXBuf[lubComNum],lubPosition)
		&& luwMaxSize <= cMaxLength[lubRXBuf[lubComNum][cBlock]])
		{
			for(i = 0;i < cStartPL;i++)															//发送缓存指令部分赋值
			{
				lubTXBuf[lubComNum][i] = lubRXBuf[lubComNum][i];
			}

			lubTXBuf[lubComNum][cFncCode] += 0x80;												//回复功能码+0x80

			lubPosition = lubRXBuf[lubComNum][cBlock];											//区块号
			for(i = 0;i < lubRXBuf[lubComNum][cLengthL];i++)
			{
				*(gubBlockPtr[lubPosition] + luwStartP + i) = lubRXBuf[lubComNum][cDataP + i];	//保存发送的数据
			}
			
			luwEepromW |= ((INT16U)1 << lubPosition);											//设置写Eeprom标志位

			lubTXBuf[lubComNum][cStartPH] = 0;
			lubTXBuf[lubComNum][cStartPL] = cACK;												//发送ACK
			lubPosition = cStartPH + 1;
		}
		else
		{
			return(cFalse);
		}
	}
	else if(lubRXBuf[lubComNum][cFncCode] == cSetBDRCmd)
	{
		luwCRC = lubRXBuf[lubComNum][cLengthL];
		luwCRC <<= 8;
		luwCRC += lubRXBuf[lubComNum][cLengthH];													//接收的CRC值
		
		if(luwCRC == fCrcCal8(lubRXBuf[lubComNum],cStartPH + 1))
		{
			for(i = 0;i < cStartPL;i++)															//发送缓存指令部分赋值
			{
				lubTXBuf[lubComNum][i] = lubRXBuf[lubComNum][i];
			}

			lubTXBuf[lubComNum][cFncCode] += 0x80;												//回复功能码+0x80

			if(lubRXBuf[lubComNum][cStartPH] != SlaveBaudrate.bit.ubBaudrate)
			{
				SlaveBaudrate.bit.ubBaudrate = lubRXBuf[lubComNum][cStartPH];
				SlaveBaudrate.bit.ubSetFlag = cTrue;
			}

			lubTXBuf[lubComNum][cStartPH] = 0;
			lubTXBuf[lubComNum][cStartPL] = cACK;												//发送ACK
			lubPosition = cStartPH + 1;
		}
		else
		{
			return(cFalse);
		}
	}
	else if(lubRXBuf[lubComNum][cFncCode] == cRegisterCmd)
	{
		luwCRC = lubRXBuf[lubComNum][cStartPL];
		luwCRC <<= 8;
		luwCRC += lubRXBuf[lubComNum][cStartPH];												//接收的CRC值

		if(luwCRC == fCrcCal8(lubRXBuf[lubComNum],cStartPL))									//校验通过
		{
			for(i = 0;i < cStartPL;i++)															//发送缓存指令部分赋值
			{
				lubTXBuf[lubComNum][i] = lubRXBuf[lubComNum][i];
			}

			lubTXBuf[lubComNum][cFncCode] += 0x80;												//回复功能码+0x80

			for(i = 0;i < 16;i++)
			{
				lubTXBuf[lubComNum][cStartPL + i] = Info.data.ubSerialNumber[i];
			}

			lubPosition = 16 + cStartPL;
		}
		else
		{
			return(cFalse);
		}
	}
	else if(lubRXBuf[lubComNum][cFncCode] == cSendAddrCmd)
	{
		luwCRC = lubRXBuf[lubComNum][cStartPH + 17];
		luwCRC <<= 8;
		luwCRC += lubRXBuf[lubComNum][cStartPH + 18];											//接收的CRC值

		if(luwCRC == fCrcCal8(lubRXBuf[lubComNum],cStartPH + 17))								//校验通过
		{
			lubTXBuf[lubComNum][cFncCode] += 0x80;												//回复功能码+0x80

			for(i = 0;i < 16;i++)
			{
				if(lubTXBuf[lubComNum][cStartPL + i] != Info.data.ubSerialNumber[i])
				{
					return(cFalse);																//SN错误，直接返回
				}
			}
			
			lubInverterAddr[lubComNum] = lubRXBuf[lubComNum][22];								//获得地址
		}
		
		return(cFalse);																			//无需回复
	}
	else if(lubRXBuf[lubComNum][cFncCode] == cDeleteAddrCmd)
	{
		luwCRC = lubRXBuf[lubComNum][cStartPL];
		luwCRC <<= 8;
		luwCRC += lubRXBuf[lubComNum][cStartPH];												//接收的CRC值
		
		if(luwCRC == fCrcCal8(lubRXBuf[lubComNum],cStartPL))									//校验通过
		{
			lubInverterAddr[lubComNum] = 0x00;
		}
		
		return(cFalse);																			//无需回复
	}
	else																						//功能码错误
	{
		return(cFalse);
	}

	luwCRC = fCrcCal8(lubTXBuf[lubComNum],lubPosition);
	lubTXBuf[lubComNum][lubPosition] = luwCRC >> 8;
	lubTXBuf[lubComNum][lubPosition + 1] = luwCRC;

	gubTXLength[lubComNum] = lubPosition + 2;													//发送长度
	if(gubTXLength[lubComNum] > cMaxTXLength)
	{
		gubTXLength[lubComNum] = cMaxTXLength;
	}

	if(lubRXBuf[lubComNum][cSlaveP] == 0xFF)													//广播模式不响应
	{
		return(cFalse);
	}
	else
	{
		return(cTrue);
	}
}

/*****************************************************************************
 * 函数名称：Eeprom数据更改设置函数。
 * 函数输入：无。
 * 函数返回值：带更改数据区号。
 * 函数描述：
 ****************************************************************************/
INT16U sGetEepromW(void)
{
	INT16U luwEepromWTemp;
	
	luwEepromWTemp = luwEepromW & 0x21F;
	if(luwEepromWTemp & 0x200)
	{
		luwEepromWTemp |= 0x20;
	}
	luwEepromWTemp <<= 8;
	luwEepromW = 0;
	return(luwEepromWTemp);
}

/*****************************************************************************
 * 函数名称：设置倒计时函数。
 * 函数输入：无。
 * 函数返回值：延时时间（单位5ms）。
 * 函数描述：一台机器不会有超过一个的注册需求，所以延时时间不是数组。
 * 计算延时时考虑生产月份、日期、工单号和流水线号，因为这些值会有很大的变化
 * 而其它值在同一个电站很可能是一样的。
 * 		延时计算方法：
 * 			第一次延时：延时基数 对10的余数，延时基数缩小10倍，计算延时基数；
 * 			第二次延时：延时基数 对100的余数，延时基数缩小100倍；
 * 			第三次延时：延时基数 对100的余数，延时基数缩小100倍；
 * 			第四次延时：延时基数 对100的余数，延时基数缩小100倍；
 * 
 * 			延时基数 = 月份 * 日期数量（31） * 工单数量（35） * 流水号数量（1000）
 * 					+ 日期 * 工单数量（35） * 流水号数量（1000）
 * 					+ 工单号 * 流水号数量（1000）
 * 					+ 流水号
 ****************************************************************************/
INT8U sSetDelayTime(void)
{
	INT8U lubTemp;
	INT32U lulSNTemp;
	
	if(++lubDelayStep >= 4)
	{
		lubDelayStep = 0;
		
		lubTemp = Info.data.ubSerialNumber[10];				//月份：0到11
		lubTemp -= 0x31;
		if(lubTemp > 9)
		{
			lubTemp -= 7;
			if(lubTemp >= 12)
			{
				lubTemp = 0;
			}
		}
		lulDataFromSN = lubTemp;
		lulDataFromSN *= 31;
		
		lubTemp = Info.data.ubSerialNumber[11];				//日期：0到30
		lubTemp -= 0x31;
		if(lubTemp > 9)
		{
			lubTemp -= 7;
			if(lubTemp >= 17)
			{
				lubTemp--;									//没有I
				if(lubTemp >= 22)
				{
					lubTemp--;								//没有O
					if(lubTemp >= 31)
					{
						lubTemp = 0;
					}
				}
			}
		}
		lulDataFromSN += lubTemp;
		lulDataFromSN *= 35;
		
		lubTemp = Info.data.ubSerialNumber[12];				//工单号
		lubTemp -= 0x31;
		if(lubTemp > 9)
		{
			lubTemp -= 7;
			if(lubTemp >= 35)
			{
				lubTemp = 0;
			}
		}
		lulDataFromSN += lubTemp;
		lulDataFromSN *= 10;
		
		lubTemp = Info.data.ubSerialNumber[13];				//流水号
		lubTemp -= 0x31;
		if(lubTemp > 9)
		{
			lubTemp = 0;
		}
		lulDataFromSN += lubTemp;
		lulDataFromSN *= 10;
		
		lubTemp = Info.data.ubSerialNumber[14];				//流水号
		lubTemp -= 0x31;
		if(lubTemp > 9)
		{
			lubTemp = 0;
		}
		lulDataFromSN += lubTemp;
		lulDataFromSN *= 10;

		lubTemp = Info.data.ubSerialNumber[15];				//流水号
		lubTemp -= 0x31;
		if(lubTemp > 9)
		{
			lubTemp = 0;
		}
		lulDataFromSN += lubTemp;							//最大值：11*31*35*100+30*35*100+34*1000+999=13019999
	}
	else
	{
		lulSNTemp = lulDataFromSN;
		lulDataFromSN /= 50;
	}
	
	return(lulSNTemp % 50);
}

/*****************************************************************************
 * 函数名称：从机地址设置函数。
 * 函数输入：从机地址，通讯口。
 * 函数返回值：无。
 * 函数描述：
 ****************************************************************************/
void sSetSlaveCom(INT8U lubAddr,INT8U lubComNum)
{
	if(lubComNum < cComNum)
	{
		lubInverterAddr[lubComNum] = lubAddr;
	}
}

/*****************************************************************************
 * 函数名称：从机地址获得函数。
 * 函数输入：通讯口。
 * 函数返回值：从机地址。
 * 函数描述：
 ****************************************************************************/
INT8U sGetSlaveCom(INT8U lubComNum)
{
	if(lubComNum < cComNum)
	{
		return(lubInverterAddr[lubComNum]);
	}
	else
	{
		return(0);
	}
}

/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Lib_MasterCom.c
 * 修改人：陈波
 * 描述：通讯
 * 初版日期：2015.05.29
 * 更改内容：
 * 1.1版--修正发送数据长度错误的bug；接收写反馈时不需要判断数据长度；
 * 1.2版--增加debug数据使能；
 * 1.3版--通讯数据长度改为偶数，所有数据先发低8位，后发高8位；
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"
#include "Lib_Communication.h"
#include "Lib_CRC.h"

/*****************************************************************************
 * 函数名称：发送缓存填充函数。
 * 函数输入：通讯口号，指令数组首地址,从机地址。
 * 函数返回值：缓存填充成功标志位。cTrue--有效；cFalse--无效
 * 函数描述：
 ****************************************************************************/
INT8U sSetTXBufM(INT8U lubComNum,INT8U *lubCmdPtr,INT8U lubSlaveAddr)
{
	INT8U i;
	INT8U lubLength;
	INT16U luwStartP;
	INT16U luwCRC;

	if(lubSlaveAddr > cMaxSlaveAddr || lubSlaveAddr == 0						//地址错误
#ifdef DebugEnable
	|| lubCmdPtr[0] >= cMaxBlockNum												//区块号超范围
#else
	|| lubCmdPtr[0] >= cMaxBlockNum	|| lubCmdPtr[0] == cDebugBlock				//区块号超范围
#endif
	|| lubCmdPtr[2] + lubCmdPtr[3] > cMaxLength[lubCmdPtr[0]])					//读取数据超范围
	{
		return(cFalse);
	}

	lubTXBuf[lubComNum][cHeadH] = cHeadHData;
	lubTXBuf[lubComNum][cHeadL] = cHeadLData;
	lubTXBuf[lubComNum][cMasterP] = cMasterAddr;
	lubTXBuf[lubComNum][cSlaveP] = lubSlaveAddr;
	lubTXBuf[lubComNum][cBlock] = lubCmdPtr[0];
	lubTXBuf[lubComNum][cFncCode] = lubCmdPtr[1];
	lubTXBuf[lubComNum][cStartPL] = lubCmdPtr[2];
	lubTXBuf[lubComNum][cStartPH] = lubCmdPtr[3];
	lubTXBuf[lubComNum][cLengthL] = lubCmdPtr[4];
	lubTXBuf[lubComNum][cLengthH] = lubCmdPtr[5];

	if(lubCmdPtr[1] == cReadCmd)
	{
		lubLength = cDataP;
	}
	else if(lubCmdPtr[1] == cWriteCmd)
	{
		luwStartP = lubCmdPtr[3];
		luwStartP <<= 8;
		luwStartP += lubCmdPtr[2];
		for(i = 0;i < lubCmdPtr[4];i++)
		{
			lubTXBuf[lubComNum][cDataP + i] = *(gubBlockPtr[lubCmdPtr[0]] + luwStartP + i);
		}
		lubLength = cDataP + lubCmdPtr[4];
	}
	else																		//功能码错误
	{
		return(cFalse);
	}

	luwCRC = fCrcCal8(lubTXBuf[lubComNum],lubLength);
	lubTXBuf[lubComNum][lubLength] = luwCRC >> 8;
	lubTXBuf[lubComNum][lubLength + 1] = luwCRC;

	gubTXLength[lubComNum] = lubLength + 2;										//发送长度
	if(gubTXLength[lubComNum] > cMaxTXLength)
	{
		gubTXLength[lubComNum] = cMaxTXLength;
	}

	return(cTrue);
}

/*****************************************************************************
 * 函数名称：接收数据分析函数。
 * 函数输入：通讯口号。
 * 函数返回值：数据有效标志位。cTrue--有效；cFalse--无效
 * 函数描述：
 ****************************************************************************/
INT8U sAnalysisM(INT8U lubComNum)
{
	INT8U i;
	INT8U lubPosition;
	INT16U luwCRC;
	INT16U luwMaxSize;

	if(lubRXBuf[lubComNum][cHeadH] != cHeadHData || lubRXBuf[lubComNum][cHeadL] != cHeadLData	//帧头错误
	|| lubRXBuf[lubComNum][cMasterP] != cMasterAddr												//主地址错误
	|| lubRXBuf[lubComNum][cSlaveP] > cMaxSlaveAddr												//从地址超范围
#ifdef DebugEnable
	|| lubRXBuf[lubComNum][cBlock] >= cMaxBlockNum)												//区块号超范围
#else
	|| lubRXBuf[lubComNum][cBlock] >= cMaxBlockNum
	|| lubRXBuf[lubComNum][cBlock] == cDebugBlock)												//区块号超范围
#endif
	{
		return(cFalse);
	}

	if(lubRXBuf[lubComNum][cFncCode] == cReadCmd + 0x80)
	{
		luwMaxSize = lubRXBuf[lubComNum][cStartPH];
		luwMaxSize <<= 8;
		luwMaxSize += lubRXBuf[lubComNum][cStartPL];
		luwMaxSize += lubRXBuf[lubComNum][cLengthL];
		
		if(luwMaxSize > cMaxLength[lubRXBuf[lubComNum][cBlock]])								//数据超范围
		{
			return(cFalse);
		}
		
		lubPosition = lubRXBuf[lubComNum][cLengthL] + cDataP;									//CRC在数据表中的位置

		luwCRC = lubRXBuf[lubComNum][lubPosition];
		luwCRC <<= 8;
		luwCRC += lubRXBuf[lubComNum][lubPosition + 1];											//接收的CRC值

		if(luwCRC == fCrcCal8(lubRXBuf[lubComNum],lubPosition))									//校验通过
		{
			for(i = 0;i < lubRXBuf[lubComNum][cLengthL];i++)
			{
				*(gubBlockPtr[lubRXBuf[lubComNum][cBlock]] + i) = lubRXBuf[lubComNum][cDataP + i];
			}
		}
		else
		{
			return(cFalse);
		}
	}
	else if(lubRXBuf[lubComNum][cFncCode] == cWriteCmd + 0x80)
	{
		if(lubRXBuf[lubComNum][cStartPL] != cACK)
		{
			return(cFalse);
		}
		else
		{
			luwCRC = lubRXBuf[lubComNum][cLengthL];
			luwCRC <<= 8;
			luwCRC += lubRXBuf[lubComNum][cLengthH];											//接收的CRC值

			if(luwCRC != fCrcCal8(lubRXBuf[lubComNum],cStartPH + 1))							//校验通过
			{
				return(cFalse);
			}
		}
	}
	else																						//功能码错误
	{
		return(cFalse);
	}

	return(cTrue);
}

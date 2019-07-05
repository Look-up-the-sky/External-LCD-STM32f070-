#include "App.h"

/**********************************************************
 * Function     : fCrcCal
 * Description  : 生成crc校验值
 * Input        : lubtPtr, 全帧的数组指针, lubtLen,帧长度-2
 * Output       : INT16U，CRC的结果
 **********************************************************/
INT16U fCrcCal8(INT8U* lubtPtr,INT16U luwtLen)
{
	INT8U j,lubtCrcLen;
	INT16U luwCrcData;

	luwCrcData = 0xFFFF;
	for(lubtCrcLen = 0; lubtCrcLen < luwtLen; lubtCrcLen++)
	{
		luwCrcData ^= lubtPtr[lubtCrcLen];
		for(j = 0; j < 8; ++j)
		{
			if(luwCrcData & 0x01)
			{
				luwCrcData = (luwCrcData >> 1) ^ 0xa001;
			}
			else
			{
				luwCrcData = (luwCrcData >> 1);
			}
		}
	}

	luwCrcData = ((luwCrcData & 0xFF) << 8) | (luwCrcData >> 8);
	return luwCrcData;
}

/**********************************************************
 * Function     : fCrcCal
 * Description  : 生成crc校验值
 * Input        : lubtPtr, 全帧的数组指针, lubtLen,帧长度-2
 * Output       : INT16U，CRC的结果
 **********************************************************/
INT16U fCrcCal(INT16U* luwtPtr,INT16U luwtLen)
{
	INT16U luwCrcData,luwtCrcLen;
	INT16U j;
	INT16U luwTemp;

	luwCrcData = 0xFFFF;
	for(luwtCrcLen = 0; luwtCrcLen < luwtLen; luwtCrcLen++)
	{
		luwTemp = (luwtPtr[luwtCrcLen] >> 8) & 0x00FF;
		luwCrcData ^= luwTemp;
		for(j = 0; j < 8; ++j)
		{
			if(luwCrcData & 0x01)
			{
				luwCrcData = (luwCrcData >> 1) ^ 0xa001;
			}
			else
			{
				luwCrcData = (luwCrcData >> 1);
			}
		}

		luwTemp = luwtPtr[luwtCrcLen] & 0x00FF;
		luwCrcData ^= luwTemp;
		for(j = 0; j < 8; ++j)
		{
			if(luwCrcData & 0x01)
			{
				luwCrcData = (luwCrcData >> 1) ^ 0xa001;
			}
			else
			{
				luwCrcData = (luwCrcData >> 1);
			}
		}
	}

	luwCrcData = ((luwCrcData & 0xFF) << 8) | (luwCrcData >> 8);
	return luwCrcData;
}

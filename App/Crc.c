/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Crc.c
 * 修改人：沈仕强
 * 修改日期：2019.06.20
 * 描述：Crc函数
 *******************************************************/
#include "Crc.h"

INT16U crc16(INT8U *pucDataBuf, INT16U usLen) 
{
	INT16U usBitCnt;
	INT16U usByteCnt;
	INT16U usCrcData;
	
	usCrcData = 0xFFFF;
	for(usByteCnt = 0;usByteCnt < usLen;usByteCnt++)
	{
		usCrcData ^=((*pucDataBuf)&0xFFFF);
		for(usBitCnt = 0;usBitCnt <8;usBitCnt++)
		{
			if(usCrcData & 0x0001)
			{
				usCrcData>>=1;
				usCrcData^=0xA001;
			}
			else
			{
				usCrcData>>=1;
			}
		}
		pucDataBuf++;
	}
	return(usCrcData);
}


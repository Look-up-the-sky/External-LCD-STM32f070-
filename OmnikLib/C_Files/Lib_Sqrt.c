/**********************************************************************
 * 版权:苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Lib_Sqrt.c
 * 修改人：沈仕强
 * 修改日期：2014.10.29
 * 描述：开方函数
 * 修改内容：
 * 1.1版--include目录改为在项目中设定；
***********************************************************************/

#include "App.h"

/*****************************************************************************
 * 函数功能：开方计算。
 * 函数输入：被开方的值
 * 函数描述：
 ****************************************************************************/
INT16U swRoot(INT32U lulSquare)
{
	INT16U luwCnt;
	INT32S lslSquareRoot;

	if(lulSquare > 1073741824)
	{
		lslSquareRoot = lulSquare / 16384 + 1023;
	}
	else if(lulSquare > 67108864)
	{
		lslSquareRoot = lulSquare / 4096 + 255;
	}
	else if(lulSquare > 4194304)
	{
		lslSquareRoot = lulSquare / 1024 + 63;
	}
	else
	{
		lslSquareRoot = lulSquare / 128 + 31;
	}

	for(luwCnt = 0; luwCnt < 6; luwCnt++)
	{
		if(lslSquareRoot <= 0)
		{
			return(0);
		}
		lslSquareRoot = (lslSquareRoot + lulSquare / lslSquareRoot) / 2;
	}
	return ((INT16U)lslSquareRoot);
}

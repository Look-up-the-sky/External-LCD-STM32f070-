/**********************************************************************
 * ��Ȩ:����ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�Lib_Sqrt.c
 * �޸��ˣ�����ǿ
 * �޸����ڣ�2014.10.29
 * ��������������
 * �޸����ݣ�
 * 1.1��--includeĿ¼��Ϊ����Ŀ���趨��
***********************************************************************/

#include "App.h"

/*****************************************************************************
 * �������ܣ��������㡣
 * �������룺��������ֵ
 * ����������
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

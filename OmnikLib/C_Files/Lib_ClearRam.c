/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾������
 * �ļ����ƣ�Lib_ClearRam.c
 * �޸��ˣ�����ǿ
 * ������ͨѶ
 * �������ڣ�2015.06.04
 * �������ݣ�
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"

/*****************************************************************************
 * �������ƣ�ram�����ֽ����㺯��
 * �������룺��
 * ��������ֵ����
 * ������������Դ��ַ��Դ��ַ�ӳ��ȵ��������㡣
 ****************************************************************************/
void sClearRamByte(INT8U *lubDim,INT32U lulLen)
{
	while(lulLen--)
	{
		*lubDim++ = 0;
	}
}

/*****************************************************************************
 * �������ƣ�ram���������㺯��
 * �������룺��
 * ��������ֵ����
 * ������������Դ��ַ��Դ��ַ�ӳ��ȵ��������㡣
 ****************************************************************************/
void sClearRamWord(INT16U *luwDim,INT32U lulLen)
{
	while(lulLen--)
	{
		*luwDim++ = 0;
	}
}

/*****************************************************************************
 * �������ƣ�ram����˫�����㺯��
 * �������룺��
 * ��������ֵ����
 * ������������Դ��ַ��Դ��ַ�ӳ��ȵ��������㡣
 ****************************************************************************/
void sClearRamLong(INT32U *lulDim,INT32U lulLen)
{
	while(lulLen--)
	{
		*lulDim++ = 0;
	}
}
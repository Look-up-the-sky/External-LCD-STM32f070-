/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�Lib_MasterCom.c
 * �޸��ˣ��²�
 * ������ͨѶ
 * �������ڣ�2015.05.29
 * �������ݣ�
 * 1.1��--�����������ݳ��ȴ����bug������д����ʱ����Ҫ�ж����ݳ��ȣ�
 * 1.2��--����debug����ʹ�ܣ�
 * 1.3��--ͨѶ���ݳ��ȸ�Ϊż�������������ȷ���8λ���󷢸�8λ��
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"
#include "Lib_Communication.h"
#include "Lib_CRC.h"

/*****************************************************************************
 * �������ƣ����ͻ�����亯����
 * �������룺ͨѶ�ںţ�ָ�������׵�ַ,�ӻ���ַ��
 * ��������ֵ���������ɹ���־λ��cTrue--��Ч��cFalse--��Ч
 * ����������
 ****************************************************************************/
INT8U sSetTXBufM(INT8U lubComNum,INT8U *lubCmdPtr,INT8U lubSlaveAddr)
{
	INT8U i;
	INT8U lubLength;
	INT16U luwStartP;
	INT16U luwCRC;

	if(lubSlaveAddr > cMaxSlaveAddr || lubSlaveAddr == 0						//��ַ����
#ifdef DebugEnable
	|| lubCmdPtr[0] >= cMaxBlockNum												//����ų���Χ
#else
	|| lubCmdPtr[0] >= cMaxBlockNum	|| lubCmdPtr[0] == cDebugBlock				//����ų���Χ
#endif
	|| lubCmdPtr[2] + lubCmdPtr[3] > cMaxLength[lubCmdPtr[0]])					//��ȡ���ݳ���Χ
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
	else																		//���������
	{
		return(cFalse);
	}

	luwCRC = fCrcCal8(lubTXBuf[lubComNum],lubLength);
	lubTXBuf[lubComNum][lubLength] = luwCRC >> 8;
	lubTXBuf[lubComNum][lubLength + 1] = luwCRC;

	gubTXLength[lubComNum] = lubLength + 2;										//���ͳ���
	if(gubTXLength[lubComNum] > cMaxTXLength)
	{
		gubTXLength[lubComNum] = cMaxTXLength;
	}

	return(cTrue);
}

/*****************************************************************************
 * �������ƣ��������ݷ���������
 * �������룺ͨѶ�ںš�
 * ��������ֵ��������Ч��־λ��cTrue--��Ч��cFalse--��Ч
 * ����������
 ****************************************************************************/
INT8U sAnalysisM(INT8U lubComNum)
{
	INT8U i;
	INT8U lubPosition;
	INT16U luwCRC;
	INT16U luwMaxSize;

	if(lubRXBuf[lubComNum][cHeadH] != cHeadHData || lubRXBuf[lubComNum][cHeadL] != cHeadLData	//֡ͷ����
	|| lubRXBuf[lubComNum][cMasterP] != cMasterAddr												//����ַ����
	|| lubRXBuf[lubComNum][cSlaveP] > cMaxSlaveAddr												//�ӵ�ַ����Χ
#ifdef DebugEnable
	|| lubRXBuf[lubComNum][cBlock] >= cMaxBlockNum)												//����ų���Χ
#else
	|| lubRXBuf[lubComNum][cBlock] >= cMaxBlockNum
	|| lubRXBuf[lubComNum][cBlock] == cDebugBlock)												//����ų���Χ
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
		
		if(luwMaxSize > cMaxLength[lubRXBuf[lubComNum][cBlock]])								//���ݳ���Χ
		{
			return(cFalse);
		}
		
		lubPosition = lubRXBuf[lubComNum][cLengthL] + cDataP;									//CRC�����ݱ��е�λ��

		luwCRC = lubRXBuf[lubComNum][lubPosition];
		luwCRC <<= 8;
		luwCRC += lubRXBuf[lubComNum][lubPosition + 1];											//���յ�CRCֵ

		if(luwCRC == fCrcCal8(lubRXBuf[lubComNum],lubPosition))									//У��ͨ��
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
			luwCRC += lubRXBuf[lubComNum][cLengthH];											//���յ�CRCֵ

			if(luwCRC != fCrcCal8(lubRXBuf[lubComNum],cStartPH + 1))							//У��ͨ��
			{
				return(cFalse);
			}
		}
	}
	else																						//���������
	{
		return(cFalse);
	}

	return(cTrue);
}

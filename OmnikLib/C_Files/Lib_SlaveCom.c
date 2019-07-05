/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�Lib_SlaveCom.c
 * �޸��ˣ�����ǿ
 * ������ͨѶ
 * �������ڣ�2015.05.29
 * �������ݣ�
 * 1.1��--���Ӳ������趨���ܣ�
 * 1.2��--������д������ʼ��ַ����0��ʼ�Ĵ���
 * 1.3��--�����ж�ע��ʱ�Ĵ���
 * 1.4��--����debug����ʹ�ܣ�
 * 1.5��--ͨѶ���ݳ��ȸ�Ϊż�������������ȷ���8λ���󷢸�8λ��
 * 1.6��--����ʱ�����ñ�־λ��
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
 * �������ƣ��������ݷ���������
 * �������룺ͨѶ�ںš�
 * ��������ֵ��������Ч��־λ��cTrue--��Ч��cFalse--��Ч
 * ����������
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

	if(lubRXBuf[lubComNum][cHeadH] != cHeadHData || lubRXBuf[lubComNum][cHeadL] != cHeadLData	//֡ͷ����
	|| lubRXBuf[lubComNum][cMasterP] != cMasterAddr												//����ַ����
	|| (lubRXBuf[lubComNum][cSlaveP] != lubInverterAddr[lubComNum] 								//�ӵ�ַ��һ��
		&& lubRXBuf[lubComNum][cSlaveP] != 0xFF)												//��Ϊ�㲥ģʽ
#ifdef DebugEnable
	|| lubRXBuf[lubComNum][cBlock] >= cMaxBlockNum)												//����ų���Χ
#else
	|| lubRXBuf[lubComNum][cBlock] >= cMaxBlockNum
	|| lubRXBuf[lubComNum][cBlock] == cDebugBlock)												//����ų���Χ
#endif
	{
		return(cFalse);
	}

	if(lubRXBuf[lubComNum][cFncCode] == cReadCmd || lubRXBuf[lubComNum][cFncCode] == cPrtctReadCmd)
	{
		luwCRC = lubRXBuf[lubComNum][cDataP];
		luwCRC <<= 8;
		luwCRC += lubRXBuf[lubComNum][cDataP + 1];												//���յ�CRCֵ

		if(luwCRC == fCrcCal8(lubRXBuf[lubComNum],cDataP)										//У��ͨ��
		&& luwMaxSize <= cMaxLength[lubRXBuf[lubComNum][cBlock]])
		{
			for(i = 0;i < cDataP;i++)															//���ͻ���ָ��ָ�ֵ
			{
				lubTXBuf[lubComNum][i] = lubRXBuf[lubComNum][i];
			}

			lubTXBuf[lubComNum][cFncCode] += 0x80;												//�ظ�������+0x80

			lubPosition = lubRXBuf[lubComNum][cBlock];											//�����
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
		luwCRC += lubRXBuf[lubComNum][lubPosition + 1];											//���յ�CRCֵ

		if(luwCRC == fCrcCal8(lubRXBuf[lubComNum],lubPosition)
		&& luwMaxSize <= cMaxLength[lubRXBuf[lubComNum][cBlock]])
		{
			for(i = 0;i < cStartPL;i++)															//���ͻ���ָ��ָ�ֵ
			{
				lubTXBuf[lubComNum][i] = lubRXBuf[lubComNum][i];
			}

			lubTXBuf[lubComNum][cFncCode] += 0x80;												//�ظ�������+0x80

			lubPosition = lubRXBuf[lubComNum][cBlock];											//�����
			for(i = 0;i < lubRXBuf[lubComNum][cLengthL];i++)
			{
				*(gubBlockPtr[lubPosition] + luwStartP + i) = lubRXBuf[lubComNum][cDataP + i];	//���淢�͵�����
			}
			
			luwEepromW |= ((INT16U)1 << lubPosition);											//����дEeprom��־λ

			lubTXBuf[lubComNum][cStartPH] = 0;
			lubTXBuf[lubComNum][cStartPL] = cACK;												//����ACK
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
		luwCRC += lubRXBuf[lubComNum][cStartPH];												//���յ�CRCֵ

		lubPosition = lubRXBuf[lubComNum][cBlock];
		if(luwCRC == fCrcCal8(lubRXBuf[lubComNum],cStartPL) && lubPosition < 5 && lubPosition != 1 && lubPosition!= 2)
		{
			for(i = 0;i < cStartPL;i++)															//���ͻ���ָ��ָ�ֵ
			{
				lubTXBuf[lubComNum][i] = lubRXBuf[lubComNum][i];
			}
			
			lubTXBuf[lubComNum][cFncCode] += 0x80;												//�ظ�������+0x80
			
			sClearRamByte(gubBlockPtr[lubPosition],cMaxLength[lubPosition]);					//������
			
			luwEepromW |= ((INT16U)1 << lubPosition);											//����дEeprom��־λ

			lubTXBuf[lubComNum][cStartPH] = 0;
			lubTXBuf[lubComNum][cStartPL] = cACK;												//����ACK
			lubPosition = cStartPH + 1;
		}
		else
		{
			return(cFalse);
		}
	}
	else if(lubRXBuf[lubComNum][cFncCode] == cPrtctWriteCmd)
	{
		if(lubRXBuf[lubComNum][cBlock] == cErrorRecordBlock										//�ܱ���д������
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
		luwCRC += lubRXBuf[lubComNum][lubPosition + 1];											//���յ�CRCֵ
		
		if(luwCRC == fCrcCal8(lubRXBuf[lubComNum],lubPosition)
		&& luwMaxSize <= cMaxLength[lubRXBuf[lubComNum][cBlock]])
		{
			for(i = 0;i < cStartPL;i++)															//���ͻ���ָ��ָ�ֵ
			{
				lubTXBuf[lubComNum][i] = lubRXBuf[lubComNum][i];
			}

			lubTXBuf[lubComNum][cFncCode] += 0x80;												//�ظ�������+0x80

			lubPosition = lubRXBuf[lubComNum][cBlock];											//�����
			for(i = 0;i < lubRXBuf[lubComNum][cLengthL];i++)
			{
				*(gubBlockPtr[lubPosition] + luwStartP + i) = lubRXBuf[lubComNum][cDataP + i];	//���淢�͵�����
			}
			
			luwEepromW |= ((INT16U)1 << lubPosition);											//����дEeprom��־λ

			lubTXBuf[lubComNum][cStartPH] = 0;
			lubTXBuf[lubComNum][cStartPL] = cACK;												//����ACK
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
		luwCRC += lubRXBuf[lubComNum][cLengthH];													//���յ�CRCֵ
		
		if(luwCRC == fCrcCal8(lubRXBuf[lubComNum],cStartPH + 1))
		{
			for(i = 0;i < cStartPL;i++)															//���ͻ���ָ��ָ�ֵ
			{
				lubTXBuf[lubComNum][i] = lubRXBuf[lubComNum][i];
			}

			lubTXBuf[lubComNum][cFncCode] += 0x80;												//�ظ�������+0x80

			if(lubRXBuf[lubComNum][cStartPH] != SlaveBaudrate.bit.ubBaudrate)
			{
				SlaveBaudrate.bit.ubBaudrate = lubRXBuf[lubComNum][cStartPH];
				SlaveBaudrate.bit.ubSetFlag = cTrue;
			}

			lubTXBuf[lubComNum][cStartPH] = 0;
			lubTXBuf[lubComNum][cStartPL] = cACK;												//����ACK
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
		luwCRC += lubRXBuf[lubComNum][cStartPH];												//���յ�CRCֵ

		if(luwCRC == fCrcCal8(lubRXBuf[lubComNum],cStartPL))									//У��ͨ��
		{
			for(i = 0;i < cStartPL;i++)															//���ͻ���ָ��ָ�ֵ
			{
				lubTXBuf[lubComNum][i] = lubRXBuf[lubComNum][i];
			}

			lubTXBuf[lubComNum][cFncCode] += 0x80;												//�ظ�������+0x80

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
		luwCRC += lubRXBuf[lubComNum][cStartPH + 18];											//���յ�CRCֵ

		if(luwCRC == fCrcCal8(lubRXBuf[lubComNum],cStartPH + 17))								//У��ͨ��
		{
			lubTXBuf[lubComNum][cFncCode] += 0x80;												//�ظ�������+0x80

			for(i = 0;i < 16;i++)
			{
				if(lubTXBuf[lubComNum][cStartPL + i] != Info.data.ubSerialNumber[i])
				{
					return(cFalse);																//SN����ֱ�ӷ���
				}
			}
			
			lubInverterAddr[lubComNum] = lubRXBuf[lubComNum][22];								//��õ�ַ
		}
		
		return(cFalse);																			//����ظ�
	}
	else if(lubRXBuf[lubComNum][cFncCode] == cDeleteAddrCmd)
	{
		luwCRC = lubRXBuf[lubComNum][cStartPL];
		luwCRC <<= 8;
		luwCRC += lubRXBuf[lubComNum][cStartPH];												//���յ�CRCֵ
		
		if(luwCRC == fCrcCal8(lubRXBuf[lubComNum],cStartPL))									//У��ͨ��
		{
			lubInverterAddr[lubComNum] = 0x00;
		}
		
		return(cFalse);																			//����ظ�
	}
	else																						//���������
	{
		return(cFalse);
	}

	luwCRC = fCrcCal8(lubTXBuf[lubComNum],lubPosition);
	lubTXBuf[lubComNum][lubPosition] = luwCRC >> 8;
	lubTXBuf[lubComNum][lubPosition + 1] = luwCRC;

	gubTXLength[lubComNum] = lubPosition + 2;													//���ͳ���
	if(gubTXLength[lubComNum] > cMaxTXLength)
	{
		gubTXLength[lubComNum] = cMaxTXLength;
	}

	if(lubRXBuf[lubComNum][cSlaveP] == 0xFF)													//�㲥ģʽ����Ӧ
	{
		return(cFalse);
	}
	else
	{
		return(cTrue);
	}
}

/*****************************************************************************
 * �������ƣ�Eeprom���ݸ������ú�����
 * �������룺�ޡ�
 * ��������ֵ���������������š�
 * ����������
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
 * �������ƣ����õ���ʱ������
 * �������룺�ޡ�
 * ��������ֵ����ʱʱ�䣨��λ5ms����
 * ����������һ̨���������г���һ����ע������������ʱʱ�䲻�����顣
 * ������ʱʱ���������·ݡ����ڡ������ź���ˮ�ߺţ���Ϊ��Щֵ���кܴ�ı仯
 * ������ֵ��ͬһ����վ�ܿ�����һ���ġ�
 * 		��ʱ���㷽����
 * 			��һ����ʱ����ʱ���� ��10����������ʱ������С10����������ʱ������
 * 			�ڶ�����ʱ����ʱ���� ��100����������ʱ������С100����
 * 			��������ʱ����ʱ���� ��100����������ʱ������С100����
 * 			���Ĵ���ʱ����ʱ���� ��100����������ʱ������С100����
 * 
 * 			��ʱ���� = �·� * ����������31�� * ����������35�� * ��ˮ��������1000��
 * 					+ ���� * ����������35�� * ��ˮ��������1000��
 * 					+ ������ * ��ˮ��������1000��
 * 					+ ��ˮ��
 ****************************************************************************/
INT8U sSetDelayTime(void)
{
	INT8U lubTemp;
	INT32U lulSNTemp;
	
	if(++lubDelayStep >= 4)
	{
		lubDelayStep = 0;
		
		lubTemp = Info.data.ubSerialNumber[10];				//�·ݣ�0��11
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
		
		lubTemp = Info.data.ubSerialNumber[11];				//���ڣ�0��30
		lubTemp -= 0x31;
		if(lubTemp > 9)
		{
			lubTemp -= 7;
			if(lubTemp >= 17)
			{
				lubTemp--;									//û��I
				if(lubTemp >= 22)
				{
					lubTemp--;								//û��O
					if(lubTemp >= 31)
					{
						lubTemp = 0;
					}
				}
			}
		}
		lulDataFromSN += lubTemp;
		lulDataFromSN *= 35;
		
		lubTemp = Info.data.ubSerialNumber[12];				//������
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
		
		lubTemp = Info.data.ubSerialNumber[13];				//��ˮ��
		lubTemp -= 0x31;
		if(lubTemp > 9)
		{
			lubTemp = 0;
		}
		lulDataFromSN += lubTemp;
		lulDataFromSN *= 10;
		
		lubTemp = Info.data.ubSerialNumber[14];				//��ˮ��
		lubTemp -= 0x31;
		if(lubTemp > 9)
		{
			lubTemp = 0;
		}
		lulDataFromSN += lubTemp;
		lulDataFromSN *= 10;

		lubTemp = Info.data.ubSerialNumber[15];				//��ˮ��
		lubTemp -= 0x31;
		if(lubTemp > 9)
		{
			lubTemp = 0;
		}
		lulDataFromSN += lubTemp;							//���ֵ��11*31*35*100+30*35*100+34*1000+999=13019999
	}
	else
	{
		lulSNTemp = lulDataFromSN;
		lulDataFromSN /= 50;
	}
	
	return(lulSNTemp % 50);
}

/*****************************************************************************
 * �������ƣ��ӻ���ַ���ú�����
 * �������룺�ӻ���ַ��ͨѶ�ڡ�
 * ��������ֵ���ޡ�
 * ����������
 ****************************************************************************/
void sSetSlaveCom(INT8U lubAddr,INT8U lubComNum)
{
	if(lubComNum < cComNum)
	{
		lubInverterAddr[lubComNum] = lubAddr;
	}
}

/*****************************************************************************
 * �������ƣ��ӻ���ַ��ú�����
 * �������룺ͨѶ�ڡ�
 * ��������ֵ���ӻ���ַ��
 * ����������
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

/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�Led_State.c
 * �޸��ˣ�����ǿ
 * �޸����ڣ�2019.06.20
 * ������ָʾ��״̬���ƺ���
 *******************************************************/
#include "Led_State.h"
#include "MF_Datecfg.h"
#include "HF_Datecfg.h"
INT8U INV_StateFlag;
INT16S Comm_StateFlag = 0;   //ͨѶ״̬
INT16U INV_Lastest_Err_Type;   //�������ǰ��������

void sLedStateTask(void)
{
	static INT8U i = 0;
	INVErrTypeAnalysis();
	if(Comm_StateFlag > 2)  //ͨѶ�ж�
	{
		cCommLEDOff();
	}
	else
	{
		cCommLEDOn();
	}
	if(PRJ_NUMBER == SUNNYBEE)
	{	
		switch(__REV16(MF_usCurrentRunningState))
		{
			case PowerOnMode:
				INV_StateFlag = 1;
			break;
			case WaitState:
				INV_StateFlag = 2;
			break;
			case Checking:
				INV_StateFlag = 3;
			break;
			case NormalState_N:
				INV_StateFlag = 4;
			break;
			case NormalState_L:
				INV_StateFlag = 4;
			break;
			case BoardDebugMode:
				INV_StateFlag = 5;
			break;
			case OpenLoopMode:
				INV_StateFlag = 6;
			break;
			case AdCalibrationMode:
				INV_StateFlag = 7;
			break;
			case FaultState:
				INV_StateFlag = 8;
			break;
			case Shutdown:
				INV_StateFlag = 9;
			break;
			case FlashState:
				INV_StateFlag = 10;
			break;
			default:
				break;
		}
		switch(INV_StateFlag)
		{
			case 1:
			{
				cNormalOff();
				cFaultOff();
				cCommLEDOff();
			}
				break;
			case 2:
			{
				i++;
				if(i < 5)
				{				
					cNormalOn();
					cFaultOff();
				}
				else if((i >= 5)&&(i < 10))
				{
					cNormalOff();
					cFaultOff();
				}
				else if(i == 10)
				{
					i = 0;
				}
			}
				break;
			case 3:
			{
				i++;
				if(i < 5)
				{				
					cNormalOn();
					cFaultOff();
				}
				else if((i >= 5)&&(i < 10))
				{
					cNormalOff();
					cFaultOff();
				}
				else if(i == 10)
				{
					i = 0;
				}
			}
				break;
			case 4:
			{
				cNormalOn();
				cFaultOff();
			}
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				break;
			case 8:
			{			
				switch(INV_Lastest_Err_Type)
				{
					case 1:
					{		
						i++;						
						if(i < 5)
						{				
							cNormalOff();
							cFaultOn();
						}
						else if((i >= 5)&&(i < 10))
						{
							cNormalOff();
							cFaultOff();
						}
						else if(i == 10)
						{
							i = 0;
						}
					}
					break;
					case 2:
					{
						i = !i;
						if(i == 0)
						{
							cNormalOff();
							cFaultOn();
						}
						else
						{
							cNormalOff();
							cFaultOff();
						}
					}
					break;
					case 3:
					{
						cNormalOff();
						cFaultOn();
					}
					break;
					case 4:
					{
						cNormalOff();
						cFaultOn();
					}
					break;					
				}
			}
				break;
			case 9:
			{
				i++;
				if(i < 5)
				{				
					cNormalOn();
					cFaultOff();
				}
				else if((i >= 5)&&(i < 10))
				{
					cNormalOff();
					cFaultOff();
				}
				else if(i == 10)
				{
					i = 0;
				}
			}
				break;
			case 10:
			{
				i++;
				if(i < 5)
				{				
					cNormalOn();
					cFaultOn();
				}
				else if((i >= 5)&&(i < 10))
				{
					cNormalOff();
					cFaultOff();
				}
				else if(i == 10)
				{
					i = 0;
				}
			}
				break;
			default:
			{
				if(i > 10)
					i = 0;
			}
			 break;
		}
	}
	else if(PRJ_NUMBER == HORNET)
	{
		switch(__REV16(HF_usCurrentRunningState))
		{
			case HF_WaitState:
				INV_StateFlag = 2;
			break;
			case HF_Checking:
				INV_StateFlag = 3;
			break;
			case HF_NormalState:
				INV_StateFlag = 4;
			break;
			case HF_StopState:
				INV_StateFlag = 8;
			break;
			case HF_FlashState:
				INV_StateFlag = 10;
			break;
			default:
				break;
		}
		switch(INV_StateFlag)
		{
			case 2:
			{
				i++;
				if(i < 5)
				{				
					cNormalOn();
					cFaultOn();
				}
				else if((i >= 5)&&(i < 10))
				{
					cNormalOff();
					cFaultOff();
				}
				else if(i == 10)
				{
					i = 0;
				}
			}
				break;
			case 3:
			{
				i++;
				if(i < 5)
				{				
					cNormalOn();
					cFaultOn();
				}
				else if((i >= 5)&&(i < 10))
				{
					cNormalOff();
					cFaultOff();
				}
				else if(i == 10)
				{
					i = 0;
				}
			}
				break;
			case 4:
			{
				cNormalOn();
				cFaultOff();
			}
				break;
			case 8:
			{
				switch(INV_Lastest_Err_Type)
				{
					case 1:
					{		
						i++;						
						if(i < 5)
						{				
							cNormalOff();
							cFaultOn();
						}
						else if((i >= 5)&&(i < 10))
						{
							cNormalOff();
							cFaultOff();
						}
						else if(i == 10)
						{
							i = 0;
						}
					}
					break;
					case 2:
					{
						i = !i;
						if(i == 0)
						{
							cNormalOff();
							cFaultOn();
						}
						else
						{
							cNormalOff();
							cFaultOff();
						}
					}
					break;
					case 3:
					{
						cNormalOff();
						cFaultOn();
					}
					break;
					case 4:
					{
						cNormalOn();
						cFaultOn();
					}
					break;					
				}
			}
				break;
			case 10:
			{
				i++;
				if(i < 5)
				{				
					cNormalOn();
					cFaultOn();
				}
				else if((i >= 5)&&(i < 10))
				{
					cNormalOff();
					cFaultOff();
				}
				else if(i == 10)
				{
					i = 0;
				}
			}
				break;
			default:
			{
				if(i > 10)
					i = 0;
			}
				break;
		}
	}
}

void INVErrTypeAnalysis(void)
{
	if(PRJ_NUMBER == SUNNYBEE)
	{
		if((__REV16(MF_EventAlarmInfo) >= 0x0100)&&(__REV16(MF_EventAlarmInfo) <= 0x0106))
		{
			INV_Lastest_Err_Type = 1;
		}
		else if((__REV16(MF_EventAlarmInfo) >= 0x0200)&&(__REV16(MF_EventAlarmInfo) <= 0x0218))
		{
			INV_Lastest_Err_Type = 2;
		}
		else if((__REV16(MF_EventAlarmInfo) >= 0x0300)&&(__REV16(MF_EventAlarmInfo) <= 0x0529))
		{
			INV_Lastest_Err_Type = 3;
		}
		else if(__REV16(MF_EventAlarmInfo) == 0x0530)
		{
			INV_Lastest_Err_Type = 4;
		}
	}
	else if(PRJ_NUMBER == HORNET)
	{
		INV_Lastest_Err_Type = __REV16(HF_strLatestAlmDisplayInfo.ALARM_TYPE);
	}
}

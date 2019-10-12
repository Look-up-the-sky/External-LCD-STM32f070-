/********************************************************
 * ��Ȩ������ŷķ�������Դ�Ƽ����޹�˾�����
 * �ļ����ƣ�Main.c
 * �޸��ˣ�����ǿ
 * �޸����ڣ�2019.06.20
 * ������Ӧ�ó���������
 * V1.0.6 ���ӻƷ���͵�֧��
 * V1.0.7 ���ӻƷ�������Լ�
		  �������������״̬��ʾ
		  ȡ������2���������ܣ��ϲ�������1
		  ����Զ�̹ػ���Ļ��ʾ
 * V1.0.8 ������Լ�ʱֻ����������Լ����ݺ���������ݵ�ͨѶ
 * V1.0.9 �޸���ʾbug
 *******************************************************/

/********************************************************************
 * Include Files
 ********************************************************************/
#include "App.h"
#include "Lib_task.h"
#include "modbus.h"
#include "Display.h"
#include "Initial.h"
#include "I2C.h"
#include "Led_State.h"
#include "Variable_Transformation.h"


INT32U CNT_1MS;
/********************************************************************
 * Macros
 ********************************************************************/
/**********OS Time***********/
#define cOS0ms		0
#define cOS1ms		1
#define cOS2ms		2
#define cOS3ms		3
#define cOS4ms		4
#define cOS5ms		5
#define cOS10ms		10
#define cOS20ms		20
#define cOS50ms		20
#define cOS100ms	100
#define cOS200ms	200
#define cOS300ms	300
#define cOS400ms	400
#define cOS1s		1000
#define cOS2s		2000
/*****************************************************************************
 * �������ƣ�Ӧ�ó�����������
 * �������룺�ޡ�
 * ��������ֵ���ޡ�
 * ��������������MCU��ʼ�����������������ʼ��������ִ������
 ****************************************************************************/
int main(void)
{
	memcpy((void*)0x20000000, (void*)0x08002800, 0x100);  //�ж�������ת�Ƶ�ram��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);	
	SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
	__set_PRIMASK(0);
	/************��ʼ��MCU************/
	sInitMCU();
	/************��ʼ���������ϵͳ************/
	sInitTaskAdr(cLedStateTask,sLedStateTask);
	sInitTaskPeriod(cLedStateTask,cOS200ms,cOS0ms);
	sInitTaskAdr(cModbusTask,sModbusTask);
	sInitTaskPeriod(cModbusTask,cOS400ms,cOS2ms);
	sInitTaskAdr(cVariableTask,sVariableTask);
	sInitTaskPeriod(cVariableTask,cOS300ms,cOS10ms);
	sInitTaskAdr(cI2CTask,sI2CTask);
	sInitTaskPeriod(cI2CTask,cOS100ms,cOS2ms);
	sInitTaskAdr(cDisplayTask,sDisplayTask);
	sInitTaskPeriod(cDisplayTask,cOS10ms,cOS4ms);


	/************��ʼ����************/
	sTaskStart();
	
}

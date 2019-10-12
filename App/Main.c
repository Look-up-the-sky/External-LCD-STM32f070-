/********************************************************
 * 版权：苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Main.c
 * 修改人：沈仕强
 * 修改日期：2019.06.20
 * 描述：应用程序主函数
 * V1.0.6 增加黄蜂机型的支持
 * V1.0.7 增加黄蜂意大利自检
		  增加逆变器升级状态显示
		  取消串口2的升级功能，合并到串口1
		  增加远程关机屏幕显示
 * V1.0.8 意大利自检时只进行意大利自检数据和逆变器数据的通讯
 * V1.0.9 修复显示bug
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
 * 函数名称：应用程序主函数。
 * 函数输入：无。
 * 函数返回值：无。
 * 函数描述：调用MCU初始化函数，调用任务初始化函数，执行任务。
 ****************************************************************************/
int main(void)
{
	memcpy((void*)0x20000000, (void*)0x08002800, 0x100);  //中断向量表转移到ram中
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);	
	SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
	__set_PRIMASK(0);
	/************初始化MCU************/
	sInitMCU();
	/************初始化任务调度系统************/
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


	/************开始任务************/
	sTaskStart();
	
}

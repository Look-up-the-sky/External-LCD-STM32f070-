#ifndef _PCF8563_H_
#define _PCF8563_H_

#include "stm32f0xx.h"

#define Second_Addr				0x02
#define Minute_Addr				0x03
#define Hour_Addr				0x04
#define Date_Addr				0x05
#define Month_Addr				0x07
#define Year_Addr				0x08

#define MaxWaitTimes			0xA000

INT8U sBcdToHex(INT8U tBcd);
INT8U sHexToBcd(INT8U tHex);
void sInitPcf8563(void);
unsigned char I2c_Pcf8563write(unsigned char Addr,unsigned char Data);
unsigned char I2c_Pcf8563read(unsigned char Addr);
void sWritePcf8563(INT8U *lubData,INT16U luwI2CAddr,INT8U lubDataLen);
void sReadPcf8563(INT8U* lubData);

#endif

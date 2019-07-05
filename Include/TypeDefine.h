#ifndef _TYPEDEFINE_H_
#define _TYPEDEFINE_H_
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "stm32f0xx.h"
/*******************user headfile************************/
/************************************************************
 * MDK的char为unsigned还是signed取决于MDK的设置，具体操作如下：
 * 		项目右键->Option for target->C/C++->"Plain Char is Signed"
 * 		选中char就是 signed char，不选就是unsigned char
 * 		本项目中要求不选中。
 ************************************************************/
#define INT8U	 char
#define INT8S	signed char
#define INT16U	unsigned short
#define INT16S	signed short
#define INT32U	unsigned int
#define INT32S	signed int
/*********************************************************************************************************
  通用数据类型重定义
*********************************************************************************************************/
typedef unsigned char       BOOLEAN;                                    /* 布尔变量                     */
typedef unsigned long long  INT64U;                                     /* 无符号64位整型变量           */
typedef signed long long    INT64S;                                     /* 有符号64位整型变量           */
typedef float               FP32;                                       /* 单精度浮点数（32位长度）     */
typedef double              FP64;                                       /* 双精度浮点数（64位长度）     */

typedef unsigned char       uint8;                                      /* 无符号8位整型变量            */
typedef unsigned short int  uint16;                                     /* 无符号16位整型变量           */
typedef unsigned int   		  uint32;                                     /* 无符号32位整型变量           */

typedef char                int8;                                       /* 有符号8位整型变量            */
typedef short int           int16;                                      /* 有符号16位整型变量           */
typedef int                 int32;                                      /* 有符号32位整型变量           */

typedef volatile int8       vint8;                                      /*  8 bits                      */
typedef volatile int16      vint16;                                     /* 16 bits                      */
typedef volatile int32      vint32;                                     /* 32 bits                      */

typedef volatile uint8      vuint8;                                     /*  8 bits                      */
typedef volatile uint16     vuint16;                                    /* 16 bits                      */
typedef volatile uint32     vuint32;                                    /* 32 bits                      */

/*******************************************globle****************************************/

#endif

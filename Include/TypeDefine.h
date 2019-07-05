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
 * MDK��charΪunsigned����signedȡ����MDK�����ã�����������£�
 * 		��Ŀ�Ҽ�->Option for target->C/C++->"Plain Char is Signed"
 * 		ѡ��char���� signed char����ѡ����unsigned char
 * 		����Ŀ��Ҫ��ѡ�С�
 ************************************************************/
#define INT8U	 char
#define INT8S	signed char
#define INT16U	unsigned short
#define INT16S	signed short
#define INT32U	unsigned int
#define INT32S	signed int
/*********************************************************************************************************
  ͨ�����������ض���
*********************************************************************************************************/
typedef unsigned char       BOOLEAN;                                    /* ��������                     */
typedef unsigned long long  INT64U;                                     /* �޷���64λ���ͱ���           */
typedef signed long long    INT64S;                                     /* �з���64λ���ͱ���           */
typedef float               FP32;                                       /* �����ȸ�������32λ���ȣ�     */
typedef double              FP64;                                       /* ˫���ȸ�������64λ���ȣ�     */

typedef unsigned char       uint8;                                      /* �޷���8λ���ͱ���            */
typedef unsigned short int  uint16;                                     /* �޷���16λ���ͱ���           */
typedef unsigned int   		  uint32;                                     /* �޷���32λ���ͱ���           */

typedef char                int8;                                       /* �з���8λ���ͱ���            */
typedef short int           int16;                                      /* �з���16λ���ͱ���           */
typedef int                 int32;                                      /* �з���32λ���ͱ���           */

typedef volatile int8       vint8;                                      /*  8 bits                      */
typedef volatile int16      vint16;                                     /* 16 bits                      */
typedef volatile int32      vint32;                                     /* 32 bits                      */

typedef volatile uint8      vuint8;                                     /*  8 bits                      */
typedef volatile uint16     vuint16;                                    /* 16 bits                      */
typedef volatile uint32     vuint32;                                    /* 32 bits                      */

/*******************************************globle****************************************/

#endif

#ifndef    _DEBUG_H
#define    _DEBUG_H

#include <stdio.h>
#include "stm32f0xx.h"

int fputc(int ch, FILE *f);
void USART2_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
void sDebugTask(void);
#endif

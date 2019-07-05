#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "stm32f0xx.h"

#define SCL_L()                 GPIOB->BRR  = GPIO_Pin_6 
#define SCL_H()                 GPIOB->BSRR = GPIO_Pin_6 

#define SDA_L()                 GPIOB->BRR  = GPIO_Pin_7 
#define SDA_H()                 GPIOB->BSRR = GPIO_Pin_7 

#define SDA_out()               {GPIOB->MODER &= 0xFFFF3FFF;GPIOB->MODER |= 0x00004000;} 
#define SDA_in()                {GPIOB->MODER &= 0xFFFF3FFF;GPIOB->MODER |= 0x00000000;}

#define SDA_High                (GPIOB->IDR&GPIO_Pin_7)

void I2c_delay(unsigned int t);
void I2c_delayms(unsigned char t);
unsigned char I2c_wait_ack(void);
void I2c_stop(void);
void I2c_start(void);
void I2c_no_wait_ack(void);
void I2c_send_ack(void);
void I2C_send_no_ack(void);
void I2c_wr8bit(unsigned char c);
unsigned char I2c_rd8bit(void);
unsigned char I2c_read(unsigned char Addr,unsigned char* Data,unsigned char DeviceID);
unsigned char I2c_write(unsigned char Addr,unsigned char* Data,unsigned char DeviceID);
void sReadI2C(INT8U* lubData,INT16U luwI2CAddr,INT8U lubDataLen);
void sWriteI2C(INT8U *lubData,INT16U luwI2CAddr,INT8U lubDataLen);

#endif

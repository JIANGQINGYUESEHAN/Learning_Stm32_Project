#ifndef __DRIVE_USART_H__
#define __DRIVE_USART_H__
#include "stdio.h"
#include "stm32f10x.h"

#define USART1_BAUD_RATE (921600)
#define USART3_BAUD_RATE (115200)
void USART1_Init(void);
void USART3_Init(void);
void USART3_WriteByte(uint8_t byte);
#endif /*__DRIVE_USART_H__*/
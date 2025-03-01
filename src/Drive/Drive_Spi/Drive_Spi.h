#ifndef __DRIVE_SPI_H__
#define __DRIVE_SPI_H__
#include "Delay.h"
#include "stm32f10x.h"
// Configure MOSI (PA7)
#define MOSI_HIGH (GPIOA->ODR |= GPIO_ODR_ODR7)
#define MOSI_LOW (GPIOA->ODR &= ~GPIO_ODR_ODR7)
// SCK (PA5)
#define SCK_HIGH (GPIOA->ODR |= GPIO_ODR_ODR5)
#define SCK_LOW (GPIOA->ODR &= ~GPIO_ODR_ODR5)
// Configure NSS (PA4)
#define CS_HIGH (GPIOA->ODR |= GPIO_ODR_ODR4)
#define CS_LOW (GPIOA->ODR &= ~GPIO_ODR_ODR4)
// Configure MISO (PA6)
#define MISO_READ (GPIOA->IDR & GPIO_IDR_IDR6)

#define SPI_DELAY Delay_us(5)
void Driver_SPI_Init(void);

void Driver_SPI_Start(void);

void Driver_SPI_Stop(void);

uint8_t Driver_SPI_SwapByte(uint8_t byte);
#endif /*__DRIVE_SPI_H__*/
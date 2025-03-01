#ifndef __DRIVE_IIC_H__
#define __DRIVE_IIC_H__

#include "Delay.h"
#include "stm32f10x.h"
#define ACK1 0
#define NACK1 1
#define SCL1_HIGH (GPIOB->ODR |= GPIO_ODR_ODR6)
#define SCL1_LOW (GPIOB->ODR &= ~GPIO_ODR_ODR6)
#define SDA1_HIGH (GPIOB->ODR |= GPIO_ODR_ODR7)
#define SDA1_LOW (GPIOB->ODR &= ~GPIO_ODR_ODR7)
#define REDA1_SDA (GPIOB->IDR & GPIO_IDR_IDR6)

#define IIC2_SDA(BitVal) GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitVal);
#define IIC2_SCL(BitVal) GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitVal);
#define READ_SDA2 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);

#define IIC_DELAY Delay_ms(10)

void Drive_I2C1_Init(void);
void Drive_I2C1_Start(void);
void Drive_I2C1_Stop(void);
void Drive_I2C1_Ack(void);
void Drive_I2C1_NAck(void);
uint8_t Drive_I2C1_WaitAck(void);
void Drive_I2C1_SendByte(uint8_t byte);
uint8_t Drive_I2C1_ReadByte(void);

void Drive_I2C2_Init(void);
int Drive_I2C2_write(uint8_t slave_addr, uint8_t reg_addr, uint8_t length,
                     uint8_t const *data);
int Drive_I2C2_read(uint8_t slave_addr, uint8_t reg_addr, uint8_t length,
                    uint8_t *data);
#endif /*__DRIVE_IIC_H__*/
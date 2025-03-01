#include "Drive_Spi.h"

void Driver_SPI_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    // Configure MOSI (PA7)
    // SCK (PA5)
    // Configure NSS (PA4) 都是复用推挽输出
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5 | GPIO_Pin_4;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure MISO (PA6)
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    /* 初始状态：片选默认不选中 SCK拉低 */
    SCK_LOW;
    CS_HIGH;
    SPI_DELAY;
}

void Driver_SPI_Start(void) { CS_HIGH; }

void Driver_SPI_Stop(void) { CS_LOW; }

uint8_t Driver_SPI_SwapByte(uint8_t byte) {
    uint8_t rbyte = 0x00;
    for (uint8_t i = 0; i < 8; i++) {
        if (byte & 0x80) {
            MOSI_HIGH;
        } else {
            MOSI_LOW;
        }
        SCK_HIGH;
        rbyte <<= 1;
        if (MISO_READ) {
            rbyte |= 0x01;
        }
        SCK_LOW;
    }
    return rbyte;
}
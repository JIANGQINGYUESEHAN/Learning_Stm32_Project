#include "Drive_IIC.h"
void Drive_I2C1_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    /* 设置 SDA 开漏输出 */
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD; // 复用开漏输入输出
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void Drive_I2C1_Start(void) {
    SDA1_HIGH;
    SCL1_HIGH;
    IIC_DELAY;
    SDA1_LOW;
    IIC_DELAY;
}
void Drive_I2C1_Stop(void) {
    SDA1_LOW;
    SCL1_HIGH;
    IIC_DELAY;
    SDA1_HIGH;
}
void Drive_I2C1_Ack(void) {
    SDA1_HIGH;
    SCL1_LOW;
    IIC_DELAY;

    SDA1_LOW;
    IIC_DELAY;

    SCL1_HIGH;
    IIC_DELAY;

    SDA1_HIGH;
    IIC_DELAY;

    SCL1_LOW;
    IIC_DELAY;
}
// 收方产生非应答信号 :代表从机把数据传到主机？
void Drive_I2C1_NAck(void) {
    SDA1_HIGH;
    SCL1_LOW;
    IIC_DELAY;
    SCL1_HIGH;
    IIC_DELAY;
    SCL1_LOW;
    IIC_DELAY;
}
// 主设备等待从数据发送信息
uint8_t Drive_I2C1_WaitAck(void) {
    // SDA拉高，控制权教程
    SDA1_HIGH;
    SCL1_LOW;
    IIC_DELAY;
    SCL1_HIGH;
    IIC_DELAY;
    uint8_t ack = 0x00;
    if (REDA1_SDA) {
        ack = SDA1_HIGH;
    }
    SCL1_LOW;
    IIC_DELAY;
    return ack;
}
void Drive_I2C1_SendByte(uint8_t byte) {
    for (uint8_t i = 0; i < 8; i++) {
        SDA1_LOW;
        SCL1_LOW;
        IIC_DELAY;

        if (byte & 0x80) {
            SDA1_HIGH;
        } else {
            SDA1_LOW;
        }
        SCL1_HIGH;

        IIC_DELAY;

        /* 4. 时钟拉低 */
        SCL1_LOW;

        IIC_DELAY;
        byte <<= 1;
    }
}
uint8_t Drive_I2C1_ReadByte(void) {
    uint8_t rbyte = 0x00;
    for (uint8_t i = 0; i < 8; i++) {
        SCL1_LOW;
        IIC_DELAY;
        SCL1_HIGH;
        IIC_DELAY;
        rbyte <<= 1;
        if (REDA1_SDA) {
            rbyte |= 0x01;
        }
        SCL1_LOW;

        /* 7. 延时 */
        IIC_DELAY;
    }
    return rbyte;
}

void Drive_I2C2_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    /* 设置 SDA 开漏输出 */
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD; // 复用开漏输入输出
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    IIC2_SDA(1);
    IIC2_SCL(1);
}
static void IIC2_W_SAD(BitAction BitVal) {
    IIC2_SDA(BitVal);
    IIC_DELAY;
}
static void IIC2_W_SCL(BitAction BitVal) {
    IIC2_SCL(BitVal);
    IIC_DELAY;
}
static uint8_t IIC2_R_SDA() {
    uint8_t BitVal = READ_SDA2;

    return BitVal;
}
static void IIC2_Start() {

    IIC2_SDA(1);
    IIC2_SCL(1);
    IIC_DELAY;
    IIC2_SDA(0);
    IIC_DELAY;
}

static void IIC2_Stop() {

    IIC2_SDA(0);
    IIC2_SCL(1);
    IIC_DELAY;
    IIC2_SDA(1);
}
static void IIC2_Send_Ack(uint8_t Ack) {
    IIC2_W_SAD(Ack);
    IIC2_SCL(1);
    IIC2_SCL(0);
    IIC_DELAY;
}

static uint8_t IIC2_Receive_Ack() {
    uint8_t ACK = 0;
    IIC2_SCL(1);
    IIC2_SDA(1);

    ACK = IIC2_R_SDA();
    IIC2_SCL(0);
    IIC_DELAY;
    return ACK;
}

static void IIC2_Send_Byte(uint8_t SendData) {
    for (uint8_t i = 0; i < 8; i++) {
        IIC2_W_SAD(SendData & (0x80 >> i));
        IIC2_W_SCL(1);
        IIC2_W_SCL(0);
    }
}

static uint8_t IIC2_Receive_Byte(void) {

    uint8_t rbyte = 0x00;
    IIC2_W_SAD(1);
    for (uint8_t i = 0; i < 8; i++) {
        IIC2_W_SCL(1);
        rbyte <<= 1;
        if (IIC2_R_SDA() == 1) {
            rbyte |= 0x01;
        }
        IIC2_W_SCL(0);
    }

    return rbyte;
}
int Drive_I2C2_write(uint8_t slave_addr, uint8_t reg_addr, uint8_t length,
                     uint8_t const *data) {
    int i = 0;
    IIC2_Start();
    IIC2_Send_Byte(slave_addr << 1);
    IIC2_Receive_Ack();
    IIC2_Send_Byte(reg_addr);
    IIC2_Receive_Ack();
    for (i = 0; i < length; i++) {
        IIC2_Send_Byte(data[i]);
        IIC2_Receive_Ack();
    }
    IIC2_Stop();
    return 0;
}
int Drive_I2C2_read(uint8_t slave_addr, uint8_t reg_addr, uint8_t length,
                    uint8_t *data) {
    IIC2_Start();
    IIC2_Send_Byte(slave_addr << 1);
    IIC2_Receive_Ack();
    IIC2_Send_Byte(reg_addr);
    IIC2_Receive_Ack();

    IIC2_Start();
    IIC2_Send_Byte((slave_addr << 1) + 1);
    IIC2_Receive_Ack();
    while (length) {
        if (length == 1) {
            *data = IIC2_Receive_Byte();
            IIC2_Send_Ack(1);
        } else {
            *data = IIC2_Receive_Byte();
            IIC2_Send_Ack(0);
        }
        data++;
        length--;
    }

    IIC2_Stop();
    return 0;
}
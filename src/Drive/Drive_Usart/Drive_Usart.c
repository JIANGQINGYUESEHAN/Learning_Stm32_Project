#include "Drive_Usart.h"

void USART1_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 |
                               RCC_APB2Periph_AFIO,
                           ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    /* Gpio初始化 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; // 推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    /* USART配置 */
    USART_InitStruct.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None; // 无控制
    USART_InitStruct.USART_BaudRate = USART1_BAUD_RATE;
    USART_InitStruct.USART_Parity = USART_Parity_No;             // 无校验位
    USART_InitStruct.USART_StopBits = USART_StopBits_1;          // 一位停止位
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;     // 每次传输8bit
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // tx,rx使能
    USART_Init(USART1, &USART_InitStruct);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 串口中断使能

    /*
      NVIC配置
    中断优先级的分组模式设置为 NVIC_PriorityGroup_1。在这个分组模式下：
    抢占优先级占用 2 位。 子优先级占用 2 位。
    */

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;        //// 设置子优先级
    NVIC_Init(&NVIC_InitStruct);
    printf("UASRT1_INIT");
}

void Drive_USART_Sendchar(uint8_t ch) {
    // 当发送寄存为空时将要发送的值放到发送寄存中，然后发送

    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == 0)
        ;
    // USART1->DR = ch;
    USART_SendData(USART1, ch);
}
int fputc(int c, FILE *file) {
    Drive_USART_Sendchar(c);
    return c;
}

void USART3_Init(void) {

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    /* GPIO的配置 TX */

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出

    /* RX 输入 */

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入

    GPIO_Init(GPIOB, &GPIO_InitStruct);
    /* USART配置 */
    USART_InitStruct.USART_BaudRate = USART3_BAUD_RATE;
    USART_InitStruct.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None; // 无控制输入输出
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;    // 无校验位
    USART_InitStruct.USART_StopBits = USART_StopBits_1; // 一位停止位
    // 开启中断
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    // 串口使能
    USART_Cmd(USART3, ENABLE);
    USART_Init(USART3, &USART_InitStruct);

    /* NVIC配置 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct);
    printf("UASRT3_INIT");
}

void USART3_WriteByte(uint8_t byte) {}

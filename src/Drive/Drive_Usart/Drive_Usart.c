#include "Drive_Usart.h"

void USART1_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 |
                               RCC_APB2Periph_AFIO,
                           ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    /* Gpio��ʼ�� */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // ��������

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; // �������
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    /* USART���� */
    USART_InitStruct.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None; // �޿���
    USART_InitStruct.USART_BaudRate = USART1_BAUD_RATE;
    USART_InitStruct.USART_Parity = USART_Parity_No;             // ��У��λ
    USART_InitStruct.USART_StopBits = USART_StopBits_1;          // һλֹͣλ
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;     // ÿ�δ���8bit
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // tx,rxʹ��
    USART_Init(USART1, &USART_InitStruct);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // �����ж�ʹ��

    /*
      NVIC����
    �ж����ȼ��ķ���ģʽ����Ϊ NVIC_PriorityGroup_1�����������ģʽ�£�
    ��ռ���ȼ�ռ�� 2 λ�� �����ȼ�ռ�� 2 λ��
    */

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; // ��ռ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;        //// ���������ȼ�
    NVIC_Init(&NVIC_InitStruct);
    printf("UASRT1_INIT");
}

void Drive_USART_Sendchar(uint8_t ch) {
    // �����ͼĴ�Ϊ��ʱ��Ҫ���͵�ֵ�ŵ����ͼĴ��У�Ȼ����

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

    /* GPIO������ TX */

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; // �������

    /* RX ���� */

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // ��������

    GPIO_Init(GPIOB, &GPIO_InitStruct);
    /* USART���� */
    USART_InitStruct.USART_BaudRate = USART3_BAUD_RATE;
    USART_InitStruct.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None; // �޿����������
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;    // ��У��λ
    USART_InitStruct.USART_StopBits = USART_StopBits_1; // һλֹͣλ
    // �����ж�
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    // ����ʹ��
    USART_Cmd(USART3, ENABLE);
    USART_Init(USART3, &USART_InitStruct);

    /* NVIC���� */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // ������ռ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct);
    printf("UASRT3_INIT");
}

void USART3_WriteByte(uint8_t byte) {}

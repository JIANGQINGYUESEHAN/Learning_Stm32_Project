#include "Drive_Key.h"
#define TIMER_FOR_BUTTON_ENABLE TIM_Cmd(TIME_FOR_BUTTON, ENABLE)   // ʹ��
#define TIMER_FOR_BUTTON_DISABLE TIM_Cmd(TIME_FOR_BUTTON, DISABLE) // ʧ��

eBUTTON_Struct BUTTON;
void BUTTON_Status_Clear() { BUTTON.status = BUTTON_IDLE; }
/*  ��ʼ������ */
void Drive_Key_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_GPIO_FOR_BUTTON, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_TIM_FOR_BUTTON, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    /* button */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = BUTTON_GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // ����
    GPIO_Init(BUTTON_GPIO, &GPIO_InitStruct);
    /* EXTI */
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger =
        EXTI_Trigger_Rising_Falling;       // �����غ��½��ض��ܴ����ж�
    EXTI_InitStruct.EXTI_LineCmd = ENABLE; // ʹ��

    EXTI_Init(&EXTI_InitStruct);

    /* NVIC */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_Init(&NVIC_InitStruct);
    /* TIM4���� */
    /* ���ò���Ƶ */
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    /* ���ü������һ���жϡ� */
    TIM_TimeBaseInitStruct.TIM_Prescaler = 720 - 1;
    TIM_TimeBaseInitStruct.TIM_Period = 100 - 1; // 1ms����һ���ж�
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���
    TIM_TimeBaseInit(TIME_FOR_BUTTON, &TIM_TimeBaseInitStruct);
    TIM_ClearFlag(TIME_FOR_BUTTON, TIM_FLAG_Update); /* ֱ�Ӳ���һ�������¼� */
    /* ����TIM4���ж� */
    NVIC_InitStruct.NVIC_IRQChannel = TIM_FOR_BUTTON_IRQN;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 4;
    NVIC_Init(&NVIC_InitStruct);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    BUTTON.status_clear = &BUTTON_Status_Clear;
}

/* �жϺ��� */
void EXTI0_IRQHandler(void) { /*������ж� */
    if (EXTI_GetITStatus(EXTI_Line0) == SET) {
        // ������EXTI�ж�ʱ��buttonΪ����ʱ��������ʱ����
        if (BUTTON.status == BUTTON_IDLE) {
            TIMER_FOR_BUTTON_ENABLE; // ��������ж�ʱ���ĵ��ж�
        }
    }

    EXTI_ClearITPendingBit(EXTI_Line0);
}
static volatile uint16_t time_release_count_ms = 0;
static volatile uint16_t time_hold_count_ms = 0;
static volatile uint8_t previous_status = 0;
void TIM4_IRQHandler(void) {
    if (TIM_GetITStatus(TIME_FOR_BUTTON, TIM_IT_Update) == SET) {
        // ��ȡ��ǰGPIO���ŵ�״̬����¼
        uint8_t Current_status =
            GPIO_ReadOutputDataBit(BUTTON_GPIO, BUTTON_GPIO_PIN);
        /* ��ʼ��previous_status ���жԱ� */
        if (previous_status == Current_status && Current_status == 0) {
            // ���ڵļ�¼��֮ǰ��ͬ������0 ��ʾ�Ѿ�����
            time_hold_count_ms++; // ���ݼ�����������ӻ�ȡ����ֵ
            if (time_hold_count_ms > 5000) {
                // ��¼��ֵ�����趨�����ֵ��ôֱ�Ӹ����Լ����趨��ֵ
                BUTTON.status = BUTTON_HOLD_LONG;
            }
        }
        // ����ť�ɿ�ʱ����ʱ�ɿ�ʱ�䡱��
        else if (previous_status == Current_status && Current_status == 1) {
            time_release_count_ms++; // ����״̬++
            /* ���ж��Ѿ����µ�ʱ�� �ж��ǳ������Ƕ̰�*/
            if (time_hold_count_ms > BUTTON_SHORT_LONG_THRESHOLD_MS) {
                // �Ѿ�������е���Сʱ����˵���ǳ���
                BUTTON.status = BUTTON_HOLD_LONG;
            } else if (time_hold_count_ms >= BUTTON_IDLE_SHORT_THRESHOLD_MS) {
                BUTTON.status = BUTTON_HOLD;
            }
            if (time_release_count_ms >= BUTTON_IDLE_SHORT_THRESHOLD_MS) {
                // ��ʾ�Ͱ�һ�¾ͽ����ˣ�����״̬�ĸ���

                time_hold_count_ms = 0;
                time_release_count_ms = 0;
                /* �رն�ʱ�� */
                TIMER_FOR_BUTTON_DISABLE;
            }
        }
        // ����״̬�ĸ���
        previous_status = Current_status;
        // �����ʱ��
        TIM_ClearITPendingBit(TIME_FOR_BUTTON, TIM_IT_Update);
    }
}
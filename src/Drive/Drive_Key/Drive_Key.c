#include "Drive_Key.h"
#define TIMER_FOR_BUTTON_ENABLE TIM_Cmd(TIME_FOR_BUTTON, ENABLE)   // 使能
#define TIMER_FOR_BUTTON_DISABLE TIM_Cmd(TIME_FOR_BUTTON, DISABLE) // 失能

eBUTTON_Struct BUTTON;
void BUTTON_Status_Clear() { BUTTON.status = BUTTON_IDLE; }
/*  初始化函数 */
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
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // 输入
    GPIO_Init(BUTTON_GPIO, &GPIO_InitStruct);
    /* EXTI */
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger =
        EXTI_Trigger_Rising_Falling;       // 上升沿和下降沿都能触发中断
    EXTI_InitStruct.EXTI_LineCmd = ENABLE; // 使能

    EXTI_Init(&EXTI_InitStruct);

    /* NVIC */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_Init(&NVIC_InitStruct);
    /* TIM4配置 */
    /* 配置不分频 */
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    /* 配置几秒产生一个中断· */
    TIM_TimeBaseInitStruct.TIM_Prescaler = 720 - 1;
    TIM_TimeBaseInitStruct.TIM_Period = 100 - 1; // 1ms产生一个中断
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数
    TIM_TimeBaseInit(TIME_FOR_BUTTON, &TIM_TimeBaseInitStruct);
    TIM_ClearFlag(TIME_FOR_BUTTON, TIM_FLAG_Update); /* 直接产生一个更新事件 */
    /* 配置TIM4的中断 */
    NVIC_InitStruct.NVIC_IRQChannel = TIM_FOR_BUTTON_IRQN;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 4;
    NVIC_Init(&NVIC_InitStruct);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    BUTTON.status_clear = &BUTTON_Status_Clear;
}

/* 中断函数 */
void EXTI0_IRQHandler(void) { /*先清除中断 */
    if (EXTI_GetITStatus(EXTI_Line0) == SET) {
        // 当进入EXTI中断时，button为空闲时，开启定时器四
        if (BUTTON.status == BUTTON_IDLE) {
            TIMER_FOR_BUTTON_ENABLE; // 开启后进行定时器四的中断
        }
    }

    EXTI_ClearITPendingBit(EXTI_Line0);
}
static volatile uint16_t time_release_count_ms = 0;
static volatile uint16_t time_hold_count_ms = 0;
static volatile uint8_t previous_status = 0;
void TIM4_IRQHandler(void) {
    if (TIM_GetITStatus(TIME_FOR_BUTTON, TIM_IT_Update) == SET) {
        // 获取当前GPIO引脚的状态并记录
        uint8_t Current_status =
            GPIO_ReadOutputDataBit(BUTTON_GPIO, BUTTON_GPIO_PIN);
        /* 开始与previous_status 进行对比 */
        if (previous_status == Current_status && Current_status == 0) {
            // 现在的记录和之前相同都等于0 表示已经按下
            time_hold_count_ms++; // 根据计数器进行相加获取具体值
            if (time_hold_count_ms > 5000) {
                // 记录的值大于设定的最大值那么直接给个自己的设定的值
                BUTTON.status = BUTTON_HOLD_LONG;
            }
        }
        // “按钮松开时，计时松开时间”。
        else if (previous_status == Current_status && Current_status == 1) {
            time_release_count_ms++; // 空闲状态++
            /* 在判断已经按下的时间 判断是长按还是短按*/
            if (time_hold_count_ms > BUTTON_SHORT_LONG_THRESHOLD_MS) {
                // 已经大过空闲的最小时间了说明是长按
                BUTTON.status = BUTTON_HOLD_LONG;
            } else if (time_hold_count_ms >= BUTTON_IDLE_SHORT_THRESHOLD_MS) {
                BUTTON.status = BUTTON_HOLD;
            }
            if (time_release_count_ms >= BUTTON_IDLE_SHORT_THRESHOLD_MS) {
                // 表示就按一下就结束了，进行状态的更新

                time_hold_count_ms = 0;
                time_release_count_ms = 0;
                /* 关闭定时器 */
                TIMER_FOR_BUTTON_DISABLE;
            }
        }
        // 进行状态的更新
        previous_status = Current_status;
        // 清除定时器
        TIM_ClearITPendingBit(TIME_FOR_BUTTON, TIM_IT_Update);
    }
}
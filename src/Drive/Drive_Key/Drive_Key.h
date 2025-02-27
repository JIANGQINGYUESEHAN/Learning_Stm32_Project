#ifndef __DRIVE_KEY_H__
#define __DRIVE_KEY_H__

#include "stdint.h"
#include "stm32f10x.h"
typedef enum eBUTTON_STATUS {
    BUTTON_IDLE = 0,     // 空闲状态
    BUTTON_RELEASE = 1,  // 释放状态
    BUTTON_HOLD = 2,     // 普通按下
    BUTTON_HOLD_LONG = 3 // 设置为长按状态
} eBUTTON_STATUS;

typedef struct eBUTTON_Struct {
    eBUTTON_STATUS status;
    void (*status_clear)(void);
} eBUTTON_Struct;

#define RCC_TIM_FOR_BUTTON RCC_APB1Periph_TIM4
#define RCC_GPIO_FOR_BUTTON RCC_APB2Periph_GPIOA // 按键用到的时钟GPIO

#define BUTTON_GPIO GPIOA // 按键用到的GPIO

#define BUTTON_GPIO_PIN GPIO_Pin_0 // 按键用到的GPIO

#define TIME_FOR_BUTTON TIM4 // 按键用到的时钟

#define TIM_FOR_BUTTON_IRQN TIM4_IRQn // 按键用的时钟

#define TIM4_PER_INTERRUPT_MS (1) // 定时器 4 每次触发中断的时间间隔为 1 毫秒。

#define BUTTON_IDLE_SHORT_THRESHOLD_MS                                         \
    (10) // 按钮处于 空闲状态 或 短按状态 的最小时间为 10 毫秒。

#define BUTTON_SHORT_LONG_THRESHOLD_MS                                         \
    (500) // 设置了按钮从 短按 到 长按 的时间阈值为 500 毫秒。

void Drive_Key_Init(void);

#endif /*__DRIVE_KEY_H__*/
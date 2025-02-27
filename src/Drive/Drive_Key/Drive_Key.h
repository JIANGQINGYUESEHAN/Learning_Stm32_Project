#ifndef __DRIVE_KEY_H__
#define __DRIVE_KEY_H__

#include "stdint.h"
#include "stm32f10x.h"
typedef enum eBUTTON_STATUS {
    BUTTON_IDLE = 0,     // ����״̬
    BUTTON_RELEASE = 1,  // �ͷ�״̬
    BUTTON_HOLD = 2,     // ��ͨ����
    BUTTON_HOLD_LONG = 3 // ����Ϊ����״̬
} eBUTTON_STATUS;

typedef struct eBUTTON_Struct {
    eBUTTON_STATUS status;
    void (*status_clear)(void);
} eBUTTON_Struct;

#define RCC_TIM_FOR_BUTTON RCC_APB1Periph_TIM4
#define RCC_GPIO_FOR_BUTTON RCC_APB2Periph_GPIOA // �����õ���ʱ��GPIO

#define BUTTON_GPIO GPIOA // �����õ���GPIO

#define BUTTON_GPIO_PIN GPIO_Pin_0 // �����õ���GPIO

#define TIME_FOR_BUTTON TIM4 // �����õ���ʱ��

#define TIM_FOR_BUTTON_IRQN TIM4_IRQn // �����õ�ʱ��

#define TIM4_PER_INTERRUPT_MS (1) // ��ʱ�� 4 ÿ�δ����жϵ�ʱ����Ϊ 1 ���롣

#define BUTTON_IDLE_SHORT_THRESHOLD_MS                                         \
    (10) // ��ť���� ����״̬ �� �̰�״̬ ����Сʱ��Ϊ 10 ���롣

#define BUTTON_SHORT_LONG_THRESHOLD_MS                                         \
    (500) // �����˰�ť�� �̰� �� ���� ��ʱ����ֵΪ 500 ���롣

void Drive_Key_Init(void);

#endif /*__DRIVE_KEY_H__*/
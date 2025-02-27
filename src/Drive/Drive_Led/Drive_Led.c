#include "Drive_Led.h"
#include "Delay.h"

#define LED_ON GPIO_WriteBit(LED_GPIO, LED_GPIO_PIN, RESET)
#define LED_OFF GPIO_WriteBit(LED_GPIO, LED_GPIO_PIN, SET)
#define LASER_LED_ON GPIO_WriteBit(GPIOA, GPIO_Pin_5, RESET)
#define LASER_LED_OFF GPIO_WriteBit(GPIOA, GPIO_Pin_5, RESET)

LED_t LED;
LED_t Laser;
void LED_Blink(eLED_STATUS status) {
    int i = 0;

    switch (status) {
    case ON:
        LED_ON;
        break;
    case 1:
        LED_OFF;
        break;
    case LED_5HZ:
        for (i = 0; i < 6; i++) {
            LED_OFF;
            Delay_ms(100);
            LED_ON;
            Delay_ms(100);
        }
        break;
    case LED_10HZ:
        for (i = 0; i < 11; i++) {
            LED_OFF;
            Delay_ms(50);
            LED_ON;
            Delay_ms(50);
        }
        break;
    case LED_2HZ:
        for (i = 0; i < 4; i++) {
            LED_OFF;
            Delay_ms(250);
            LED_ON;
            Delay_ms(250);
        }
        break;
    }
}
#ifdef LASER_ENABLE
void Lazer_operate(eLED_STATUS status) {
    switch (status) {
    case ON:
        LASER_LED_ON;
        break;
    case 1:
        LASER_LED_OFF;
        break;

    default:
        break;
    }
}

#endif // DEBUG

void Drive_Led_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 时钟使能
    GPIO_InitTypeDef GPIO_InitStruct; // 获取GPIO的实例然后进行配置
    GPIO_InitStruct.GPIO_Pin = LED_GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; // 推挽输出模式
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_GPIO, &GPIO_InitStruct);
#ifdef LASER_ENABLE

    GPIO_InitStruct = GPIO_Pin_5;
    GPIO_InitStruct = GPIO_Mode_AF_PP; // 推挽输出模式
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    Laser.status = OFF;
    Laser.Operate = &Lazer_operate;
    Laser.Operate(Laser.status);

#endif // DEBUG
    LED.status = OFF;
    LED.Operate = &LED_Blink;
    LED.Operate(LED.status);
}

#ifndef __DRIVE_LED_H__
#define __DRIVE_LED_H__
#include "stm32f10x.h"
typedef enum eLED_LED {
    ON = 0,
    OFF = 1,
    LED_2HZ = 2,
    LED_5HZ = 3,
    LED_10HZ = 4,

} eLED_STATUS;
typedef struct LED_t {
    void (*Operate)(eLED_STATUS status);
    eLED_STATUS status;
} LED_t;
#define LED_GPIO GPIOA
#define LED_GPIO_PIN GPIO_Pin_1

extern struct LED_t LED;
#ifdef LASER_ENABLE
extern struct LED_t Laser;
#endif

void Drive_Led_Init(void);
#endif /*__DRIVE_LED_H__*/

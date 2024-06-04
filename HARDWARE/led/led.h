// led.h

#ifndef LED_H
#define LED_H

#include "stm32f4xx.h"


#define LED_PIN GPIO_PIN_10
#define LED_PORT GPIOF


void LED_Init(void);


void LED_On(void);

void LED_Off(void);

#endif /* LED_H */

// led.h

#ifndef LED_H
#define LED_H

#include "stm32f4xx.h"

// ??LED?????
#define LED_PIN GPIO_PIN_10
#define LED_PORT GPIOF

// ???LED??
void LED_Init(void);

// ??LED
void LED_On(void);

// ??LED
void LED_Off(void);

void CheckTemperatureAndControlLED(void);

#endif /* LED_H */

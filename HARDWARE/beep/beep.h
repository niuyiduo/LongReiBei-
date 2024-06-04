#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

// Function prototypes
void BEEP_Init(void);
void BEEP_On(void);
void BEEP_Off(void);
void CheckTemperatureAndControlBuzzer(void);

#endif // __BEEP_H

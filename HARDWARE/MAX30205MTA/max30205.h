#ifndef MAX30205_H
#define MAX30205_H

#include "stm32f4xx.h"



void MAX30205_Init(void);
float MAX30205_ReadTemperature(void);

#endif /* MAX30205_H */

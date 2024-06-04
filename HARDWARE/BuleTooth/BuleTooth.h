#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include <stdint.h>
//extern typedef unsigned int u32; // Define u32 as an unsigned int
//Function prototypes
void USART3_SendString(char* str);
void BuleTooth_Init(void);
void BuleToothDisplayTemperature(void); 
#endif /* BLUETOOTH_H */

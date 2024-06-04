#include "beep.h"
#include "max30205.h"
#include "led.h"
#include "stm32f4xx.h"
#include "usart.h"
#include <stdint.h>
#include "delay.h"
#include <string.h>
#include <stdio.h>
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "max30205.h"
#define MAX_TEMP_THRESHOLD 20.0f // Define the temperature threshold

//有源蜂鸣器的正极接PF8，负极接GND

// Initialize the BEEP GPIO
void BEEP_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    // Enable the clock for GPIOF
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    // Configure the GPIO pin for the buzzer (PF8)
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOF, &GPIO_InitStruct);
	  GPIO_ResetBits(GPIOF, GPIO_Pin_8);
}

// Turn on the buzzer
void BEEP_On(void) {
    GPIO_SetBits(GPIOF, GPIO_Pin_8);
}

// Turn off the buzzer
void BEEP_Off(void) {
    GPIO_ResetBits(GPIOF, GPIO_Pin_8);
}


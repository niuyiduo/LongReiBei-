// led.c

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

//��LED�������������ţ����ӵ�STM32F407ZGT6��PB12�����ϣ���LED�����������ţ�������GND�ϡ�


#define MAX_TEMP_THRESHOLD 20.0f // Define the temperature threshold

// LED?????
void LED_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    // ??GPIOB??
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    // ??LED???????
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12; 
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStruct); 
}

// ??LED??
void LED_On(void) {
    GPIO_SetBits(GPIOB, GPIO_Pin_12); // ??LED,?
}

// ??LED??
void LED_Off(void) {
    GPIO_ResetBits(GPIOB, GPIO_Pin_12); // ??LED,
}


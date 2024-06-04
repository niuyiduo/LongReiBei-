

#include "stm32f4xx.h"
#include "usart.h"
#include "BuleTooth.h"
#include <stdint.h>
#include "delay.h"
#include <string.h>
#include <stdio.h>
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "max30205.h"
#include "stm32f4xx_i2c.h"

//硬件连接：PB10接HC-05上的RX，PB11接HC-05上TX，电源5V，接地GND，按照板子上给的提示连接就行(引用唇唇串口USART3)

void LED_On(void);
void LED_Off(void);
void BEEP_On(void);
void BEEP_Off(void);



void USART3_SendString(char* str) {
    while (*str) {
        while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
        USART_SendData(USART3, *str++);
    }
}


void BuleTooth_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

   
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

    
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    USART_Cmd(USART3, ENABLE);

   
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void BuleToothDisplayTemperature(void) {
	
	  char buffer[50];
	  float temperature;
    USART3_SendString("Reading temperature...\r\n");
    temperature = MAX30205_ReadTemperature();
	  if(temperature < 0)
		{
			USART3_SendString("Failed to read temperature\r\n");
			return;
		}
	 sprintf(buffer, "Temperature: %.2f\r\n", (temperature - 10.0f));
	 USART3_SendString(buffer);
		
	
    if ((temperature - 10.0f) < 20.0f) {
        LED_On(); 
    } else {
        LED_Off(); 
    }

    if (((temperature - 10.0f) < 20.0f) && GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_12)) {
        BEEP_On(); 
    } else {
        BEEP_Off(); 
    }

    
    if ((temperature - 10.0f) < 20.0f) {
        USART3_SendString("Warning: Temperature is very low!\r\n");
    }
     		
}

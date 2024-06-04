

#include "BuleTooth.h"
#include "stm32f4xx.h"
#include "system_stm32f4xx.h"
#include "sys.h"
#include "delay.h" 
#include "usart.h"
#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "misc.h"
#include "gps.h"
#include "led.h"
#include "usart.h"
#include "beep.h"
#include "max30205.h"
#include "led.h"

// Send a character to USART2
void USART2_SendChar(char c) {
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = c;
}

// Send a string to USART2
void USART2_SendString(char* str) {
    while (*str) {
        USART2_SendChar(*str++);
    }
}



int main(void) {
	
  
	  MAX30205_Init();
	  LED_Init();
	  BEEP_Init();
    GPS_Init();
    delay_init(168);     
    NB_IoT_Init();
	  BuleTooth_Init();  //HC-05初始化
	  uart_init(9600);	//串口初始化波特率为9600
	
		
		while (1) {
			  char gpsData[100];
			  char latitude[15];
        char longitude[15];
				
        GPS_GetData(gpsData, sizeof(gpsData));
			  GPS_ParseData(gpsData, latitude, longitude);
			
			  // 发送GPS到云端
        NB_IoT_SendGPSData(latitude, longitude);
			
        USART3_SendString("Hello, Bluetooth!\r\n");
     
        BuleToothDisplayTemperature();

        delay_ms(1000);
    }
		
		
}
		



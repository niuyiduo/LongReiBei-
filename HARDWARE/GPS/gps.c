#include "stm32f4xx.h"
#include "gps.h"
#include "delay.h"
#include <string.h>
#include <stdio.h> 

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"

//上传代码用下载口，云端用UART口，串口监视器用板子引脚上的5V，GND，U2_RXD，U2_TXD（三个只能分开用）
//NB-IoT上的所有RX接在PA2上，所有TX接PA3上

void GPS_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

   
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  

    GPIO_Init(GPIOA, &GPIO_InitStructure);  

    
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);  
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);  

   
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);
    USART_Cmd(USART2, ENABLE);
}




void GPS_GetData(char* buffer, int bufferSize) {
    int i = 0;
    char c;
    while (i < bufferSize - 1) {
        while (!(USART2->SR & USART_SR_RXNE));  // Wait for data to be received
        c = USART2->DR & 0xFF;       // Read the received data
        buffer[i++] = c;
        if (c == '\n') {
            break;
        }
    }
    
    buffer[i] = '\0';  // Null-terminate the string
}

void GPS_ParseData(char* nmea, char* latitude, char* longitude)
{
    /*char* token;
    const char delim[2] = ",";
	  int i;

    // Find the GPGGA sentence
    token = strstr(nmea, "$GPGGA");
    if (token == NULL)
    {
        latitude[0] = '\0';
        longitude[0] = '\0';
        return;
    }

    // Skip to latitude
    token = strtok(token, delim); // Skip $GPGGA
    for (i = 0; i < 2; i++)
    {
        token = strtok(NULL, delim);
    }
    strcpy(latitude, token);

    // Skip the N/S indicator
    token = strtok(NULL, delim);

    // Get longitude
    token = strtok(NULL, delim);
    strcpy(longitude, token);*/
		
		// Simplified GPS parsing function, assuming raw data is well-formed
    sscanf(nmea, "%*[^,],%[^,],%*[^,],%[^,]", latitude, longitude);
}



void NB_IoT_SendData(char* data) {
    while (*data) {
        while (!(USART2->SR & USART_SR_TXE));  // Wait for transmit buffer to be empty
        USART2->DR = *data++;  // Send data
    }
}

void NB_IoT_Init(void) {
    // Ensure the module is ready
    NB_IoT_SendData("AT\r\n");
    delay_ms(1000);

    // Attach to the network
    NB_IoT_SendData("AT+CGATT=1\r\n");
    delay_ms(2000);

    // Set the APN
    NB_IoT_SendData("AT+CGDCONT=1,\"IP\",\"CMIOT\"\r\n");  //中移的APN配置
    delay_ms(1000);

    // Open a connection to the cloud platform
    NB_IoT_SendData("AT+NSOCR=DGRAM,17,50000,1\r\n");
    delay_ms(1000);
}

void NB_IoT_SendGPSData(char* latitude, char* longitude) {
    
    char data[100];
    char command[150];

    
    sprintf(data, "Latitude: %s, Longitude: %s", latitude, longitude);

   
    sprintf(command, "AT+NSOST=0,\"183.230.40.96\",1883,%d,\"%s\"\r\n", strlen(data), data);  //ONENET平台上的MOTT协议IP地址和端口号配置
    
    NB_IoT_SendData(command);
    delay_ms(1000);  
}


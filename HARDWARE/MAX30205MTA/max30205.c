#include "max30205.h"
#include "led.h"
#include "beep.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx.h"
#include "usart.h"
#include <stdint.h>
#include "delay.h"
#include <string.h>
#include <stdio.h>
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"

#define MAX30205_ADDRESS 0x48
#define I2C_TIMEOUT 10000

void USART3_SendString(char* str);
// 接3.3V电压，接GND，PB6为12C1_SCL，接SCL，接PB7为12C1_SDA，接SDA，A0,A1,A2均接GND
void MAX30205_Init(void) {
    I2C_InitTypeDef I2C_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    // Enable clock for I2C1 and GPIOB
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    // Configure the GPIO pins for I2C
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Connect the pins to I2C peripheral
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);

    // Initialize I2C
    I2C_InitStructure.I2C_ClockSpeed = 100000;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &I2C_InitStructure);

    // Enable I2C
    I2C_Cmd(I2C1, ENABLE);
}

float MAX30205_ReadTemperature(void) {
    uint8_t data[2];
    uint16_t rawTemp;
    float temperature;
		char buffer[50];
    uint32_t timeout;
    // Start I2C communication
    I2C_GenerateSTART(I2C1, ENABLE);
	  timeout = I2C_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		{
			if(timeout-- == 0)
			{
				USART3_SendString("I2C start timeout\r\n");
				return -1;
			}
		}

    // Send address of MAX30205 and write mode
    I2C_Send7bitAddress(I2C1, (uint8_t)(MAX30205_ADDRESS << 1), I2C_Direction_Transmitter);
		timeout = I2C_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		{
			if(timeout-- == 0)
			{
				USART3_SendString("I2C address write timeout\r\n");
				return -1;
			}
		}

    // Send register address to read from
    I2C_SendData(I2C1, 0x00); // Temperature register address
		timeout = I2C_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			if(timeout-- == 0)
			{
				USART3_SendString("I2C register write timeout\r\n");
				return -1;
			}
		}

    // Restart I2C communication
    I2C_GenerateSTART(I2C1, ENABLE);
		timeout = I2C_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		{
			if(timeout-- == 0)
			{
				USART3_SendString("I2C restart timeout\r\n");
				return -1;
			}
		}

    // Send address of MAX30205 and read mode
    I2C_Send7bitAddress(I2C1, (uint8_t)(MAX30205_ADDRESS << 1), I2C_Direction_Receiver);
		timeout = I2C_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		{
			if(timeout-- == 0)
			{
				USART3_SendString("I2C address read timeout\r\n");
				return -1;
			}
		}

		  
    // Read two bytes of temperature data
		timeout = I2C_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
		{
			if(timeout-- == 0)
			{
				USART3_SendString("I2C data read timeout 1\r\n");
				return -1;
			}
		}
    data[0] = I2C_ReceiveData(I2C1);
		
		timeout = I2C_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
		{
			if(timeout-- == 0)
			{
				USART3_SendString("I2C data read timeout 2\r\n");
				return -1;
			}
		}
    data[1] = I2C_ReceiveData(I2C1);

    // Stop I2C communication
    I2C_GenerateSTOP(I2C1, ENABLE);

    // Combine the two bytes to form the raw temperature value
    rawTemp = ((uint16_t)data[0] << 8) | data[1];

    // Convert raw temperature to Celsius
    temperature = rawTemp * 0.00390625; 
		
	
    sprintf(buffer, "Raw temperature: %u\n", rawTemp);
		USART3_SendString(buffer);
		
		
    return temperature;
}

#ifndef __GPS_H
#define __GPS_H


void GPS_Init(void);
void GPS_GetData(char* buffer, int bufferSize);
void GPS_ParseData(char* nmea, char* latitude, char* longitude);
void NB_IoT_Init(void);
void NB_IoT_SendGPSData(char* latitude, char* longitude); 
#endif

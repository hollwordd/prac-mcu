#ifndef _WIFI_H_
#define _WIFI_H_

#include <stm32f10x.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "usart.h"
#include "lcd.h"

#define WIFI_USART USART2

int Wifi_Test(void);
void Find_Wifi(void);
void Parson_WifiData(u8 *buf);
void Find_Wifi(void);
int Connect_Wifi(char *name,char *pwd);
int Connect_TCPServer(char *type,char *ip,int port);
void SendData_Wifi(char *data);
void RecvData_Wifi(char *buf);
void Parson_RecvData(u8 *data,char *saveRevData);
#endif

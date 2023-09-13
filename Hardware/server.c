#include "server.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wifi.h"



extern char ques[256]; 
extern char answA[256];
extern char answB[256];
extern char answC[256];
extern char answD[256];

int join_answer(char *name)//加入答题
{
	char str[256] = {'\0'};
	RecvData_Wifi(str);
	RecvData_Wifi(str);
	SendData_Wifi("MCU");
	RecvData_Wifi(str);
	RecvData_Wifi(str);
	SendData_Wifi(name);
	RecvData_Wifi(str);
	SendData_Wifi("Q");
	RecvData_Wifi(ques);
	SendData_Wifi("A");
	RecvData_Wifi(answA);
	SendData_Wifi("B");
	RecvData_Wifi(answB);
	SendData_Wifi("C");
	RecvData_Wifi(answC);
	SendData_Wifi("D");
	RecvData_Wifi(answD);
	printf("%s\r\n",(char *)ques);
	return 1;
}

int ready_answer()//准备答题
{
	char str[256] = {'\0'};
	SendData_Wifi("F");
	RecvData_Wifi(str);
	return 1;
}

int send_answer(char *answer)//发送答案
{
	SendData_Wifi(answer);
	return 1;
}

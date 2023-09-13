#ifndef _H_SERVER_H_
#define _H_SERVER_H_
#include <stm32f10x.h>
#include "usart.h"

#define WIFI_USART USART2

int join_answer(char *name);
int send_answer(char *answer);
#endif

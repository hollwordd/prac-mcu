#ifndef	_BEEP_H_
#define _BEEP_H_

#include <stm32f10x.h>

void Beep_Config(void);
void Beep_Control(int num,int onTime,int offTime);
void Beep_On(void);
void Beep_Off(void);
void delay(int n);
#endif

#ifndef _USART_H_
#define _USART_H_

#include <stm32f10x.h>

void Usart_Config(uint32_t BandRate);
void Usart_Config_Wifi(uint32_t BaudRate);
void Usart_Send(USART_TypeDef* USARTx, uint16_t Data);
void Usart_Send_Str(USART_TypeDef* USARTx,const char *String);
uint8_t Usart_Rec(USART_TypeDef* USARTx);
void NVIC_Config(void);
#endif

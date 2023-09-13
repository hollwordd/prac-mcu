#ifndef CH452_H
#define CH452_H
#include<stm32f10x.h>

#define DCLK(x) (x == 0)? GPIO_ResetBits(GPIOD,GPIO_Pin_0):GPIO_SetBits(GPIOD,GPIO_Pin_0)
#define DIN(x) (x == 0)? GPIO_ResetBits(GPIOD,GPIO_Pin_1):GPIO_SetBits(GPIOD,GPIO_Pin_1)
#define LOAD(x) (x == 0)? GPIO_ResetBits(GPIOD,GPIO_Pin_2):GPIO_SetBits(GPIOD,GPIO_Pin_2)
#define DOUT GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)

#define	K4_1	124
#define	K4_2	116
#define	K4_3	108
#define	K4_4	100
#define K3_2	117
#define K3_3	109

void Ch452_Config(void);
void Send_Ch452Cmd(u16 cmd);
int Read_Key(void);

#endif

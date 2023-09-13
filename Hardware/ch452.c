#include "ch452.h"

void Send_Ch452Cmd(u16 cmd);

u8 Led_Buffer[36]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,    
0x77,0x7c,0x39,0x5e,0x79,0x71,0x3d,0x76,0x0f,0x0e,
0x75,0x38,0x37,0x54,0x5c,0x73,0x67,0x31,0x49,0x78,
0x3e,0x1c,0x7e,0x64,0x6e,0x59
};



void Ch452_Config(void)
{
	GPIO_InitTypeDef Gpio_Struct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	Gpio_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	Gpio_Struct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2;
	Gpio_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&Gpio_Struct);
	Gpio_Struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	Gpio_Struct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOD,&Gpio_Struct);
	
	DCLK(1);
	DIN(1);
	LOAD(1);
	Send_Ch452Cmd(0x201);
	Send_Ch452Cmd(0x403);
	Send_Ch452Cmd(0x0800|Led_Buffer[2]);
	Send_Ch452Cmd(0x0900|Led_Buffer[0]);
	Send_Ch452Cmd(0x0A00|Led_Buffer[2]);
	Send_Ch452Cmd(0x0B00|Led_Buffer[3]);
	Send_Ch452Cmd(0x0C00|Led_Buffer[0]);
	Send_Ch452Cmd(0x0D00|Led_Buffer[9]);
	Send_Ch452Cmd(0x0E00|Led_Buffer[0]);
	Send_Ch452Cmd(0x0F00|Led_Buffer[1]);
}

void Send_Ch452Cmd(u16 cmd)
{
	int i;
	for(i=0; i<12;i++)
	{
		if(cmd & (0x01)<<i)
		{
			DIN(1);
		
		}
		else
		{
			DIN(0);
		}
		DCLK(0);
		DCLK(1);
	}
	LOAD(0);
	LOAD(1);
}

/*
	当 CH452 检测到有效按键时，DOUT 输出低电平有效的键盘中断；单片机被中断后，发出读取按键代码命令，CH452 在 LOAD
的上升沿后从 DOUT 输出 7 位按键代码的最高位；单片机继续输出串行时钟，在 DCLK 的每个下降沿，
CH452 依次从 DOUT 输出 7 位按键代码的剩余 6 位，顺序是高位在前，低位在后；7 位按键代码输出完
毕后，无论 DCLK 如何变化，CH452 都将 DOUT 恢复为默认的高电平。
*/

int Read_Key(void)
{
	int i;
	int key = 0;
	if(!DOUT)
	{
		Send_Ch452Cmd(0x700);	//发送读取按键代码命令
		//读取7位按键代码的第7位
		key = DOUT << 6;
		for(i=5;i>=0;i--)
		{
			DCLK(0);
			key |= DOUT << i;
			DCLK(1);
		}
	}
	DCLK(0);
	DCLK(1);
//	GPIO_InitTypeDef Gpio_Struct;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
//	
//	Gpio_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
//	Gpio_Struct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2;
//	Gpio_Struct.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOD,&Gpio_Struct);
//	
//	Gpio_Struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	Gpio_Struct.GPIO_Pin = GPIO_Pin_3;
//	GPIO_Init(GPIOD,&Gpio_Struct);
//	
//	DCLK(1);
//	DIN(1);
//	LOAD(1);
//	
	return key;
}

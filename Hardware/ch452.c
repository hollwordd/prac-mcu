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
	�� CH452 ��⵽��Ч����ʱ��DOUT ����͵�ƽ��Ч�ļ����жϣ���Ƭ�����жϺ󣬷�����ȡ�����������CH452 �� LOAD
�������غ�� DOUT ��� 7 λ������������λ����Ƭ�������������ʱ�ӣ��� DCLK ��ÿ���½��أ�
CH452 ���δ� DOUT ��� 7 λ���������ʣ�� 6 λ��˳���Ǹ�λ��ǰ����λ�ں�7 λ�������������
�Ϻ����� DCLK ��α仯��CH452 ���� DOUT �ָ�ΪĬ�ϵĸߵ�ƽ��
*/

int Read_Key(void)
{
	int i;
	int key = 0;
	if(!DOUT)
	{
		Send_Ch452Cmd(0x700);	//���Ͷ�ȡ������������
		//��ȡ7λ��������ĵ�7λ
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

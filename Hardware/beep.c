#include "beep.h"

void Beep_Config(void)
{
	//初始化
	GPIO_InitTypeDef Gpio_Struct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//结构体赋值
	Gpio_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	Gpio_Struct.GPIO_Pin = GPIO_Pin_3;
	Gpio_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&Gpio_Struct);
	
	//测试配置
	//GPIO_SetBits(GPIOA,GPIO_Pin_3);
}

//延迟
void delay(int n)
{
	int i,j;
	for(i = 0;i < n;i++)
	{
		for(j = 0;j<10000;j++)
		{
			;
		}
	}
}

//控制蜂鸣器间接工作
void Beep_On(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
}

void Beep_Off(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
}

//蜂鸣器控制
void Beep_Control(int num,int onTime,int offTime)
{
	int i;
	for(i = 0;i < num;i++)
	{
		Beep_On();
		delay(onTime);
		Beep_Off();
		delay(offTime);
	}
}

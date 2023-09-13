#include "usart.h"

void Usart_Config(uint32_t BaudRate)
{
	//初始化
	GPIO_InitTypeDef Gpio_Struct;
	USART_InitTypeDef Usart_Struct;
	
	//打开相应的时钟信号
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//配置引脚
	//tx --> 输出
	Gpio_Struct.GPIO_Pin = GPIO_Pin_9;
	Gpio_Struct.GPIO_Mode = GPIO_Mode_AF_PP;
	Gpio_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&Gpio_Struct);
	
	//rx --> 输入
	Gpio_Struct.GPIO_Pin = GPIO_Pin_10;
	Gpio_Struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	//Gpio_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&Gpio_Struct);
	
	//数据接受和发送，需要满足一个条件：指定两个设备的通信协议.例：115200 1 n 8
	Usart_Struct.USART_BaudRate = BaudRate;
	Usart_Struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	Usart_Struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	Usart_Struct.USART_Parity = USART_Parity_No;
	Usart_Struct.USART_StopBits = USART_StopBits_1;
	Usart_Struct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&Usart_Struct);
	
	//打开接收中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	//打开空闲中断
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	
	//除了GPIO口之外，其他外设都需要使用xxx_Cmd使能
	USART_Cmd(USART1,ENABLE);
}

void Usart_Config_Wifi(uint32_t BaudRate)
{
	//初始化
	GPIO_InitTypeDef Gpio_Struct;
	USART_InitTypeDef Usart_Struct;
	
	//打开相应的时钟信号
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
	
	//配置引脚
	//tx --> 输出
	Gpio_Struct.GPIO_Pin = GPIO_Pin_5;
	Gpio_Struct.GPIO_Mode = GPIO_Mode_AF_PP;
	Gpio_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&Gpio_Struct);
	
	//rx --> 输入
	Gpio_Struct.GPIO_Pin = GPIO_Pin_6;
	Gpio_Struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	//Gpio_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&Gpio_Struct);
	
	//数据接受和发送，需要满足一个条件：指定两个设备的通信协议.例：115200 1 n 8
	Usart_Struct.USART_BaudRate = BaudRate;
	Usart_Struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	Usart_Struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	Usart_Struct.USART_Parity = USART_Parity_No;
	Usart_Struct.USART_StopBits = USART_StopBits_1;
	Usart_Struct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&Usart_Struct);
	
	//打开接收中断
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	//打开空闲中断
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
	
	//除了GPIO口之外，其他外设都需要使用xxx_Cmd使能
	USART_Cmd(USART2,ENABLE);
}

//用单片机向计算机发送或者接收一个字符
//解决连续发送单个数据的覆盖问题
void Usart_Send(USART_TypeDef* USARTx, uint16_t Data)
{
	//在发送数据之前，判断发送数据的缓冲区是否为空
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
	USART_SendData(USARTx,Data);
}

//发送一串字符“2345 hello”
void Usart_Send_Str(USART_TypeDef* USARTx,const char *String)
{
	int i;
	for(i = 0;String[i] != '\0';i++)
	{
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
		USART_SendData(USARTx,String[i]);
	}
}

//接收单字节数据并发送回上位机
//void Usart_Rec(USART_TypeDef* USARTx)
//{
//	uint16_t Data;
//	while(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE) != RESET)
//	Data = USART_ReceiveData(USARTx);
//	while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
//	USART_SendData(USARTx,Data);
//}
uint8_t Usart_Rec(USART_TypeDef* USARTx)
{
	while(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE) == RESET);
	return USART_ReceiveData(USARTx);
}

//接收一串字符	确定数据什么时候接收完毕
//实现中断服务程序
//1.打开串口的接收中断
//2.中断分组 NVIC（嵌套向量表）	5个组 0-4
//每个组内分为抢占式优先级（第一优先级）和响应优先级（第二优先级）	数字越低，优先级越高
// 0组 第一优先级设置个数 2的0次方	第二优先级个数 2的4次方
// 1	1	3
// 2	2	2	
// 3	3	1
// 4	4	0
//两个中断同时到达时，先判断第一优先级，后判断第二优先级，根据硬件中断优先级执行
//高优先级可以打断低优先级
//3.实现中断服务程序，并且中断服务程序不需要手动调用、中断服务程序不可随意实现 --> 所有中断服务程序在启动文件中已经定义（.s）
void NVIC_Config(void)
{
	//设置当前中断到哪个组中
	NVIC_InitTypeDef NVIC_Struct;
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_1);
	
	NVIC_Struct.NVIC_IRQChannel = USART1_IRQn;	//串口1通道
	NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;	//使能
	NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 0;	//抢占式优先级
	NVIC_Struct.NVIC_IRQChannelSubPriority = 0;	//响应优先级
	
	NVIC_Init(&NVIC_Struct);
	
	NVIC_Struct.NVIC_IRQChannel = USART2_IRQn;	//串口2通道
	NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;	//使能
	NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 0;	//抢占式优先级
	NVIC_Struct.NVIC_IRQChannelSubPriority = 0;	//响应优先级
	
	NVIC_Init(&NVIC_Struct);
}

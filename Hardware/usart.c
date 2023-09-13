#include "usart.h"

void Usart_Config(uint32_t BaudRate)
{
	//��ʼ��
	GPIO_InitTypeDef Gpio_Struct;
	USART_InitTypeDef Usart_Struct;
	
	//����Ӧ��ʱ���ź�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//��������
	//tx --> ���
	Gpio_Struct.GPIO_Pin = GPIO_Pin_9;
	Gpio_Struct.GPIO_Mode = GPIO_Mode_AF_PP;
	Gpio_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&Gpio_Struct);
	
	//rx --> ����
	Gpio_Struct.GPIO_Pin = GPIO_Pin_10;
	Gpio_Struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	//Gpio_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&Gpio_Struct);
	
	//���ݽ��ܺͷ��ͣ���Ҫ����һ��������ָ�������豸��ͨ��Э��.����115200 1 n 8
	Usart_Struct.USART_BaudRate = BaudRate;
	Usart_Struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	Usart_Struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	Usart_Struct.USART_Parity = USART_Parity_No;
	Usart_Struct.USART_StopBits = USART_StopBits_1;
	Usart_Struct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&Usart_Struct);
	
	//�򿪽����ж�
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	//�򿪿����ж�
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	
	//����GPIO��֮�⣬�������趼��Ҫʹ��xxx_Cmdʹ��
	USART_Cmd(USART1,ENABLE);
}

void Usart_Config_Wifi(uint32_t BaudRate)
{
	//��ʼ��
	GPIO_InitTypeDef Gpio_Struct;
	USART_InitTypeDef Usart_Struct;
	
	//����Ӧ��ʱ���ź�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
	
	//��������
	//tx --> ���
	Gpio_Struct.GPIO_Pin = GPIO_Pin_5;
	Gpio_Struct.GPIO_Mode = GPIO_Mode_AF_PP;
	Gpio_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&Gpio_Struct);
	
	//rx --> ����
	Gpio_Struct.GPIO_Pin = GPIO_Pin_6;
	Gpio_Struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	//Gpio_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&Gpio_Struct);
	
	//���ݽ��ܺͷ��ͣ���Ҫ����һ��������ָ�������豸��ͨ��Э��.����115200 1 n 8
	Usart_Struct.USART_BaudRate = BaudRate;
	Usart_Struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	Usart_Struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	Usart_Struct.USART_Parity = USART_Parity_No;
	Usart_Struct.USART_StopBits = USART_StopBits_1;
	Usart_Struct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&Usart_Struct);
	
	//�򿪽����ж�
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	//�򿪿����ж�
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
	
	//����GPIO��֮�⣬�������趼��Ҫʹ��xxx_Cmdʹ��
	USART_Cmd(USART2,ENABLE);
}

//�õ�Ƭ�����������ͻ��߽���һ���ַ�
//����������͵������ݵĸ�������
void Usart_Send(USART_TypeDef* USARTx, uint16_t Data)
{
	//�ڷ�������֮ǰ���жϷ������ݵĻ������Ƿ�Ϊ��
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
	USART_SendData(USARTx,Data);
}

//����һ���ַ���2345 hello��
void Usart_Send_Str(USART_TypeDef* USARTx,const char *String)
{
	int i;
	for(i = 0;String[i] != '\0';i++)
	{
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
		USART_SendData(USARTx,String[i]);
	}
}

//���յ��ֽ����ݲ����ͻ���λ��
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

//����һ���ַ�	ȷ������ʲôʱ��������
//ʵ���жϷ������
//1.�򿪴��ڵĽ����ж�
//2.�жϷ��� NVIC��Ƕ��������	5���� 0-4
//ÿ�����ڷ�Ϊ��ռʽ���ȼ�����һ���ȼ�������Ӧ���ȼ����ڶ����ȼ���	����Խ�ͣ����ȼ�Խ��
// 0�� ��һ���ȼ����ø��� 2��0�η�	�ڶ����ȼ����� 2��4�η�
// 1	1	3
// 2	2	2	
// 3	3	1
// 4	4	0
//�����ж�ͬʱ����ʱ�����жϵ�һ���ȼ������жϵڶ����ȼ�������Ӳ���ж����ȼ�ִ��
//�����ȼ����Դ�ϵ����ȼ�
//3.ʵ���жϷ�����򣬲����жϷ��������Ҫ�ֶ����á��жϷ�����򲻿�����ʵ�� --> �����жϷ�������������ļ����Ѿ����壨.s��
void NVIC_Config(void)
{
	//���õ�ǰ�жϵ��ĸ�����
	NVIC_InitTypeDef NVIC_Struct;
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_1);
	
	NVIC_Struct.NVIC_IRQChannel = USART1_IRQn;	//����1ͨ��
	NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;	//ʹ��
	NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 0;	//��ռʽ���ȼ�
	NVIC_Struct.NVIC_IRQChannelSubPriority = 0;	//��Ӧ���ȼ�
	
	NVIC_Init(&NVIC_Struct);
	
	NVIC_Struct.NVIC_IRQChannel = USART2_IRQn;	//����2ͨ��
	NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;	//ʹ��
	NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 0;	//��ռʽ���ȼ�
	NVIC_Struct.NVIC_IRQChannelSubPriority = 0;	//��Ӧ���ȼ�
	
	NVIC_Init(&NVIC_Struct);
}

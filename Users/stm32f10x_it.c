/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "ch452.h"
/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

//����1�жϷ������
u8 g_Rec_Buf[100] = {'\0'};
int g_Rec_Len = 0;
int g_Rec_Flag = 0;

void USART1_IRQHandler(void)
{
	//�жϵ�ǰ����һ���жϱ�־����
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
	{
		//�����ݵ����������
		g_Rec_Buf[g_Rec_Len] = USART_ReceiveData(USART1);
		g_Rec_Len++;
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
	else if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET)
	{
		g_Rec_Flag = 1;
		//IDLE�ȶ�DR״̬�Ĵ������ٶ�DR���ݼĴ������������
		USART1->SR;
		USART1->DR;
	}
}

//����2�жϷ������

extern u8 g_Wifi_Rec[2048];
extern int g_Wifi_Len;
extern int g_Wifi_Finish;	//�������ݽ������ �����Ϊ1

void USART2_IRQHandler(void)
{
	//�жϵ�ǰ����һ���жϱ�־����
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
	{
		//�����ݵ����������
		g_Wifi_Rec[g_Wifi_Len] = USART_ReceiveData(USART2);
		g_Wifi_Len++;
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
	else if(USART_GetITStatus(USART2,USART_IT_IDLE) != RESET)
	{
		g_Wifi_Finish = 1;
		//IDLE�ȶ�DR״̬�Ĵ������ٶ�DR���ݼĴ������������
		USART2->SR;
		USART2->DR;
	}
}

/******************* (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


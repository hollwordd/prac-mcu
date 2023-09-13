#include <stdio.h>
#include <stm32f10x.h>
#include <string.h>
#include "server.h"
#include "beep.h"
#include "usart.h"
#include "lcd.h"
#include "spi_flash.h"
#include "ch452.h"
#include "wifi.h"

extern u8 g_Rec_Buf[100];
extern int g_Rec_Flag;
extern int g_Rec_Len;


char ques[100] = {'\0'}; 
char answA[100]= {'\0'};
char answB[100]= {'\0'};
char answC[100]= {'\0'};
char answD[100]= {'\0'};




int fputc(int ch,FILE *f)
{
	Usart_Send(USART1,ch);
	return ch;
}


int main(void)
{
//	int key = 0;
	Beep_Config();
	Usart_Config(115200);
	Usart_Config_Wifi(115200);
	NVIC_Config();
	LCD_Initializtion();
	SPI_FLASH_Init();
	Ch452_Config();
	//Wifi_Test();
	//Find_Wifi();
	
	if(Wifi_Test())
	{
		if(Connect_Wifi("bgm","xewxewxew"))
		{
			printf("连接成功\r\n");
			if(Connect_TCPServer("TCP","migow.club",12345))
			{
				printf("%d",join_answer("DSB"));
				printf("%s",ques);
			}
		}
		else
		{
			printf("连接失败\r\n");
		}
	  
	}
	
	

	//Parson_WifiData(g_Rec_Buf);
	//SPI_FLASH_ReadHzbuf("啊");
	//SPI_FLASH_ReadHzbuf_Str(100,100,Hz);
	//	Lcd_Draw_Rectangle(100,80,120,80,Red);
//	Lcd_Draw_FullRectangle(110,90,100,60,Blue);
//	Lcd_Draw_FullCricle(180,120,30,Green);
//	Lcd_Draw_Cricle(180,120,10,Yellow);	
//	Lcd_DisplayImage(0,0,319,240,gImage_lth);
//  Lcd_DisplayAnySizeChar(10,10,56,75,Black,White,Font);
	//Lcd_DispEnglish(10,10,Black,White,'A');
//	Lcd_DispEnglishorChinese_Str(10,10,Black,White,"玩玩手机拍拍照，夜袭桃aLTH");
//	Lcd_DispEnglishorChinese_Str(10,26,Black,White,"好好好!");
//	Usart_Send(USART1,'2');
//	Usart_Send(USART1,'3');
//	Usart_Send(USART1,'4');
//	Usart_Send(USART1,'5');
//	Usart_Send(USART1,' ');
//	Usart_Send_Str(USART2,"2345 hello\r\n");
//	Data = Usart_Rec(USART1);
//	Usart_Send(USART1,Data);
	while(1)
	{
//		if(g_Rec_Flag)
//		{
//			Usart_Send_Str(USART2,(const char*)g_Rec_Buf);
//			printf("%s\r\n",g_Rec_Buf);
//			g_Rec_Flag = 0;
//			g_Rec_Len = 0;
//			memset(g_Rec_Buf,'\0',sizeof(g_Rec_Buf));
//		}
		
//		key = Read_Key();
//		if(key != 0)
//		{
//			printf("key = %d\r\n",key);
//			if(key == K4_1)
//			{
//				printf("Beep_On\r\n");
//				Beep_On();
//				Lcd_Clear(Cyan);
//				Lcd_DispEnglishorChinese_Str(10,10,Black,White,"Beep_On");
//			}
//			else if(key == K4_2)
//			{
//				printf("Beep_Off\r\n");
//				Beep_Off();
//				Lcd_Clear(Cyan);
//				Lcd_DispEnglishorChinese_Str(10,10,Black,White,"Beep_Off");
//			}
//		}
	};
}

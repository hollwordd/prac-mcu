/******************************************************************************
* @file    lcd_1289.c
* @version V1.0
* @brief   TFT Driver program
******************************************************************************/
/*****************************
**    硬件连接说明          **
** STM32        TFT_LCD(SSD1289)    **
** PE0~15 <----> DB0~15     **
** PD15   <----> nRD        **
** PD13   <----> RS         **
** PB14   <----> nWR        **
** PC8    <----> nCS        **
** Reset  <----> nReset     **
** VCC    <----> BK_LED     **
******************************/

#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "lcd.h"


/* Private typedef -----------------------------------------------------------*/

/*******************************************************************************
* Function Name  : LCD_DB_AS_InPut
* Description    : config MCU LCD_DB pins AS InPut
* Input          : None
* Output         : None
* Return         : None.
*******************************************************************************/
void LCD_DB_AS_InPut(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// DB15--0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : LCD_DB_AS_OutPut
* Description    : config MCU LCD_DB pins AS OutPut
* Input          : None
* Output         : None
* Return         : None.
*******************************************************************************/
void LCD_DB_AS_OutPut(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// DB15--0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name  : LCD_Pins_Config
* Description    : Configures LCD Pins
Push-Pull mode.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Pins_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(LCD_CLK_RS | RCC_APB2Periph_GPIOE |
		LCD_CLK_WR | LCD_CLK_RD |
		LCD_CLK_CS, ENABLE);
	
	// DB15--0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	//LCD_Pin_WR
	GPIO_InitStructure.GPIO_Pin = LCD_Pin_WR;
	GPIO_Init(LCD_PORT_WR, &GPIO_InitStructure);
	
	//LCD_Pin_CS
	GPIO_InitStructure.GPIO_Pin = LCD_Pin_CS;
	GPIO_Init(LCD_PORT_CS, &GPIO_InitStructure);
	
	//LCD_Pin_RS
	GPIO_InitStructure.GPIO_Pin = LCD_Pin_RS;
	GPIO_Init(LCD_PORT_RS, &GPIO_InitStructure);
	
	//LCD_Pin_RD
	GPIO_InitStructure.GPIO_Pin = LCD_Pin_RD;
	GPIO_Init(LCD_PORT_RD, &GPIO_InitStructure);
	
	SetCs
}



/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
* author         : www.armjishu.com 
*******************************************************************************/
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{
	/* Write 16-bit Index, then Write Reg */
	ClrCs
		ClrRs
		ClrWr
		LCD_Write(LCD_Reg);
	SetWr
		/* Write 16-bit Reg */
		SetRs
		ClrWr
		LCD_Write(LCD_RegValue);
	SetWr
		SetCs
}

/*******************************************************************************
* Function Name  : LCD_ReadReg
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : None
* Return         : LCD Register Value.
*******************************************************************************/
u16 LCD_ReadReg(u8 LCD_Reg)
{
	u16 data;
	
	/* Write 16-bit Index (then Read Reg) */
	ClrCs
		ClrRs
		ClrWr
		LCD_Write(LCD_Reg);
	SetWr
		
		/* Read 16-bit Reg */
		SetRs
		ClrRd
		SetRd
		data = LCD_Read(); 
	SetCs
		
		return    data;
}

u16 LCD_ReadSta(void)
{
	u16 data;
	
	/* Write 16-bit Index, then Write Reg */
	SetRs
		ClrRd
		SetRd
		data = LCD_Read(); 
	SetCs
		
		return    data;
}

void LCD_WriteCommand(u16 LCD_RegValue)
{
	/* Write 16-bit Index, then Write Reg */
	ClrCs
		ClrRs
		ClrWr
		LCD_Write(LCD_RegValue);
	SetWr
		SetCs
}

/*******************************************************************************
* Function Name  : LCD_WriteRAM_Prepare
* Description    : Prepare to write to the LCD RAM.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteRAM_Prepare(void)
{
	/* Write 16-bit Index, then Write Reg */
	ClrCs
		ClrRs
		ClrWr
		LCD_Write(R34);
	SetWr
		SetCs
}

/*******************************************************************************
* Function Name  : LCD_WriteRAM
* Description    : Writes to the LCD RAM.
* Input          : - RGB_Code: the pixel color in RGB mode (5-6-5).
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteRAM(u16 RGB_Code)					 
{
	/* Write 16-bit Index, then Write Reg */
	ClrCs
		SetRs
		ClrWr
		LCD_Write(RGB_Code);
	SetWr
		SetCs
}

/*******************************************************************************
* Function Name  : LCD_ReadRAM
* Description    : Reads the LCD RAM.
* Input          : None
* Output         : None
* Return         : LCD RAM Value.
*******************************************************************************/
u16 LCD_ReadRAM(void)
{
	u16 dummy;
	u16 data;
	LCD_WriteRAM_Prepare();
	LCD_DB_AS_InPut();
	dummy = LCD_ReadSta();
	dummy++;
	data = LCD_ReadSta();
	LCD_DB_AS_OutPut();
	
	return    data;
}


void Delay(__IO u32 nCount)
{
    __IO  u32 TimingDelay; 
    while(nCount--)
    {
        for(TimingDelay=0;TimingDelay<1000;TimingDelay++);
    }
}

/****************************************************************************
* 名    称：void LCD_Initializtion()
* 功    能：初始化 SSD1289控制器
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：LCD_Initializtion();
****************************************************************************/
void LCD_Initializtion(void)
{
    u16 DeviceIdCode;
	LCD_Pins_Config(); /* 配置管脚 Configure the LCD Control pins */
	Delay(5); /* delay 50 ms */
	LCD_WriteReg(0x0000,0x0001);  
	Delay(5); /* delay 50 ms */			//start internal osc
	LCD_DB_AS_InPut();
	Delay(1); /* delay 50 ms */			//start internal osc
	DeviceIdCode = LCD_ReadReg(0x0000);
	printf("\n\r ###### www.armjishu.com DeviceIdCode = 0x%x ###### ", DeviceIdCode);
	LCD_DB_AS_OutPut();
	Delay(1); /* delay 50 ms */			//start internal osc
	if(DeviceIdCode == 0x8989)
    {
		printf("\n\r This LCD is SSD1289 ");
		LCD_WriteReg(0x0000,0x0001);		//打开晶振
		LCD_WriteReg(0x0003,0xA8A4);		//0xA8A4
		LCD_WriteReg(0x000C,0x0000);        
		LCD_WriteReg(0x000D,0x080C);        
		LCD_WriteReg(0x000E,0x2B00);        
		LCD_WriteReg(0x001E,0x00B0);        
		LCD_WriteReg(0x0001,0x6B3F);		//驱动输出控制320*240  0x693F  0x2B3F
		LCD_WriteReg(0x0002,0x0600);     	//LCD Driving Waveform control
		LCD_WriteReg(0x0010,0x0000);     
		LCD_WriteReg(0x0011,0x6078);		//0x4030	//定义数据格式  16位色	横屏 0x6058
		LCD_WriteReg(0x0005,0x0000);     
		LCD_WriteReg(0x0006,0x0000);     
		LCD_WriteReg(0x0016,0xEF1C);     
		LCD_WriteReg(0x0017,0x0003);     
		LCD_WriteReg(0x0007,0x0233);		//0x0233       
		LCD_WriteReg(0x000B,0x0000);     
		LCD_WriteReg(0x000F,0x0000);		//扫描开始地址
		LCD_WriteReg(0x0041,0x0000);     
		LCD_WriteReg(0x0042,0x0000);     
		LCD_WriteReg(0x0048,0x0000);     
		LCD_WriteReg(0x0049,0x013F);     
		LCD_WriteReg(0x004A,0x0000);     
		LCD_WriteReg(0x004B,0x0000);     
		LCD_WriteReg(0x0044,0xEF00);     
		LCD_WriteReg(0x0045,0x0000);     
		LCD_WriteReg(0x0046,0x013F);     
		LCD_WriteReg(0x0030,0x0707);     
		LCD_WriteReg(0x0031,0x0204);     
		LCD_WriteReg(0x0032,0x0204);     
		LCD_WriteReg(0x0033,0x0502);     
		LCD_WriteReg(0x0034,0x0507);     
		LCD_WriteReg(0x0035,0x0204);     
		LCD_WriteReg(0x0036,0x0204);     
		LCD_WriteReg(0x0037,0x0502);     
		LCD_WriteReg(0x003A,0x0302);     
		LCD_WriteReg(0x003B,0x0302);     
		LCD_WriteReg(0x0023,0x0000);     
		LCD_WriteReg(0x0024,0x0000);     
		LCD_WriteReg(0x0025,0x8000);     
		LCD_WriteReg(0x004e,0);        //列(X)首址0
		LCD_WriteReg(0x004f,0);        //行(Y)首址0
    }
    else
    {
		printf("\n\r ###### Err: Is Not SD1289 DeviceIdCode 0x%x ###### ", DeviceIdCode);
    }
	
    Delay(2000);

    Lcd_Clear(Cyan);
}

/****************************************************************************
* 名    称：void Lcd_SetCursor(u16 x,u16 y)
* 功    能：设置屏幕座标
* 入口参数：x      行座标
*           y      列座标
* 出口参数：无
* 说    明：
* 调用方法：Lcd_SetCursor(10,10);
****************************************************************************/
void Lcd_SetCursor(u16 x,u8 y)
{
    LCD_WriteReg(0x004e,y);        //行
    LCD_WriteReg(0x004f,x);  //列
}


/****************************************************************************
* 名    称：Lcd_SetWindows(u8 StartX,u16 StartY,u8 EndX,u16 EndY)
* 功    能：设置屏幕窗口范围
* 入口参数：StartX     起始行座标
*           StartY     起始列座标
*           EndX       末始行座标
*           EndY       末始列座标
* 出口参数：无
* 说    明：
* 调用方法：Lcd_SetWindows(10,10，200,200);
****************************************************************************/
void Lcd_SetWindows(u16 StartX,u8 StartY,u16 EndX,u8 EndY)
{
	Lcd_SetCursor(StartX,StartY);
	LCD_WriteReg(0x0044, StartY|EndY<<8);
	LCD_WriteReg(0x0045, StartX);
	LCD_WriteReg(0x0046, EndX);
}


/****************************************************************************
* 名    称：void Lcd_SetPoint(u16 x,u16 y,u16 point)
* 功    能：在指定座标画点
* 入口参数：x      行座标
*           y      列座标
*           point  点的颜色
* 出口参数：无
* 说    明：
* 调用方法：Lcd_SetPoint(10,10,0x0fe0);
****************************************************************************/
void Lcd_SetPoint(u16 x,u8 y,u16 point)
{
	if ( (x>320)||(y>240) ) return;
	Lcd_SetCursor(x,y);
	LCD_WriteRAM_Prepare();
	LCD_WriteRAM(point);
}


void Lcd_Clear(u16 Color)
{
	int i;
	Lcd_SetWindows(0,0,319,239); 
	LCD_WriteRAM_Prepare();
	for(i=0 ; i<240 * 320 ; i++)
	{
		LCD_WriteRAM(Color);
	}
}

/****************************************************************************
* 名    称：void Lcd_Rectangle(u16 x1,u16 x2,u16 y1,u16 y2,u16 color)
* 功    能：在指定座标画矩形
* 入口参数：x1,x2      行座标
*           y1,y2      列座标
*           color  矩形的颜色
* 出口参数：无
* 说    明：
* 调用方法：Lcd_Rectangle(10,10,0x0fe0);
****************************************************************************/
//#define PI 3.1415926
//void Lcd_Circle(int m,int n,int r,int color)
//{
//int i;
//int x,y;
//int N = 360;
//  for(i=0;i<N;i++)
//  {
//      x = m+r*cos(2.0*PI*i/N);
//      y = n+r*sin(2.0*PI*i/N);
//      Lcd_SetPoint(x,y,color);
//  }
//}

//void Lcd_Full_Circle(int m,int n,int r,int color)
//{
//	int i;
//	for(i=0;i<+r;i++)
//	{
//		Lcd_Circle(m,n,i,color);
//	}
//}

/****************************************************************************
* 名    称：void Lcd_Rectangle(u16 x1,u16 x2,u16 y1,u16 y2,u16 color)
* 功    能：在指定座标画矩形
* 入口参数：x1,x2      行座标
*           y1,y2      列座标
*           color  矩形的颜色
* 出口参数：无
* 说    明：
* 调用方法：Lcd_Rectangle(10,10,0x0fe0);
****************************************************************************/
//void Lcd_Rectangle(u16 x1,u16 x2,u16 y1,u16 y2,u16 color)
//{
//	int i,j;
//	int xa,xb,ya,yb;
//	for(i=0;i<(y2-y1);i++)
//	{
//		for(j=0;j<(x2-x1);j++)
//		{
//			xa = x1 + i;
//			ya = y1 + j;
//			Lcd_SetPoint(xa,ya,color);
//			xb = x2 + i;
//			yb = y2 + i;
//			Lcd_SetPoint(xb,yb,color);
//		}
//	}
//}

//在LCD屏幕上实现填充矩形，非填充矩形，圆
//填充矩形
void Lcd_Draw_FullRectangle(u16 x,u16 y,u16 width,u16 height,u16 color)
{
	int i,j;
	for(i=x;i<x+width;i++)
	{
		for(j=y;j<y+height;j++)
		{
			Lcd_SetPoint(i,j,color);
		}
	}
}

//非填充矩形
void Lcd_Draw_Rectangle(u16 x,u16 y,u16 width,u16 height,u16 color)
{
	int i,j;
	for(i=x;i<x+width;i++)
	{
		for(j=y;j<y+height;j++)
		{
			if(i == x || j == y || i == x+width-1 || j == y+height-1)
			{
				Lcd_SetPoint(i,j,color);				
			}
		}
	}
}

//填充圆
void Lcd_Draw_FullCricle(u16 x,u16 y,u16 r,u16 color)
{
	int i,j;
	for(i=x-r;i<x+r;i++)
	{
		for(j=y-r;j<y+r;j++)
		{
			if((i-x)*(i-x)+(j-y)*(j-y) <= r*r)
			{
				Lcd_SetPoint(i,j,color);
			}
		}
	}
}

//非填充圆
void Lcd_Draw_Cricle(u16 x,u16 y,u16 r,u16 color)
{
	int i,j;
	for(i=x-r;i<x+r;i++)
	{
		for(j=y-r;j<y+r;j++)
		{
			if((i-x)*(i-x)+(j-y)*(j-y) == r*r)
			{
				Lcd_SetPoint(i,j,color);
			}
		}
	}
}

//A
void Lcd_Draw_AVICIIcon(u16 x1,u16 x2,u16 y,u16 width,u16 height,u16 color)
{
	int i,j;
	for(i=x1;i>x1-width;i--)
	{
		for(j=y;j<j+height;j++)
		{
			if(i<x1 || i>i-width-1)
			Lcd_SetPoint(i,j,color);
		}
	}
}

//裸机开发 图片解析器 --> .jpg .png
//图片无法解析 --> 提取图片数据
//C语言 --> 读取图片文件
void Lcd_DisplayImage(u16 x,u16 y,u16 width,u16 height,const unsigned char* ImgBuf)
{
	int i,j;
	int k = 0;
	u16 color;
	for(j=y;j<y+height;j++)	//行
	{
		for(i=x;i<x+width;i++)	//列
		{
			//低位在前，偶数位为低位
			color = ImgBuf[2*k+1] << 8 | ImgBuf[2*k];
			Lcd_SetPoint(i,j,color);
			k++;
		}
	}
}

//显示任意大小的文字
//解析文字
//一位代表一个点阵的颜色值
const unsigned char Font[4200] = 
{
/*--  文字:  魑  --*/
/*--  微软雅黑42;  此字体下对应的点阵为：宽x高=56x75   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x00,0x00,0x0C,
0x00,0x00,0x00,0x3F,0x00,0x00,0x7E,0x00,0x00,0x00,0x3E,0x00,0x00,0x3E,0x00,0x00,
0x00,0x7E,0x00,0x00,0x3F,0x00,0x00,0x00,0x7C,0x00,0x00,0x1F,0x00,0x00,0x00,0x7C,
0x01,0xFF,0xFF,0xFF,0xF8,0x3F,0xFF,0xFD,0xFF,0xFF,0xFF,0xF8,0x3F,0xFF,0xFD,0xFF,
0xFF,0xFF,0xF8,0x3F,0xFF,0xFD,0xFF,0xFF,0xFF,0xF8,0x3F,0xFF,0xFC,0x00,0x00,0x00,
0x00,0x3C,0x3C,0x3C,0x01,0xE0,0xF0,0x00,0x3C,0x3C,0x3C,0x01,0xF1,0xF8,0x00,0x3C,
0x3C,0x3C,0x7F,0xFF,0xFB,0xE0,0x3C,0x3C,0x3C,0x7C,0xFF,0xE3,0xE0,0x3C,0x3C,0x3C,
0x7C,0x3F,0xC3,0xE0,0x3C,0x3C,0x3C,0x7C,0x7F,0xE3,0xE0,0x3F,0xFF,0xFC,0x7C,0xFF,
0xFB,0xE0,0x3F,0xFF,0xFC,0x7F,0xFF,0xFF,0xE0,0x3F,0xFF,0xFC,0x7D,0xF0,0xFB,0xE0,
0x3F,0xFF,0xFC,0x7D,0xE0,0x73,0xE0,0x3C,0x7C,0x3C,0x7C,0x00,0x03,0xE0,0x3C,0x7C,
0x3C,0x7F,0xFF,0xFF,0xE0,0x3C,0x7C,0x3C,0x7F,0xFF,0xFF,0xE0,0x3C,0x7C,0x3C,0x7F,
0xFF,0xFF,0xE0,0x3C,0x7C,0x3C,0x7F,0xFF,0xFF,0xE0,0x3C,0x7C,0x3C,0x00,0x1F,0x03,
0xE0,0x3C,0x7C,0x3C,0x00,0x1F,0x00,0x00,0x3F,0xFF,0xFC,0x00,0x3E,0x00,0x00,0x3F,
0xFF,0xFC,0x7F,0xFF,0xFF,0xF0,0x3F,0xFF,0xFC,0x7F,0xFF,0xFF,0xF0,0x3F,0xFF,0xFC,
0x7F,0xFF,0xFF,0xF0,0x3C,0xF8,0x00,0x7F,0xFF,0xFF,0xF0,0x3C,0xFF,0x3C,0x7C,0x78,
0x01,0xF0,0x00,0xFF,0x3C,0x7C,0xF8,0xE1,0xF0,0x00,0xFF,0x7C,0x7C,0xF9,0xF1,0xF0,
0x01,0xFF,0x78,0x7C,0xF1,0xF1,0xF0,0x01,0xFF,0x7F,0x7D,0xF0,0xF9,0xF0,0x01,0xFF,
0xFF,0xFF,0xE3,0xF9,0xF0,0x01,0xFF,0xF7,0xFF,0xFF,0xFD,0xF0,0x03,0xFF,0xF7,0xFF,
0xFF,0xFD,0xF0,0x03,0xFF,0xE3,0xFF,0xFF,0xFF,0xFC,0x03,0xFF,0xFF,0xFF,0xE0,0x3F,
0xFF,0x07,0xFF,0xFF,0xFC,0x00,0x19,0xFF,0x07,0xDF,0xFF,0xFC,0x00,0x03,0xFF,0x0F,
0xDF,0xC1,0xFC,0x01,0xF7,0xFF,0x0F,0x9F,0x00,0x7C,0x01,0xFF,0xDF,0x1F,0x9F,0x00,
0x7C,0x00,0xFF,0x9F,0x1F,0x1F,0x80,0x7C,0x00,0xFF,0x3E,0x3F,0x0F,0x80,0x00,0x00,
0x00,0x3E,0x7E,0x0F,0xE0,0x00,0x00,0x00,0xFE,0x7E,0x0F,0xFF,0xFF,0xFF,0xFF,0xFC,
0xFC,0x07,0xFF,0xFF,0xFF,0xFF,0xFC,0x7C,0x03,0xFF,0xFF,0xFF,0xFF,0xF8,0x38,0x00,
0x7F,0xFF,0xFF,0xFF,0x80,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

void Lcd_DisplayAnySizeChar(u16 x,u16 y,u16 width,u16 height,u16 fcolor,u16 bcolor,const unsigned char*FontBuf)
{
	int i,j;
	int k = 0;
	int n = 0;
	for(j=y;j<y+height;j++)
	{
		for(i=x;i<x+width;i++)
		{
			if(FontBuf[k] & (0x80 >> n))
			{
				Lcd_SetPoint(i,j,fcolor);
			}
			else
			{
				Lcd_SetPoint(i,j,bcolor);
			}
			n++;
			if(n == 8)
			{
				n = 0;
				k++;
			}
		}
	}

}

//英文 0-127
extern const unsigned char ascii_8x16[1536];

void Lcd_DispEnglish(u16 x,u16 y,u16 fcolor,u16 bcolor,const unsigned char ch)
{
	//先计算在数组中的起始位置+16
	int pos = (ch-32)*(8*16/8);
	Lcd_DisplayAnySizeChar(x,y,8,16,fcolor,bcolor,&ascii_8x16[pos]);
}

//显示一串英文
void Lcd_DispEnglish_Str(u16 x,u16 y,u16 fcolor,u16 bcolor,const char* buf)
{
	int i;
	int tmp_x = x;
	for(i=0;buf[i]!='\0';i++)
	{
		int pos = (buf[i]-32)*(8*16)/8;
		Lcd_DisplayAnySizeChar(tmp_x,y,8,16,fcolor,bcolor,&ascii_8x16[pos]);
		tmp_x+=8;
	}
}

//显示一串中英文
//如何判断当前的字符是中文还是英文
// char --> ASCII 0 -127 > 127 < 0 -->中文
void Lcd_DispEnglishorChinese_Str(u16 x,u16 y,u16 fcolor,u16 bcolor,const char* buf)
{
	int i;
	int tmp_x = x;
		for(i=0;buf[i]!='\0';i++)
		{
			if(buf[i]>0 && buf[i]<127)
			{
				int pos = (buf[i]-32)*(8*16)/8;
				Lcd_DisplayAnySizeChar(tmp_x,y,8,16,fcolor,bcolor,&ascii_8x16[pos]);
				tmp_x+=8;
//				printf("6");
			}
			else
			{
				unsigned char Fontbuf[32] = {'\0'};
				int pos = ((buf[i]-0xA1)*94 + buf[i+1]-0xA1)*32;
				i++;
				SPI_FLASH_BufferRead(Fontbuf,pos,32);
				Lcd_DisplayAnySizeChar(tmp_x,y,16,16,Black,White,Fontbuf);
				tmp_x+=16;
//				printf("7");
			}
		}
}



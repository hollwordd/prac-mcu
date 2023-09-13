#include "wifi.h"

u8 g_Wifi_Rec[2048] = {"\0"};
int g_Wifi_Len = 0;
int g_Wifi_Finish = 0;	//代表数据接收完毕 完成置为1

int Wifi_Test(void)
{
	int status = 0;
	//怎么发送AT指令"AT\r\n"
	Usart_Send_Str(USART2,"AT\r\n");
	//等WIFI模块反馈信息	成功/失败
	while(g_Wifi_Finish == 0);	//阻塞当前等待WIFI反馈的信息接收完毕(建议做超时处理)
	g_Wifi_Finish = 0;
	
	//判断反馈信息是否有"OK",有代表启动成功
	//strstr(字符串1，字符串2),判断字符串1中是否出现字符串2 如果存在则返回真
	printf("recv = %s\r\n",g_Wifi_Rec);
	if(strstr((char *)g_Wifi_Rec,"OK"))
	{
		printf("AT测试成功\r\n");
		status = 1;
	}
	else
	{
		printf("AT测试失败\r\n");
		status = 0;
	}
	g_Wifi_Len = 0;
	memset(g_Wifi_Rec,'\0',sizeof(g_Wifi_Rec));
	return status;
}

u8 g_Save_WifiName[10][100] = {'\0'};
int g_Save_Num = 0;

//解析wifi
void Parson_WifiData(u8 *buf)
{
	int i = 11;
	int j = 0;
	while(1)
	{
		//解析wifi名称
		if(strncmp((char *)buf+i-11,"+CWLAP:",7) == 0)	//判断当前wifi是否是完整数据，每一条数据都是以+CWLAP：开头
		{
			//提取wifi名称数据
			for(j=0; ;i++,j++)
			{
				if(buf[i] == '\"')
				{
					break;
				}
				g_Save_WifiName[g_Save_Num][j] = buf[i];
			}
			g_Save_Num++;
			if(g_Save_Num >= 10)
			{
				break;
			}
			while(1)
			{
				i++;
				if(buf[i]=='\r')
				{
					i++;
					if(buf[i]=='\n')
					{
						i += 12;
						break;
					}
				}
			}
		}
		if(strcmp((char *)buf+i,"OK\r\n") == 0 || strcmp((char *)buf+i,"ERROR\r\n") == 0)
		{
			printf("break");
			break;
		}
	}
		for(i=0;i<g_Save_Num;i++)
	{
		printf("%s\r\n",g_Save_WifiName[i]);
	}
}

//列出当前有那些wifi可用
void Find_Wifi(void)
{
	Usart_Send_Str(USART2,"AT+CWLAP\r\n");
	
	while(g_Wifi_Finish == 0);	//阻塞当前等待WIFI反馈的信息接收完毕(建议做超时处理)
	g_Wifi_Finish = 0;
	
	printf("recv = %s\r\n",g_Wifi_Rec);
	g_Wifi_Len = 0;
	memset(g_Wifi_Rec,'\0',sizeof(g_Wifi_Rec));
	
	while(g_Wifi_Finish == 0);	//阻塞当前等待WIFI反馈的信息接收完毕(建议做超时处理)
	g_Wifi_Finish = 0;
	
	printf("recv = %s\r\n",g_Wifi_Rec);
	
	Parson_WifiData(g_Wifi_Rec);
	
	g_Wifi_Len = 0;
	memset(g_Wifi_Rec,'\0',sizeof(g_Wifi_Rec));	
}

//连接wifi
int Connect_Wifi(char *name,char *pwd)
{
	int status = 0;
	//组包
	char str[100] = {'\0'};
	sprintf(str,"AT+CWJAP=\"%s\",\"%s\"\r\n",name,pwd);
	Usart_Send_Str(WIFI_USART,str);
		
		//等待数据接收完毕
	while(g_Wifi_Finish == 0);		//回写命令
	g_Wifi_Finish = 0;
	g_Wifi_Len = 0;
	memset(g_Wifi_Rec,'\0',sizeof(g_Wifi_Rec));	
	
	while(g_Wifi_Finish == 0);	//阻塞当前等待WIFI反馈的信息接收完毕(建议做超时处理)
	g_Wifi_Finish = 0;
	while(1)
	{
		if(strstr((char *)g_Wifi_Rec,"ERROR")||strstr((char *)g_Wifi_Rec,"FAIL")||strstr((char *)g_Wifi_Rec,"OK"))
		{
			break;
		}
	}
	if(strstr((char *)g_Wifi_Rec,"WIFI CONNECTED"))
	{
		status = 1;	
		Lcd_DispEnglishorChinese_Str(10,10,Black,White,"WIFI CONNECTED");
	}
	else if(strstr((char *)g_Wifi_Rec,"ERROR")||strstr((char *)g_Wifi_Rec,"FAIL"))
	{
		status = 0;	
	}		
	g_Wifi_Len = 0;
	memset(g_Wifi_Rec,'\0',sizeof(g_Wifi_Rec));	
		
	return status;
}

//连接服务器：服务端和客户段必须是同一网络
//设置端口 >5000 <1000 http 80 ftp:21 0-65535
//设置IP不能回环地址 一般都是计算机IP地址 win+r 
//关闭防火墙

int Connect_TCPServer(char *type,char *ip,int port)
{
	int status;
	char str[100] = {'\0'};
	
	//组包"AT+CIPSTART=\"%s\",\"%s\",%d\r\n"
	sprintf(str,"AT+CIPSTART=\"%s\",\"%s\",%d\r\n",type,ip,port);
	Usart_Send_Str(WIFI_USART,str);
	
	//等待数据接收完毕
//	while(g_Wifi_Finish == 0);		//回写命令
//	g_Wifi_Finish = 0;
//	g_Wifi_Len = 0;
//	memset(g_Wifi_Rec,'\0',sizeof(g_Wifi_Rec));
//	
//	while(g_Wifi_Finish == 0);		//回写命令
//	g_Wifi_Finish = 0;
//	printf("recv= %s\r\n",g_Wifi_Rec);
//	if(strstr((char *)g_Wifi_Rec,"OK")||strstr((char *)g_Wifi_Rec,"CONNECT"))
//	{
//		status = 1;
//	}
//	else
//	{
//		status = 0;
//	}
	while(1)
	{
		if(strstr((char *)g_Wifi_Rec,"ERROR")||strstr((char *)g_Wifi_Rec,"OK"))
		{
			break;
		}
	}
	if(strstr((char *)g_Wifi_Rec,"OK")||strstr((char *)g_Wifi_Rec,"CONNECT"))
	{
		status = 1;
	}
	else
	{
		status = 0;
	}
	
	printf("recv= %s\r\n",g_Wifi_Rec);
	g_Wifi_Len = 0;
	memset(g_Wifi_Rec,'\0',sizeof(g_Wifi_Rec));
	g_Wifi_Finish = 0;
	return status;
}

//客户端向服务端 发送数据
//len:需要发送数据的长度
//反馈OK+>
//发送需要发送的数据
void SendData_Wifi(char *data)
{
	char str[100] = {'\0'};
	sprintf(str,"AT+CIPSEND=%d\r\n",strlen(data));
	Usart_Send_Str(USART2,str);
	
	while(g_Wifi_Finish == 0);	//等待接收数据（命令+OK+>）
	g_Wifi_Finish = 0;
	//printf("recv == %s\r\n",g_Wifi_Rec);

//	while(g_Wifi_Finish == 0);	//等待接收数据（命令+OK+>）
//	g_Wifi_Finish = 0;
	
	if(strstr((char *)g_Wifi_Rec,">"))
	{
		//代表可以发送数据
		Usart_Send_Str(USART2,data);
		//等待当前服务器反馈信息 数据是否发送成功 如果发送成功 返回SEND OK
		while(1)
		{
			if(strstr((char *)g_Wifi_Rec,"SEND OK")||strstr((char *)g_Wifi_Rec,"ERROR"))
			{
				break;
			}
		}
//		while(g_Wifi_Finish == 0);	//等待接收数据（SEND OK）
//		g_Wifi_Finish = 0;
//		while(g_Wifi_Finish == 0);	//等待接收数据（SEND OK）
//		g_Wifi_Finish = 0;
		if(strstr((char *)g_Wifi_Rec,"SEND OK"))
		{
			printf("发送成功\r\n");
		}
	}
	else
	{
		
	}
	//printf("recv == %s\r\n",g_Wifi_Rec);
	g_Wifi_Finish = 0; 
	g_Wifi_Len = 0;
	memset(g_Wifi_Rec,'\0',sizeof(g_Wifi_Rec));
	
}
 
void Parson_RecvData(u8 *data,char *saveRevData)
{
	char lenstr[5] = {"\0"};
	int i = 0;
	int j = 0;
	int len = 0;
	int index = 0;
	if(strncmp((char *)data,"\r\n+IPD",6) == 0)
	{
		//判断数据的协议 是否是以\r\n+IPD开头 接收数据协议是正常的，否则接收无效
		//先将数据长度提取出来 \r\n+IPD,11:hello world
		for(i=7;*(data+i)!=':';i++)
		{
			lenstr[i-7] = *(data+i);
		}
		index = i+1;
		//需要将提取出来的字符类型转换成int
		len = atoi(lenstr);
		//提取实际数据
		for(i=index;i<index+len;i++)
		{
			saveRevData[j++] = data[i];
		}
		printf("提取的数据为：%s\r\n",saveRevData);
	}
}

void RecvData_Wifi(char *buf)
{
	while(g_Wifi_Finish == 0);	//阻塞当前等待WIFI反馈的信息接收完毕(建议做超时处理)
	g_Wifi_Finish = 0;
	//printf("recv == %s\r\n",g_Wifi_Rec);
	Parson_RecvData(g_Wifi_Rec,buf);
	//printf("%s\r\n",buf);
	g_Wifi_Len = 0;
	memset(g_Wifi_Rec,'\0',sizeof(g_Wifi_Rec));
	Lcd_DispEnglishorChinese_Str(10,30,Black,White,buf);

}

void Quit_Wifi(void)
{
	Usart_Send_Str(USART2,"AT+CWQAP\r\n");
}

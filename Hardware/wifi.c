#include "wifi.h"

u8 g_Wifi_Rec[2048] = {"\0"};
int g_Wifi_Len = 0;
int g_Wifi_Finish = 0;	//�������ݽ������ �����Ϊ1

int Wifi_Test(void)
{
	int status = 0;
	//��ô����ATָ��"AT\r\n"
	Usart_Send_Str(USART2,"AT\r\n");
	//��WIFIģ�鷴����Ϣ	�ɹ�/ʧ��
	while(g_Wifi_Finish == 0);	//������ǰ�ȴ�WIFI��������Ϣ�������(��������ʱ����)
	g_Wifi_Finish = 0;
	
	//�жϷ�����Ϣ�Ƿ���"OK",�д��������ɹ�
	//strstr(�ַ���1���ַ���2),�ж��ַ���1���Ƿ�����ַ���2 ��������򷵻���
	printf("recv = %s\r\n",g_Wifi_Rec);
	if(strstr((char *)g_Wifi_Rec,"OK"))
	{
		printf("AT���Գɹ�\r\n");
		status = 1;
	}
	else
	{
		printf("AT����ʧ��\r\n");
		status = 0;
	}
	g_Wifi_Len = 0;
	memset(g_Wifi_Rec,'\0',sizeof(g_Wifi_Rec));
	return status;
}

u8 g_Save_WifiName[10][100] = {'\0'};
int g_Save_Num = 0;

//����wifi
void Parson_WifiData(u8 *buf)
{
	int i = 11;
	int j = 0;
	while(1)
	{
		//����wifi����
		if(strncmp((char *)buf+i-11,"+CWLAP:",7) == 0)	//�жϵ�ǰwifi�Ƿ����������ݣ�ÿһ�����ݶ�����+CWLAP����ͷ
		{
			//��ȡwifi��������
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

//�г���ǰ����Щwifi����
void Find_Wifi(void)
{
	Usart_Send_Str(USART2,"AT+CWLAP\r\n");
	
	while(g_Wifi_Finish == 0);	//������ǰ�ȴ�WIFI��������Ϣ�������(��������ʱ����)
	g_Wifi_Finish = 0;
	
	printf("recv = %s\r\n",g_Wifi_Rec);
	g_Wifi_Len = 0;
	memset(g_Wifi_Rec,'\0',sizeof(g_Wifi_Rec));
	
	while(g_Wifi_Finish == 0);	//������ǰ�ȴ�WIFI��������Ϣ�������(��������ʱ����)
	g_Wifi_Finish = 0;
	
	printf("recv = %s\r\n",g_Wifi_Rec);
	
	Parson_WifiData(g_Wifi_Rec);
	
	g_Wifi_Len = 0;
	memset(g_Wifi_Rec,'\0',sizeof(g_Wifi_Rec));	
}

//����wifi
int Connect_Wifi(char *name,char *pwd)
{
	int status = 0;
	//���
	char str[100] = {'\0'};
	sprintf(str,"AT+CWJAP=\"%s\",\"%s\"\r\n",name,pwd);
	Usart_Send_Str(WIFI_USART,str);
		
		//�ȴ����ݽ������
	while(g_Wifi_Finish == 0);		//��д����
	g_Wifi_Finish = 0;
	g_Wifi_Len = 0;
	memset(g_Wifi_Rec,'\0',sizeof(g_Wifi_Rec));	
	
	while(g_Wifi_Finish == 0);	//������ǰ�ȴ�WIFI��������Ϣ�������(��������ʱ����)
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

//���ӷ�����������˺Ϳͻ��α�����ͬһ����
//���ö˿� >5000 <1000 http 80 ftp:21 0-65535
//����IP���ܻػ���ַ һ�㶼�Ǽ����IP��ַ win+r 
//�رշ���ǽ

int Connect_TCPServer(char *type,char *ip,int port)
{
	int status;
	char str[100] = {'\0'};
	
	//���"AT+CIPSTART=\"%s\",\"%s\",%d\r\n"
	sprintf(str,"AT+CIPSTART=\"%s\",\"%s\",%d\r\n",type,ip,port);
	Usart_Send_Str(WIFI_USART,str);
	
	//�ȴ����ݽ������
//	while(g_Wifi_Finish == 0);		//��д����
//	g_Wifi_Finish = 0;
//	g_Wifi_Len = 0;
//	memset(g_Wifi_Rec,'\0',sizeof(g_Wifi_Rec));
//	
//	while(g_Wifi_Finish == 0);		//��д����
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

//�ͻ��������� ��������
//len:��Ҫ�������ݵĳ���
//����OK+>
//������Ҫ���͵�����
void SendData_Wifi(char *data)
{
	char str[100] = {'\0'};
	sprintf(str,"AT+CIPSEND=%d\r\n",strlen(data));
	Usart_Send_Str(USART2,str);
	
	while(g_Wifi_Finish == 0);	//�ȴ��������ݣ�����+OK+>��
	g_Wifi_Finish = 0;
	//printf("recv == %s\r\n",g_Wifi_Rec);

//	while(g_Wifi_Finish == 0);	//�ȴ��������ݣ�����+OK+>��
//	g_Wifi_Finish = 0;
	
	if(strstr((char *)g_Wifi_Rec,">"))
	{
		//������Է�������
		Usart_Send_Str(USART2,data);
		//�ȴ���ǰ������������Ϣ �����Ƿ��ͳɹ� ������ͳɹ� ����SEND OK
		while(1)
		{
			if(strstr((char *)g_Wifi_Rec,"SEND OK")||strstr((char *)g_Wifi_Rec,"ERROR"))
			{
				break;
			}
		}
//		while(g_Wifi_Finish == 0);	//�ȴ��������ݣ�SEND OK��
//		g_Wifi_Finish = 0;
//		while(g_Wifi_Finish == 0);	//�ȴ��������ݣ�SEND OK��
//		g_Wifi_Finish = 0;
		if(strstr((char *)g_Wifi_Rec,"SEND OK"))
		{
			printf("���ͳɹ�\r\n");
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
		//�ж����ݵ�Э�� �Ƿ�����\r\n+IPD��ͷ ��������Э���������ģ����������Ч
		//�Ƚ����ݳ�����ȡ���� \r\n+IPD,11:hello world
		for(i=7;*(data+i)!=':';i++)
		{
			lenstr[i-7] = *(data+i);
		}
		index = i+1;
		//��Ҫ����ȡ�������ַ�����ת����int
		len = atoi(lenstr);
		//��ȡʵ������
		for(i=index;i<index+len;i++)
		{
			saveRevData[j++] = data[i];
		}
		printf("��ȡ������Ϊ��%s\r\n",saveRevData);
	}
}

void RecvData_Wifi(char *buf)
{
	while(g_Wifi_Finish == 0);	//������ǰ�ȴ�WIFI��������Ϣ�������(��������ʱ����)
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

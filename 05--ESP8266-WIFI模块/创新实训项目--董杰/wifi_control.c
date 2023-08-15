#include <delay.h>
#include <wifi_control.h>
#include <UART.H>
#include <smg.c>
#include <ds18b20.h>
#define u8 unsigned char
#define u16 unsigned int
//ESP8266 WIFI�������ݵ�APP
//pbuf������
void ESP8266_SendData(unsigned char *pbuf)
{
	ESP8266_SendCmd("AT+CIPSEND=0,16");
	delay_ms(10);
	while(*pbuf!='\0') //�����ո�����ѭ��	
	{
		UART_SendData(*pbuf);
		delay_10us(5);
		pbuf++;	
	}
	UART_SendData('\n');//����	
//	delay_ms(10);
}

//ESP8266 WIFI����ATָ��
//pbuf��ATָ��ַ�����ʽ���磺"AT"
void ESP8266_SendCmd(unsigned char *pbuf)
{
	while(*pbuf!='\0') //�����ո�����ѭ��	
	{
		UART_SendData(*pbuf);
		delay_10us(5);
		pbuf++;	
	}
	delay_10us(5);
	UART_SendData('\r');//�س�
	delay_10us(5);
	UART_SendData('\n');//����
	delay_ms(10);
}

//ESP8266-WIFIģ�鹤��ģʽ��ʼ��
void ESP8266_ModeInit(void)
{
	ESP8266_SendCmd("AT+CWMODE=2");//����·����ģʽ 1 staTIonģʽ 2 AP�� ·����ģʽ 3 station+AP���ģʽ
	ESP8266_SendCmd("AT+CWSAP=\"ESP8266\",\"123456\",11,0"); //����WIFI�ȵ���������
	ESP8266_SendCmd("AT+CIPAP=\"192.168.4.1\"");
	ESP8266_SendCmd("AT+RST");//��������wifiģ��
	delay_ms(2000);
	ESP8266_SendCmd("AT+CIPMUX=1");	//����������ģʽ�����������ͻ��˽���
	ESP8266_SendCmd("AT+CIPSERVER=1,8080");	//����TCP/IP �˿�Ϊ8080 ʵ�ֻ����������	
} 


////WIFI����
//void wifi_control(void)
//{
//	u16 i=0;
//	u8 temp_buf[5];
//	int temp_value;
//	u8 wifi_send_buf[7];

//	while(1)
//	{
//		i++;
//		if(i%50==0)//���һ��ʱ���ȡ�¶�ֵ�����ʱ��Ҫ�����¶ȴ�����ת���¶�ʱ��
//			temp_value=read_temp()*10;//�����¶�ֵС����һλ
//		if(temp_value<0)//���¶�
//		{
//			temp_value=-temp_value;
//			temp_buf[0]=0x40;//��ʾ����
//			wifi_send_buf[0]='-';	
//		}
//		else
//		{
//			temp_buf[0]=0x00;//����ʾ
//			wifi_send_buf[0]='+';
//		}		
//		temp_buf[1]=gsmg_code[temp_value/1000];//��λ
//		temp_buf[2]=gsmg_code[temp_value%1000/100];//ʮλ
//		temp_buf[3]=gsmg_code[temp_value%1000%100/10]|0x80;//��λ+С����
//		temp_buf[4]=gsmg_code[temp_value%1000%100%10];//С�����һλ
//		smg_display(temp_buf,4);
//		if(i%200==0)//���һ��ʱ�佫��ȡ���¶ȷ��͵�APP
//		{
//			wifi_send_buf[1]=temp_value/1000+0x30;
//			wifi_send_buf[2]=temp_value%1000/100+0x30;
//			wifi_send_buf[3]=temp_value%1000%100/10+0x30;
//			wifi_send_buf[4]='.';
//			wifi_send_buf[5]=temp_value%1000%100%10+0x30;
//			wifi_send_buf[6]='\0';
//			ESP8266_SendData(wifi_send_buf);//ͨ�����ڷ����¶����ݵ�APP
//		}			
//	}	
//}


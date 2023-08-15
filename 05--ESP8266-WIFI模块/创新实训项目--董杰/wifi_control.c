#include <delay.h>
#include <wifi_control.h>
#include <UART.H>
#include <smg.c>
#include <ds18b20.h>
#define u8 unsigned char
#define u16 unsigned int
//ESP8266 WIFI发送数据到APP
//pbuf：数据
void ESP8266_SendData(unsigned char *pbuf)
{
	ESP8266_SendCmd("AT+CIPSEND=0,16");
	delay_ms(10);
	while(*pbuf!='\0') //遇到空格跳出循环	
	{
		UART_SendData(*pbuf);
		delay_10us(5);
		pbuf++;	
	}
	UART_SendData('\n');//换行	
//	delay_ms(10);
}

//ESP8266 WIFI发送AT指令
//pbuf：AT指令，字符串格式，如："AT"
void ESP8266_SendCmd(unsigned char *pbuf)
{
	while(*pbuf!='\0') //遇到空格跳出循环	
	{
		UART_SendData(*pbuf);
		delay_10us(5);
		pbuf++;	
	}
	delay_10us(5);
	UART_SendData('\r');//回车
	delay_10us(5);
	UART_SendData('\n');//换行
	delay_ms(10);
}

//ESP8266-WIFI模块工作模式初始化
void ESP8266_ModeInit(void)
{
	ESP8266_SendCmd("AT+CWMODE=2");//设置路由器模式 1 staTIon模式 2 AP点 路由器模式 3 station+AP混合模式
	ESP8266_SendCmd("AT+CWSAP=\"ESP8266\",\"123456\",11,0"); //设置WIFI热点名及密码
	ESP8266_SendCmd("AT+CIPAP=\"192.168.4.1\"");
	ESP8266_SendCmd("AT+RST");//重新启动wifi模块
	delay_ms(2000);
	ESP8266_SendCmd("AT+CIPMUX=1");	//开启多连接模式，允许多个各客户端接入
	ESP8266_SendCmd("AT+CIPSERVER=1,8080");	//启动TCP/IP 端口为8080 实现基于网络控制	
} 


////WIFI控制
//void wifi_control(void)
//{
//	u16 i=0;
//	u8 temp_buf[5];
//	int temp_value;
//	u8 wifi_send_buf[7];

//	while(1)
//	{
//		i++;
//		if(i%50==0)//间隔一段时间读取温度值，间隔时间要大于温度传感器转换温度时间
//			temp_value=read_temp()*10;//保留温度值小数后一位
//		if(temp_value<0)//负温度
//		{
//			temp_value=-temp_value;
//			temp_buf[0]=0x40;//显示负号
//			wifi_send_buf[0]='-';	
//		}
//		else
//		{
//			temp_buf[0]=0x00;//不显示
//			wifi_send_buf[0]='+';
//		}		
//		temp_buf[1]=gsmg_code[temp_value/1000];//百位
//		temp_buf[2]=gsmg_code[temp_value%1000/100];//十位
//		temp_buf[3]=gsmg_code[temp_value%1000%100/10]|0x80;//个位+小数点
//		temp_buf[4]=gsmg_code[temp_value%1000%100%10];//小数点后一位
//		smg_display(temp_buf,4);
//		if(i%200==0)//间隔一段时间将读取的温度发送到APP
//		{
//			wifi_send_buf[1]=temp_value/1000+0x30;
//			wifi_send_buf[2]=temp_value%1000/100+0x30;
//			wifi_send_buf[3]=temp_value%1000%100/10+0x30;
//			wifi_send_buf[4]='.';
//			wifi_send_buf[5]=temp_value%1000%100%10+0x30;
//			wifi_send_buf[6]='\0';
//			ESP8266_SendData(wifi_send_buf);//通过串口发送温度数据到APP
//		}			
//	}	
//}


#include <REGX51.H>
#include <stdio.h>
#include <stdlib.h>
#include <UART.H>
#include <wifi_control.h>
#include <key.h>
#include <delay.h>
#include <12864.h>
#include <smg.h>
#include <ds18b20.h>
//*****变量区********//
int  temperature;    //温度变量
unsigned char timer[]={55,59,23};//时钟
unsigned char timer2[]={0,58,22};//设置吃药时间阿莫西林
unsigned char timer3[]={0,58,22};//板蓝根吃药时间
//定义LED1管脚
sbit LED1=P2^0;
sbit Buzzer=P1^5;//蜂鸣器端口
//定义WIFI控制命令
#define LED1_ON_CMD			'1'
#define LED1_OFF_CMD		'2'
#define amoxilin			  '3'
#define banlangen		    '4'
u8 UART_RX_BUF[20];//最大接收数据缓存
//温度保存*******
u16 j=0;
u8 temp_buf[5];//温度缓存
int temp_value;
u8 wifi_send_buf[7];

sbit led=P2^0;//led闪烁提醒吃药
unsigned char keynum=0;
unsigned char i=0;//发送计次
unsigned char control=0;//是否进入设置时间和吃药量模式
unsigned char chiyaoliang1=0;//吃药量1//板蓝根
unsigned char chiyaoliang2=0;//吃药量2//阿莫新林
bit timeflag=0;//如果时间到了且按时吃药则置1
unsigned char chiyao_count=0;//吃药量
//****************//
void key_control()
{
	switch(keynum)
	{
		case 5:
			keynum=0;
		  chiyao_count++;
		  if(chiyao_count==chiyaoliang1)
			{
				chiyao_count=0;
				ESP8266_SendData("已完成板蓝根药量");
			}
			break;
		case 6:
		keynum=0;
		chiyao_count++;
		if(chiyao_count==chiyaoliang2)
		{
			chiyao_count=0;
			ESP8266_SendData("已完成阿莫西林药量");
		}
		break;
		case 2:
			{
				ES=0;
				keynum=0;
				led=1;
				ES=1;
			  ESP8266_SendData("正在吃药");	//通过串口发巳数据到APP
				timeflag=1;
			}
			break;
		case 3:
			break;
		case 4:
		{
//			addr_12864(4,5);
//			write_dat(shuzi[tim[0]]);
//			write_dat(shuzi[tim[1]]);
//			write_dat(shuzi[tim[2]]);
//			write_dat(shuzi[tim[3]]);
//			write_dat(shuzi[tim[4]]);
		}
			break;
		case 13:        //调时++
		{
			if(control==1)
			{
				timer2[2]++;
				if(timer2[2]>23){timer2[2]=0;}
				addr_12864(4,5);
				write_dat(shuzi[timer2[2]/10]);
				write_dat(shuzi[timer2[2]%10]);
				keynum=0;
			}
			else if(control==2)
			{
				timer3[2]++;
				if(timer3[2]>23){timer3[2]=0;}
				addr_12864(4,5);
				write_dat(shuzi[timer3[2]/10]);
				write_dat(shuzi[timer3[2]%10]);
				keynum=0;
			}
		}			
		break;
		case 14:        //调分++
		{
			if(control==1)//阿莫西林调分
			{
				timer2[1]++;
				if(timer2[1]>59){timer2[1]=0;}
        addr_12864(4,7);
				write_dat(shuzi[timer2[1]/10]);
				write_dat(shuzi[timer2[1]%10]);
				keynum=0;
			}	
			else if(control==2)//板蓝根调分
			{
				timer3[1]++;
				if(timer3[1]>59){timer3[1]=0;}
        addr_12864(4,7);
				write_dat(shuzi[timer3[1]/10]);
				write_dat(shuzi[timer3[1]%10]);
				keynum=0;
			}	
		}			
		break;
		case 15:        //设置吃药量
		{
			if(control==1)
			{
				chiyaoliang1++;
				if(chiyaoliang1>9){chiyaoliang1=0;}
				addr_12864(3,3);
				write_dat(shuzi[chiyaoliang1]);
				keynum=0;
			}
			else if(control==2)
			{
				chiyaoliang2++;
				if(chiyaoliang2>9){chiyaoliang2=0;}
				addr_12864(3,3);
				write_dat(shuzi[chiyaoliang2]);
				keynum=0;
			}
		}
			break;
		case 16:        //设置确认，清除显示
		{
			keynum=0;
			if(control==1)
			{
				write_string(3,3,"  ");
				write_string(4,0,"        ");
				write_string(4,5,"        ");
			}
			else if(control==2)
			{
				write_string(3,3,"  ");
				write_string(4,0,"        ");
				write_string(4,5,"        ");
			}
		}
			break;
		default:
			break;
	}
}

/******************时间芯片显示函数**********************/	   
void ds1302_dis()
{
			addr_12864(2,3);	  //地址转换
			write_dat(' ');		
			write_dat(shuzi[timer[2]/10]);	
			write_dat(shuzi[timer[2]%10]);	
			write_dat(':');	
			write_dat(shuzi[timer[1]/10]);	
			write_dat(shuzi[timer[1]%10]);	
			write_dat(':');	
			write_dat(shuzi[timer[0]/10]);	
			write_dat(shuzi[timer[0]%10]);
	   ///******温度*******//
	    addr_12864(3,4);
	    write_dat(' ');	
			write_dat(shuzi[temp_value/100]);	
			write_dat(shuzi[temp_value%100/10]);	
			write_dat('.');	
			write_dat(shuzi[temp_value%10]);		
}

void Timer0Init(void)
{
	TMOD &= 0xF0;		//???????
	TMOD |= 0x01;		//???????
	TL0 = 0x18;		//??????
	TH0 = 0xFC;		//??????
	TF0 = 0;		//??TF0??
	TR0 = 1;		//???0????
	ET0=1;
	EA=1;
	PT0=0;
}
void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//??????
	T0Count++;
	if(T0Count>=1000)	//定时器分频  1s
	{
		T0Count=0;
		timer[0]++;			//1??,Sec??
		if(timer[0]>=60)
		{
			timer[0]=0;		//60??,Sec?0,Min??
			timer[1]++;
			if(timer[1]>=60)
			{
				timer[1]=0;	//60???,Min?0,Hour??
				timer[2]++;
				if(timer[2]>=24)
				{
					timer[2]=0;	//24???,Hour?0
				}
			}
		}
	}
}


//******主程序*******//
void main()
{
  UART_Init();
	init_12864();
  Timer0Init();
	ESP8266_ModeInit();
	//ES=1;//允许串口中断
	init_12864_dis();
	while(1)
	{
		keynum=MatrixKey();
		if((timer2[1]==timer[1]&&timer2[2]==timer[2]&&timeflag==0)||(timer3[1]==timer[1]&&timer3[2]==timer[2]&&timeflag==0))//如果设定时间到了，提醒吃药
		{
			 ESP8266_SendData("吃药时间到了");
			 Buzzer=~Buzzer;
			 {
				ES=0;
				led=~led;
				ES=1;
				ESP8266_SendData("请按时吃药");
			 }
		}
		key_control();
		j++;
		if(j%25==0)//间隔一段时间读取温度值，间隔时间要大于温度传感器转换温度时间
		{
			ES=0;
		  temp_value=ds18b20_read_temperture()*10;//保留温度值小数后一位
			ds1302_dis();
			ES=1;
		}
		temp_buf[1]=gsmg_code[temp_value/1000];//百位
		temp_buf[2]=gsmg_code[temp_value%1000/100];//十位
		temp_buf[3]=gsmg_code[temp_value%1000%100/10]|0x80;//个位+小数点
		temp_buf[4]=gsmg_code[temp_value%1000%100%10];//小数点后一位
		smg_display(temp_buf,4);
		
		if(j%200==0)//间隔一段时间将读取的温度发送到APP
		{
			wifi_send_buf[1]=temp_value/1000+0x30;
			wifi_send_buf[2]=temp_value%1000/100+0x30;
			wifi_send_buf[3]=temp_value%1000%100/10+0x30;
			wifi_send_buf[4]='.';
			wifi_send_buf[5]=temp_value%1000%100%10+0x30;
			wifi_send_buf[6]='\0';
			ESP8266_SendData(wifi_send_buf);//通过串口发送温度数据到APP
		}

/////手机app按键控制继电器   串口中断   取某一药品   app取多少次药量 时间报警 
		
		
/////lcd屏幕录入药品名称  药品使用时间  药品用量  

	}
}
//串口中断服务函数
//接收手机APP发送的信号后控制板载资源
void UART_Routine() interrupt 4
{
	u8 i;//数组遍历
	if(RI)
	{
		RI=0;
		UART_RX_BUF[i]=SBUF;//读取接收到的数据
		if(UART_RX_BUF[0]=='+')i++;
		else i=0;
		if(i==10)
		{
			i=0;
			//WIFI控制
			if(UART_RX_BUF[9]==LED1_ON_CMD)
			{ 
				LED1=0;
			}
			else if(UART_RX_BUF[9]==LED1_OFF_CMD)
			{ 
				LED1=1;
				write_string(3,3,"  ");
				write_string(4,0,"        ");
				write_string(4,5,"        ");
			}	
			else if(UART_RX_BUF[9]==amoxilin)//阿莫西林
			{ 
				write_string(3,3,"0");
				write_string(4,0,"阿莫西林");
				write_string(4,5,"00: 00");
				control=1;
			}
			else if(UART_RX_BUF[9]==banlangen)//板蓝根
			{ 
				write_string(3,3,"0");
				write_string(4,0,"板蓝根");
				write_string(4,5,"00: 00");
				control=2;
			}
		}	
	}	
}
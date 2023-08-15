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
//*****������********//
int  temperature;    //�¶ȱ���
unsigned char timer[]={55,59,23};//ʱ��
unsigned char timer2[]={0,58,22};//���ó�ҩʱ�䰢Ī����
unsigned char timer3[]={0,58,22};//��������ҩʱ��
//����LED1�ܽ�
sbit LED1=P2^0;
sbit Buzzer=P1^5;//�������˿�
//����WIFI��������
#define LED1_ON_CMD			'1'
#define LED1_OFF_CMD		'2'
#define amoxilin			  '3'
#define banlangen		    '4'
u8 UART_RX_BUF[20];//���������ݻ���
//�¶ȱ���*******
u16 j=0;
u8 temp_buf[5];//�¶Ȼ���
int temp_value;
u8 wifi_send_buf[7];

sbit led=P2^0;//led��˸���ѳ�ҩ
unsigned char keynum=0;
unsigned char i=0;//���ͼƴ�
unsigned char control=0;//�Ƿ��������ʱ��ͳ�ҩ��ģʽ
unsigned char chiyaoliang1=0;//��ҩ��1//������
unsigned char chiyaoliang2=0;//��ҩ��2//��Ī����
bit timeflag=0;//���ʱ�䵽���Ұ�ʱ��ҩ����1
unsigned char chiyao_count=0;//��ҩ��
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
				ESP8266_SendData("����ɰ�����ҩ��");
			}
			break;
		case 6:
		keynum=0;
		chiyao_count++;
		if(chiyao_count==chiyaoliang2)
		{
			chiyao_count=0;
			ESP8266_SendData("����ɰ�Ī����ҩ��");
		}
		break;
		case 2:
			{
				ES=0;
				keynum=0;
				led=1;
				ES=1;
			  ESP8266_SendData("���ڳ�ҩ");	//ͨ�����ڷ������ݵ�APP
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
		case 13:        //��ʱ++
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
		case 14:        //����++
		{
			if(control==1)//��Ī���ֵ���
			{
				timer2[1]++;
				if(timer2[1]>59){timer2[1]=0;}
        addr_12864(4,7);
				write_dat(shuzi[timer2[1]/10]);
				write_dat(shuzi[timer2[1]%10]);
				keynum=0;
			}	
			else if(control==2)//����������
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
		case 15:        //���ó�ҩ��
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
		case 16:        //����ȷ�ϣ������ʾ
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

/******************ʱ��оƬ��ʾ����**********************/	   
void ds1302_dis()
{
			addr_12864(2,3);	  //��ַת��
			write_dat(' ');		
			write_dat(shuzi[timer[2]/10]);	
			write_dat(shuzi[timer[2]%10]);	
			write_dat(':');	
			write_dat(shuzi[timer[1]/10]);	
			write_dat(shuzi[timer[1]%10]);	
			write_dat(':');	
			write_dat(shuzi[timer[0]/10]);	
			write_dat(shuzi[timer[0]%10]);
	   ///******�¶�*******//
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
	TL0 = 0x18;		//���ö�ʱ��ֵ
	TH0 = 0xFC;		//??????
	T0Count++;
	if(T0Count>=1000)	//��ʱ����Ƶ  1s
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


//******������*******//
void main()
{
  UART_Init();
	init_12864();
  Timer0Init();
	ESP8266_ModeInit();
	//ES=1;//�������ж�
	init_12864_dis();
	while(1)
	{
		keynum=MatrixKey();
		if((timer2[1]==timer[1]&&timer2[2]==timer[2]&&timeflag==0)||(timer3[1]==timer[1]&&timer3[2]==timer[2]&&timeflag==0))//����趨ʱ�䵽�ˣ����ѳ�ҩ
		{
			 ESP8266_SendData("��ҩʱ�䵽��");
			 Buzzer=~Buzzer;
			 {
				ES=0;
				led=~led;
				ES=1;
				ESP8266_SendData("�밴ʱ��ҩ");
			 }
		}
		key_control();
		j++;
		if(j%25==0)//���һ��ʱ���ȡ�¶�ֵ�����ʱ��Ҫ�����¶ȴ�����ת���¶�ʱ��
		{
			ES=0;
		  temp_value=ds18b20_read_temperture()*10;//�����¶�ֵС����һλ
			ds1302_dis();
			ES=1;
		}
		temp_buf[1]=gsmg_code[temp_value/1000];//��λ
		temp_buf[2]=gsmg_code[temp_value%1000/100];//ʮλ
		temp_buf[3]=gsmg_code[temp_value%1000%100/10]|0x80;//��λ+С����
		temp_buf[4]=gsmg_code[temp_value%1000%100%10];//С�����һλ
		smg_display(temp_buf,4);
		
		if(j%200==0)//���һ��ʱ�佫��ȡ���¶ȷ��͵�APP
		{
			wifi_send_buf[1]=temp_value/1000+0x30;
			wifi_send_buf[2]=temp_value%1000/100+0x30;
			wifi_send_buf[3]=temp_value%1000%100/10+0x30;
			wifi_send_buf[4]='.';
			wifi_send_buf[5]=temp_value%1000%100%10+0x30;
			wifi_send_buf[6]='\0';
			ESP8266_SendData(wifi_send_buf);//ͨ�����ڷ����¶����ݵ�APP
		}

/////�ֻ�app�������Ƽ̵���   �����ж�   ȡĳһҩƷ   appȡ���ٴ�ҩ�� ʱ�䱨�� 
		
		
/////lcd��Ļ¼��ҩƷ����  ҩƷʹ��ʱ��  ҩƷ����  

	}
}
//�����жϷ�����
//�����ֻ�APP���͵��źź���ư�����Դ
void UART_Routine() interrupt 4
{
	u8 i;//�������
	if(RI)
	{
		RI=0;
		UART_RX_BUF[i]=SBUF;//��ȡ���յ�������
		if(UART_RX_BUF[0]=='+')i++;
		else i=0;
		if(i==10)
		{
			i=0;
			//WIFI����
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
			else if(UART_RX_BUF[9]==amoxilin)//��Ī����
			{ 
				write_string(3,3,"0");
				write_string(4,0,"��Ī����");
				write_string(4,5,"00: 00");
				control=1;
			}
			else if(UART_RX_BUF[9]==banlangen)//������
			{ 
				write_string(3,3,"0");
				write_string(4,0,"������");
				write_string(4,5,"00: 00");
				control=2;
			}
		}	
	}	
}
#include <reg52.h>	         //���õ�Ƭ��ͷ�ļ�
#define uchar unsigned char  //�޷����ַ��� �궨��	������Χ0~255
#define uint  unsigned int	 //�޷������� �궨��	������Χ0~65535
#include "nongli.h"

uchar code shuzi[]="0123456789ABCDEF";
char  code shuzi_c[]="0123456789ABCDEF";//uchar xdata dat_12864[64][16];

sbit rs=P2^6;	 //�Ĵ���ѡ���ź� H:���ݼĴ���  	L:ָ��Ĵ���
sbit rw=P2^5;	 //�Ĵ���ѡ���ź� H:���ݼĴ���  	L:ָ��Ĵ���
sbit e =P2^7;	 //Ƭѡ�ź�   �½��ش���

sbit beep = P3^7;	//������IO��

sbit dq   = P3^1;	  //18b20 IO�ڵĶ���
uint temperature ;    //�¶ȱ���


bit flag_200ms = 1;
uchar menu_1;
uchar flag_clock_en;	//���ӱ�־λ
uchar open1;	//���ӿ���־λ
uchar flag_c_en;
bit flag_100ms;

uchar n_nian,n_yue,n_ri;

/********************************************************************
* ���� : delay_1ms()
* ���� : ��ʱ1ms����
* ���� : q
* ��� : ��
***********************************************************************/
void delay_1ms(uint q)
{
	uint i,j;
	for(i=0;i<q;i++)
		for(j=0;j<115;j++);
}


#include "ds1302.h"


/*********************��ʱ��0����ʱ��1��ʼ��******************/
void time0_init()	  
{					 
	EA  = 1;	 	  //�����ж�
	TMOD = 0X11;	  //��ʱ��0����ʱ��1������ʽ1
	ET0 = 1;		  //����ʱ��0�ж� 
	TR0 = 1;		  //����ʱ��0��ʱ
}
						   
					   
/************ ��ʱ����  *****************/
void delay_uint(uint z)
{
   while(z--);
}

/***************�������� ��λ���λ ��λ���λ*****************/
uchar change_dat(uchar dat)
{
	uchar i,value;
	for(i=0;i<8;i++)
	{	
		value <<= 1; 
		if((dat & 0x01) == 0x01)
			value |= 0x01;
		dat >>= 1;	
	}
	return value;
}

/************ д�����  *****************/
void write_com(uchar com)
{
 P0=com;
   //P0=change_dat(com);
   rw=0;
   rs=0;
   delay_uint(25);
   e=1;
   delay_uint(50);
   e=0;
}
/************ д���ݺ���  *****************/
void write_dat(uchar dat)
{
P0=dat;
   //P0=change_dat(dat);;
   rw=0;
   rs=1;
   delay_uint(25);
   e=1;
   delay_uint(50);
   e=0;
}

/**********************************************************
*������: addr_12864
*����:д��ַ
*˵��:д����ʱҪд����ַ
*����:��
*����:��
**********************************************************/
void addr_12864(uchar hang,uchar lie)	  //��ַת��
{
	uchar address;
	switch(hang)
	{
		case 1: address=0x80 + lie ;
				break;
		case 2: address=0x90 + lie ;
				break;
		case 3: address=0x88 + lie ;
				break;
		case 4: address=0x98 + lie ;
				break;
		default : 
				break;
	}	
	write_com(address);
}

/**********************************************************
*������: write_shu16
*����:��12864����ʾ16��������
*˵��:
*����:��
*����:��
**********************************************************/
void write_shu16(uchar hang,uchar lie,uchar dat)
{
	addr_12864(hang,lie);	  //��ַת��	
	write_dat(shuzi[dat/16]);	
	write_dat(shuzi[dat%16]);	
}

/**********************************************************
*������: write_string
*����:��12864����ʾ�ַ�����
*˵��:
*����:��
*����:��
**********************************************************/

void write_string(uchar hang,uchar lie,uchar *p)
{
	addr_12864(hang,lie);	  //��ַת��
	while(*p != '\0')
	{
		write_dat(*p);
		p++;
		delay_uint(50);
	}
}


/*****************���ƹ�꺯��********************/
void write_guanbiao(uchar hang,uchar lie,uchar date)
{		
	addr_12864(hang,lie);	 
	if(date == 1)
		write_com(0x0f);     //��ʾ��겢����˸	
	else 
		write_com(0xc0);   //�رչ��
}	 


/************12864Һ����ʼ������*****************/
void init_12864()
{
   write_com(0x30);
   write_com(0x0c);
   write_com(0x01);
}

/*------------------------���12864------------------------------*/
void clear_12864()
{
	write_string(1,0,"                ");	//1��	
	write_string(2,0,"                ");	//2��	
	write_string(3,0,"                ");	//3��	
	write_string(4,0,"                ");	//4��	
}


/************��ʼ����ʾ*****************/
void init_12864_dis()
{
	write_string(1,0,"  �๦�ܵ�������");
	write_string(2,0,"����2000��00��00");
	write_string(3,0,"ũ��00-00 ����  ");
	write_string(4,0," 00:00:00 00.0��");
	write_guanbiao(1,1,0);
}

/************��ʼ��������ʾ*****************/
void init_12864_set1()
{
	clear_12864();
	write_string(1,0,"2013��00��00��  ");
	write_string(2,0,"  00ʱ00��00��  ");
	write_string(3,0,"����  ����01    ");
	write_string(4,0,"���ӿ�  00ʱ00��");
	if(open1 == 1)
		write_string(4,2,"��");
	else 
		write_string(4,2,"��");
	write_guanbiao(1,1,1);
}

/***********************18b20��ʼ������*****************************/
void init_18b20()
{
	bit q;
	dq = 1;				//�������ø�
	delay_uint(1);	    //15us
	dq = 0;				//����λ����
	delay_uint(80);		//750us
	dq = 1;				//�������ø� �ȴ�
	delay_uint(10);		//110us
	q = dq;				//��ȡ18b20��ʼ���ź�
	delay_uint(20);		//200us
	dq = 1;				//�������ø� �ͷ�����
}

/*************д18b20�ڵ�����***************/
void write_18b20(uchar dat)
{
	uchar i;
	for(i=0;i<8;i++)
	{					 //д�����ǵ�λ��ʼ
		dq = 0;			 //�������õ�дʱ��϶��ʼ 
		dq = dat & 0x01; //��18b20����д������
		delay_uint(5);	 // 60us
		dq = 1;			 //�ͷ�����
		dat >>= 1;
	}	
}

/*************��ȡ18b20�ڵ�����***************/
uchar read_18b20()
{
	uchar i,value;
	for(i=0;i<8;i++)
	{
		dq = 0;			 //�������õͶ�ʱ��϶��ʼ 
		value >>= 1;	 //�������ǵ�λ��ʼ
		dq = 1;			 //�ͷ�����
		if(dq == 1)		 //��ʼ��д���� 
			value |= 0x80;
		delay_uint(5);	 //60us	��һ��ʱ��϶����Ҫ����60us��ʱ��
	}
	return value;		 //��������
}

/*************��ȡ�¶ȵ�ֵ ����������С��***************/
uint read_temp()
{
	uint value;
	uchar low;			   //�ڶ�ȡ�¶ȵ�ʱ������жϵ�̫Ƶ���ˣ���Ӧ�ð��жϸ����ˣ������Ӱ�쵽18b20��ʱ��
	init_18b20();		   //��ʼ��18b20
	write_18b20(0xcc);	   //����64λROM
	write_18b20(0x44);	   //����һ���¶�ת������
	delay_uint(50);		   //500us

	init_18b20();		   //��ʼ��18b20
	
	write_18b20(0xcc);	   //����64λROM
	write_18b20(0xbe);	   //������ȡ�ݴ�������
	
	EA = 0;
	low = read_18b20();	   //���¶ȵ��ֽ�
	value = read_18b20();  //���¶ȸ��ֽ�
	EA = 1;
	value <<= 8;		   //���¶ȵĸ�λ����8λ
	value |= low;		   //�Ѷ������¶ȵ�λ�ŵ�value�ĵͰ�λ��
	value *= 0.625;	       //ת�����¶�ֵ С��
	return value;		   //���ض������¶� ��С��
}


/******************ʱ��оƬ��ʾ����**********************/	   
void ds1302_dis()
{
	uchar miao_1;
	if(menu_1 == 0)
	{
		read_time();
		if(miao_1 != miao)
		{
			miao_1 = miao;
			Conversion(0,nian,yue,ri);	//ũ��ת��				     
			n_nian = year_moon ;
			n_yue  = month_moon ;
			n_ri   = day_moon ;
			write_shu16(2,3,nian);	   //��ʾ��
			write_shu16(2,5,yue);	   //��ʾ��
			write_shu16(2,7,ri);	   //��ʾ��
			write_shu16(3,7,week);	   //��ʾ����
			
			addr_12864(3,2);	  //��ַת��	
			write_dat(shuzi[n_yue/16]);	
			write_dat(shuzi[n_yue%16]);	
			write_dat('-');	
			write_dat(shuzi[n_ri/16]);	
			write_dat(shuzi[n_ri%16]);	
					
			addr_12864(4,0);	  //��ַת��
			write_dat(' ');		
			write_dat(shuzi[shi/16]);	
			write_dat(shuzi[shi%16]);	
			write_dat(':');	
			write_dat(shuzi[fen/16]);	
			write_dat(shuzi[fen%16]);	
			write_dat(':');	
			write_dat(shuzi[miao/16]);	
			write_dat(shuzi[miao%16]);	

			write_dat(' ');	
			write_dat(shuzi[temperature/100%10]);	
			write_dat(shuzi[temperature/10%10]);	
			write_dat('.');	
			write_dat(shuzi[temperature%10]);	


		}
	}
}



/********************������������*****************/
uchar key_can;	 //����ֵ

void key()	 //������������
{
	static uchar key_new;
	key_can = 20;                   //����ֵ��ԭ
	P3 |= 0x78;                     //��Ӧ�İ���IO�����Ϊ1
	if((P3 & 0x78) != 0x78)		//��������
	{
		delay_1ms(1);	     	//����������
		if(((P3 & 0x78) != 0x78) && (key_new == 1))
		{						//ȷ���ǰ�������
			key_new = 0;
			switch(P3 & 0x78)
			{
				case 0x70:  key_can = 4;  break;	 //�õ�����ֵ
				case 0x68:  key_can = 3;  break;	 //�õ�����ֵ
				case 0x58:  key_can = 2;  break;	 //�õ�����ֵ
				case 0x38:  key_can = 1;  break;	 //�õ�����ֵ
			}
//			write_sfm2(1,0,key_can);				 //��ʾ����ֵ
		}			
	}
	else 
		key_new = 1;	
}

/**********************���ú���************************/
void key_with()
{
	if(key_can == 4)   //�˳���
	{
		init_12864();
		init_12864_dis();
		flag_200ms = 1;
		menu_1 = 0;
	}
	if(key_can == 1)	//���ü�
	{
		menu_1 ++;
		flag_200ms = 1;
		if(menu_1 == 1)
			init_12864_set1();
		if(menu_1 > 10)
			menu_1 = 1;
	}
	if(menu_1 != 0)
	{
		if(menu_1 == 1)		  //������
		{
			if(key_can == 2)	//��
				set_ds1302time(1,&nian,0x99);		
			if(key_can == 3)	//��	  
				set_ds1302time(0,&nian,0x99);	
		}
		if(menu_1 == 2)		  //������
		{
			if(key_can == 2)	//��
				set_ds1302time(1,&yue,0x13);		
			if(key_can == 3)	//��	  
				set_ds1302time(0,&yue,0x13);	
		}
		if(menu_1 == 3)		  //������
		{
			if(key_can == 2)	//��
				set_ds1302time(1,&ri,0x32);		
			if(key_can == 3)	//��	  
				set_ds1302time(0,&ri,0x32);	
		}
		if(menu_1 == 4)		  //����ʱ
		{
			if(key_can == 2)	//��
				set_ds1302time(1,&shi,0x24);		
			if(key_can == 3)	//��	  
				set_ds1302time(0,&shi,0x24);	
		}
		if(menu_1 == 5)		  //���÷�
		{
			if(key_can == 2)	//��
				set_ds1302time(1,&fen,0x60);		
			if(key_can == 3)	//��	  
				set_ds1302time(0,&fen,0x60);	
		}
		if(menu_1 == 6)		  //������
		{
			if(key_can == 2)	//��
				set_ds1302time(1,&miao,0x60);		
			if(key_can == 3)	//��	  
				set_ds1302time(0,&miao,0x60);	
		}
		if(menu_1 == 7)		  //��������
		{
			if(key_can == 2)	//��
				set_ds1302time(1,&week,0x8);		
			if(key_can == 3)	//��	  
				set_ds1302time(0,&week,0x8);	
		}
		if(menu_1 == 8)		  //��������ʱ
		{
			if(key_can == 2)	//��
				set_ds1302time(1,&shi1,0x24);		
			if(key_can == 3)	//��	  
				set_ds1302time(0,&shi1,0x24);	
			clock1_save();
		}
		if(menu_1 == 9)		  //�������ӷ�
		{
			if(key_can == 2)	//��
				set_ds1302time(1,&fen1,0x60);		
			if(key_can == 3)	//��	  
				set_ds1302time(0,&fen1,0x60);
			clock1_save();	
		}
		if(menu_1 == 10)		  //�������ӷ�
		{
			if((key_can == 2) || (key_can == 3))	//��
				open1 ++;
			if(open1 >= 2)
				open1 = 0;
			if(open1 == 1)
				write_string(4,2,"��");
			else 
				write_string(4,2,"��");	
			clock1_save();	
		}
		write_shu16(1,1,nian);	   //��ʾ��
		write_shu16(1,3,yue);	   //��ʾ��
		write_shu16(1,5,ri);	   //��ʾ��
		write_shu16(2,1,shi);	   //��ʾʱ
		write_shu16(2,3,fen);	   //��ʾ��
		write_shu16(2,5,miao);	   //��ʾ��
		write_shu16(3,5,week);	   //��ʾ����					
		write_shu16(4,4,shi1);	   //��ʾʱ��
		write_shu16(4,6,fen1);	   //��ʾ����
		switch(menu_1)	   // �����ʾ
		{
			case 1:  write_guanbiao(1,1,1);  break;
			case 2:  write_guanbiao(1,3,1);  break;
			case 3:  write_guanbiao(1,5,1);  break;
			case 4:  write_guanbiao(2,1,1);  break;
			case 5:  write_guanbiao(2,3,1);  break;
			case 6:  write_guanbiao(2,5,1);  break;
			case 7:  write_guanbiao(3,5,1);  break;
			case 8:  write_guanbiao(4,4,1);  break;
			case 9:  write_guanbiao(4,6,1);  break;
			case 10: write_guanbiao(4,2,1);  break;
		}
		write_time();	   //��ʱ��д��ȥ
	}			
}

/*************���ӱ�������***************/
void menu_dis()
{
	if(menu_1 == 0)
	if(open1 == 1)	//������Ӵ�
	{
		if((fen == fen1) && (shi == shi1) && (miao == 0)) 
		{		
			flag_clock_en = 1;	//�б��� �򿪷�������ı�־λ				
		}		
		if(flag_clock_en == 1)	//�����Ա���
		{	
			beep = ~beep;	   //��������3��
		}
	}	
}


/************������**************/
void main()
{
	beep = 0;		                //������һ��   
	delay_1ms(150);
	P0 = P1 = P2 = P3 = 0xff;		//��Ƭ��IO�ڳ�ʼ��Ϊ1	
	init_ds1302();	   //ds1302��ʼ��
	read_clock();
	init_12864();	   //lcd12864��ʼ��
	init_12864_dis();  //lcd12864��ʼ����ʾ
	time0_init();	   //��ʼ����ʱ��
	temperature = read_temp();	//�ȶ����¶ȵ�ֵ
	delay_1ms(650);
	temperature = read_temp();	//�ȶ����¶ȵ�ֵ
	while(1)
	{
//		beep = 0;
		key();	
		if(key_can < 10)
		{
			if(flag_clock_en == 0)  //ֻ�����ӹ��˵�ʱ����ܽ�������
				key_with();	
			else 
			{
				beep = 1;
				flag_clock_en = 0;	 //����������ɹر�����
			}
		}
		if(flag_200ms == 1)
		{
			flag_200ms = 0;
			ds1302_dis();    //ʱ����ʾ����
			temperature = read_temp();	//�ȶ����¶ȵ�ֵ
		}
		if(flag_100ms == 1)
		{
			menu_dis();		 //���ӱ�������
			flag_100ms = 0;
		}
		delay_1ms(1);		
	}
}

/*********************��ʱ��0�жϷ������************************/
void time0_int() interrupt 1  
{						   
	static uchar value;	//��ʱ10ms�ж�һ��
	TH0 = 0x3c;
	TL0 = 0xb0;         //50ms
	value ++;
	if(value % 2 == 0)
	{
		flag_100ms = 1;		
	}
	if(value % 20 == 0)
	{
		flag_200ms = 1;
	}
}

#include <reg52.h>	         //调用单片机头文件
#define uchar unsigned char  //无符号字符型 宏定义	变量范围0~255
#define uint  unsigned int	 //无符号整型 宏定义	变量范围0~65535
#include "nongli.h"

uchar code shuzi[]="0123456789ABCDEF";
char  code shuzi_c[]="0123456789ABCDEF";//uchar xdata dat_12864[64][16];

sbit rs=P2^6;	 //寄存器选择信号 H:数据寄存器  	L:指令寄存器
sbit rw=P2^5;	 //寄存器选择信号 H:数据寄存器  	L:指令寄存器
sbit e =P2^7;	 //片选信号   下降沿触发

sbit beep = P3^7;	//蜂鸣器IO口

sbit dq   = P3^1;	  //18b20 IO口的定义
uint temperature ;    //温度变量


bit flag_200ms = 1;
uchar menu_1;
uchar flag_clock_en;	//闹钟标志位
uchar open1;	//闹钟开标志位
uchar flag_c_en;
bit flag_100ms;

uchar n_nian,n_yue,n_ri;

/********************************************************************
* 名称 : delay_1ms()
* 功能 : 延时1ms函数
* 输入 : q
* 输出 : 无
***********************************************************************/
void delay_1ms(uint q)
{
	uint i,j;
	for(i=0;i<q;i++)
		for(j=0;j<115;j++);
}


#include "ds1302.h"


/*********************定时器0、定时器1初始化******************/
void time0_init()	  
{					 
	EA  = 1;	 	  //开总中断
	TMOD = 0X11;	  //定时器0、定时器1工作方式1
	ET0 = 1;		  //开定时器0中断 
	TR0 = 1;		  //允许定时器0定时
}
						   
					   
/************ 延时函数  *****************/
void delay_uint(uint z)
{
   while(z--);
}

/***************交换函数 高位变低位 低位变高位*****************/
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

/************ 写命令函数  *****************/
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
/************ 写数据函数  *****************/
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
*函数名: addr_12864
*功能:写地址
*说明:写汉字时要写个地址
*输入:无
*返回:无
**********************************************************/
void addr_12864(uchar hang,uchar lie)	  //地址转换
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
*函数名: write_shu16
*功能:在12864上显示16进制数据
*说明:
*输入:无
*返回:无
**********************************************************/
void write_shu16(uchar hang,uchar lie,uchar dat)
{
	addr_12864(hang,lie);	  //地址转换	
	write_dat(shuzi[dat/16]);	
	write_dat(shuzi[dat%16]);	
}

/**********************************************************
*函数名: write_string
*功能:在12864上显示字符数据
*说明:
*输入:无
*返回:无
**********************************************************/

void write_string(uchar hang,uchar lie,uchar *p)
{
	addr_12864(hang,lie);	  //地址转换
	while(*p != '\0')
	{
		write_dat(*p);
		p++;
		delay_uint(50);
	}
}


/*****************控制光标函数********************/
void write_guanbiao(uchar hang,uchar lie,uchar date)
{		
	addr_12864(hang,lie);	 
	if(date == 1)
		write_com(0x0f);     //显示光标并且闪烁	
	else 
		write_com(0xc0);   //关闭光标
}	 


/************12864液晶初始化函数*****************/
void init_12864()
{
   write_com(0x30);
   write_com(0x0c);
   write_com(0x01);
}

/*------------------------清除12864------------------------------*/
void clear_12864()
{
	write_string(1,0,"                ");	//1行	
	write_string(2,0,"                ");	//2行	
	write_string(3,0,"                ");	//3行	
	write_string(4,0,"                ");	//4行	
}


/************初始化显示*****************/
void init_12864_dis()
{
	write_string(1,0,"  多功能电子日历");
	write_string(2,0,"阳历2000年00月00");
	write_string(3,0,"农历00-00 星期  ");
	write_string(4,0," 00:00:00 00.0℃");
	write_guanbiao(1,1,0);
}

/************初始化设置显示*****************/
void init_12864_set1()
{
	clear_12864();
	write_string(1,0,"2013年00月00日  ");
	write_string(2,0,"  00时00分00秒  ");
	write_string(3,0,"设置  星期01    ");
	write_string(4,0,"闹钟开  00时00分");
	if(open1 == 1)
		write_string(4,2,"开");
	else 
		write_string(4,2,"关");
	write_guanbiao(1,1,1);
}

/***********************18b20初始化函数*****************************/
void init_18b20()
{
	bit q;
	dq = 1;				//把总线拿高
	delay_uint(1);	    //15us
	dq = 0;				//给复位脉冲
	delay_uint(80);		//750us
	dq = 1;				//把总线拿高 等待
	delay_uint(10);		//110us
	q = dq;				//读取18b20初始化信号
	delay_uint(20);		//200us
	dq = 1;				//把总线拿高 释放总线
}

/*************写18b20内的数据***************/
void write_18b20(uchar dat)
{
	uchar i;
	for(i=0;i<8;i++)
	{					 //写数据是低位开始
		dq = 0;			 //把总线拿低写时间隙开始 
		dq = dat & 0x01; //向18b20总线写数据了
		delay_uint(5);	 // 60us
		dq = 1;			 //释放总线
		dat >>= 1;
	}	
}

/*************读取18b20内的数据***************/
uchar read_18b20()
{
	uchar i,value;
	for(i=0;i<8;i++)
	{
		dq = 0;			 //把总线拿低读时间隙开始 
		value >>= 1;	 //读数据是低位开始
		dq = 1;			 //释放总线
		if(dq == 1)		 //开始读写数据 
			value |= 0x80;
		delay_uint(5);	 //60us	读一个时间隙最少要保持60us的时间
	}
	return value;		 //返回数据
}

/*************读取温度的值 读出来的是小数***************/
uint read_temp()
{
	uint value;
	uchar low;			   //在读取温度的时候如果中断的太频繁了，就应该把中断给关了，否则会影响到18b20的时序
	init_18b20();		   //初始化18b20
	write_18b20(0xcc);	   //跳过64位ROM
	write_18b20(0x44);	   //启动一次温度转换命令
	delay_uint(50);		   //500us

	init_18b20();		   //初始化18b20
	
	write_18b20(0xcc);	   //跳过64位ROM
	write_18b20(0xbe);	   //发出读取暂存器命令
	
	EA = 0;
	low = read_18b20();	   //读温度低字节
	value = read_18b20();  //读温度高字节
	EA = 1;
	value <<= 8;		   //把温度的高位左移8位
	value |= low;		   //把读出的温度低位放到value的低八位中
	value *= 0.625;	       //转换到温度值 小数
	return value;		   //返回读出的温度 带小数
}


/******************时间芯片显示函数**********************/	   
void ds1302_dis()
{
	uchar miao_1;
	if(menu_1 == 0)
	{
		read_time();
		if(miao_1 != miao)
		{
			miao_1 = miao;
			Conversion(0,nian,yue,ri);	//农历转换				     
			n_nian = year_moon ;
			n_yue  = month_moon ;
			n_ri   = day_moon ;
			write_shu16(2,3,nian);	   //显示年
			write_shu16(2,5,yue);	   //显示月
			write_shu16(2,7,ri);	   //显示日
			write_shu16(3,7,week);	   //显示星期
			
			addr_12864(3,2);	  //地址转换	
			write_dat(shuzi[n_yue/16]);	
			write_dat(shuzi[n_yue%16]);	
			write_dat('-');	
			write_dat(shuzi[n_ri/16]);	
			write_dat(shuzi[n_ri%16]);	
					
			addr_12864(4,0);	  //地址转换
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



/********************独立按键程序*****************/
uchar key_can;	 //按键值

void key()	 //独立按键程序
{
	static uchar key_new;
	key_can = 20;                   //按键值还原
	P3 |= 0x78;                     //对应的按键IO口输出为1
	if((P3 & 0x78) != 0x78)		//按键按下
	{
		delay_1ms(1);	     	//按键消抖动
		if(((P3 & 0x78) != 0x78) && (key_new == 1))
		{						//确认是按键按下
			key_new = 0;
			switch(P3 & 0x78)
			{
				case 0x70:  key_can = 4;  break;	 //得到按键值
				case 0x68:  key_can = 3;  break;	 //得到按键值
				case 0x58:  key_can = 2;  break;	 //得到按键值
				case 0x38:  key_can = 1;  break;	 //得到按键值
			}
//			write_sfm2(1,0,key_can);				 //显示按键值
		}			
	}
	else 
		key_new = 1;	
}

/**********************设置函数************************/
void key_with()
{
	if(key_can == 4)   //退出键
	{
		init_12864();
		init_12864_dis();
		flag_200ms = 1;
		menu_1 = 0;
	}
	if(key_can == 1)	//设置键
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
		if(menu_1 == 1)		  //设置年
		{
			if(key_can == 2)	//加
				set_ds1302time(1,&nian,0x99);		
			if(key_can == 3)	//减	  
				set_ds1302time(0,&nian,0x99);	
		}
		if(menu_1 == 2)		  //设置月
		{
			if(key_can == 2)	//加
				set_ds1302time(1,&yue,0x13);		
			if(key_can == 3)	//减	  
				set_ds1302time(0,&yue,0x13);	
		}
		if(menu_1 == 3)		  //设置日
		{
			if(key_can == 2)	//加
				set_ds1302time(1,&ri,0x32);		
			if(key_can == 3)	//减	  
				set_ds1302time(0,&ri,0x32);	
		}
		if(menu_1 == 4)		  //设置时
		{
			if(key_can == 2)	//加
				set_ds1302time(1,&shi,0x24);		
			if(key_can == 3)	//减	  
				set_ds1302time(0,&shi,0x24);	
		}
		if(menu_1 == 5)		  //设置分
		{
			if(key_can == 2)	//加
				set_ds1302time(1,&fen,0x60);		
			if(key_can == 3)	//减	  
				set_ds1302time(0,&fen,0x60);	
		}
		if(menu_1 == 6)		  //设置秒
		{
			if(key_can == 2)	//加
				set_ds1302time(1,&miao,0x60);		
			if(key_can == 3)	//减	  
				set_ds1302time(0,&miao,0x60);	
		}
		if(menu_1 == 7)		  //设置星期
		{
			if(key_can == 2)	//加
				set_ds1302time(1,&week,0x8);		
			if(key_can == 3)	//减	  
				set_ds1302time(0,&week,0x8);	
		}
		if(menu_1 == 8)		  //设置闹钟时
		{
			if(key_can == 2)	//加
				set_ds1302time(1,&shi1,0x24);		
			if(key_can == 3)	//减	  
				set_ds1302time(0,&shi1,0x24);	
			clock1_save();
		}
		if(menu_1 == 9)		  //设置闹钟分
		{
			if(key_can == 2)	//加
				set_ds1302time(1,&fen1,0x60);		
			if(key_can == 3)	//减	  
				set_ds1302time(0,&fen1,0x60);
			clock1_save();	
		}
		if(menu_1 == 10)		  //设置闹钟分
		{
			if((key_can == 2) || (key_can == 3))	//加
				open1 ++;
			if(open1 >= 2)
				open1 = 0;
			if(open1 == 1)
				write_string(4,2,"开");
			else 
				write_string(4,2,"关");	
			clock1_save();	
		}
		write_shu16(1,1,nian);	   //显示年
		write_shu16(1,3,yue);	   //显示月
		write_shu16(1,5,ri);	   //显示日
		write_shu16(2,1,shi);	   //显示时
		write_shu16(2,3,fen);	   //显示分
		write_shu16(2,5,miao);	   //显示秒
		write_shu16(3,5,week);	   //显示星期					
		write_shu16(4,4,shi1);	   //显示时钟
		write_shu16(4,6,fen1);	   //显示分钟
		switch(menu_1)	   // 光标显示
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
		write_time();	   //把时间写进去
	}			
}

/*************闹钟报警函数***************/
void menu_dis()
{
	if(menu_1 == 0)
	if(open1 == 1)	//如果闹钟打开
	{
		if((fen == fen1) && (shi == shi1) && (miao == 0)) 
		{		
			flag_clock_en = 1;	//有报警 打开蜂鸣器响的标志位				
		}		
		if(flag_clock_en == 1)	//闹钟以被打开
		{	
			beep = ~beep;	   //蜂鸣器叫3秒
		}
	}	
}


/************主函数**************/
void main()
{
	beep = 0;		                //开机叫一声   
	delay_1ms(150);
	P0 = P1 = P2 = P3 = 0xff;		//单片机IO口初始化为1	
	init_ds1302();	   //ds1302初始化
	read_clock();
	init_12864();	   //lcd12864初始化
	init_12864_dis();  //lcd12864初始化显示
	time0_init();	   //初始化定时器
	temperature = read_temp();	//先读出温度的值
	delay_1ms(650);
	temperature = read_temp();	//先读出温度的值
	while(1)
	{
//		beep = 0;
		key();	
		if(key_can < 10)
		{
			if(flag_clock_en == 0)  //只有闹钟关了的时候才能进入设置
				key_with();	
			else 
			{
				beep = 1;
				flag_clock_en = 0;	 //按下任意键可关闭闹钟
			}
		}
		if(flag_200ms == 1)
		{
			flag_200ms = 0;
			ds1302_dis();    //时钟显示函数
			temperature = read_temp();	//先读出温度的值
		}
		if(flag_100ms == 1)
		{
			menu_dis();		 //闹钟报警函数
			flag_100ms = 0;
		}
		delay_1ms(1);		
	}
}

/*********************定时器0中断服务程序************************/
void time0_int() interrupt 1  
{						   
	static uchar value;	//定时10ms中断一次
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

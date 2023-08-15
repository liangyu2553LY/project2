#include <12864.h>
uchar code shuzi[]="0123456789ABCDEF";
char  code shuzi_c[]="0123456789ABCDEF";//uchar xdata dat_12864[64][16];
/************ 延时函数  *****************/
void delay_uint(uint z)
{
   while(z--);
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
	write_string(1,0,"--智能药箱系统--");
	write_string(2,0,"时间：");
	write_string(3,0,"列表：        ℃");
	write_string(4,0,"                ");
	write_guanbiao(1,1,0);
}


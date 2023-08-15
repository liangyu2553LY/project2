#include <12864.h>
uchar code shuzi[]="0123456789ABCDEF";
char  code shuzi_c[]="0123456789ABCDEF";//uchar xdata dat_12864[64][16];
/************ ��ʱ����  *****************/
void delay_uint(uint z)
{
   while(z--);
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
	write_string(1,0,"--����ҩ��ϵͳ--");
	write_string(2,0,"ʱ�䣺");
	write_string(3,0,"�б�        ��");
	write_string(4,0,"                ");
	write_guanbiao(1,1,0);
}


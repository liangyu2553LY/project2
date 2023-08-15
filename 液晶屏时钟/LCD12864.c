#include <LCD12864.h>
 
void Read_busy()
{
	P0 = 0X00;
	DI = 0;
	RW = 1;
	EN = 1;
	while(P0 & 0x80)
	{;}
	EN = 0;
}

void write_LCD_command(uint8 value)	//д�����
{
	Read_busy();
	DI = 0; //0:дָ��
	RW = 0;	//0:д����
	LCD_databus = value;
	EN = 1;	//EN�½���������Ч����
	_nop_();
	_nop_();
	_nop_();//��ָ�������ʱ
	EN = 0;
}

void write_LCD_data(uint8 value)	//д���ݺ���
{
	Read_busy();
	DI = 1; //1:д����
	RW = 0;	//д����
	LCD_databus = value;
	EN = 1; //EN�½���������Ч����
	_nop_();
	_nop_();
	_nop_();//��ָ�������ʱ
	EN = 0;
}

void Set_page(uint8 page)	//������ʾ��ʼҳ
{
	page = 0xB8 | page;	//ҳ���׵�ַΪ0xB8
	write_LCD_command(page);
}

void Set_line(uint8 startline)	//������ʾ��ʼ��
{
	startline = 0xC0 | startline;
	write_LCD_command(startline);
}

void Set_column(uint8 column)	//������ʾ����
{
	column = column & 0x3F;	//�е����ֵΪ64
	column = column | 0x40;	//�е��׵�ַΪ0x40
	write_LCD_command(column);
}	

void SetOnOff(uint8 onoff)	//��ʾ���غ�����0x3E:�� 0x3F:��
{
	onoff = 0x3E | onoff;	//onoffΪ0ʱ����ʾ��Ϊ1ʱ����ʾ
	write_LCD_command(onoff);
}

void SelectScreen(uint8 screen)	//ѡ����Ļ
{
	switch(screen)
	{
		case 0:CS1 = 0;CS2 = 0;break;//ȫ��
		case 1:CS1 = 0;CS2 = 1;break;//�����
		case 2:CS1 = 1;CS2 = 0;break;//�Ұ���
		default:break;
	}
}

void ClearScreen(uint8 screen)	//����
{
	uint8 i,j;
	SelectScreen(screen);
	for(i=0;i<8;i++)
	{
		Set_page(i);
		Set_column(0);
		for(j=0;j<64;j++)
		{
			write_LCD_data(0x00);	//д��0����ַָ���Զ���1
		}
	}
}

void InitLCD()
{
	Read_busy();
	SelectScreen(0);
	SetOnOff(0);
	SelectScreen(0);
	SetOnOff(1);
	SelectScreen(0);
	ClearScreen(0);
	Set_line(0);
}

void show_ch(uint8 screen,uint8 page,uint8 column,uint8 *p) //��ʾ����
{
	uint8 i;
	SelectScreen(screen);
	Set_page(page);
	Set_column(column);
	
	for(i=0;i<8;i++)	//����16*8����ģ
	{
		write_LCD_data(p[i]);
	}
	
	Set_page(page+1);
	Set_column(column);
	for(i=0;i<8;i++)	//����16*8����ģ,"С�ĺ���"
	{
		write_LCD_data(p[i+8]);
	}
}

void show_im(uint8 screen,uint8 page,uint8 column,uint8 *p)
{
	uint8 i;
	SelectScreen(screen);
	Set_page(page);
	Set_column(column);

	for(i=0;i<32;i++)	//����16*16����ģ
	{
		write_LCD_data(p[i]);
	}
	
	Set_page(page+1);
	Set_column(column);
	for(i=0;i<32;i++)	//����16*16����ģ,"С�ĺ���"
	{
		write_LCD_data(p[i+32]);
	}
	
	Set_page(page+2);
	Set_column(column);
	for(i=0;i<32;i++)	//����16*16����ģ,"С�ĺ���"
	{
		write_LCD_data(p[i+64]);
	}
	
	Set_page(page+3);
	Set_column(column);
	for(i=0;i<32;i++)	//����16*16����ģ,"С�ĺ���"
	{
		write_LCD_data(p[i+96]);
	}
}







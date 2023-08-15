#ifndef _LCD12864_H
#define _LCD12864_H
#include <reg52.h>
#include <intrins.h>
#define uint8 unsigned char
#define uint16 unsigned int
#define LCD_databus P0 //���߽ӿڣ�ע��51��P0����Ҫ�����������
sbit DI = P2^6;	//DIΪ 0��дָ����״̬��1������
sbit RW = P2^5;	//1:д 0����
sbit EN = P2^7;	//ʹ��
sbit CS1 = P3^2;	//Ƭѡ1.�͵�ƽ��Ч�����������
sbit CS2 = P3^3;  //Ƭѡ2���͵�ƽ��Ч�������Ұ���

//��������
void Read_busy();
void write_LCD_command(uint8);
void write_LCD_data(uint8);
void Set_page(uint8);
void Set_line(uint8);
void Set_column(uint8);
void SetOnOff(uint8);
void SelectScreen(uint8);
void ClearScreen(uint8);
void InitLCD();
void show_ch(uint8,uint8,uint8,uint8 *);
void show_im(uint8,uint8,uint8,uint8 *);
#endif
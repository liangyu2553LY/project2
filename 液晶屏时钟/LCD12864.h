#ifndef _LCD12864_H
#define _LCD12864_H
#include <reg52.h>
#include <intrins.h>
#define uint8 unsigned char
#define uint16 unsigned int
#define LCD_databus P0 //总线接口，注意51的P0口需要外接上拉电阻
sbit DI = P2^6;	//DI为 0：写指令或读状态，1：数据
sbit RW = P2^5;	//1:写 0：读
sbit EN = P2^7;	//使能
sbit CS1 = P3^2;	//片选1.低电平有效，控制左半屏
sbit CS2 = P3^3;  //片选2，低电平有效，控制右半屏

//声明函数
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
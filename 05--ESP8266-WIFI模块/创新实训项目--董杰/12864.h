#include <REGX51.H>

#ifndef __12864_H__
#define __12864_H__

#define uchar unsigned char
#define uint  unsigned int

/*12864port??*/
#define LCD_data  P0             //???
sbit rs  =  P2^6;            //??????? 
sbit rw  =  P2^5;            //???/???
sbit e  =  P2^7;            //??????

sbit LCD_PSB =  P1^2;            //?/?????

extern uchar code shuzi[];
extern char  code shuzi_c[];//uchar xdata dat_12864[64][16];
/*????*/
void write_com(uchar com);
void write_dat(uchar dat);
void addr_12864(uchar hang,uchar lie);	  //µØÖ·×ª»»
void write_shu16(uchar hang,uchar lie,uchar dat);
void write_string(uchar hang,uchar lie,uchar *p);
void write_guanbiao(uchar hang,uchar lie,uchar date);
void init_12864();
void clear_12864();
void init_12864_dis();
#endif



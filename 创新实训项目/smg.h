#ifndef _smg_H_
#define _smg_H_

typedef unsigned int u16;	//��ϵͳĬ���������ͽ����ض���
typedef unsigned char u8;


#define SMG_A_DP_PORT	P0	//ʹ�ú궨������ܶ����

//���������λѡ�źſ��ƽ�
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

extern u8 gsmg_code[17];

void smg_display(u8 dat[],u8 pos);

#endif
#ifndef _ds18b20_H
#define _ds18b20_H
#include <REGX51.H>

//�ܽŶ���
sbit DS18B20_PORT=P3^7;	//DS18B20���ݿڶ���


//��������
unsigned char ds18b20_init(void);
float ds18b20_read_temperture(void);

#endif
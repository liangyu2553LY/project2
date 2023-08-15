#include <REGX51.H>
#include <delay.h>
#define RELOAD_COUNT 0xFA //宏定义波特率发生器的载入值 9600

void UART_Init(void)
{
	SCON|=0X50;			//设置为工作方式1
	TMOD|=0X20;			//设置计数器工作方式2
	PCON=0X80;			//波特率加倍
	TH1=RELOAD_COUNT;	//计数器初始值设置
	TL1=TH1;
	ES=0;				//关闭接收中断
	EA=1;				//打开总中断
	TR1=1;				//打开计数器
//	TI=1;          //发送中断标记位，如果使用printf函数的必须设置	
}

/**
  * @brief  串口发送一个字节
  * @param  Byte要发送的数据
  * @retval ?
  */
void UART_SendData(unsigned char dat)
{
	ES=0; //关闭串口中断
	TI=0; //清发送完毕中断请求标志位
	SBUF=dat; //发送
	while(TI==0); //等待发送完毕
	TI=0; //清发送完毕中断请求标志位
	ES=1; //允许串口中断
}

void UART_SendString(unsigned char *pbuf)
{
	while(*pbuf!='\0') //遇到空格跳出循环	
	{
		UART_SendData(*pbuf);
		delay_10us(5);
		pbuf++;	
	}
}



/*????????
void UART_Routine() interrupt 4
{
	if(RI==1)
	{
		
		RI=0;
	}
}
*/

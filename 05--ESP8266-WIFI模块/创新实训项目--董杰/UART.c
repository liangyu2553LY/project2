#include <REGX51.H>
#include <delay.h>
#define RELOAD_COUNT 0xFA //�궨�岨���ʷ�����������ֵ 9600

void UART_Init(void)
{
	SCON|=0X50;			//����Ϊ������ʽ1
	TMOD|=0X20;			//���ü�����������ʽ2
	PCON=0X80;			//�����ʼӱ�
	TH1=RELOAD_COUNT;	//��������ʼֵ����
	TL1=TH1;
	ES=0;				//�رս����ж�
	EA=1;				//�����ж�
	TR1=1;				//�򿪼�����
//	TI=1;          //�����жϱ��λ�����ʹ��printf�����ı�������	
}

/**
  * @brief  ���ڷ���һ���ֽ�
  * @param  ByteҪ���͵�����
  * @retval ?
  */
void UART_SendData(unsigned char dat)
{
	ES=0; //�رմ����ж�
	TI=0; //�巢������ж������־λ
	SBUF=dat; //����
	while(TI==0); //�ȴ��������
	TI=0; //�巢������ж������־λ
	ES=1; //�������ж�
}

void UART_SendString(unsigned char *pbuf)
{
	while(*pbuf!='\0') //�����ո�����ѭ��	
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

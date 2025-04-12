#include <STC15F2K60S2.H>
#include "Uart.h"

void Uart1_Init(void)	//9600bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR &= 0xFB;		//定时器时钟12T模式
	T2L = 0xE6;			//设置定时初始值
	T2H = 0xFF;			//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
	ES=1;
	EA=1;
}

void Uart_Send_byte(unsigned char val)
{
	SBUF=val;
	while(TI==0);
	TI=0;
}

void Uart_Send_str(unsigned char* str)
{
	unsigned char i=0;
	while(str[i]!='\0')
	{
		Uart_Send_byte(str[i]);
		i++;
	}
}
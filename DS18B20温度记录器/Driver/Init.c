#include <STC15F2K60S2.H>
#include "Init.h"

void System_init()
{
	P0=0xFF;
	P2 &= 0x1F;
	P2 |= 0x80;
	P2 &= 0x1F;
	
	P0=0x00;
	P2 &= 0x1F;
	P2 |= 0xA0;
	P2 &= 0x1F;
}

void Timer0_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x18;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	EA=1;
	ET0=1;
}
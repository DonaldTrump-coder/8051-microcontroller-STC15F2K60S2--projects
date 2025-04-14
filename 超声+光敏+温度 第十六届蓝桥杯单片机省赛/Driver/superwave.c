#include <STC15F2K60S2.H>
#include "intrins.h"
#include "superwave.h"

sbit NA1=P1^0;
sbit NB1=P1^1;

void Delay12us(void)	//@12.000MHz
{
	unsigned char data i;

	_nop_();
	_nop_();
	i = 33;
	while (--i);
}

void wave_init()
{
	unsigned char i;
	for(i=0;i<1;i++)
	{
		NA1=1;
		Delay12us();
		NA1=0;
		Delay12us();
	}
}

float get_distance()
{
	unsigned int time;
	float distance;
	AUXR &= 0xBF;			//定时器时钟12T模式
	//TMOD |= 0x10;			//设置定时器模式
	TL1 = 0;				//设置定时初始值
	TH1 = 0;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	
	EA=0;
	wave_init();
	TR1 = 1;				//定时器1开始计时
	while(TF1==0 && NB1==1);
	TR1=0;
	if(TF1==0)
	{
		time=((TH1<<8) | TL1);
		EA=1;
		distance=((float)time)*0.017;
		return distance;
	}
	else
	{
		TF1=0;
		EA=1;
		return 0;
	}

}
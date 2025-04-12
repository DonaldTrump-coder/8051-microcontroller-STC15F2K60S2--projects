#include <STC15F2K60S2.H>
#include "Seg.h"
#include "Uart.h"
#include "Init.h"

unsigned char chara;
unsigned char* Seg_buf[]={10,10,10,10,10,10,10,10};
unsigned char Seg_index=0;
unsigned char Seg_counter=0;
unsigned char chara_counter=0;
bit flag=0;

void Seg_proc()
{
	if(Seg_counter>=2)
	{
		Seg_counter=0;
		Seg_index++;
		if(Seg_index==8)
			Seg_index=0;
		Seg_show(Seg_index,Seg_buf[Seg_index]);
	}
}

void Other_proc()
{
	if(chara_counter>=50)
	{
		chara_counter=0;
		if(flag==1)
		{
			if(chara=='A')
				Seg_buf[7]=11;
			else
				Seg_buf[7]=12;
		}
	}
}

void Timer0_Rountine() interrupt 1
{
	Seg_counter++;
	chara_counter++;
}

void Uart1_Routine() interrupt 4
{
	if(RI==1)
	{
		flag=1;
		chara=SBUF;
		RI=0;
	}
}

void main()
{
	System_Init();
	Timer0_Init();
	Uart1_Init();
	while(1)
	{
		Seg_proc();
		Other_proc();
	}
}
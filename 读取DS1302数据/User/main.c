#include <STC15F2K60S2.H>
#include "Seg.h"
#include "init.h"
#include "ds1302.h"

unsigned char Seg_buf[]={10,10,11,10,10,11,10,10};
unsigned char Seg_index=0;
unsigned char Seg_slow=0;
unsigned int DS1302_slow=0;
unsigned char time[]={23,59,59};

void Seg_proc()
{
	if(Seg_slow==2)
	{
		Seg_slow=0;
		Seg_index++;
		if(Seg_index==8)
			Seg_index=0;
		Seg_show(Seg_index,Seg_buf[Seg_index]);
	}
}

void Other_proc()
{
	if(DS1302_slow==500)
	{
		DS1302_slow=0;
		DS1302_Get_time(time);
		Seg_buf[0]=time[0]/10;
		Seg_buf[1]=time[0]%10;
		Seg_buf[3]=time[1]/10;
		Seg_buf[4]=time[1]%10;
		Seg_buf[6]=time[2]/10;
		Seg_buf[7]=time[2]%10;
	}
}

void Timer0_Routine() interrupt 1
{
	Seg_slow++;
	DS1302_slow++;
}

void main()
{
	System_Init();
	Timer0_Init();
	//DS1302_Write_time(time);
	while(1)
	{
		Seg_proc();
		Other_proc();
	}
}
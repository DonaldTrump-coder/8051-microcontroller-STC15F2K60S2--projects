#include <STC15F2K60S2.H>
#include "Seg.h"
#include "iic.h"
#include "init.h"

unsigned char Seg_buf[]={11,10,10,10,10,10,10,10};
unsigned char Seg_index=0;
unsigned char Seg_counter=0;
unsigned char voltage_counter=0;
unsigned char voltage_A;
unsigned int vol;
float voltage;

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
	if(voltage_counter>=1)
	{
		voltage_counter=0;
		voltage_A=AD_read(0x43);
		voltage=((float)voltage_A)/51.0;
		vol=(int)(voltage*100);
		Seg_buf[5]=(vol/100)%10;
		Seg_buf[6]=(vol/10)%10;
		Seg_buf[7]=vol%10;
	}
}

void Timer0_Routine() interrupt 1
{
	Seg_counter++;
	voltage_counter++;
}

void main()
{
	System_init();
	Timer0_Init();
	while(1)
	{
		Seg_proc();
		Other_proc();
	}
}

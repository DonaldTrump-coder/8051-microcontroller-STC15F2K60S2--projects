#include <STC15F2K60S2.H>
#include "onewire.h"
#include "Init.h"
#include "Seg.h"
#include "intrins.h"

unsigned char display_table[]={10,10,10,10,10,10,10,10};
unsigned char Seg_index=0;
unsigned char Seg_counter=0;
unsigned int DS18B20_counter=0;
float temperature;
unsigned char present_temp,low_temp,high_temp;

void Delay750ms(void)	//@12.000MHz
{
	unsigned char data i, j, k;

	_nop_();
	_nop_();
	i = 35;
	j = 51;
	k = 182;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Seg_proc()
{
	if(Seg_counter>=2)
	{
		Seg_counter=0;
		Seg_index++;
		if(Seg_index==8)
			Seg_index=0;
		Seg_show(Seg_index,display_table[Seg_index]);
	}
}

void Other_proc()
{
	if(DS18B20_counter>=750)
	{
		DS18B20_counter=0;
		temperature=get_temperature();
		present_temp=(unsigned char)temperature;
		if(present_temp<low_temp)
			low_temp=present_temp;
		if(present_temp>high_temp)
			high_temp=present_temp;
		display_table[0]=present_temp/10;
		display_table[1]=present_temp%10;
		display_table[3]=low_temp/10;
		display_table[4]=low_temp%10;
		display_table[6]=high_temp/10;
		display_table[7]=high_temp%10;
	}
}

void Timer0_Routine() interrupt 1
{
	Seg_counter++;
	DS18B20_counter++;
}

void main()
{
	System_init();
	Timer0_Init();
	get_temperature();
	Delay750ms();
	Other_proc();
	low_temp=present_temp;
	high_temp=present_temp;
	while(1)
	{
		Seg_proc();
		Other_proc();
	}
}
#include <STC15F2K60S2.H>
#include "LED.h"

unsigned char last_LED=0xFF;

void LED_show(unsigned char add,bit enable)
{
	unsigned char temp=last_LED;
	if(enable==1)
	{
		temp=temp & (~(0x01<<(add-1)));
	}
	else
	{
		temp=temp | (0x01<<(add-1));
	}
	
	if(temp!=last_LED)
	{
		P0=temp;
		P2 &= 0x1F;
		P2 |= 0x80;
		P2 &= 0x1F;
		last_LED=temp;
	}
}
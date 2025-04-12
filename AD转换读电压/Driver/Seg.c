#include <STC15F2K60S2.H>
#include "Seg.h"

unsigned char duan[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xFF,0xC1,0x7F};
                    //0    1    2    3    4    5    6    7    8    9    无   U    .

unsigned char wei[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

unsigned char points[]={0,0,0,0,0,1,0,0};

void Seg_show(unsigned char add,unsigned char val)
{
	P0=duan[10];
	P2 &= 0x1F;
	P2 |= 0xE0;
	P2 &= 0x1F;
	
	P0=wei[add];
	P2 &= 0x1F;
	P2 |= 0xC0;
	P2 &= 0x1F;
	
	if(points[add]==1)
		P0=duan[val] & duan[12];
	else
		P0=duan[val];
	P2 &= 0x1F;
	P2 |= 0xE0;
	P2 &= 0x1F;
}
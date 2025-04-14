#include <STC15F2K60S2.H>
#include "Key.h"

unsigned char get_key()
{
	P44=0;P42=1;P35=1;P34=1;
	if(P33==0)return 4;
	if(P32==0)return 5;
	if(P31==0)return 6;
	if(P30==0)return 7;
	
	P44=1;P42=0;P35=1;P34=1;
	if(P33==P32 && P33==0)return 20;
	if(P33==0)return 8;
	if(P32==0)return 9;
	if(P31==0)return 10;
	if(P30==0)return 11;
	
	P44=1;P42=1;P35=0;P34=1;
	if(P33==0)return 12;
	if(P32==0)return 13;
	if(P31==0)return 14;
	if(P30==0)return 15;
	
	P44=1;P42=1;P35=1;P34=0;
	if(P33==0)return 16;
	if(P32==0)return 17;
	if(P31==0)return 18;
	if(P30==0)return 19;
	return 0;
}
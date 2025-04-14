#include <STC15F2K60S2.H>
#include "Relay.h"

bit last_relay=0;

void Relay_control(bit enable)//0断开，1吸合
{
	unsigned char temp;
	if(enable==1)
	{
		temp=0x10;
	}
	else
		temp=0x00;
	if(last_relay!=enable)
	{
		P0=temp;
		P2 &= 0x1F;
		P2 |= 0xA0;
		P2 &= 0x1F;
		last_relay=enable;
	}
}
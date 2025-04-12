#include <STC15F2K60S2.H>
#include "iic.h"
#include "Key.h"
#include "init.h"
#include "Uart.h"

unsigned char Key_counter=0;
unsigned char Key_value=0,last_key=0;
unsigned char Key_up,Key_down;
unsigned char counts=0;
unsigned char Uart_Rec[10];
unsigned char Uart_Rec_index=0;

void Key_proc()
{
	if(Key_counter>=5)
	{
		Key_counter=0;
		Key_value=get_key();
		Key_down=Key_value & (Key_value^last_key);
		Key_up=(~Key_value) & (Key_value^last_key);
		last_key=Key_value;
	}
}

void Other_proc()
{
	if(Key_down==4)
	{
		Uart1_Init();
		Uart_Send_str("Number:");
		Uart_Send_byte(counts+'0');
	}
}

void Timer0_Routine() interrupt 1
{
	Key_counter++;
}

void Uartq_Rountine() interrupt 4
{
	if(RI==1)
	{
		Uart_Rec[Uart_Rec_index]=SBUF;
		Uart_Rec_index++;
		if(Uart_Rec[Uart_Rec_index]=='\0')
			Uart_Rec_index=0;
		RI=0;
	}
}

void main()
{
	System_init();
	Timer0_Init();
	EEPROM_Read(&counts,0,1);
	counts++;
	EEPROM_Write(&counts,0,1);
	
	while(1)
	{
		Key_proc();
		Other_proc();
	}
}

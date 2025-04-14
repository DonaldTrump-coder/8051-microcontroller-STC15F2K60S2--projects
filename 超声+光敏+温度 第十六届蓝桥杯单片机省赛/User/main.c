#include <STC15F2K60S2.H>
#include "LED.h"
#include "init.h"
#include "Seg.h"
#include "Key.h"
#include "iic.h"
#include "onewire.h"
#include "Relay.h"
#include "superwave.h"

unsigned int temperature;
unsigned char light;
float light_voltage;
unsigned char light_degree;
unsigned char Seg_mode=0;
unsigned char Seg_buf0[]={11,10,10,10,10,10,12,10};//环境状态
unsigned char Seg_buf1[]={13,10,10,10,10,10,10,10};//运动检测
unsigned char Seg_buf2[]={14,11,10,10,10,10,10,10};//温度参数设置
unsigned char Seg_buf3[]={12,11,10,10,10,10,10,10};//统计数据
unsigned char Seg_buf5[]={14,13,10,10,10,10,10,10};//距离参数设置
unsigned char Seg_index=0;
unsigned char Seg_counter=0;
unsigned int wave_counter=0;
unsigned char Key_counter=0;
unsigned char Key_value=0;
unsigned char last_key=0;
unsigned char Key_down=0;
unsigned char Key_up=0;
bit status_lock=0;
unsigned char temperature_set=30;
unsigned char distance_set=30;
unsigned int Relay_time=0;
unsigned char Other_counter=0;
bit near=0;bit high=0;

unsigned char motion=1;
unsigned char last_motion=1;
unsigned int motion_lock_3s=0;
unsigned int distance[2]={0,0};
unsigned int delta=0;

bit lightingLED=0;

unsigned int timer1s=0;

unsigned char LED_counter=0;

bit Relay_last=0;

void Delay750ms(void)	//@12.000MHz
{
	unsigned char data i, j, k;

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

void Delay2s(void)	//@12.000MHz
{
	unsigned char data i, j;
	unsigned int k=2000;
	while(k--)
	{
		i = 12;
		j = 169;
		do
		{
			while (--j);
		} while (--i);
	}
}

void Key_proc()
{
	if(timer1s>=100)
	{
		timer1s=0;
		lightingLED=~lightingLED;
	}
	
	if(Key_counter>=15)
	{
		Key_value=get_key();
		Key_down=Key_value & (Key_value ^ last_key);
		Key_up=(~Key_value) & (Key_value ^ last_key);
		last_key=Key_value;
		
		if(Seg_mode==2|Seg_mode==5)status_lock=1;
		else status_lock=0;
		
		if(Key_down==4)
		{
			Seg_mode++;
			if(Seg_mode==4)Seg_mode=0;
			if(Seg_mode==6)Seg_mode=3;
		}
		
		if(Key_down==5)
		{
			if(Seg_mode==2)Seg_mode=5;
			else if(Seg_mode==5)Seg_mode=2;
		}
		
		if(Key_down==8)
		{
			if(Seg_mode==2)
			{
				temperature_set++;
				if(temperature_set>80)temperature_set=20;
			}
			if(Seg_mode==5)
			{
				distance_set+=5;
				if(distance_set>80)distance_set=20;
			}
		}
		
		if(Key_down==9)
		{
			if(Seg_mode==2)
			{
				temperature_set--;
				if(temperature_set<20)temperature_set=80;
			}
			if(Seg_mode==5)
			{
				distance_set-=5;
				if(distance_set<20)distance_set=80;
			}
		}
		
		if(Key_down==20)
		{
			Delay2s();
			if(Key_value==20)
			{
				Relay_time=0;
			}
		}
	}
}

void Seg_proc()
{
	if(Seg_counter>=1)
	{
		Seg_counter=0;
		Seg_index++;
		if(Seg_index==8)
			Seg_index=0;
		switch(Seg_mode)
		{
			case 0:Seg_show(Seg_index,Seg_buf0[Seg_index]);break;
			case 1:Seg_show(Seg_index,Seg_buf1[Seg_index]);break;
			case 2:Seg_show(Seg_index,Seg_buf2[Seg_index]);break;
			case 3:Seg_show(Seg_index,Seg_buf3[Seg_index]);break;
			case 5:Seg_show(Seg_index,Seg_buf5[Seg_index]);break;
		}
		
		Seg_buf0[1]=temperature/10;
		Seg_buf0[2]=temperature%10;
		Seg_buf0[7]=light_degree;
		Seg_buf1[1]=motion;
		Seg_buf1[5]=(unsigned char)(distance[1]/100);
		Seg_buf1[6]=(unsigned char)((distance[1]/10)%10);
		Seg_buf1[7]=(unsigned char)(distance[1]%10);
		Seg_buf2[6]=temperature_set/10;
		Seg_buf2[7]=temperature_set%10;
		Seg_buf5[6]=distance_set/10;
		Seg_buf5[7]=distance_set%10;
		Seg_buf3[4]=(unsigned char)(Relay_time/1000);if(Seg_buf3[4]==0)Seg_buf3[4]=10;
		Seg_buf3[5]=(unsigned char)((Relay_time/100)%10);if(Seg_buf3[5]==0)Seg_buf3[5]=10;
		Seg_buf3[6]=(unsigned char)((Relay_time/10)%10);if(Seg_buf3[6]==0)Seg_buf3[6]=10;
		Seg_buf3[7]=(unsigned char)(Relay_time%10);
	}
}

void Other_proc()
{
	if(LED_counter>=10)
	{
		if(Seg_mode!=2 &&Seg_mode!=5)
		{
			if(near==0)
			{
				LED_show(1,0);
				LED_show(2,0);
				LED_show(3,0);
				LED_show(4,0);
			}
			else
			{
				switch(light_degree)
				{
					case 1:LED_show(1,1);
						   LED_show(2,0);
						   LED_show(3,0);
						   LED_show(4,0);break;
					case 2:LED_show(1,1);
						   LED_show(2,1);
						   LED_show(3,0);
						   LED_show(4,0);break;
					case 3:LED_show(1,1);
						   LED_show(2,1);
						   LED_show(3,1);
						   LED_show(4,0);break;
					case 4:LED_show(1,1);
						   LED_show(2,1);
						   LED_show(3,1);
						   LED_show(4,1);break;
				}
			}
			
			if(motion==1)
				LED_show(8,0);
			if(motion==2)
				LED_show(8,1);
			if(motion==3 && lightingLED==1)
				LED_show(8,1);
			else if(motion==3 && lightingLED==0)
				LED_show(8,0);
		}
	}
	if(motion_lock_3s>=3000)
	{
		motion_lock_3s=0;
		if(status_lock==1)
			status_lock=0;
	}
	
	if(Other_counter>=50)
	{
		Other_counter=0;
		temperature=(unsigned int)get_temperature();
		if(temperature>temperature_set)high=1;
		else high=0;
		if(Seg_mode!=2 && Seg_mode!=5)
		{
			if(high==1 && near==1 )
			{
				if(Relay_last==0)
				{
					Relay_control(1);
					Relay_last=1;
					Relay_time++;
				}
			}
			else
			{
				if(Relay_last==1)
				{
					Relay_control(0);
					Relay_last=0;
				}
			}	
		}
		
		light=AD_read(0x41);
		light_voltage=(float)(light/51.0);
		if(light_voltage>=5.0)
			light_degree=1;
		else if(light_voltage>=2.0)
			light_degree=2;
		else if(light_voltage>=0.5)
			light_degree=3;
		else
			light_degree=4;
	}
	if(wave_counter>=1000)
	{
		wave_counter=0;
		distance[0]=distance[1];
		distance[1]=(unsigned int)get_distance();
		if(distance[1]<distance_set)near=1;
		else near=0;
		
		if(distance[1]>=distance[0])
			delta=distance[1]-distance[0];
		else
			delta=distance[0]-distance[1];
		if(status_lock==0)
		{
			if(delta<5)
				motion=1;
			else if (delta<10)
				motion=2;
			else
				motion=3;
			
			if(motion!=last_motion)
			{
				motion_lock_3s=0;
				status_lock=1;
				last_motion=motion;
			}
		}
	}
}

void Timer0_Routine() interrupt 1
{
	Seg_counter++;
	Key_counter++;
	Other_counter++;
	wave_counter++;
	motion_lock_3s++;
	LED_counter++;
	timer1s++;
}

void main()
{
	System_init();
	Timer0_Init();
	
	init_ds18b20();
	Write_DS18B20(0xCC);
	Write_DS18B20(0x44);
	Delay750ms();
	
	while(1)
	{
		Seg_proc();
		Key_proc();
		Other_proc();
	}
}
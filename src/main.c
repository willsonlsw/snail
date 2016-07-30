#include<reg52.h>
#include"section.h"
#include"LCD_driver.h"

unsigned char W;//占空比0-100将一个周期分为100份
//extern xdata u8 record_data[50];
extern char srec;
extern bit erec;
//extern unsigned int rotset,rotact;

void Init_t0(void)
{
    W=0;
	EA=1;
	TMOD=0X51;
	TH0=248;//0xfd;
	TL0=48;//0x17;
	TH1=0;
	TL1=0;
	ET0=1;
	ET1=1;
	ET2=1;
	TR0=1;
	TR1=1;
	EX0=0;
	EX1=0;
	ES=0;
	IT0=1;
}	

main()
{
	Init_t0();
	srec=-1;
	erec=0;
	while(1)
	{
		StartUI();
	}
}
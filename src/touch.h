#ifndef __TOUCH_H__
#define __TOUCH_H__

#include<reg52.h>
#include <intrins.h>

#define u16   unsigned int
#define u8    unsigned char

sbit T_CS      =    P2^2;//P0^4;//P3^4;
sbit DIN       =    P3^2;//P0^0;//P3^5;
sbit DCLK	   =    P3^7;//P0^1;//P3^6;
sbit DOUT      =    P2^4;//P0^2;//P3^7;

//sbit BUSY      =    P2^1;	 
sbit IRQ       =    P2^3;//P3^3;   //��ⴥ������Ӧ�ź�

// A/D ͨ��ѡ�������ֺ͹����Ĵ���
#define	CMD_RDX 	0xD0
#define	CMD_RDY 	0x90
#define READ_TIMES 12 //��ȡ����
#define LOST_VAL    2    //����ֵ
#define ERR_RANGE 50 //��Χ 

#define LCDWIDTH		240
#define LCDHEIGHT		320





 extern u16 AD7843(u8 CMD) ;
 void inttostr(int dd,unsigned char *str);
 unsigned int ReadFromCharFrom7843();
 void WriteCharTo7843(unsigned char num) ;
 void spistar();



//������ȡREAD_TIMES������,����Щ������������,
//Ȼ��ȥ����ͺ����LOST_VAL����,ȡƽ��ֵ 
//unsigned int ADS_Read_AD(u8 CMD);
unsigned int ADS_Read_XY(u8 xy);//
unsigned char Read_ADS(u16 *x,u16 *y);////��Сֵ��������100.
unsigned char Read_ADS2(u16 *x,u16 *y);//2�ζ�ȡADS7846,������ȡ2����Ч��ADֵ,�������ε�ƫ��ܳ���ERR_RANGE

void Get_touch_coor(u16 *xx,u16 *yy);





#endif
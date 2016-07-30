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
sbit IRQ       =    P2^3;//P3^3;   //检测触摸屏响应信号

// A/D 通道选择命令字和工作寄存器
#define	CMD_RDX 	0xD0
#define	CMD_RDY 	0x90
#define READ_TIMES 12 //读取次数
#define LOST_VAL    2    //丢弃值
#define ERR_RANGE 50 //误差范围 

#define LCDWIDTH		240
#define LCDHEIGHT		320





 extern u16 AD7843(u8 CMD) ;
 void inttostr(int dd,unsigned char *str);
 unsigned int ReadFromCharFrom7843();
 void WriteCharTo7843(unsigned char num) ;
 void spistar();



//连续读取READ_TIMES次数据,对这些数据升序排列,
//然后去掉最低和最高LOST_VAL个数,取平均值 
//unsigned int ADS_Read_AD(u8 CMD);
unsigned int ADS_Read_XY(u8 xy);//
unsigned char Read_ADS(u16 *x,u16 *y);////最小值不能少于100.
unsigned char Read_ADS2(u16 *x,u16 *y);//2次读取ADS7846,连续读取2次有效的AD值,且这两次的偏差不能超过ERR_RANGE

void Get_touch_coor(u16 *xx,u16 *yy);





#endif
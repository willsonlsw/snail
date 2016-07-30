#include<reg52.h>
#include<I2C.H>
//#include<I2C.c>
#include<math.H>
#include <intrins.h>
#include"sensor.h"
#define  PCF8591 0x90    //PCF8591 地址

bit ISendByte(unsigned char sla,unsigned char c)
{
   Start_I2c();              //启动总线
   SendByte(sla);            //发送器件地址
   if(ack==0)return(0);
   SendByte(c);              //发送数据
   if(ack==0)return(0);
   Stop_I2c();               //结束总线
   return(1);
}

unsigned char IRcvByte(unsigned char sla)
{  unsigned char c;

   Start_I2c();          //启动总线
   SendByte(sla+1);      //发送器件地址
   if(ack==0)return(0);
   c=RcvByte();          //读取数据0
   Ack_I2c(1);           //发送非就答位
   Stop_I2c();           //结束总线
   return(c);
}

void GetAcc(int *Roll,int *Pitch)
{
	idata double X,Y,Z;
	idata unsigned int  D[4];

 	ISendByte(PCF8591,0x41);
    D[0]=IRcvByte(PCF8591)*2;  //ADC0 模数转换1  放大2倍显示  
 	_nop_();_nop_();_nop_();_nop_();_nop_();
	ISendByte(PCF8591,0x42);
    D[1]=IRcvByte(PCF8591)*2;  //ADC1  模数转换2  
 	_nop_();_nop_();_nop_();_nop_();_nop_();
	ISendByte(PCF8591,0x43);
    D[2]=IRcvByte(PCF8591)*2;  //ADC2	模数转换3  
 	_nop_();_nop_();_nop_();_nop_();_nop_();
	ISendByte(PCF8591,0x40);
    D[3]=IRcvByte(PCF8591)*2;  //ADC3   模数转换4  

	X=-(D[1]-165);
	Y=D[2]-165;
	Z=D[3]-165;
	*Roll=((atan2(Z,X)*180)/3.14159265)+180;
	*Pitch=((atan2(Z,Y)*180)/3.14159265)+180;
	return;
}

unsigned int GetDis(int *d0)
{
	idata unsigned int  D,tmp;
	idata double temp=0;

	ISendByte(PCF8591,0x41);
    D=IRcvByte(PCF8591)*2;  //ADC1  模数转换2  

	*d0=D;

	temp=(double)D/100.0;
    tmp=-34.6638*temp*temp*temp+191.8747*temp*temp-360.4113*temp+264.9415;
    if(tmp<=20){  tmp=0;  }
    else if(tmp>=150) { tmp =0; }

	return tmp;
}

unsigned int GetDisAll(int *d0,int *d1,int *d2,int *d3)
{
	idata unsigned int  D[4],tmp;
	idata double temp=0;

 	ISendByte(PCF8591,0x40);
    D[0]=IRcvByte(PCF8591)*2;  //ADC0 模数转换1  放大2倍显示  
	ISendByte(PCF8591,0x41);
    D[1]=IRcvByte(PCF8591)*2;  //ADC1  模数转换2  
	ISendByte(PCF8591,0x42);
    D[2]=IRcvByte(PCF8591)*2;  //ADC2	模数转换3  
	ISendByte(PCF8591,0x43);
    D[3]=IRcvByte(PCF8591)*2;  //ADC3   模数转换4  

	*d0=D[0];
	*d1=D[1];
	*d2=D[2];
	*d3=D[3];

	temp=(double)D[1]/100.0;
    tmp=-34.6638*temp*temp*temp+191.8747*temp*temp-360.4113*temp+264.9415;
    if(tmp<=20){  tmp=0;  }
    else if(tmp>=150) { tmp =0; }

	return tmp;
}

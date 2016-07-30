#include<reg52.h>
#include<I2C.H>
#include<I2C.c>
#include<math.H>
#define  PCF8591 0x90    //PCF8591 ��ַ

//else IO
sbit    LS138A=P2^2;  
sbit    LS138B=P2^3;
sbit    LS138C=P2^4;  

//�˱�Ϊ LED ����ģ, ��������� 0-9  - 
unsigned char code Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f}; 

unsigned char AD_CHANNEL;
unsigned long xdata  LedOut[8];
unsigned int  D[4];//tmp;
//double temp=0;
int Roll,Pitch;
double X,Y,Z;
/*******************************************************************
ADC�����ֽ�[����]���ݺ���               
*******************************************************************/
bit ISendByte(unsigned char sla,unsigned char c)
{
   Start_I2c();              //��������
   SendByte(sla);            //����������ַ
   if(ack==0)return(0);
   SendByte(c);              //��������
   if(ack==0)return(0);
   Stop_I2c();               //��������
   return(1);
}

/*******************************************************************
ADC���ֽ����ݺ���               
*******************************************************************/
unsigned char IRcvByte(unsigned char sla)
{  unsigned char c;

   Start_I2c();          //��������
   SendByte(sla+1);      //����������ַ
   if(ack==0)return(0);
   c=RcvByte();          //��ȡ����0
   Ack_I2c(1);           //���ͷǾʹ�λ
   Stop_I2c();           //��������
   return(c);
}

//******************************************************************/
main()
{  char i,j;

 while(1)
 {/********����AD-DA����*************/  
   switch(AD_CHANNEL)
   {
     case 0: ISendByte(PCF8591,0x41);
             D[0]=IRcvByte(PCF8591)*2;  //ADC0 ģ��ת��1  �Ŵ�2����ʾ
			 break;  
 
	 case 1: ISendByte(PCF8591,0x42);
             D[1]=IRcvByte(PCF8591)*2;  //ADC1  ģ��ת��2
			 break;  

	 case 2: ISendByte(PCF8591,0x43);
             D[2]=IRcvByte(PCF8591)*2;  //ADC2	ģ��ת��3
			 break;  

	 case 3: ISendByte(PCF8591,0x40);
             D[3]=IRcvByte(PCF8591)*2;  //ADC3   ģ��ת��4
			 break;  
   }   

   if(++AD_CHANNEL>3) AD_CHANNEL=0;

 /********���½�AD��ֵ�͵�LED�������ʾ*************/
     /*��������ֵת��*/
     //temp=D[0]/100;
     //tmp=-34.6638*temp*temp*temp+191.8747*temp*temp-360.4113*temp+264.9415;
     //if(tmp<=20){  tmp=0;  }
     //else if(tmp>=150) { tmp =0; }	 
     //LedOut[0]=Disp_Tab[tmp%10000/1000];
     //LedOut[1]=Disp_Tab[tmp%1000/100];
     //LedOut[2]=Disp_Tab[tmp%100/10];
     //LedOut[3]=Disp_Tab[tmp%10];
	 /*���ٶȴ�������ֵת��*/
	 X=-(D[1]-165);
	 Y=D[2]-165;
	 Z=D[3]-165;
	 Roll=((atan2(Z,X)*180)/3.14159265)+180;
	 Pitch=((atan2(Z,Y)*180)/3.14159265)+180;
	 LedOut[0]=Disp_Tab[Roll%10000/1000];
     LedOut[1]=Disp_Tab[Roll%1000/100];
     LedOut[2]=Disp_Tab[Roll%100/10];
     LedOut[3]=Disp_Tab[Roll%10];
	 LedOut[4]=Disp_Tab[Pitch%10000/1000];
     LedOut[5]=Disp_Tab[Pitch%1000/100];
     LedOut[6]=Disp_Tab[Pitch%100/10];
     LedOut[7]=Disp_Tab[Pitch%10];
     /******************/
	 for( i=0; i<8; i++) 
	 {	 P0 = LedOut[i];
			
	     switch(i)	  //ʹ��switch ������138������  Ҳ�������ò��ķ�ʽ ѧԱ���������Լ��޸�				  
	     {	    
			case 0:LS138A=0; LS138B=0; LS138C=0; break;         
	        case 1:LS138A=1; LS138B=0; LS138C=0; break;             	
	        case 2:LS138A=0; LS138B=1; LS138C=0; break; 
	        case 3:LS138A=1; LS138B=1; LS138C=0; break;
			case 4:LS138A=0; LS138B=0; LS138C=1; break;
			case 5:LS138A=1; LS138B=0; LS138C=1; break;
			case 6:LS138A=0; LS138B=1; LS138C=1; break;
			case 7:LS138A=1; LS138B=1; LS138C=1; break; 			
	     } 
	     for (j = 0 ; j<90 ;j++) { ;}	   //ɨ����ʱ��
	  }

	    P0 = 0; 

 }  
}




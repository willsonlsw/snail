

#include "touch.h"


//**********************************************************
void spistar()                                     //SPI��ʼ
{
	DCLK=0;
	T_CS=1;
	_nop_();_nop_();_nop_();
	DIN=1;
	DCLK=1;
	_nop_();_nop_();_nop_(); 
	T_CS=0;
	_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();

}
//**********************************************************
void WriteCharTo7843(unsigned char num)          //SPIд����
{
	unsigned char count=0;
	DCLK=0;
	for(count=0;count<8;count++)
	{
	num<<=1;
	DIN=CY;
	DCLK=0; _nop_();_nop_();_nop_();                //��������Ч
	DCLK=1; _nop_();_nop_();_nop_();
	}
}
//**********************************************************
unsigned int ReadFromCharFrom7843()             //SPI ������
{
	unsigned char count=0;
	unsigned int Num=0;
	for(count=0;count<12;count++)
	{
	Num<<=1;
	DCLK=1; _nop_();_nop_();_nop_();                //�½�����Ч
	DCLK=0; _nop_();_nop_();_nop_();
	if(DOUT) Num++;
	}
	return(Num);
}


void inttostr(int dd,unsigned char *str)
{
	str[0]=dd/10000+48;	    //��λ  48����˼�ǣ�����IC��12λ��AD�� ��48�����λ��4λ��ȡ��12λ������
	str[1]=(dd/1000)-((dd/10000)*10)+48; //ʮλ
	str[2]=(dd/100)-((dd/1000)*10)+48;	//��λ
	str[3]=(dd/10)-((dd/100)*10)+48; //ǧλ
	str[4]=dd-((dd/10)*10)+48; //��λ
	str[5]=0;
}
u16 AD7843(u8 CMD)              //�ⲿ�ж�0 �������ܼ��̷���������
{
	u16 AD_Data;
	//delayms(1);                     //�жϺ���ʱ������������ʹ�ò������ݸ�׼ȷ
	spistar();                       //����SPI
	//while(BUSY);                //���BUSY�źŲ���ʹ����ɾ������
	//delayms(1);
	WriteCharTo7843(CMD);        //�Ϳ����� 10010000 ���ò�ַ�ʽ��X���� ��ϸ����й�����
	//while(BUSY);               //���BUSY�źŲ���ʹ����ɾ������
	//delayms(1);
	DCLK=1; _nop_();_nop_();_nop_();_nop_();
	DCLK=0; _nop_();_nop_();_nop_();_nop_();
	AD_Data =ReadFromCharFrom7843();
	//WriteCharTo7843(0xD0);       //�Ϳ����� 11010000 ���ò�ַ�ʽ��Y���� ��ϸ����й�����
	//DCLK=1; _nop_();_nop_();_nop_();_nop_();
	//DCLK=0; _nop_();_nop_();_nop_();_nop_();
	//TP_X=ReadFromCharFrom7843();
	_nop_();_nop_();_nop_(); 
	_nop_();_nop_();_nop_(); 
	_nop_();_nop_();_nop_(); 
	_nop_();_nop_();_nop_(); 
	T_CS=1;
	
	return  AD_Data	;
}

//��ȡһ������ֵ
//������ȡREAD_TIMES������,����Щ������������,
//Ȼ��ȥ����ͺ����LOST_VAL����,ȡƽ��ֵ 

unsigned int ADS_Read_XY(u8 xy)
{
	u16 xdata i, j;
	u16 xdata buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)
	{				 
	  	buf[i]=AD7843(xy);	    
	}				    
	for(i=0;i<READ_TIMES-1; i++)//����
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf
			[i]>buf[j])//��������
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 


//2�ζ�ȡADS7846,������ȡ2����Ч��ADֵ,�������ε�ƫ��ܳ���
//50,��������,����Ϊ������ȷ,�����������.	   
//�ú����ܴ�����׼ȷ��

unsigned char Read_ADS2(u16 *x,u16 *y) 
{
    u16 xdata  x1,y1;
    u16 xdata x2,y2;
    u8 xdata flag;    
    flag=Read_ADS(&x1,&y1);   
    if(flag==0)return(0);
    flag=Read_ADS(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//ǰ�����β�����+-50��
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
} 


//���˲��������ȡ
//��Сֵ��������100.
unsigned char Read_ADS(u16 *x,u16 *y)
{
	u16 xdata xtemp,ytemp;			 	 		  
	xtemp=ADS_Read_XY(CMD_RDX);
	ytemp=ADS_Read_XY(CMD_RDY);	   												   
	if(xtemp<100||ytemp<100)return 0;//����ʧ��
	*x=xtemp;
	*y=ytemp;
	return 1;//�����ɹ�
}

//�������������ֵ
void Get_touch_coor(u16 *xx,u16 *yy)
{
	u16 Xdata,Ydata;
	unsigned long i;
	Read_ADS2(&Xdata,&Ydata);
	i=Xdata;
	i=i*240;
	*xx=i/3830;
	if(*xx>240) *xx=240;
	i=Ydata;
	i=i*320;
	*yy=i/3804;
	if(*yy>320) *yy=320;
	if(*yy<=320) *yy=320- *yy;
}


#include<reg52.h>
#include<intrins.h>
#include"LCD_driver.h"
#include"touch.h"
#include"section.h"
#include"sensor.h"

idata unsigned int rotset=0,rottmp=0;//rotact=0;
xdata unsigned char buttonID;//计算每个周期脉冲数
idata unsigned int T;
idata unsigned int TT=0,count_time=0,count=0,Acc_time=0;
idata char chart_time=0;
idata u16 corx,cory; 
idata u8 temp_w[51],temp[51];
xdata u8 record_w[51],record_data[51];
idata u8 Left,Right,ReChk_time=0,Chk=2,Chk_label=0;
char srec;
idata bit erec,resetrot;
idata bit Acc_enable=0,Dis_enable=0,ConstantPower=0;
idata u16 Acc_data[10];
u8 tex[5];

void time0_server_(void)interrupt 1	 //中段服务程序
{
	TH0=248;//0xfe;  //重置计数器
	TL0=48;//0x17;
	T++;
	TT++;
	count_time++;
	if(T>W)	 //0-W内输出高电平
	{
		 PWM=0;
	}
	else
	{
		 PWM=1;
	}
	if(T==100)	 
	{
		T=0;
	}
	if(TT==500) TT=0;
	if(count_time==900)
 	{
		count=TH1*256+TL1;
		TH1=0;
		TL1=0;
	}
	else if(count_time==1000) count_time=0;
}	

void Fill4(u16 color)
{
	LCD_Write_DATA(color>>8,color);
	LCD_Write_DATA(color>>8,color);
	LCD_Write_DATA(color>>8,color);
	LCD_Write_DATA(color>>8,color);
}
//创建按钮
void Button(u8 x1,u16 y1,u8 x2,u16 y2,u8 k,char num,u16 col1,u16 col2)
{
	idata u16 i,j,l;
	Address_set(x1,y1,x2,y2);
	for(j=y1;j<=y2;j++)
		for(i=x1;i<=x2;i++)
			 LCD_Write_DATA(0xff,0xff);			
	for(i=x1;i<=x2;i++)
	{
		Address_set(i,y1,i,y1+3);
		Fill4(col1);
	}
	for(i=x1;i<=x2;i++)
	{
		Address_set(i,y2-3,i,y2);
		Fill4(col2);
	}
	for(i=y1;i<=y2;i++)
	{
		Address_set(x1,i,x1+3,i);
		Fill4(col1);
	}
	for(i=y1;i<=y2;i++)
	{
		Address_set(x2-3,i,x2,i);
		Fill4(col2);
	}
	for(j=0;j<=3;j++)
	{
		for(i=0;i<=j;i++)
		{
			Address_set(x1+3-i,y2-3+j,x1+3-i,y2-3+j);
			LCD_Write_DATA(col2>>8,col2);
		}
	}
	Address_set(x2-3,y1,x2-1,y1);
	LCD_Write_DATA(col1>>8,col1);
	LCD_Write_DATA(col1>>8,col1);
	LCD_Write_DATA(col1>>8,col1);
	Address_set(x2-3,y1+1,x2-2,y1+1);
	LCD_Write_DATA(col1>>8,col1);
	LCD_Write_DATA(col1>>8,col1);
	Address_set(x2-3,y1+2,x2-3,y1+2);
	LCD_Write_DATA(col1>>8,col1);
	if(num==-1)
	{
		l=((x2-x1)-k*24)/2;
		j=((y2-y1)-24)/2;
		for(i=0;i<k;i++)
		{
			ChineseChar(l+x1,y1+j,1,colors[4],colors[7],tex[i]+1);	
			l+=24;
		}
	}
	else
	{
		l=((x2-x1)-16)/2;
		j=((y2-y1)-24)/2;
		LCD_ShowChar(l+x1,j+y1,colors[4],colors[7],num+0x30,16);
	} 
	return;
}

void LCD_ShowChinese(u8 x,u16 y,u8 size,u16 col1,u16 col2,u8 k)
{
	u16 i,j=0,ll;
	if(size==16) ll=0;
	else if(size==24) ll=1;
	for(i=0;i<k;i++)
	{
		ChineseChar(j+x,y,ll,col1,col2,tex[i]);
		j+=size;
	}
}

//按钮按下效果
void ButtonDown(u8 x1,u16 y1,u8 x2,u16 y2,u16 col1,u16 col2)
{	
	idata u16 i;
	for(i=x1;i<=x2;i++)
	{
		Address_set(i,y1,i,y1+3);
		Fill4(col2);
	}
	for(i=x1;i<=x2;i++)
	{
		Address_set(i,y2-3,i,y2);
		Fill4(col1);
	}
	for(i=y1;i<=y2;i++)
	{
		Address_set(x1,i,x1+3,i);
		Fill4(col2);
	}
	for(i=y1;i<=y2;i++)
	{
		Address_set(x2-3,i,x2,i);
		Fill4(col1);
	}
}

//按钮弹起效果
void ButtonUp(u8 x1,u16 y1,u8 x2,u16 y2,u16 col1,u16 col2)
{
	idata u16 i,j;
	for(i=x1;i<=x2;i++)
	{
		Address_set(i,y1,i,y1+3);
		Fill4(col1);
	}
	for(i=x1;i<=x2;i++)
	{
		Address_set(i,y2-3,i,y2);
		Fill4(col2);
	}
	for(i=y1;i<=y2;i++)
	{
		Address_set(x1,i,x1+3,i);
		Fill4(col1);
	}
	for(i=y1;i<=y2;i++)
	{
		Address_set(x2-3,i,x2,i);
		Fill4(col2);
	}
	for(j=0;j<=3;j++)
	{
		for(i=0;i<=j;i++)
		{
			Address_set(x1+3-i,y2-3+j,x1+3-i,y2-3+j);
			LCD_Write_DATA(col2>>8,col2);
		}
	}
	Address_set(x2-3,y1,x2-1,y1);
	LCD_Write_DATA(col1>>8,col1);
	LCD_Write_DATA(col1>>8,col1);
	LCD_Write_DATA(col1>>8,col1);
	Address_set(x2-3,y1+1,x2-2,y1+1);
	LCD_Write_DATA(col1>>8,col1);
	LCD_Write_DATA(col1>>8,col1);
	Address_set(x2-3,y1+2,x2-3,y1+2);
	LCD_Write_DATA(col1>>8,col1);
}	 

//快速画线
void LineF(int x0,int y0,int x1,int y1,u16 color)
{
	idata u16 x,y,yo;
	float m;
	m=(float)(y1-y0)/(x1-x0);
	yo=y0;
	for(x=x0;x<=x1;x++)
	{
		y=(float)m*(x-x0)+y0;
		if(y>yo) FillRec(x,yo+1,x,y,color);
		else if(y<yo) FillRec(x,y,x,yo-1,color);
		else FillRec(x,y,x,y,color);
		yo=y;
	}
}


void FillRec(u8 x1,u16 y1,u8 x2,u16 y2,u16 col)
{
	u16 i,j;
	Address_set(x1,y1,x2,y2);
	for(i=y1;i<=y2;i++)
		for(j=x1;j<=x2;j++)
			LCD_Write_DATA(col>>8,col);
}

void ShowRotSpeed()
{
	srec++;
	ReChk_time++;
	if(srec>=50) 
	{
		if(erec==0)
		{
			CopyToTemp();
			if(count>170) temp[49]=170;
			else temp[49]=count;
			temp_w[49]=W;
			erec=1;
		}
		srec=0;
	}
	if(count>170) record_data[srec]=170; 
	else record_data[srec]=count;
	record_w[srec]=W;
	LCD_ShowString(200,24,colors[4],colors[6],"/Sec",8);
    LCD_ShowNumber(156,24,colors[4],colors[6],count,8);
	LCD_ShowString(200,7,colors[4],colors[6],"%PWM",8);
	LCD_ShowNumber(156,7,colors[4],colors[6],W,8);
}

int GetPWM(int rs)
{
	double p=8.095*0.000001*rs*rs*rs*rs*rs*rs*rs*rs+7.905;
	return p;
}

void RotChk()
{
	u8 d;
	if(Left+1>=Right)
	{
		//if(Chk_label>=2) Chk=1;
		//else Chk_label++;
		//if(count+1<rotset) Right+=32;
		//else if(count-1>rotset) Left-=32;
/*
		if(count+5<rotset) Right+=32;
		else if(count+1<rotset) Right+=2;
		else if(count-5>rotset) Left-=32;
		else if(count-1>rotset) Left-=2;
*/
		if(count+1<rotset) Right+=(rotset-count);
		else if(count-1>rotset) Left-=(count-rotset);
		if(Right>101) Right=101;
		if(Left<0) Left=0;		
	}
	if(rotset==0) W=0;
	else if(count<rotset)
	{
/*		if(resetrot==1) 
		{
			Right=GetPWM(rotset)+5;
			resetrot=0;
		}
		if(Right>101) Right=101;
*/
		Left=W;
		W=(Left+Right)>>1;
	}
	else if(rotset<count)
	{
		Right=W-1;
		W=(Left+Right)>>1;
	}
	if(Left>Right)
	{
		d=Left;
		Left=Right;
		Right=d;
	}
	if(W>100) W=100;
	if(W<0) W=0;
}

void StartUI()
{		 
	int aa,bb,d0;
	idata u16 dis;

    LCD_Init();
	Pant(0xf8,0x1f);
	LCD_CS=0;
	
	tex[0]=3;tex[1]=11;
	Button(50,50,190,100,2,-1,0x07ff,0x00ff);
	tex[0]=0;tex[1]=1;
	Button(50,110,190,160,2,-1,0x07ff,0x00ff);
	tex[0]=2;tex[1]=1;tex[2]=3;tex[3]=4;
	Button(50,170,190,220,4,-1,0x07ff,0x00ff);
	tex[0]=2;tex[1]=1;tex[2]=12;tex[3]=13;
	Button(50,230,190,280,4,-1,0x07ff,0x00ff);
	LCD_ShowString(45,300,colors[4],colors[6],"Version:Snail 1.1.3",8);
	LCD_CS=1;
	while(1)
	{
		LCD_CS=0;
		buttonID=-1;

/////////////////////////////////////////////////////
		if(count_time==800)
		{
			GetAcc(&aa,&bb);
			LCD_ShowNumber(10,10,colors[4],colors[6],aa,8);
			LCD_ShowNumber(10,30,colors[4],colors[6],bb,8);
		}
		if(count_time==500)
		{
			dis=GetDis(&d0);
			LCD_ShowNumber(10,50,colors[4],colors[6],dis,8);
			LCD_ShowNumber(10,70,colors[4],colors[6],d0,8);

		}
/////////////////////////////////////////////////////

		if(count_time==999) 
		{
			ShowRotSpeed();
			if(ConstantPower==0 && ReChk_time>=Chk) 
			{
				RotChk();
				ReChk_time=0;
			}
		}
		if(IRQ==0)
		{
			Get_touch_coor(&corx,&cory);
			if(50<=corx && corx<=190 && 110<= cory && cory<=160)
			{
				ButtonDown(50,110,190,160,0x07ff,0x00ff);
				buttonID=0;
			}
			else if(50<=corx && corx<=190 && 170<= cory && cory<=220)
			{
				ButtonDown(50,170,190,220,0x07ff,0x00ff);
				buttonID=1;
			}
			else if(50<=corx && corx<=190 && 230<=cory && cory<=280)
			{
				ButtonDown(50,230,190,280,0x07ff,0x00ff);
				buttonID=2;
			}
			else if(50<=corx && corx<=190 && 50<=cory && cory<=100)
			{
				ButtonDown(50,50,190,100,0x07ff,0x00ff);
				buttonID=3;
			}
			while(IRQ==0);
			if(buttonID==0) 
			{
				FixedSpeedUI();
				return;
			}
			else if(buttonID==1)
			{
				SetSpeedUI();
				return;
			}
			else if(buttonID==2)
			{
				RSrecordUI();
				return;
			}
			else if(buttonID==3)
			{
				SetUI();
				return;
			}
		}
	} 
}

void SelButtonUp(u8 x1,u16 y1)
{
	FillRec(x1,y1,x1+29,y1+29,0xc618);
	ChineseChar(x1+3,y1+3,1,0x8410,0xc618,27);
	//FillRec(x1+8,y1+8,x1+21,y1+21,0x8410);
}

void SelButtonDown(u8 x1,u16 y1)
{	
	FillRec(x1,y1,x1+29,y1+29,0xc618);
	ChineseChar(x1+3,y1+3,1,0x001f,0xc618,27);
	//FillRec(x1+8,y1+8,x1+21,y1+21,colors[5]);
}
	

void SetUI()
{
	Pant(0xf8,0x1f);
	LCD_CS=0;
	tex[0]=5;tex[1]=6;
	Button(10,10,70,50,2,-1,0x07ff,0x00ff);

	tex[0]=11;tex[1]=17;tex[2]=18;tex[3]=19;
	LCD_ShowChinese(30,100,24,colors[4],colors[6],4);
	if(!Acc_enable) SelButtonUp(180,97);
	else SelButtonDown(180,97);
	tex[0]=20;tex[1]=21;tex[2]=22;tex[3]=23;
	LCD_ShowChinese(30,140,24,colors[4],colors[6],4);
	if(!Dis_enable) SelButtonUp(180,137);
	else SelButtonDown(180,137);
	tex[0]=24;tex[1]=5;tex[2]=25;tex[3]=26;
	LCD_ShowChinese(30,180,24,colors[4],colors[6],4);
	if(!ConstantPower) SelButtonUp(180,177);
	else SelButtonDown(180,177);
	tex[0]=1;tex[1]=5;tex[2]=3;tex[3]=2;
	LCD_ShowChinese(30,220,24,colors[4],colors[6],4);
	if(ConstantPower) SelButtonUp(180,217);
	else SelButtonDown(180,217);
 
	while(1)
	{
		LCD_CS=0;
	    buttonID=-1;		
		if(count_time==999)
	 	{
			ShowRotSpeed();
			if(ConstantPower==0 && ReChk_time>=Chk) 
			{
				RotChk();
				ReChk_time=0;
			}
		}

		if(IRQ==0)
		{
			Get_touch_coor(&corx,&cory);
			if(10<=corx && corx<=70 && 10<=cory && cory<=50)
			{
				buttonID=0;
				ButtonDown(10,10,70,50,0x07ff,0x00ff);
			}
			else if(180<=corx && corx<=210 && 97<=cory && cory<=127) buttonID=1;
			else if(180<=corx && corx<=210 && 137<=cory && cory<=167) buttonID=2;
			else if(180<=corx && corx<=210 && 177<=cory && cory<=207) buttonID=3;
			else if(180<=corx && corx<=210 && 217<=cory && cory<=247) buttonID=4;

			while(IRQ==0);

			if(buttonID==0) return;
			if(buttonID==1)
			{
				if(!Acc_enable)
				{
					Acc_enable=1;
					SelButtonDown(180,97);
				}
				else {
						Acc_enable=0;
						SelButtonUp(180,97);
					 }
			}
			else if(buttonID==2)
			{
				if(!Dis_enable)
				{
					Dis_enable=1;
					SelButtonDown(180,137);
				}
				else {
						Dis_enable=0;
						SelButtonUp(180,137);
					 }
			}
			else if(buttonID==3)
			{
				if(!ConstantPower)
				{
					ConstantPower=1;
					SelButtonDown(180,177);
					SelButtonUp(180,217);
				}
				else {
						ConstantPower=0;
						SelButtonUp(180,177);
						SelButtonDown(180,217);
					 }
			}
			else if(buttonID==4)
			{
				if(ConstantPower)
				{
					ConstantPower=0;
					SelButtonDown(180,217);
					SelButtonUp(180,177);
				}
				else {
						ConstantPower=1;
						SelButtonUp(180,217);
						SelButtonDown(180,177);
					 }
			}				
		}
	}
}

void FixedSpeedUI()
{
	Pant(0xf8,0x1f);
	LCD_CS=0;
	tex[0]=5;tex[1]=6;
	Button(10,10,70,50,2,-1,0x07ff,0x00ff);
	tex[0]=7;tex[1]=1;
	Button(30,160,100,280,2,-1,0x07ff,0x00ff);
	tex[0]=8;tex[1]=1;
	Button(140,160,210,280,2,-1,0x07ff,0x00ff);
	FillRec(110,126,195,128,0xffe0);
	
	if(ConstantPower)
	{
    	LCD_ShowString(200,100,colors[4],colors[6],"%",16);
		LCD_ShowString(40,100,colors[4],colors[6],"PWM:",16);
		LCD_ShowNumber(110,100,colors[4],colors[6],W,16);
	}
	else
	{
		ChineseChar(40,100,1,colors[4],colors[6],3);
		ChineseChar(65,100,1,colors[4],colors[6],2);
		LCD_ShowChar(91,100,colors[4],colors[6],':',16);
		LCD_ShowString(200,100,colors[4],colors[6],"/s",16);
		LCD_ShowNumber(110,100,colors[4],colors[6],rotset,16);
	}

	while(1)
	{
		LCD_CS=0;
	    buttonID=-1;
		
		if(count_time==999) 
		{
			ShowRotSpeed();
			if(ConstantPower==0 && ReChk_time>=Chk) 
			{
				RotChk();
				ReChk_time=0;
			}
		}
		if(IRQ==0)
		{
			Get_touch_coor(&corx,&cory);
			if(10<=corx && corx<=70 && 10<=cory && cory<=50)
			{
				buttonID=0;
				ButtonDown(10,10,70,50,0x07ff,0x00ff);
			}
			else if(30<=corx && corx<=100 && 160<=cory && cory<=280)
			{
				buttonID=1;
				ButtonDown(30,160,100,280,0x07ff,0x00ff);
			}
			else if(140<=corx && corx<=210 && 160<=cory && cory<=280)
			{
				buttonID=2;
				ButtonDown(140,160,210,280,0x07ff,0x00ff);
			}

			while(IRQ==0)
			{
				if(buttonID==1)
				{
					//电机加速代码
					if(!ConstantPower)
					{
							if(rotset==1000)
							{
								rotset=1000;
							}
							else
							{
								rotset+=1;
							}
							LCD_ShowNumber(110,100,colors[4],colors[6],rotset,16);
							Left=0;Right=101;Chk=2;rottmp=rotset;resetrot=1;
					}
					else 
					{
							if(W==100)
							{
								W=1000;
							}
							else
							{
								W+=1;
							}
							LCD_ShowNumber(110,100,colors[4],colors[6],W,16);	
					}
				}
				else if(buttonID==2)
				{
					//电机减速代码
					if(!ConstantPower)
					{
							if(rotset==0)
							{
								rotset=0;
							}
							else
							{
								rotset-=1;
							}
							LCD_ShowNumber(110,100,colors[4],colors[6],rotset,16);
							Left=0;Right=101;Chk=2;rottmp=rotset;resetrot=1;
					}
					else
					{
							if(W==0)
							{
								W=0;
							}
							else
							{
								W-=1;
							}
							LCD_ShowNumber(110,100,colors[4],colors[6],W,16);
					}
				}
				while(IRQ==0)
				{
					if(TT==400) break;
					if(count_time==999) ShowRotSpeed();
				}
			}

//			while(IRQ==0);
			if(buttonID==0) return;
			else if(buttonID==1)
			{
				ButtonUp(30,160,100,280,0x07ff,0x00ff);
			}
			else if(buttonID==2)
			{						
				ButtonUp(140,160,210,280,0x07ff,0x00ff);
			}
		}
	}		
} 

void SetSpeedUI()
{
	idata unsigned int tmp=0;	

	Pant(0xf8,0x1f);
	LCD_CS=0;
	tex[0]=5;tex[1]=6;
	Button(10,10,70,50,2,-1,0x07ff,0x001f);

    Button(15,130,84,169,0,7,0xffe0,0xfca0);
	Button(85,130,154,169,0,8,0xffe0,0xfca0);
	Button(155,130,225,169,0,9,0xffe0,0xfca0);
	Button(15,170,84,209,0,4,0xffe0,0xfca0);
	Button(85,170,154,209,0,5,0xffe0,0xfca0);
	Button(155,170,225,209,0,6,0xffe0,0xfca0);
	Button(15,210,84,249,0,1,0xffe0,0xfca0);
	Button(85,210,154,249,0,2,0xffe0,0xfca0);
	Button(155,210,225,249,0,3,0xffe0,0xfca0);
	Button(15,250,84,290,0,0,0xffe0,0xfca0);
    tex[0]=9;tex[1]=4;
	Button(85,250,154,290,2,-1,0xffe0,0xfca0);
	tex[0]=10;tex[1]=11;
	Button(155,250,225,290,2,-1,0xffe0,0xfca0);		

//  LCD_ShowString(190,24,colors[4],colors[6],"/min",8);
//	LCD_ShowString(170,80,colors[4],colors[6],"/min",16);
//	FillRec(30,110,165,112,0x07e0);
	
//	rotset=0;
	if(ConstantPower)
	{
    	LCD_ShowNumber(110,80,colors[4],colors[6],W,16);
	    LCD_ShowString(40,80,colors[4],colors[6],"PWM:",16);
        LCD_ShowString(200,80,colors[4],colors[6],"%",16);
	}
	else 
	{
		ChineseChar(40,80,1,colors[4],colors[6],3);
		ChineseChar(65,80,1,colors[4],colors[6],2);
		LCD_ShowChar(91,80,colors[4],colors[6],':',16);
		LCD_ShowString(200,80,colors[4],colors[6],"/s",16);	
		LCD_ShowNumber(110,80,colors[4],colors[6],rotset,16);
	}

	FillRec(110,106,195,108,0xffe0);		
    while(1)
	{
		LCD_CS=0;
		buttonID=-1;
		if(count_time==999)
	 	{
			ShowRotSpeed();
			if(ConstantPower==0 && ReChk_time>=Chk) 
			{
				RotChk();
				ReChk_time=0;
			}
		}
		if(IRQ==0)
		{
			Get_touch_coor(&corx,&cory);
			if(10<=corx && corx<=70 && 10<=cory && cory<=50) 
			{
				buttonID=10;
				ButtonDown(10,10,70,50,0x07ff,0x00ff);
			}
			else if(15<=corx && corx<=84 && 130<=cory && cory<=169)
			{
				buttonID=7;
				ButtonDown(15,130,84,169,0xffe0,0xfca0);
			}
			else if(85<=corx && corx<=154 && 130<=cory && cory<=169)
			{
				buttonID=8;
				ButtonDown(85,130,154,169,0xffe0,0xfca0);
			}
			else if(155<=corx && corx<=225 && 130<=cory && cory<=169)
			{
				buttonID=9;
				ButtonDown(155,130,225,169,0xffe0,0xfca0);
			}
			else if(15<=corx && corx<=84 && 170<=cory && cory<=209)
			{
				buttonID=4;
				ButtonDown(15,170,84,209,0xffe0,0xfca0);
			}
			else if(85<=corx && corx<=154 && 170<=cory && cory<=209)
			{
				buttonID=5;
				ButtonDown(85,170,154,209,0xffe0,0xfca0);
			}
			else if(155<=corx && corx<=225 && 170<=cory && cory<=209)
			{
				buttonID=6;
				ButtonDown(155,170,225,209,0xffe0,0xfca0);
			}
			else if(15<=corx && corx<=84 && 210<=cory && cory<=249)
			{
				buttonID=1;
				ButtonDown(15,210,84,249,0xffe0,0xfca0);
			}
			else if(85<=corx && corx<=154 && 210<=cory && cory<=249)
			{
				buttonID=2;
				ButtonDown(85,210,154,249,0xffe0,0xfca0);
			}
			else if(155<=corx && corx<=225 && 210<=cory && cory<=249)
			{
				buttonID=3;
				ButtonDown(155,210,225,249,0xffe0,0xfca0);
			}
			else if(15<=corx && corx<=84 && 250<=cory && cory<=290)
			{
				buttonID=0;
				ButtonDown(15,250,84,290,0xffe0,0xfca0);
			}
			else if(85<=corx && corx<=154 && 250<=cory && cory<=290)
			{
				buttonID=11;
				ButtonDown(85,250,154,290,0xffe0,0xfca0);
			}
			else if(155<=corx && corx<=225 && 250<=cory && cory<=290)
			{
				buttonID=12;
				ButtonDown(155,250,225,290,0xffe0,0xfca0);	
			}

			while(IRQ==0);
			if(buttonID==10) return;

			if(0<=buttonID && buttonID<=9)
			{
				if(ConstantPower==0)
				{
					if(tmp*10+buttonID<=1000)
						tmp=tmp*10+buttonID;
					else tmp=1000;
//             	LCD_ShowNumber(70,80,colors[4],colors[6],rotset,16);
					LCD_ShowNumber(110,80,colors[4],colors[6],tmp,16);	
				}
				else 
			   	{
					if(tmp*10+buttonID<=100)
						tmp=tmp*10+buttonID;
					else tmp=100;
					LCD_ShowNumber(110,80,colors[4],colors[6],tmp,16);
				}
			}
			else if(buttonID==11)
			{
				//调控电机转速的代码
				if(ConstantPower==0)
				{
					rotset=tmp;
					Left=0;Right=101;Chk=2;rottmp=rotset;resetrot=1;
				}
				else W=tmp;
//				LCD_ShowNumber(70,80,colors[0],colors[6],W,16);
				LCD_ShowNumber(110,80,0x001f,colors[6],tmp,16);
				tmp=0;
			}
			else if(buttonID==12)
			{
				tmp=0;
//				LCD_ShowNumber(70,80,colors[4],colors[6],rotset,16);
				LCD_ShowNumber(110,80,colors[4],colors[6],tmp,16);		
			}

			if(buttonID==7) ButtonUp(15,130,84,169,0xffe0,0xfca0);
			else if(buttonID==8) ButtonUp(85,130,154,169,0xffe0,0xfca0);
			else if(buttonID==9) ButtonUp(155,130,225,169,0xffe0,0xfca0);
			else if(buttonID==4) ButtonUp(15,170,84,209,0xffe0,0xfca0);
			else if(buttonID==5) ButtonUp(85,170,154,209,0xffe0,0xfca0);
			else if(buttonID==6) ButtonUp(155,170,225,209,0xffe0,0xfca0);
			else if(buttonID==1) ButtonUp(15,210,84,249,0xffe0,0xfca0);
			else if(buttonID==2) ButtonUp(85,210,154,249,0xffe0,0xfca0);
			else if(buttonID==3) ButtonUp(155,210,225,249,0xffe0,0xfca0);
			else if(buttonID==0) ButtonUp(15,250,84,290,0xffe0,0xfca0);
			else if(buttonID==11) ButtonUp(85,250,154,290,0xffe0,0xfca0);
			else if(buttonID==12) ButtonUp(155,250,225,290,0xffe0,0xfca0);
		} 
	}    			
}

void ShowChart()
{
//	FillRec(44,70,240,280,0xf81f);
	FillRec(40,70,44,234,0x001f);
	FillRec(40,230,240,234,0x001f);

	FillRec(44,80,240,80,0x07e0);
	FillRec(44,130,240,130,0x07e0);
	FillRec(44,180,240,180,0x07e0);
//	FillRec(44,230,240,230,0x07e0);
	FillRec(80,70,80,230,0x07e0);
	FillRec(120,70,120,230,0x07e0);
	FillRec(160,70,160,230,0x07e0);
	FillRec(200,70,200,230,0x07e0);
}

void CopyToTemp()
{
	u8 i,j;
	if(erec==0) 
	for(i=0;i<=srec;i++) 
	{
		temp[i]=record_data[i];
		temp_w[i]=record_w[i];
	}
	else
	{
		j=0;
		for(i=srec+1;i<=49;i++)
		{
			temp[j]=record_data[i];
			temp_w[j]=record_w[i];
			j++;
		}
		for(i=0;i<=srec;i++)
		{
			temp[j]=record_data[i];
			temp_w[j]=record_w[i];
			j++;
		}
	}
}
	
void RSrecordUI()
{
	idata u8 i,j;
	int dis,d0,d1,d2,d3;

	Pant(0xf8,0x1f);
	LCD_CS=0;
	tex[0]=5;tex[1]=6;
	Button(10,10,70,50,2,-1,0x07ff,0x00ff);
	tex[0]=14;tex[1]=15;
	Button(90,10,150,50,2,-1,0xffe0,0xfca0);
	ShowRotSpeed();
	FillRec(234,11,240,17,0xffe0);
	FillRec(234,29,240,35,0xf800);
	ShowChart();
	Acc_data[0]=0;
//	LCD_ShowNumber(0,274,colors[4],colors[6],0,8);
	LCD_ShowNumber(0,224,colors[4],colors[6],0,8);
	LCD_ShowNumber(0,174,colors[4],colors[6],50,8);
	LCD_ShowNumber(0,124,colors[4],colors[6],100,8);
	LCD_ShowNumber(0,74,colors[4],colors[6],150,8);

	tex[0]=28;tex[1]=29;
	LCD_ShowChinese(5,260,24,colors[4],colors[6],2);
	LCD_ShowChar(55,260,colors[4],colors[6],':',16);
	tex[0]=33;tex[1]=34;tex[2]=35;tex[3]=22;
	LCD_ShowChinese(5,290,24,colors[4],colors[6],4);
	LCD_ShowChar(103,290,colors[4],colors[6],':',16);

	if(!Acc_enable)
 	{
		tex[0]=30;tex[1]=32;tex[2]=43;
		LCD_ShowChinese(75,260,24,colors[4],colors[6],3);
	}
	if(!Dis_enable)
	{
		tex[0]=30;tex[1]=32;tex[2]=43;
		LCD_ShowChinese(123,290,24,colors[4],colors[6],3);
	}
	else LCD_ShowString(170,290,colors[4],colors[6],"cm",16);

	CopyToTemp();
	if(erec==0)
		for(i=1;i<=srec;i++)
		{
			LineF(40+i*4,230-temp_w[i-1],44+i*4,230-temp_w[i],0xffe0);
			LineF(40+i*4,230-temp[i-1],44+i*4,230-temp[i],0xf800);
		}
	else for(i=1;i<=49;i++)
		{
			LineF(40+i*4,230-temp_w[i-1],44+i*4,230-temp_w[i],0xffe0);
			LineF(40+i*4,230-temp[i-1],44+i*4,230-temp[i],0xf800);
		}

	while(1)
	{
/*
		if(count_time==500)
		{
			dis=GetDisAll(&d0,&d1,&d2,&d3);
			LCD_ShowNumber(10,50,colors[4],colors[6],dis,8);
			LCD_ShowNumber(10,70,colors[4],colors[6],d0,8);
			LCD_ShowNumber(10,90,colors[4],colors[6],d1,8);
			LCD_ShowNumber(10,110,colors[4],colors[6],d2,8);
			LCD_ShowNumber(10,130,colors[4],colors[6],d3,8);
		}	
*/
		LCD_CS=0;
		buttonID=-1;
		if(count_time==999) 
		{
			chart_time++;
			ShowRotSpeed();
			if(ConstantPower==0 && ReChk_time>=Chk) 
			{
				RotChk();
				ReChk_time=0;
			}
			if(erec==0 && srec>=1)
			{
				LineF(40+srec*4,230-record_w[srec-1],44+srec*4,230-record_w[srec],0xffe0);
				LineF(40+srec*4,230-record_data[srec-1],44+srec*4,230-record_data[srec],0xf800);
				if(srec==49) chart_time=0;
			}
		}

		if(erec==1 && chart_time>=5)
		{
			chart_time=0;
			for(i=1;i<=49;i++)
			{
				LineF(40+i*4,230-temp_w[i-1],44+i*4,230-temp_w[i],0xf81f);
				LineF(40+i*4,230-temp[i-1],44+i*4,230-temp[i],0xf81f);
			}
			ShowChart();
			CopyToTemp();
			for(i=1;i<=49;i++)
			{
				LineF(40+i*4,230-temp_w[i-1],44+i*4,230-temp_w[i],0xffe0);
				LineF(40+i*4,230-temp[i-1],44+i*4,230-temp[i],0xf800);
			}
		}

		if(Acc_enable && (count_time==200 || count_time==700))
		{
			Acc_data[0]++;
			GetAcc(&dis,&d0);
			Acc_data[Acc_data[0]]=d0;
			if(Acc_data[0]==8)
			{
				Acc_data[0]=0;
				for(i=1;i<8;i++)
					for(j=i+1;j<=8;j++)
						if(Acc_data[i]>Acc_data[j])
						{
							dis=Acc_data[i];
							Acc_data[i]=Acc_data[j];
							Acc_data[j]=dis;
						}
				d0=0;d1=0;d3=0;
				for(i=1;i<=8;i++)
					if(Acc_data[i]>=250) d0++;
					else if(Acc_data[i]<=245) d1++;
				if(d0>=2 && d1>=2) d3=1;
				else
				{
					d2=0;
					for(i=2;i<=7;i++)
						d2+=Acc_data[i];
					d2=d2/6;
					if(d2<245) d3=2;
					else if(d2>250) d3=3;
				}

				switch(d3)
			  	{
					case 0:
						tex[0]=36;tex[1]=37;
						break;
				   	case 1:
						tex[0]=38;tex[1]=39;
						break;
					case 2:
						tex[0]=40;tex[1]=41;
						break;
					case 3:
						tex[0]=40;tex[1]=42;
						break;
				}
				LCD_ShowChinese(75,260,24,colors[4],colors[6],2);

			   	if(d3==1) rotset=rotset*3/5;
				else rotset=rottmp;

			}
		}

		if(Dis_enable && (count_time==100 || count_time==600))
		{
			dis=GetDisAll(&d0,&d1,&d2,&d3);
			LCD_ShowNumber3(123,290,colors[4],colors[6],dis,16);
			
			if(dis>20)
			{
				if(dis<=30)	rotset=rottmp*0.1;
				else if(dis<=40) rotset=rottmp*0.2;
				else if(dis<=50) rotset=rottmp*0.3;
				else if(dis<=60) rotset=rottmp*0.4;
				else if(dis<=70) rotset=rottmp*0.5;
				else if(dis<=80) rotset=rottmp*0.6;
				else if(dis<=90) rotset=rottmp*0.7;
				else if(dis<=100) rotset=rottmp*0.8;
				else rotset=rottmp;
			}
//			dis=GetDis(&d0);
//			LCD_ShowNumber(10,150,colors[4],colors[6],dis,8);			
		}

		if(IRQ==0)
		{
			Get_touch_coor(&corx,&cory);
			if(10<=corx && corx<=70 && 10<=cory && cory<=50)
			{
				buttonID=0;
				ButtonDown(10,10,70,50,0x07ff,0x00ff);
			}
			else if(90<=corx && corx<=150 && 10<=cory && cory<=50)
			{
				buttonID=1;
				ButtonDown(90,10,150,50,0xffe0,0xfca0);
			}
			while(IRQ==0);
			if(buttonID==0) return;
			else if(buttonID==1)
		 	{
				ButtonUp(90,10,150,50,0xffe0,0xfca0);
				erec=0;
				srec=-1;
				FillRec(44,70,240,230,0xf81f);
				ShowChart();
			}
 		}
	}
}

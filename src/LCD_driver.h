
#define uchar unsigned char
#define uint unsigned int



#define  LCD_DataPortH P1     //高8位数据口,8位模式下只使用高8位 
#define  LCD_DataPortL P0//P2     //低8位数据口	,8位模式下低8位可以不接线

sbit LCD_REST  = P3^0;//P0^3;//P2^7;    //RST 引脚定义
//sbit LCD_RD   =  P3^2;	 //RD  引脚定义
sbit LCD_WR   =  P2^6;//P0^6;//P3^0;//P2^5;	 //WR  引脚定义
sbit LCD_RS   =  P2^5;//P0^5;//P3^1;//P2^6;	 //RS  引脚定义
sbit LCD_CS   =  P2^7;//P0^7;//P3^2;//P2^7;	 //CS  引脚定义 


#define  LCD_SIZE_X		240
#define  LCD_SIZE_Y		320

extern  uint colors[];

void LCD_Init(void);
void LCD_ShowString(uint x,uint y,uint For_color,uint Bk_color,char *p,uchar size);
void LCD_ShowNumber(uint x,uint y,uint For_color,uint Bk_color,unsigned long num,uchar size);
void LCD_ShowNumber3(uint x,uint y,uint For_color,uint Bk_color,unsigned int num,uchar size);
void LCD_ShowChar(uint x,uint y,uint For_color,uint Bk_color, char ch,uchar size);
void ChineseChar(uint x,uint y,int size,uint For_color,uint Bk_color ,char c);
void delayms(int count) ;
void LCD_Writ_Bus(char VH,char VL);
void LCD_Writ_Bus(char VH,char VL);
void LCD_Write_COM(char VH,char VL);
void LCD_Write_DATA(char VH,char VL);
void Pant(char VH,char VL);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);





#define uchar unsigned char
#define uint unsigned int



#define  LCD_DataPortH P1     //��8λ���ݿ�,8λģʽ��ֻʹ�ø�8λ 
#define  LCD_DataPortL P0//P2     //��8λ���ݿ�	,8λģʽ�µ�8λ���Բ�����

sbit LCD_REST  = P3^0;//P0^3;//P2^7;    //RST ���Ŷ���
//sbit LCD_RD   =  P3^2;	 //RD  ���Ŷ���
sbit LCD_WR   =  P2^6;//P0^6;//P3^0;//P2^5;	 //WR  ���Ŷ���
sbit LCD_RS   =  P2^5;//P0^5;//P3^1;//P2^6;	 //RS  ���Ŷ���
sbit LCD_CS   =  P2^7;//P0^7;//P3^2;//P2^7;	 //CS  ���Ŷ��� 


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




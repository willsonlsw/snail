

#define u16 unsigned int
#define u8 unsigned char

sbit PWM=P3^1;//P3^2;
extern u8 tex[5];
extern unsigned char W;//ռ�ձ�0-100��һ�����ڷ�Ϊ100��

void Button(u8 x1,u16 y1,u8 x2,u16 y2,u8 k,char num,u16 col1,u16 col2);
void ButtonDown(u8 x1,u16 y1,u8 x2,u16 y2,u16 col1,u16 col2);
void ButtonUp(u8 x1,u16 y1,u8 x2,u16 y2,u16 col1,u16 col2);
void FillRec(u8 x1,u16 y1,u8 x2,u16 y2,u16 col);
void LCD_ShowChinese(u8 x,u16 y,u8 size,u16 col1,u16 col2,u8 k);
void CopyToTemp();
void StartUI();
void SetUI();
void FixedSpeedUI();
void SetSpeedUI();
void RSrecordUI();

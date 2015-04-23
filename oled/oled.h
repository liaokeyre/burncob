#ifndef __OLED_H
#define __OLED_H
#include "main.h"			  	 	  
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����
#define OLED_MODE 0

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	  
/* ���崰�����ݽṹ */
typedef  struct
{  uint8  x;			// ����λ��(���Ͻǵ�x����)
   uint8  y;			// ����λ��(���Ͻǵ�y����)
    
   uint8  with;		    // ���ڿ��
   uint8  hight;		// ���ڸ߶�
   
   uint8   *title;		// ���������ָ�� (�����ַ�ΪASCII�ַ������������ܴ�������)
   uint8   *text;       // ��������
   uint8   *state;		// ����״̬��ָ�� (��Ϊ��ʱ����ʾ״̬��)   
} WINDOWS;  						  
//-----------------OLED�˿ڶ���---------------- 

void delay_ms(unsigned int ms); 		  
//OLED�����ú���
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_Refresh_Gram(void);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size, u8 mode);
void OLED_ShowFont16(u8 x,u8 y,u8 fno,u8 mode);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 t);
void OLED_Draw_Square(u8 x0, u8 y0, u8  with, u8 t);
void OLED_Draw_SquareFill(u8 x0, u8 y0, u8  with, u8 t);
void OLED_DrawCircle(u8 x,u8 y,u16 r,u8 t);
u8   OLED_Draw_WindowsDraw(WINDOWS *win);
void OLED_DrawBMP(u8 x,u8 y,u8 mode);
void OLED_ShowName(u8 x,u8 y,u8 fno,u8 mode);
void OLED_ShowAdd(u8 x,u8 y,u32 Add1,u32 Add2);
void Disp_Menu_Main(u8 num);
void Disp_Menu_1(u8 num);
void OLED_DrawBar(u8 x1, u8 y1, u8 x2, u8 y2, u8 t);
extern u8 OLED_GRAM[128][4];
extern u8 OLED_GRAM_TMP[128][4];
#endif  
	 




/***********************************************************************************************
����˵������֧�ֻ��㣬���ߣ������Σ���Բ�������ڣ���BMP
��ֲ��liaokeyre
************************************************************************************************/
//�ӿڶ������oled.h�ļ�
//����SPI��Ҫ���߸��ߣ�����RES���Բ��ӣ����ǲ���ʱ���������
// GND,VCC,CS,RES,D/C,CLK,DIN
//���ڵ�ѹ���⣻����ͻ���Ҫ��ʱ���ȶ�����������ȫ������3.3V��Դ����ƽ�ź�Ҳ����3.3V��
//����ͻ���Ҫ��5V��Դ����Ȼ5V����������������ʽ���ǲ�Ҫ��ʱ�乤�����������ܻ����������������VCC�˴�һС������ܻ��һЩ��
#include "oled.h"
#include "oledfont.h" 
#include "24c02.h"
#include "cd4052.h"
#define GUI_LCM_XMAX 128
#define GUI_LCM_YMAX  32	 
//#include "delay.h"
//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127
//--------------------------------------12832	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 
u8 OLED_GRAM[128][4];//�Դ�
u8 OLED_GRAM_TMP[128][4];

/****************************************************************************
�ֽ���ߵ�λ�������
��Ҫ����һ���ֿ�ȡ�뷽���Ƿ���
�������ֿ����鷳��������
****************************************************************************/	
u8 ByteMirro(u8 dat)
{
 dat = (dat & 0xf0) >> 4 | (dat & 0x0f)  << 4;//��4λΪ��λ���н���    abcd  1234
 dat = (dat & 0xcc) >> 2 | (dat & 0x33)  << 2;//��2λΪ��λ���н���    cdab  3412
 dat = (dat & 0xaa) >> 1 | (dat & 0x55)  << 1;//��1λΪ��λ���н���   dcba  4321
 return dat;
}
		   
void delay_ms(unsigned int ms)
{                         
	unsigned int a;
	while(ms)
	{
		a=1800;
		while(a--);
		ms--;
	}
//	return;
}
#if OLED_MODE==1
//��SSD1106д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	DATAOUT(dat);	    
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		   
	OLED_CS_Clr();
	OLED_WR_Clr();	 
	OLED_WR_Set();
	OLED_CS_Set();	  
	OLED_DC_Set();	 
} 	    	    
#else
//��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		{
		  OLED_SDIN_Set();
		}
        else
	    OLED_SDIN_Clr();
	    OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
} 
#endif

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
} 
/*  	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}
*/
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<4;i++)  
	{  
		OLED_WR_Byte (0xb4+i,OLED_CMD);    //����ҳ��ַ��0~7��b4-->b7
		OLED_WR_Byte (0x04,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}
		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<4;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
//	OLED_Refresh_Gram();//������ʾ
}  
//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=7-y/8;            //�õ�����ҳ
	bx=y%8;				  //�õ���ǰҳ��ȷλ��
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}

//////////////////////////////////
//��x1,y1Ϊ��㣬x2,y2Ϊ�յ㻭��//
/////////(t=1���㣬t=0���)///////
//////////////////////////////////
//���ߺ���
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 t) //y=kx+b
{
	s16 dy;
	s16 dx;
	s8 dirx,diry,move;
	dy=y2-y1; 
	dx=x2-x1; 
	if(dy<0)  
	{
		dy=-dy;
		diry=-1;
	}
	else 
	{
		diry=1;
	}
	if(dx<0)
	{
		dx=-dx;
		dirx=-1;
	}
	else
	{
		dirx=1;
	}
//	dy<<=1;	
//	dx<<=1;
	OLED_DrawPoint(x1,y1,t);
	if(dx>dy)	//////////////////
	{
		move=dy-(dx>>1); 
		while(x1!=x2)
		{
			if(move>=0)
			{
				y1+=diry; 
				move-=dx;
			}
			x1+=dirx;
			move+=dy;  
			OLED_DrawPoint(x1,y1,t);
		}
	}
	else
	{
		move=dx-(dy>>1);
		while(y1!=y2)
		{
			if(move>=0)
			{
				x1+=dirx;
				move-=dy;
			}
			y1+=diry;
			move+=dx;
			OLED_DrawPoint(x1,y1,t);
		}
	}
}

/****************************************************************************
* ���ܣ���ˮƽ�ߡ�
* ��ڲ�����x0      ˮƽ����������е�λ��
*           y0      ˮƽ����������е�λ��
*           x1      ˮƽ���յ������е�λ��
*           t       ��ʾ��ɫ(���ںڰ�ɫLCM��Ϊ0ʱ��Ϊ1ʱ��ʾ)
* ���ڲ�������
* ˵�������ڵ�ɫ��4���Ҷȵ�Һ������ͨ���޸Ĵ˺�����ͼ����ٶȣ��絥ɫLCM������һ�θ�
*      ��8���㣬������Ҫһ����һ�����д��LCM�С�
****************************************************************************/
void  OLED_Draw_HLine(u8 x0, u8 y0, u8 x1, u8 t)
{
    u8  temp;
    if(x0>x1)               // ��x0��x1��С�������У��Ա㻭ͼ
    {
        temp = x1;
        x1 = x0;
        x0 = temp;
    }
    do
    {
        OLED_DrawPoint(x0, y0, t);   // �����ʾ�������ֱ��
        x0++;
    }
    while(x1>=x0);
}
/****************************************************************************
* ���ܣ�����ֱ�ߡ�
* ��ڲ�����x0      ��ֱ����������е�λ��
*           y0      ��ֱ����������е�λ��
*           y1      ��ֱ���յ������е�λ��
*           t       ��ʾ��ɫ
* ���ڲ�������
* ˵�������ڵ�ɫ��4���Ҷȵ�Һ������ͨ���޸Ĵ˺�����ͼ����ٶȣ��絥ɫLCM������һ�θ�
*      ��8���㣬������Ҫһ����һ�����д��LCM�С�
****************************************************************************/
void  OLED_Draw_RLine(u8 x0, u8 y0, u8 y1, u8 t)
{
    u8  temp;
    if(y0>y1)       // ��y0��y1��С�������У��Ա㻭ͼ
    {
        temp = y1;
        y1 = y0;
        y0 = temp;
    }
    do
    {
        OLED_DrawPoint(x0, y0, t);   // �����ʾ�������ֱ��
        y0++;
    }
    while(y1>=y0);
}
/****************************************************************************
* ���ܣ������Ρ�
* ��ڲ�����x0		�������Ͻǵ�x����ֵ
*           y0		�������Ͻǵ�y����ֵ
*           x1      �������½ǵ�x����ֵ
*           y1      �������½ǵ�y����ֵ
*           t	    1���㣬0���
* ���ڲ�������
* ˵��������ʧ��ԭ����ָ����ַ������Ч��Χ��
****************************************************************************/
void  OLED_DrawRectangle(u8 x1, u8 y1, u8 x2, u8 y2, u8 t)
{  OLED_Draw_HLine(x1, y1, x2, t);
   OLED_Draw_HLine(x1, y2, x2, t);
   OLED_Draw_RLine(x1, y1, y2, t);
   OLED_Draw_RLine(x2, y1, y2, t);
}
/****************************************************************************
* ���ܣ������Ρ���һ�����ľ��Σ����ɫ��߿�ɫһ����
* ��ڲ�����x0		�������Ͻǵ�x����ֵ
*           y0		�������Ͻǵ�y����ֵ
*           x1      �������½ǵ�x����ֵ
*           y1      �������½ǵ�y����ֵ
*           t	    �����ɫ
* ���ڲ�������
* ˵��������ʧ��ԭ����ָ����ַ������Ч��Χ��
****************************************************************************/
void  OLED_Draw_RectangleFill(u8 x0, u8 y0, u8 x1, u8 y1, u8 t)
{  u8  i;

   /* ���ҳ��������Ͻ������½ǵ������㣬������(x0,y0)��(x1,y1) */
   if(x0>x1) 						// ��x0>x1����x0��x1����
   {  i = x0;
      x0 = x1;
      x1 = i;
   }
   if(y0>y1)						// ��y0>y1����y0��y1����
   {  i = y0;
      y0 = y1;
      y1 = i;
   }
   
   /* �ж��Ƿ�ֻ��ֱ�� */
   if(y0==y1) 
   {  OLED_Draw_HLine(x0, y0, x1, t);
      return;
   }
   if(x0==x1) 
   {  (x0, y0, y1, t);
      return;
   }

   while(y0<=y1)						
   {  OLED_Draw_HLine(x0, y0, x1, t);	// ��ǰ��ˮƽ��
      y0++;							// ��һ��
   }
}
/****************************************************************************
* ���ܣ��������Ρ�
* ��ڲ�����x0		���������Ͻǵ�x����ֵ
*           y0		���������Ͻǵ�y����ֵ
*           with    �����εı߳�
*           t	    ��ʾ��ɫ
* ���ڲ�������
* ˵��������ʧ��ԭ����ָ����ַ������Ч��Χ��
****************************************************************************/
void  OLED_Draw_Square(u8 x0, u8 y0, u8  with, u8 t)
{ 
    if(with==0) return;
	if( (x0+with) > GUI_LCM_XMAX ) return;
	if( (y0+with) > GUI_LCM_YMAX ) return;
	OLED_DrawRectangle(x0, y0, x0+with, y0+with, t);
}
/****************************************************************************
* ���ܣ�����������䡣
* ��ڲ�����x0		���������Ͻǵ�x����ֵ
*           y0		���������Ͻǵ�y����ֵ
*           with    �����εı߳�
*           t	    ��ʾ��ɫ
* ���ڲ�������
* ˵��������ʧ��ԭ����ָ����ַ������Ч��Χ��
****************************************************************************/
void  OLED_Draw_SquareFill(u8 x0, u8 y0, u8  with, u8 t)
{ 
    if(with==0) return;
	if( (x0+with) > GUI_LCM_XMAX ) return;
	if( (y0+with) > GUI_LCM_YMAX ) return;
	OLED_Draw_RectangleFill(x0, y0, x0+with, y0+with, t);
}
/****************************************************************************
* ���ܣ������֮����������ߡ��ӵ�һ�������ڶ��㣬������������...
* ��ڲ�����points  ������������ݵ�ָ�룬��������Ϊ(x0,y0)��(x1,y1)��(x2,y2)...
*           no      ����Ŀ������Ҫ����1
*           t	��ʾ��ɫ
* ���ڲ�������
* ˵��������ʧ��ԭ����ָ����ַ������Ч��Χ��
****************************************************************************/
void  OLED_Draw_LineS(u8 const *points, u8 no, u8 t)
{  u8  x0, y0;
   u8  x1, y1;
   u8  i;

   /* ��ڲ������� */
   if(0==no) return;
   if(1==no)						// ����
   {  x0 = *points++;
      y0 = *points;
      OLED_DrawPoint(x0, y0, t);
   }
   
   /* ���������� */
   x0 = *points++;					// ȡ����һ������ֵ����Ϊԭ�������ֵ
   y0 = *points++;
   for(i=1; i<no; i++)
   {  x1 = *points++;				// ȡ����һ������ֵ
      y1 = *points++;
      OLED_DrawLine(x0, y0, x1, y1, t);
      x0 = x1;						// ����ԭ�������
      y0 = y1;
   }
}
//////////////////////////////
//��x,yΪԲ�ģ���rΪ�뾶��Բ//
/////(t=1���㣬t=0���)///////
//////////////////////////////
//��Բ����
void OLED_DrawCircle(u8 x,u8 y,u16 r,u8 t)
{
	u8 xc=0;
	u8 yc;
	s8 p;
	yc=r;
	p=3-(r<<1);
	while(xc<=yc)  
	{		  
		OLED_DrawPoint(x+xc,y+yc,t);
		OLED_DrawPoint(x+xc,y-yc,t);
		OLED_DrawPoint(x-xc,y+yc,t);
		OLED_DrawPoint(x-xc,y-yc,t);
		OLED_DrawPoint(x+yc,y+xc,t);
		OLED_DrawPoint(x+yc,y-xc,t);
		OLED_DrawPoint(x-yc,y+xc,t);
		OLED_DrawPoint(x-yc,y-xc,t);
		if(p<0)
		p+=((xc++)<<2)+6;
		else
		p+=(((xc++)-(yc--))<<2)+10;
	}
}

/****************************************************************************
* ���ܣ���ʾ���ڡ������ṩ�Ĵ��ڲ������л����ڡ�
* ��ڲ�����win		���ھ��
* ���ڲ���������0��ʾ����ʧ�ܣ�����1��ʾ�����ɹ�
****************************************************************************/
u8  OLED_Draw_WindowsDraw(WINDOWS *win)
{  u8  *str;
   int8  bak, bak1, bak2;
   
   /* �������ˣ������������Χ���򷵻�0 */
   if( ( (win->with)<20 ) || ( (win->hight)<20 ) ) return(0);		// ��ȡ��߶ȼ�飬������С����
   if( (win->x + win->with ) > GUI_LCM_XMAX ) return(0);			// ���ڿ���Ƿ����
   if( (win->y + win->hight ) > GUI_LCM_YMAX ) return(0);			// ���ڸ߶��Ƿ����
    
   /* ��ʼ������ */
   OLED_Draw_RectangleFill(win->x, win->y, win->x + win->with - 1, win->y + win->hight - 1, 0);	//���һƬ���򻭵���
   OLED_DrawRectangle(win->x, win->y, win->x + win->with - 1, win->y + win->hight - 1, 1);	// ������
   OLED_Draw_HLine(win->x, win->y + 9, win->x + win->with - 1, 1);							// ������Ŀ��
//   OLED_Draw_RLine(win->x + 9, win->y, win->y + 9, 1);										// ���رմ��Ű�ť
//   OLED_DrawLine(win->x, win->y, win->x + 9, win->y + 9, 1);
//   OLED_DrawLine(win->x + 9, win->y, win->x, win->y + 9, 1);
   OLED_Draw_RLine(win->x + win->with - 10, win->y, win->y + 9, 1);										// ���رմ��Ű�ť
   OLED_DrawLine(win->x + win->with - 10, win->y, win->x + win->with - 1 , win->y + 9, 1);
   OLED_DrawLine(win->x + win->with - 1, win->y, win->x + win->with - 10, win->y + 9, 1);
   

   /* д���� */
   if( win->title != 0 ) 
   {  str = win->title;
      bak = win->x + 2;
      bak1 = win->y + 1;		  //titleλ��
      bak2 = win->x + win->with -1;
     
      while(1)						
      {  if( (bak+8) > bak2 ) break;								// �жϱ����Ƿ����
         if(*str=='\0') break;										// �ж��ַ����Ƿ����
         
         OLED_ShowChar(bak, bak1, *str++,7,1);							// ��ʾ����
         bak += 6;
      }
   }

   /* д���� */
   if( win->text != 0 ) 
   {  str = win->text;
      bak = win->x + 2;
      bak1 = win->y + 12;		  //textλ��
      bak2 = win->x + win->with -1;
     
      while(1)						
      {  if( (bak+8) > bak2 ) break;								// �жϱ����Ƿ����
         if(*str=='\0') break;										// �ж��ַ����Ƿ����
         
         OLED_ShowChar(bak, bak1, *str++,7,1);							// ��ʾ����
         bak += 6;
      }
   }

   /* д״̬�� */
   if( win->state != 0 )
   {  if( win->hight < 25) return(0);								// �ж��Ƿ���Ի�״̬��
      /* ��״̬�� */
      OLED_Draw_HLine(win->x, win->y + win->hight - 10, win->x + win->with - 1, 1);
      
      str = win->state;
      bak = win->x + 3;
      bak1 = win->y + win->hight - 9;
      bak2 = win->x + win->with -1;
      
      while(1)						
      {  if( (bak+8) > bak2 ) break;								// �жϱ����Ƿ����
         if(*str=='\0') break;										// �ж��ַ����Ƿ����
         
         OLED_ShowChar(bak, bak1, *str++,7,1);							// ��ʾ����
         bak += 6;
      }      
   }
   
   return(1);

}
/*******************************************************************************************
������: void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)
˵��: ���ָ������RAM
********************************************************************************************/
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//������ʾ
}


/*******************************************************************************************
������: void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
˵��: ��ʾ�ַ�
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12/7 
********************************************************************************************/
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ				   
    for(t=0;t<size;t++)
    {   
	    if(size==7)temp=ByteMirro(F7x5[chr][t]); //����0806����  ��Ϊ�ֿ��Ƿ��ģ����Ծ������ˣ�
		else if(size==12)temp=asc2_1206[chr][t];  //����1206����
		else temp=asc2_1608[chr][t];		 //����1608���� 	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
//m^n����
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  

/*******************************************************************************************
������: void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
˵��: ��ʾ����
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);
********************************************************************************************/	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
/*******************************************************************************************
������: void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
˵��: ��ʾ�ַ���
//��ʾ�ַ���
//x,y:�������  
//*p:�ַ�����ʼ��ַ
********************************************************************************************/
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size, u8 mode)
{
           
    while(*p!='\0')
    { 
	    if(size == 16)
		{      
	        if(x>122){x=0;y+=16;}	//����
	        if(y>32){y=x=0;OLED_Clear();} //��ҳ
	        OLED_ShowChar(x,y,*p,16,mode);	 
	        x+=8;
		}
		else if(size == 12)
		{
	        if(x>122){x=0;y+=12;}	//����
	        if(y>32){y=x=0;OLED_Clear();} //��ҳ
	        OLED_ShowChar(x,y,*p,12,mode);	 
	        x+=6;		
		}
		else
		{
	        if(x>122){x=0;y+=8;}	//����
	        if(y>32){y=x=0;OLED_Clear();} //��ҳ
	        OLED_ShowChar(x,y,*p,7,mode);	 
	        x+=6;		
		}
        p++;
    }  
}

/*******************************************************************************************
������: void OLED_ShowFont16(u8 x,u8 y,u8 fno,u8 mode)
˵��: ��ʾ����
//��ʾ����
//��ָ��λ����ʾһ������
//x:0~127
//y:0~63
//fno:���ֱ��
//mode:0,������ʾ;1,������ʾ				 
//16*16��С���ַ�
********************************************************************************************/
void OLED_ShowFont16(u8 x,u8 y,u8 fno,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;				   
    for(t=0;t<32;t++)
    {   
		if(t<16)temp=Hzk[fno*2][t];    	                          
        else temp=Hzk[fno*2+1][t-16];   
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
/*******************************************************************************************
������: void OLED_DrawTriangle(u8 x1, u8 y1, u8 len,u8 t)
˵��: �������Σ� ��ʼλ������  
********************************************************************************************/
void OLED_DrawTriangle(u8 x1, u8 y1, u8 len,u8 t)
{
  OLED_DrawLine(x1,y1,x1-len,y1+len,t);
  OLED_DrawLine(x1,y1,x1+len,y1+len,t);
  OLED_DrawLine(x1+len,y1+len,x1-len,y1+len,t);	//��������
}
/*******************************************************************************************
������: void OLED_DrawBar(u8 x1, u8 y1, u8 x2, u8 y2, u8 t)
˵��: ���������� ��ʼλ������  
********************************************************************************************/
void OLED_DrawSetBar(u8 x1, u8 y1, u8 x2, u8 y2, u8 num, u8 num1,u8 t)
{ 
//  u8 i; 
  u8 xtmp;
  xtmp = (x2-x1)/num;
  OLED_Draw_RectangleFill(x1-3, y1-2, x2+4, y2+12, 0);   			    
  OLED_DrawRectangle(x1, y1, x2, y2, t);

  OLED_DrawTriangle(x1+xtmp*num1, y2+1, 3,1);
  OLED_Draw_RectangleFill(x1+1, y1+1, x1+xtmp*num1, y2-1, t);
  OLED_ShowChar(x1+xtmp*num1-6,y2+6,Hex2Dat(((num1/10)&0x0f)),7,1);
  OLED_ShowChar(x1+xtmp*num1,y2+6,Hex2Dat(((num1%10)&0x0f)),7,1);
  OLED_Refresh_Gram(); 
     
}
/*******************************************************************************************
������: void OLED_DrawBar(u8 x1, u8 y1, u8 x2, u8 y2, u8 t)
˵��: ���������� ��ʼλ������  
********************************************************************************************/
void OLED_DrawBar(u8 x1, u8 y1, u8 x2, u8 y2, u8 t)
{ 
  u8 i;     			    
  OLED_DrawRectangle(x1, y1, x2, y2, t);
  for(i=0;i<x2-x1;i++)
  {
    OLED_Draw_RectangleFill(x1+1, y1+1, x1+i, y2-1, t); 
	OLED_Refresh_Gram();
	delay_10ms();
	delay_10ms();
  }      
}
/*******************************************************************************************
������: void OLED_DrawBMP(u8 x,u8 y)
˵��: ��λͼ ��ʼλ������ x,y
********************************************************************************************/
void OLED_DrawBMP(u8 x,u8 y,u8 mode)
{      			    
	u16 temp,t,t1;
	u16 y0=y;					    				   
    for(t=0;t<512;t++)
    {  
	    temp = logo[t]; 	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode); 
			else OLED_DrawPoint(x,y,!mode);		   
			temp<<=1;
			y++;
			if((y-y0)==32)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}

/*******************************************************************************************
void OLED_DrawICON(u8 x,u8 y,u16 size ,u8 mode)
˵��: ��λͼ ��ʼλ������ x,y
********************************************************************************************/
void OLED_DrawICON(u8 x,u8 y,u8 mode,void *icon)
{      			    
	u16 temp,t,t1;
	u16 y0=y;					    				   
    for(t=0;t<128;t++)
    {  
	    temp = *(u8*)icon ; 
		((u8*)icon)++;	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode); 
			else OLED_DrawPoint(x,y,!mode);		   
			temp<<=1;
			y++;
			if((y-y0)==32)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}

void Disp_Menu_Main(u8 num)
{
    OLED_ShowChar(0,11,'<',12,1);
    OLED_ShowChar(120,11,'>',12,1);
	if(num == 0)
	{
	    OLED_DrawICON(10,0,0,set_ico);
	    OLED_DrawICON(48,0,1,cut_ico);
	    OLED_DrawICON(86,0,1,test_ico);
	} 
	else if(num == 1)
	{
	    OLED_DrawICON(10,0,1,set_ico);
	    OLED_DrawICON(48,0,0,cut_ico);
	    OLED_DrawICON(86,0,1,test_ico);
	}
	else
	{
	    OLED_DrawICON(10,0,1,set_ico);
	    OLED_DrawICON(48,0,1,cut_ico);
	    OLED_DrawICON(86,0,0,test_ico);
	}
//	OLED_Refresh_Gram();
}
void Disp_Menu_1(u8 num)
{
//    OLED_ShowChar(0,11,'<',12,1);
//    OLED_ShowChar(120,11,'>',12,1);
	if(num == 0)
	{
	    OLED_ShowString(0,0,"1.VT62538-B1",12,0);
	    OLED_ShowString(0,15,"2.VT62538-B2",12,1);
	    OLED_ShowString(78,0,"3.COB",12,1);
	    OLED_ShowString(78,15,"4.RETURN",12,1);
	} 
	else if(num == 1)
	{
	    OLED_ShowString(0,0,"1.VT62538-B1",12,1);
	    OLED_ShowString(0,15,"2.VT62538-B2",12,0);
	    OLED_ShowString(78,0,"3.COB",12,1);
	    OLED_ShowString(78,15,"4.RETURN",12,1);
	}
	else if(num == 2)
	{
	    OLED_ShowString(0,0,"1.VT62538-B1",12,1);
	    OLED_ShowString(0,15,"2.VT62538-B2",12,1);
	    OLED_ShowString(78,0,"3.COB",12,0);
	    OLED_ShowString(78,15,"4.RETURN",12,1);
	}
	else
	{
	    OLED_ShowString(0,0,"1.VT62538-B1",12,1);
	    OLED_ShowString(0,15,"2.VT62538-B2",12,1);
	    OLED_ShowString(78,0,"3.COB",12,1);
	    OLED_ShowString(78,15,"4.RETURN",12,0);
	}
}

void Disp_Menu_0(u8 num)
{
	if(num == 0)
	{
	    OLED_ShowString(0,0,"1.SET LINE",12,0);
	    OLED_ShowString(0,15,"2.SET SENS",12,1);
	    OLED_ShowString(78,0,"3.ERASE",12,1);
	    OLED_ShowString(78,15,"4.RETURN",12,1);
	} 
	else if(num == 1)
	{
	    OLED_ShowString(0,0,"1.SET LINE",12,1);
	    OLED_ShowString(0,15,"2.SET SENS",12,0);
	    OLED_ShowString(78,0,"3.ERASE",12,1);
	    OLED_ShowString(78,15,"4.RETURN",12,1);
	}
	else if(num == 2)
	{
	    OLED_ShowString(0,0,"1.SET LINE",12,1);
	    OLED_ShowString(0,15,"2.SET SENS",12,1);
	    OLED_ShowString(78,0,"3.ERASE",12,0);
	    OLED_ShowString(78,15,"4.RETURN",12,1);
	}
	else
	{
	    OLED_ShowString(0,0,"1.SET LINE",12,1);
	    OLED_ShowString(0,15,"2.SET SENS",12,1);
	    OLED_ShowString(78,0,"3.ERASE",12,1);
	    OLED_ShowString(78,15,"4.RETURN",12,0);
	} 
}

/*******************************************************************************************
������: void OLED_ShowAdd(u8 x,u8 y,u32 Add1,u32 Add2)
˵��: ��λͼ ��ʼλ������ x,y
********************************************************************************************/
void OLED_ShowAdd(u8 x,u8 y,u32 Add1,u32 Add2)
{     			    
  OLED_ShowString(x+4,y,"SADD:",7,1); 	 //��һ���ַ�
  OLED_ShowString(x+4,y+16,"RADD:",7,1);   //�������ַ� 
  OLED_DrawLine(38,0,38,31,1); 
  OLED_DrawLine(38,0,40,0,1); 
  OLED_DrawLine(38,15,40,15,1);
  OLED_DrawLine(38,31,40,31,1);			 //���ָ���
  x = x+36;
  OLED_ShowChar(x+6,y,((Add1>>23)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*2,y,((Add1>>22)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*3,y,((Add1>>21)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*4,y,((Add1>>20)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*5,y,'-',7,1);
  OLED_ShowChar(x+6*6,y,((Add1>>19)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*7,y,((Add1>>18)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*8,y,((Add1>>17)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*9,y,((Add1>>16)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*10,y,'-',7,1);
  OLED_ShowChar(x+6*11,y,((Add1>>15)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*12,y,((Add1>>14)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*13,y,((Add1>>13)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*14,y,((Add1>>12)&0x01)+'0',7,1);
 // OLED_ShowChar(x+6*15,y,'-',7,1);
  OLED_ShowChar(x+6,y+8,((Add1>>11)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*2,y+8,((Add1>>10)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*3,y+8,((Add1>>9)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*4,y+8,((Add1>>8)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*5,y+8,'-',7,1);
  OLED_ShowChar(x+6*6,y+8,((Add1>>7)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*7,y+8,((Add1>>6)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*8,y+8,((Add1>>5)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*9,y+8,((Add1>>4)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*10,y+8,'-',7,1);
  OLED_ShowChar(x+6*11,y+8,((Add1>>3)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*12,y+8,((Add1>>2)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*13,y+8,((Add1>>1)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*14,y+8,((Add1)&0x01)+'0',7,1);

  OLED_ShowChar(x+6,y+16,((Add2>>23)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*2,y+16,((Add2>>22)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*3,y+16,((Add2>>21)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*4,y+16,((Add2>>20)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*5,y+16,'-',7,1);
  OLED_ShowChar(x+6*6,y+16,((Add2>>19)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*7,y+16,((Add2>>18)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*8,y+16,((Add2>>17)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*9,y+16,((Add2>>16)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*10,y+16,'-',7,1);
  OLED_ShowChar(x+6*11,y+16,((Add2>>15)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*12,y+16,((Add2>>14)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*13,y+16,((Add2>>13)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*14,y+16,((Add2>>12)&0x01)+'0',7,1);
//  OLED_ShowChar(x+6*15,y+16,'-',7,1);

  OLED_ShowChar(x+6,y+24,((Add2>>11)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*2,y+24,((Add2>>10)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*3,y+24,((Add2>>9)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*4,y+24,((Add2>>8)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*5,y+24,'-',7,1);
  OLED_ShowChar(x+6*6,y+24,((Add2>>7)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*7,y+24,((Add2>>6)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*8,y+24,((Add2>>5)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*9,y+24,((Add2>>4)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*10,y+24,'-',7,1);
  OLED_ShowChar(x+6*11,y+24,((Add2>>3)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*12,y+24,((Add2>>2)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*13,y+24,((Add2>>1)&0x01)+'0',7,1);
  OLED_ShowChar(x+6*14,y+24,((Add2)&0x01)+'0',7,1);    
}
/*******************************************************************************************
������: void OLED_Init(void)
˵��: OLED����ʼ��
********************************************************************************************/				    
void OLED_Init(void)
{
    OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(100);
	OLED_RST_Set(); 

	OLED_WR_Byte(0xFD,OLED_CMD);//�ڲ���Ƭ��д����
	OLED_WR_Byte(0x12,OLED_CMD);//Ĭ�Ϲر��ڲ������������������ָ��翪��������ֻ����FDָ�� ��Ĭ��Ϊ0��Ϊ1�򱣻� bit 0 0 0 1 0 ��1 0
		
	OLED_WR_Byte(0xAE,OLED_CMD);//�ر���ʾ

	OLED_WR_Byte(0x20,OLED_CMD);
	OLED_WR_Byte(0x02,OLED_CMD);//page mode

	OLED_WR_Byte(0xB4,OLED_CMD);

	OLED_WR_Byte(0x00,OLED_CMD);//
	OLED_WR_Byte(0x10,OLED_CMD);//

//	OLED_WR_Byte(0x40,OLED_CMD);//������ʾ��ʼ��40H+0~63
	OLED_WR_Byte(0x60,OLED_CMD);//������ʾ��ʼ��40H+0~63

	OLED_WR_Byte(0xD5,OLED_CMD);//����ʱ�ӷ�Ƶ���ӣ���Ƶ��
	OLED_WR_Byte(0xf0,OLED_CMD);//[3:0],��Ƶ����;[7:4],��Ƶ��	Ĭ��0111 0000   PS�󲿷�������ѡ����0x80

	OLED_WR_Byte(0xA8,OLED_CMD);//���ö���ͨ��
	OLED_WR_Byte(0x1F,OLED_CMD);//Ĭ��0X3F

	OLED_WR_Byte(0xD3,OLED_CMD);//������ʾƫ��
	OLED_WR_Byte(0x00,OLED_CMD);//Ĭ�ϲ�ƫ��[5:0]



	OLED_WR_Byte(0xA1,OLED_CMD);//���ض�������,bit0:   0,0->0;  1,0->127

//	OLED_WR_Byte(0xA0,OLED_CMD);//���ض�������,bit0:   0,0->0;  1,0->127

//	OLED_WR_Byte(0xC8,OLED_CMD);//����COMɨ�跽��;bit3: 0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0

    OLED_WR_Byte(0xC0,OLED_CMD);//����COMɨ�跽��;bit3: 0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0

	OLED_WR_Byte(0xD8,OLED_CMD);//
	OLED_WR_Byte(0x05,OLED_CMD);

	OLED_WR_Byte(0xDA,OLED_CMD);//����COMӲ����������
	OLED_WR_Byte(0x12,OLED_CMD);//Ĭ��[5:4]����

//	OLED_WR_Byte(0x00,OLED_CMD);// ��������ûɶ�ð�........

	OLED_WR_Byte(0xD9,OLED_CMD);//����Ԥ�������
	OLED_WR_Byte(0xF1,OLED_CMD);//Ĭ��[3:0],PHASE 1;[7:4],PHASE 2;

	OLED_WR_Byte(0x81,OLED_CMD);//�Աȶ�����
	OLED_WR_Byte(0xBF,OLED_CMD);//1~255;Ĭ��0X7F (��������,Խ��Խ��)

//	OLED_WR_Byte(0xD9,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
//	OLED_WR_Byte(0x25,OLED_CMD);//

	OLED_WR_Byte(0xDB,OLED_CMD);//����VCOMH ��ѹ����
	OLED_WR_Byte(0x34,OLED_CMD);//[5:2] 0000,0.64*vcc;1101,0.78*vcc;1111,0.84*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD);//ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����) A4/A5
	OLED_WR_Byte(0xA6,OLED_CMD);//������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ     A6/A7
	OLED_WR_Byte(0xAF,OLED_CMD);//������ʾ          						  AE/AF

	OLED_Clear();
	OLED_Set_Pos(0,0); 			
}  
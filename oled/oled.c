/***********************************************************************************************
功能说明：已支持画点，画线，画矩形，画圆，画窗口，画BMP
移植：liaokeyre
************************************************************************************************/
//接口定义参照oled.h文件
//四线SPI需要接七根线，其中RES可以不接；但是不接时候必须拉高
// GND,VCC,CS,RES,D/C,CLK,DIN
//关于电压问题；如果客户需要长时间稳定工作；建议全部采用3.3V电源；电平信号也采用3.3V的
//如果客户需要用5V电源，虽然5V可以正常工作，但式ㄒ榍不要长时间工作；这样可能会减少屏的寿命；在VCC端串一小电阻可能会好一些。
#include "oled.h"
#include "oledfont.h" 
#include "24c02.h"
#include "cd4052.h"
#define GUI_LCM_XMAX 128
#define GUI_LCM_YMAX  32	 
//#include "delay.h"
//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127
//--------------------------------------12832	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 
u8 OLED_GRAM[128][4];//显存
u8 OLED_GRAM_TMP[128][4];

/****************************************************************************
字节码高低位镜像操作
主要是有一个字库取码方向是反的
重新做字库又麻烦。。。。
****************************************************************************/	
u8 ByteMirro(u8 dat)
{
 dat = (dat & 0xf0) >> 4 | (dat & 0x0f)  << 4;//以4位为单位进行交换    abcd  1234
 dat = (dat & 0xcc) >> 2 | (dat & 0x33)  << 2;//以2位为单位进行交换    cdab  3412
 dat = (dat & 0xaa) >> 1 | (dat & 0x55)  << 1;//以1位为单位进行交换   dcba  4321
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
//向SSD1106写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
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
//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
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
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}
*/
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<4;i++)  
	{  
		OLED_WR_Byte (0xb4+i,OLED_CMD);    //设置页地址（0~7）b4-->b7
		OLED_WR_Byte (0x04,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}
		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<4;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
//	OLED_Refresh_Gram();//更新显示
}  
//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;            //得到所在页
	bx=y%8;				  //得到当前页精确位置
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}

//////////////////////////////////
//以x1,y1为起点，x2,y2为终点画线//
/////////(t=1画点，t=0清点)///////
//////////////////////////////////
//画线函数
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
* 功能：画水平线。
* 入口参数：x0      水平线起点所在列的位置
*           y0      水平线起点所在行的位置
*           x1      水平线终点所在列的位置
*           t       显示颜色(对于黑白色LCM，为0时灭，为1时显示)
* 出口参数：无
* 说明：对于单色、4级灰度的液晶，可通过修改此函数作图提高速度，如单色LCM，可以一次更
*      新8个点，而不需要一个点一个点的写到LCM中。
****************************************************************************/
void  OLED_Draw_HLine(u8 x0, u8 y0, u8 x1, u8 t)
{
    u8  temp;
    if(x0>x1)               // 对x0、x1大小进行排列，以便画图
    {
        temp = x1;
        x1 = x0;
        x0 = temp;
    }
    do
    {
        OLED_DrawPoint(x0, y0, t);   // 逐点显示，描出垂直线
        x0++;
    }
    while(x1>=x0);
}
/****************************************************************************
* 功能：画垂直线。
* 入口参数：x0      垂直线起点所在列的位置
*           y0      垂直线起点所在行的位置
*           y1      垂直线终点所在行的位置
*           t       显示颜色
* 出口参数：无
* 说明：对于单色、4级灰度的液晶，可通过修改此函数作图提高速度，如单色LCM，可以一次更
*      新8个点，而不需要一个点一个点的写到LCM中。
****************************************************************************/
void  OLED_Draw_RLine(u8 x0, u8 y0, u8 y1, u8 t)
{
    u8  temp;
    if(y0>y1)       // 对y0、y1大小进行排列，以便画图
    {
        temp = y1;
        y1 = y0;
        y0 = temp;
    }
    do
    {
        OLED_DrawPoint(x0, y0, t);   // 逐点显示，描出垂直线
        y0++;
    }
    while(y1>=y0);
}
/****************************************************************************
* 功能：画矩形。
* 入口参数：x0		矩形左上角的x坐标值
*           y0		矩形左上角的y坐标值
*           x1      矩形右下角的x坐标值
*           y1      矩形右下角的y坐标值
*           t	    1画点，0清点
* 出口参数：无
* 说明：操作失败原因是指定地址超出有效范围。
****************************************************************************/
void  OLED_DrawRectangle(u8 x1, u8 y1, u8 x2, u8 y2, u8 t)
{  OLED_Draw_HLine(x1, y1, x2, t);
   OLED_Draw_HLine(x1, y2, x2, t);
   OLED_Draw_RLine(x1, y1, y2, t);
   OLED_Draw_RLine(x2, y1, y2, t);
}
/****************************************************************************
* 功能：填充矩形。画一个填充的矩形，填充色与边框色一样。
* 入口参数：x0		矩形左上角的x坐标值
*           y0		矩形左上角的y坐标值
*           x1      矩形右下角的x坐标值
*           y1      矩形右下角的y坐标值
*           t	    填充颜色
* 出口参数：无
* 说明：操作失败原因是指定地址超出有效范围。
****************************************************************************/
void  OLED_Draw_RectangleFill(u8 x0, u8 y0, u8 x1, u8 y1, u8 t)
{  u8  i;

   /* 先找出矩形左上角与右下角的两个点，保存在(x0,y0)，(x1,y1) */
   if(x0>x1) 						// 若x0>x1，则x0与x1交换
   {  i = x0;
      x0 = x1;
      x1 = i;
   }
   if(y0>y1)						// 若y0>y1，则y0与y1交换
   {  i = y0;
      y0 = y1;
      y1 = i;
   }
   
   /* 判断是否只是直线 */
   if(y0==y1) 
   {  OLED_Draw_HLine(x0, y0, x1, t);
      return;
   }
   if(x0==x1) 
   {  (x0, y0, y1, t);
      return;
   }

   while(y0<=y1)						
   {  OLED_Draw_HLine(x0, y0, x1, t);	// 当前画水平线
      y0++;							// 下一行
   }
}
/****************************************************************************
* 功能：画正方形。
* 入口参数：x0		正方形左上角的x坐标值
*           y0		正方形左上角的y坐标值
*           with    正方形的边长
*           t	    显示颜色
* 出口参数：无
* 说明：操作失败原因是指定地址超出有效范围。
****************************************************************************/
void  OLED_Draw_Square(u8 x0, u8 y0, u8  with, u8 t)
{ 
    if(with==0) return;
	if( (x0+with) > GUI_LCM_XMAX ) return;
	if( (y0+with) > GUI_LCM_YMAX ) return;
	OLED_DrawRectangle(x0, y0, x0+with, y0+with, t);
}
/****************************************************************************
* 功能：画正方形填充。
* 入口参数：x0		正方形左上角的x坐标值
*           y0		正方形左上角的y坐标值
*           with    正方形的边长
*           t	    显示颜色
* 出口参数：无
* 说明：操作失败原因是指定地址超出有效范围。
****************************************************************************/
void  OLED_Draw_SquareFill(u8 x0, u8 y0, u8  with, u8 t)
{ 
    if(with==0) return;
	if( (x0+with) > GUI_LCM_XMAX ) return;
	if( (y0+with) > GUI_LCM_YMAX ) return;
	OLED_Draw_RectangleFill(x0, y0, x0+with, y0+with, t);
}
/****************************************************************************
* 功能：多个点之间的连续连线。从第一点连到第二点，再连到第三点...
* 入口参数：points  多个点坐标数据的指针，数据排列为(x0,y0)、(x1,y1)、(x2,y2)...
*           no      点数目，至少要大于1
*           t	显示颜色
* 出口参数：无
* 说明：操作失败原因是指定地址超出有效范围。
****************************************************************************/
void  OLED_Draw_LineS(u8 const *points, u8 no, u8 t)
{  u8  x0, y0;
   u8  x1, y1;
   u8  i;

   /* 入口参数过滤 */
   if(0==no) return;
   if(1==no)						// 单点
   {  x0 = *points++;
      y0 = *points;
      OLED_DrawPoint(x0, y0, t);
   }
   
   /* 画多条线条 */
   x0 = *points++;					// 取出第一点坐标值，作为原起点坐标值
   y0 = *points++;
   for(i=1; i<no; i++)
   {  x1 = *points++;				// 取出下一点坐标值
      y1 = *points++;
      OLED_DrawLine(x0, y0, x1, y1, t);
      x0 = x1;						// 更新原起点坐标
      y0 = y1;
   }
}
//////////////////////////////
//以x,y为圆心，以r为半径画圆//
/////(t=1画点，t=0清点)///////
//////////////////////////////
//画圆函数
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
* 功能：显示窗口。根据提供的窗口参数进行画窗口。
* 入口参数：win		窗口句柄
* 出口参数：返回0表示操作失败，返回1表示操作成功
****************************************************************************/
u8  OLED_Draw_WindowsDraw(WINDOWS *win)
{  u8  *str;
   int8  bak, bak1, bak2;
   
   /* 参数过滤，若窗口起出范围，则返回0 */
   if( ( (win->with)<20 ) || ( (win->hight)<20 ) ) return(0);		// 宽度、高度检查，限制最小窗口
   if( (win->x + win->with ) > GUI_LCM_XMAX ) return(0);			// 窗口宽度是否溢出
   if( (win->y + win->hight ) > GUI_LCM_YMAX ) return(0);			// 窗口高度是否溢出
    
   /* 开始画窗口 */
   OLED_Draw_RectangleFill(win->x, win->y, win->x + win->with - 1, win->y + win->hight - 1, 0);	//清出一片区域画弹窗
   OLED_DrawRectangle(win->x, win->y, win->x + win->with - 1, win->y + win->hight - 1, 1);	// 画窗口
   OLED_Draw_HLine(win->x, win->y + 9, win->x + win->with - 1, 1);							// 画标题目栏
//   OLED_Draw_RLine(win->x + 9, win->y, win->y + 9, 1);										// 画关闭窗号按钮
//   OLED_DrawLine(win->x, win->y, win->x + 9, win->y + 9, 1);
//   OLED_DrawLine(win->x + 9, win->y, win->x, win->y + 9, 1);
   OLED_Draw_RLine(win->x + win->with - 10, win->y, win->y + 9, 1);										// 画关闭窗号按钮
   OLED_DrawLine(win->x + win->with - 10, win->y, win->x + win->with - 1 , win->y + 9, 1);
   OLED_DrawLine(win->x + win->with - 1, win->y, win->x + win->with - 10, win->y + 9, 1);
   

   /* 写标题 */
   if( win->title != 0 ) 
   {  str = win->title;
      bak = win->x + 2;
      bak1 = win->y + 1;		  //title位置
      bak2 = win->x + win->with -1;
     
      while(1)						
      {  if( (bak+8) > bak2 ) break;								// 判断标题是否溢出
         if(*str=='\0') break;										// 判断字符串是否结束
         
         OLED_ShowChar(bak, bak1, *str++,7,1);							// 显示标题
         bak += 6;
      }
   }

   /* 写内容 */
   if( win->text != 0 ) 
   {  str = win->text;
      bak = win->x + 2;
      bak1 = win->y + 12;		  //text位置
      bak2 = win->x + win->with -1;
     
      while(1)						
      {  if( (bak+8) > bak2 ) break;								// 判断标题是否溢出
         if(*str=='\0') break;										// 判断字符串是否结束
         
         OLED_ShowChar(bak, bak1, *str++,7,1);							// 显示标题
         bak += 6;
      }
   }

   /* 写状态栏 */
   if( win->state != 0 )
   {  if( win->hight < 25) return(0);								// 判断是否可以画状态栏
      /* 画状态栏 */
      OLED_Draw_HLine(win->x, win->y + win->hight - 10, win->x + win->with - 1, 1);
      
      str = win->state;
      bak = win->x + 3;
      bak1 = win->y + win->hight - 9;
      bak2 = win->x + win->with -1;
      
      while(1)						
      {  if( (bak+8) > bak2 ) break;								// 判断标题是否溢出
         if(*str=='\0') break;										// 判断字符串是否结束
         
         OLED_ShowChar(bak, bak1, *str++,7,1);							// 显示标题
         bak += 6;
      }      
   }
   
   return(1);

}
/*******************************************************************************************
函数名: void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)
说明: 填充指定区域RAM
********************************************************************************************/
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//更新显示
}


/*******************************************************************************************
函数名: void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
说明: 显示字符
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12/7 
********************************************************************************************/
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	chr=chr-' ';//得到偏移后的值				   
    for(t=0;t<size;t++)
    {   
	    if(size==7)temp=ByteMirro(F7x5[chr][t]); //调用0806字体  因为字库是反的，所以镜像处理了！
		else if(size==12)temp=asc2_1206[chr][t];  //调用1206字体
		else temp=asc2_1608[chr][t];		 //调用1608字体 	                          
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
//m^n函数
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  

/*******************************************************************************************
函数名: void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
说明: 显示数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);
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
函数名: void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
说明: 显示字符串
//显示字符串
//x,y:起点坐标  
//*p:字符串起始地址
********************************************************************************************/
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size, u8 mode)
{
           
    while(*p!='\0')
    { 
	    if(size == 16)
		{      
	        if(x>122){x=0;y+=16;}	//换行
	        if(y>32){y=x=0;OLED_Clear();} //换页
	        OLED_ShowChar(x,y,*p,16,mode);	 
	        x+=8;
		}
		else if(size == 12)
		{
	        if(x>122){x=0;y+=12;}	//换行
	        if(y>32){y=x=0;OLED_Clear();} //换页
	        OLED_ShowChar(x,y,*p,12,mode);	 
	        x+=6;		
		}
		else
		{
	        if(x>122){x=0;y+=8;}	//换行
	        if(y>32){y=x=0;OLED_Clear();} //换页
	        OLED_ShowChar(x,y,*p,7,mode);	 
	        x+=6;		
		}
        p++;
    }  
}

/*******************************************************************************************
函数名: void OLED_ShowFont16(u8 x,u8 y,u8 fno,u8 mode)
说明: 显示汉字
//显示汉字
//在指定位置显示一个汉字
//x:0~127
//y:0~63
//fno:汉字编号
//mode:0,反白显示;1,正常显示				 
//16*16大小的字符
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
函数名: void OLED_DrawTriangle(u8 x1, u8 y1, u8 len,u8 t)
说明: 画三角形， 起始位置坐标  
********************************************************************************************/
void OLED_DrawTriangle(u8 x1, u8 y1, u8 len,u8 t)
{
  OLED_DrawLine(x1,y1,x1-len,y1+len,t);
  OLED_DrawLine(x1,y1,x1+len,y1+len,t);
  OLED_DrawLine(x1+len,y1+len,x1-len,y1+len,t);	//画三角形
}
/*******************************************************************************************
函数名: void OLED_DrawBar(u8 x1, u8 y1, u8 x2, u8 y2, u8 t)
说明: 画进度条， 起始位置坐标  
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
函数名: void OLED_DrawBar(u8 x1, u8 y1, u8 x2, u8 y2, u8 t)
说明: 画进度条， 起始位置坐标  
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
函数名: void OLED_DrawBMP(u8 x,u8 y)
说明: 画位图 起始位置坐标 x,y
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
说明: 画位图 起始位置坐标 x,y
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
函数名: void OLED_ShowAdd(u8 x,u8 y,u32 Add1,u32 Add2)
说明: 画位图 起始位置坐标 x,y
********************************************************************************************/
void OLED_ShowAdd(u8 x,u8 y,u32 Add1,u32 Add2)
{     			    
  OLED_ShowString(x+4,y,"SADD:",7,1); 	 //第一行字符
  OLED_ShowString(x+4,y+16,"RADD:",7,1);   //第三行字符 
  OLED_DrawLine(38,0,38,31,1); 
  OLED_DrawLine(38,0,40,0,1); 
  OLED_DrawLine(38,15,40,15,1);
  OLED_DrawLine(38,31,40,31,1);			 //画分割线
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
函数名: void OLED_Init(void)
说明: OLED屏初始化
********************************************************************************************/				    
void OLED_Init(void)
{
    OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(100);
	OLED_RST_Set(); 

	OLED_WR_Byte(0xFD,OLED_CMD);//内部单片机写保护
	OLED_WR_Byte(0x12,OLED_CMD);//默认关闭内部保护，允许接收所有指令，如开启保护则只接受FD指令 ？默认为0，为1则保护 bit 0 0 0 1 0 ？1 0
		
	OLED_WR_Byte(0xAE,OLED_CMD);//关闭显示

	OLED_WR_Byte(0x20,OLED_CMD);
	OLED_WR_Byte(0x02,OLED_CMD);//page mode

	OLED_WR_Byte(0xB4,OLED_CMD);

	OLED_WR_Byte(0x00,OLED_CMD);//
	OLED_WR_Byte(0x10,OLED_CMD);//

//	OLED_WR_Byte(0x40,OLED_CMD);//设置显示开始行40H+0~63
	OLED_WR_Byte(0x60,OLED_CMD);//设置显示开始行40H+0~63

	OLED_WR_Byte(0xD5,OLED_CMD);//设置时钟分频因子，振荡频率
	OLED_WR_Byte(0xf0,OLED_CMD);//[3:0],分频因子;[7:4],震荡频率	默认0111 0000   PS大部分驱动都选择是0x80

	OLED_WR_Byte(0xA8,OLED_CMD);//设置多重通道
	OLED_WR_Byte(0x1F,OLED_CMD);//默认0X3F

	OLED_WR_Byte(0xD3,OLED_CMD);//设置显示偏移
	OLED_WR_Byte(0x00,OLED_CMD);//默认不偏移[5:0]



	OLED_WR_Byte(0xA1,OLED_CMD);//段重定义设置,bit0:   0,0->0;  1,0->127

//	OLED_WR_Byte(0xA0,OLED_CMD);//段重定义设置,bit0:   0,0->0;  1,0->127

//	OLED_WR_Byte(0xC8,OLED_CMD);//设置COM扫描方向;bit3: 0,普通模式;1,重定义模式 COM[N-1]->COM0

    OLED_WR_Byte(0xC0,OLED_CMD);//设置COM扫描方向;bit3: 0,普通模式;1,重定义模式 COM[N-1]->COM0

	OLED_WR_Byte(0xD8,OLED_CMD);//
	OLED_WR_Byte(0x05,OLED_CMD);

	OLED_WR_Byte(0xDA,OLED_CMD);//设置COM硬件引脚配置
	OLED_WR_Byte(0x12,OLED_CMD);//默认[5:4]配置

//	OLED_WR_Byte(0x00,OLED_CMD);// ？？？？没啥用吧........

	OLED_WR_Byte(0xD9,OLED_CMD);//设置预充电周期
	OLED_WR_Byte(0xF1,OLED_CMD);//默认[3:0],PHASE 1;[7:4],PHASE 2;

	OLED_WR_Byte(0x81,OLED_CMD);//对比度设置
	OLED_WR_Byte(0xBF,OLED_CMD);//1~255;默认0X7F (亮度设置,越大越亮)

//	OLED_WR_Byte(0xD9,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
//	OLED_WR_Byte(0x25,OLED_CMD);//

	OLED_WR_Byte(0xDB,OLED_CMD);//设置VCOMH 电压倍率
	OLED_WR_Byte(0x34,OLED_CMD);//[5:2] 0000,0.64*vcc;1101,0.78*vcc;1111,0.84*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD);//全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏) A4/A5
	OLED_WR_Byte(0xA6,OLED_CMD);//设置显示方式;bit0:1,反相显示;0,正常显示     A6/A7
	OLED_WR_Byte(0xAF,OLED_CMD);//开启显示          						  AE/AF

	OLED_Clear();
	OLED_Set_Pos(0,0); 			
}  
#include "main.h"
#include "uart.h"
#include "oled.h"
#include "24c02.h"
#include "cd4052.h"
#include "_595.h"
#include "isr.h"
#include "key.h"

u8 MODE;
u32 AddNo;    //割码地址值 

u8 Cutflag;
u8 BeepFlag;
u8 PKG;		   //封装型号
u32 OK_COUNT;
u32 NG_COUNT;

u8 POP;      //弹窗标志
extern void tsteicon(void);
WINDOWS windemo;
void init_windows(u8 x ,u8 y,u8 with,u8 height,u8 * title,u8 * text,u8 * state)
{
  windemo.x =x;
  windemo.y =y;
  windemo.with =with;
  windemo.hight =height;
  windemo.title = title;
  windemo.text = text;
  windemo.state = state;
} 
#ifdef PRINT_AUTHOR_INFO
void printInfo(void)
{
   send_string ("=======================================================\n");
   send_string ("********************* Burn Code X *********************\n");
   send_string ("*                                                     *\n");
   send_string ("*    Version: SDK1.00.00(11:20:16 Apr 2  2015)        *\n");
   send_string ("*     Author: Liaokeyre<Liaokeyre@foxmail.com>        *\n");
   send_string ("*  Author: Cherry@ycsemi.com.cn @yancheng company     *\n");
   send_string ("*                                                     *\n");
   send_string ("*******************************************************\n"); 
   send_string ("=======================================================\n");
   send_string ("\n\n\nSystem Loading...\n");
   send_string ("\n\n\nSystem Load Ok!\n");
}
#endif

void initAddNo(void)	 //判断地址值是否正常
{
  u32 tmp;
  at24c02_rddat(0x10,(u8*)&tmp,4);
  if((tmp == 0xffffffff)||((tmp > 0xfffff))) //初始状态 || 累加溢出？
  {
   AddNo = 0;
   at24c02_wrdat(0x10,(u8*)&AddNo,4);
  }
}
void oledInit(void)
{
	OLED_Init();  //初始化OLED  
	OLED_Clear(); 
	/*
    OLED_ShowFont16(0,10,0,1);
	OLED_ShowFont16(16,10,1,1);
	OLED_ShowFont16(32,10,2,1);
	OLED_ShowFont16(48,10,3,1);
	OLED_ShowFont16(64,10,4,1);
	OLED_ShowFont16(80,10,5,1);
	OLED_ShowFont16(96,10,6,1);
	OLED_ShowFont16(112,10,7,1);
	*/
#ifdef SHOW_LOGO
	OLED_DrawBMP(0,0,1);			// show logo
    OLED_DrawBar(36, 26, 124, 30, 1);
#endif
	OLED_Refresh_Gram();
}

void ioInit(void)
{
      P1M1 = 0;
	  P1M0 = 0; 
      P2M1 = 0;
	  P2M0 = 0;
      P3M1 = 0;
	  P3M0 = 0; 
      P4M1 = 0;
	  P4M0 = 0;
      P5M1 = 0;
	  P5M0 = 0; 
      P0M1 = 0;
	  P0M0 = 0;

	  P4M0 |= 0x22;

	  P1M1 |= 0Xff;
	  P1M0 &= ~0Xff; //高阻输入	ADC输入端口配置

	  P2M1 &= ~0XF8;
	  P2M0 |= 0XF8;	//推挽

	  P3M1 |= 0x04;
	  P3M1 &= ~0x04;  //	p32 输入

	  P5M1 &= ~0X20;
	  P5M0 |= 0X20;    //P55推挽
}

void sysInit(void)
{
	ioInit();
	InitADC();
	UartInit();
	oledInit();
	timerInit();
	BeepFlag = 0;
	OK_COUNT = 0;
	NG_COUNT = 0; //复位计数器
	DSENVMOS();		          // 关mos电源
	DSENABLE595();  		  // 关595控制mos开关
	Cutflag = 0;
	MODE = cutmode;
	POP = 0;
	DSENVSOC();
	K1_OUT = 0;
}
void set_mode(void)
{
    u8 Done;
	u8 InMode;
	InMode = 0;
	Done = 0;
	Disp_Menu_Main(setmode);
	OLED_Refresh_Gram();
	while(!Done)
	{
	  if(Key_change)
	  {
	  	switch(Key_back)
		{
		  case PR_ADD:
		  		Key_change=0; 
		        BeepFlag =1;
				if(InMode)
				{
				
				}
				else
				{
				  MODE = rstmode;
				  Done = 1;
				}
		  break;
		  case PR_SUB:
		  		Key_change=0; 
		        BeepFlag =1;
				if(InMode)
				{
				
				}
				else
				{
				  MODE = cutmode;
				  Done = 1;
				}
		  break;
		  case PR_OK:
		  		Key_change=0; 
		        BeepFlag =1;
				if(InMode)
				{
				
				}
				else
				{
				  memcpy(OLED_GRAM_TMP,OLED_GRAM,512);
		          init_windows(30,0,60,32,"Notice"," NO MENU!",0);
		          OLED_Draw_WindowsDraw(&windemo);
				  POP = 1;
 	              OLED_Refresh_Gram();
				  //InMode = 1;
				}
		  break;
		}
	  
	  }
	}

}
void cut_mode(void)
{
    u8 Done;
	u8 InMode;
	u8 menucnt;
	menucnt = 0;
	InMode = 0;
	Done = 0;
	Disp_Menu_Main(cutmode);
	OLED_Refresh_Gram();
	while(!Done)
	{
	  if(Key_change)
	  {
	  	switch(Key_back)
		{
		  case PR_ADD:
		  		Key_change=0; 
		        BeepFlag =1;
				if(InMode)
				{
				  if(menucnt --<=0)
				  menucnt = 3;
				  Disp_Menu_1(menucnt);
	              OLED_Refresh_Gram();
				}
				else
				{
				  MODE = setmode;
				  Done = 1;
				}
		  break;
		  case PR_SUB:
		  		Key_change=0; 
		        BeepFlag =1;
				if(InMode)
				{
				  if(menucnt ++>=3)
				  menucnt = 0;
				  Disp_Menu_1(menucnt);
	              OLED_Refresh_Gram();				  				
				}
				else
				{
				  MODE = rstmode;
				  Done = 1;
				}
		  break;
		  case PR_OK:
		  		Key_change=0; 
		        BeepFlag =1;
				if(InMode)
				{
				  if(menucnt ==3)	 //返回
				  {
				    OLED_Clear();	 //清屏 
				    Done = 1;
				    return;
				  }
				  else
				  {
					  if(menucnt ==0)	 //PKG-1
					  {
						  memcpy(OLED_GRAM_TMP,OLED_GRAM,512);
				          init_windows(20,0,80,32,"Notice"," VT62538-B1",0);
				          OLED_Draw_WindowsDraw(&windemo);
						  POP = 1;
		 	              OLED_Refresh_Gram();				    
					  }
					  else if(menucnt ==1)	 //PKG-2
					  {
						  memcpy(OLED_GRAM_TMP,OLED_GRAM,512);
				          init_windows(20,0,80,32,"Notice"," VT62538-B2",0);
				          OLED_Draw_WindowsDraw(&windemo);
						  POP = 1;
		 	              OLED_Refresh_Gram();					    
					  }
					  else if(menucnt ==2)	 //COB
					  {
						  memcpy(OLED_GRAM_TMP,OLED_GRAM,512);
				          init_windows(20,0,80,32,"Notice"," COB",0);
				          OLED_Draw_WindowsDraw(&windemo);
						  POP = 1;
		 	              OLED_Refresh_Gram();					    
					  }
					  OLED_Clear();
					  PKG = menucnt; //注意增加型号需要判断是否对应
					  OK_COUNT = 0;
					  NG_COUNT = 0;
					  CutRun(PKG);
				  }
				}
				else
				{
				  OLED_Clear();
				  Disp_Menu_1(0);
	              OLED_Refresh_Gram();
				  InMode = 1;
				}
		  break;
		}
	  
	  }
	}
}
void rst_mode(void)
{
    u8 Done;
	u8 InMode;
	InMode = 0;
	Done = 0;
	Disp_Menu_Main(rstmode);
	OLED_Refresh_Gram();
	while(!Done)
	{
	  if(Key_change)
	  {
	  	switch(Key_back)
		{
		  case PR_ADD:
		  		Key_change=0; 
		        BeepFlag =1;
				if(InMode)
				{
				
				}
				else
				{
				  MODE = cutmode;
				  Done = 1;
				}
		  break;
		  case PR_SUB:
		  		Key_change=0; 
		        BeepFlag =1;
				if(InMode)
				{
				
				}
				else
				{
				  MODE = setmode;
				  Done = 1;
				}
		  break;
		  case PR_OK:
		  		Key_change=0; 
		        BeepFlag =1;
				if(InMode)
				{
				
				}
				else
				{
				  Flush_24c02();
				  memcpy(OLED_GRAM_TMP,OLED_GRAM,512);
		          init_windows(10,0,100,32,"Notice"," FLUSH 2402 OK!",0);
		          OLED_Draw_WindowsDraw(&windemo);
				  POP = 1;
 	              OLED_Refresh_Gram();
				  //InMode = 1;
				}
		  break;
		}
	  
	  }
	}
}

void lookinfo(void)
{

}

void main(void)
{
    SPEAKER = 0;
    sysInit();
	SPEAKER = 1;
#ifdef PRINT_AUTHOR_INFO
	printInfo();
#endif
    EA = 1;
	SPEAKER = 0;
	AddNo = 1;
	OLED_Clear();
	while(1)
	{
	   switch(MODE)
	   {
	   	 case setmode:
		   set_mode();
		 break;
	   	 case cutmode:
		   cut_mode();
		 break;
	   	 case rstmode:
		   rst_mode();
		 break;
	   }

/*
	  if((Key_back == PR_ADD)&&(Key_change))
	  {
	   AddNo >>= 1;		storge595(&AddNo,4); Key_change=0; BeepFlag =1;	
	  }
	  else if((Key_back == PR_SUB)&&(Key_change))
	  {
	   AddNo <<= 1;	 	storge595(&AddNo,4);  Key_change=0;	BeepFlag =1;
	  }
	  else if((Key_back == PR_OK)&&(Key_change))
	  {
	   StartCut(0); Key_change=0;	BeepFlag =1;
	   
	  }
 */	  
	}
}
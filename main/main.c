#include "main.h"
#include "uart.h"
#include "oled.h"
#include "24c02.h"
#include "cd4052.h"
#include "isr.h"
/*********************************************************************/
#define PRINT_AUTHOR_INFO	   //打印作者信息
#define SHOW_LOGO			   //显示屏显示logo
/*********************************************************************///宏开关，控制以上功能选项
u32 AddNo;    //割码地址值 
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
	OLED_DrawBMP(0,0);			// show logo
#endif
	OLED_Refresh_Gram();
}

void ioInit(void)
{

}

void sysInit(void)
{
  ioInit();
  UartInit();
  oledInit();
  timerInit();
}

void main(void)
{
    sysInit();
#ifdef PRINT_AUTHOR_INFO
	printInfo();
#endif
////////////////////////////////////////////////////////////////////////////
    P2M1 =0x04;
	P2M1 = 0;
////////////////////////////////////////////////////////////////////////////TEST
    EA = 1;
	while(1)
	{
	  TestPn();
	}
}
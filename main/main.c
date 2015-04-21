#include "main.h"
#include "uart.h"
#include "oled.h"
#include "24c02.h"
#include "cd4052.h"
#include "_595.h"
#include "isr.h"
#include "key.h"
/*********************************************************************/
#define PRINT_AUTHOR_INFO	   //打印作者信息
#define SHOW_LOGO			   //显示屏显示logo
/*********************************************************************///宏开关，控制以上功能选项
u32 AddNo;    //割码地址值 
#ifdef PRINT_AUTHOR_INFO

u8 Cutflag;
u8 BeepFlag;
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

}

void sysInit(void)
{
  ioInit();
  InitADC();
  UartInit();
  oledInit();
  timerInit();
  BeepFlag = 0;
}

void main(void)
{
	u16 tmp;
    sysInit();
	SPEAKER = 1;
#ifdef PRINT_AUTHOR_INFO
	printInfo();
#endif
    EA = 1;
	SPEAKER = 0;
	OLED_DrawBMP(0,0);
	AddNo = 1;
	storge595(&AddNo,4);
	ENABLE595(); 
		DSENVMOS();		          // 关mos电源
		DSENABLE595();  		  // 关595控制mos开关
		Cutflag = 0;
	OLED_Clear();
	while(1)
	{
//  A1_4052 = 1;
//  A2_4052 = 1;
//  B1_4052 = 1;
//  B2_4052 = 1;
//  NOP40();
//  tmp = Adc2Res(GetADCResult(CH2));
//  printf_u8(tmp>>8);
//  printf_u8(tmp);
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
//	  OLED_ShowAdd(0,0,AddNo,AddNo);
//	  OLED_Refresh_Gram();
//	  
	}
}
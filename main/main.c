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

u8 Maxline;
u32 MaxlineHex;
u8 POP;      //弹窗标志
extern Pad_Res RES;
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

	  P5M1 &= ~0X28;
	  P5M0 |= 0X28;    //P55推挽
}
u32 EXP2(u8 num)
{
  u8 i;
  u32 tmp;
  tmp = 1;
  for (i=0;i<num;i++)
  tmp*=2;
  return tmp;
}
void sysInit(void)
{
	ioInit();
	InitADC();
	InitSign();
	UartInit();
	oledInit();
	timerInit();
	BeepFlag = 0;
	OK_COUNT = 0;
	NG_COUNT = 0;             //复位计数器
	DSENVMOS();		          // 关mos电源
	DSENABLE595();  		  // 关595控制mos开关
	Cutflag = 0;
	MODE = cutmode;
	POP = 0;
	DSENVSOC();
	K1_OUT_LOW();
	Maxline = at24c02_rd(LINE_NUM_ADD);
	if((Maxline >20)||(Maxline < 1))
	{
	  Maxline = 20;
	  at24c02_wr(LINE_NUM_ADD, Maxline);
	}
	MaxlineHex = EXP2(Maxline);
}
void SetLine(void)
{
	u8 Done;
	u8 Linetmp;
	Done = 0;
#ifdef PRINT_INFO
	send_string ("\n 设置割码线 \n");
#endif
	Linetmp = at24c02_rd(LINE_NUM_ADD);
	Maxline = Linetmp;
	if((Maxline >20)||(Maxline < 1))
	{
	  Maxline = 20;
	  at24c02_wr(LINE_NUM_ADD, Maxline);
	}
	Linetmp = Maxline;
	MaxlineHex = EXP2(Maxline);
	OLED_Clear();
    OLED_ShowChar(0,14,'0',7,1);
    OLED_ShowChar(115,14,'2',7,1);
    OLED_ShowChar(121,14,'0',7,1); 
	OLED_ShowString(20,0,"Set max line",12,1);
	while(!Done)
	{
	  	OLED_DrawSetBar(9, 15, 110, 19, 20,Linetmp, 1);
		//OLED_ShowChar(x1-8,18,Hex2Dat(((PN_TMP.PadByte)&0x0f)),12,1);
	    OLED_Refresh_Gram();
      if(Key_change)
	  {
	  	switch(Key_back)
		{
		  case PR_ADD:
		  		Key_change=0; 
		        BeepFlag =1;
				if(Linetmp-- <= 1)
				Linetmp = 20;
		  break;
		  case PR_SUB:
		  		Key_change=0; 
		        BeepFlag =1;
				if(Linetmp++ >= 20)
				Linetmp = 1;		
		  break;
		  case PR_OK:
		  		Key_change=0; 
		        BeepFlag =1;
	            init_windows(20,0,80,32,"Notice"," Set OK!",0);
	            OLED_Draw_WindowsDraw(&windemo);
                OLED_Refresh_Gram();
			    Maxline = Linetmp;
				MaxlineHex = EXP2(Maxline);
				at24c02_wr(LINE_NUM_ADD, Maxline);				
				delay_10ms();
				delay_10ms();
				delay_10ms();
				delay_10ms();
#ifdef PRINT_INFO
	            send_string ("\n割码线设置为 : ");
                put_char((Maxline/10)+'0');
	            put_char((Maxline%10)+'0');
                send_string (" 条\n");
#endif
				Done = 1;	
		  break;
		  case PR_MOD:
		  		Key_change=0; 
		        BeepFlag =1;
				OLED_Clear();
				Done = 1;
		  break;
		}
	  }	
	}
}

void set_mode(void)
{
    u8 Done;
	u8 InMode;
	u8 menucnt;
	u8 Eraseflag;
	u8 YesNo;
	YesNo = 0;
	Eraseflag = 0;    //删除内存的二次确认
	menucnt = 0;
	InMode = 0;
	Done = 0;
	Disp_Menu_Main(setmode);
	OLED_Refresh_Gram();
#ifdef PRINT_INFO
	send_string ("\n【设置】 割码 测试\n");
#endif
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
				  if(Eraseflag == 1)
				  {
					  if(YesNo ++ >=1)
					  YesNo = 0;
					  OLED_ShowString(70,20,"YES",12,!YesNo);
					  OLED_ShowString(110,20,"NO",12,YesNo);
					  OLED_Refresh_Gram();	
				  }
				  else
				  {
					  if(menucnt --<=0)
					  menucnt = 3;
					  Disp_Menu_0(menucnt);
		              OLED_Refresh_Gram();				  
				  }				
				}
				else
				{
				  MODE = testmode;
				  Done = 1;
				}
		  break;
		  case PR_SUB:
		  		Key_change=0; 
		        BeepFlag =1;
				if(InMode)
				{
				  if(Eraseflag == 1)
				  {
					  if(YesNo -- <= 0)
					  YesNo = 1;
					  OLED_ShowString(70,20,"YES",12,!YesNo);
					  OLED_ShowString(110,20,"NO",12,YesNo);
					  OLED_Refresh_Gram();
				  }
				  else
				  {
					  if(menucnt ++>=3)
					  menucnt = 0;
					  Disp_Menu_0(menucnt);
		              OLED_Refresh_Gram();
				  }					
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
					  if(menucnt ==2)	 
					  {
					      if(Eraseflag == 1)
						  {
					        
							if(YesNo)
							{
							    OLED_Clear();
						  	    OLED_ShowString(2,10,"ERASE MEMORRY...",12,1);					 
								OLED_Refresh_Gram();
							    Flush_24c02();
							    OLED_Clear();
						  	    OLED_ShowString(2,10,"ERASE DONE!",12,1);					 
								OLED_Refresh_Gram();
#ifdef PRINT_INFO
					            send_string ("\n记录擦除完成\n");
#endif
							}

						    OLED_Clear();
						    Disp_Menu_0(menucnt);
							OLED_Refresh_Gram();						    
							Eraseflag = 0;
							YesNo = 0;
						  }
						  else
						  {
						    Eraseflag = 1;
					        OLED_Clear();
					  	    OLED_ShowString(2,0,"Clear all memorry?",12,1);	
							OLED_ShowString(70,20,"YES",12,1);
							OLED_ShowString(110,20,"NO",12,0);				 
		 	                OLED_Refresh_Gram();				  
						  }
						  				    
					  }
					  else if(menucnt ==0)	 
					  {
					      SetLine();
			   			  OLED_Clear();
				          Disp_Menu_0(menucnt);
		 	              OLED_Refresh_Gram();					    
					  }
/*					  else if(menucnt ==2)
					  {
						  memcpy(OLED_GRAM_TMP,OLED_GRAM,512);
				          init_windows(20,0,80,32,"Notice"," COB",0);
				          OLED_Draw_WindowsDraw(&windemo);
						  POP = 1;
		 	              OLED_Refresh_Gram();					    
					  }	 
*/					  				
				}
				else
				{
				  OLED_Clear();
				  Disp_Menu_0(menucnt);
	              OLED_Refresh_Gram();
				  InMode = 1;
				}
		  break;
		  case PR_MOD:
		  		Key_change=0; 
		        BeepFlag =1;
				OLED_Clear();
				Done = 1;
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
	menucnt = 2;      //直接选择到晶元模式
	InMode = 0;
	Done = 0;
	Disp_Menu_Main(cutmode);
	OLED_Refresh_Gram();
#ifdef PRINT_INFO
	send_string ("\n设置 【割码】 测试\n");
#endif
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
				  MODE = testmode;
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
				  /*
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
					  */
					  OLED_Clear();
					  PKG = menucnt; //注意增加型号需要判断是否对应
					  OK_COUNT = 0;
					  NG_COUNT = 0;
					  CutRun(PKG);	  
				      OLED_Clear();	  //退出了割码操作
				      Disp_Menu_1(menucnt);
	                  OLED_Refresh_Gram();
				  }
				}
				else
				{
				  OLED_Clear();
				  Disp_Menu_1(menucnt);
	              OLED_Refresh_Gram();
				  InMode = 1;
				}
		  break;
		  case PR_MOD:
		  		Key_change=0; 
		        BeepFlag =1;
				OLED_Clear();
				Done = 1;
		  break;
		}
	  
	  }
	}
}
void test_mode(void)
{
    u8 Done;
	u8 InMode;
	u32 IRtmp;
	IRtmp = 0;
	InMode = 0;
	Done = 0;
	DsDecode();
	Disp_Menu_Main(testmode);
	OLED_Refresh_Gram();
#ifdef PRINT_INFO
	send_string ("\n设置 割码 【测试】\n");
#endif
	while(!Done)
	{
	  if(InMode)
	  {

	    if((ReIRcode & 0xf) != 0)
		{
		   IRtmp = ReIRcode;
		}
		if(IRtmp)
		{
		    OLED_ShowChar(36,16,Hex2Dat(((IRtmp>>16)>>4)&0x0f),12,1);
		    OLED_ShowChar(42,16,Hex2Dat(((IRtmp>>16)&0x0f)),12,1);
		
			OLED_ShowChar(54,16,Hex2Dat(((IRtmp>>8)>>4)&0x0f),12,1);
			OLED_ShowChar(60,16,Hex2Dat(((IRtmp>>8)&0x0f)),12,1);
		
			OLED_ShowChar(72,16,Hex2Dat(((IRtmp)>>4)&0x0f),12,1);
		 	OLED_ShowChar(78,16,Hex2Dat(((IRtmp)&0x0f)),12,1);
			OLED_Refresh_Gram();
		}   
	  }

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
				 /*
				  Flush_24c02();
				  memcpy(OLED_GRAM_TMP,OLED_GRAM,512);
		          init_windows(10,0,100,32,"Notice"," FLUSH 2402 OK!",0);
		          OLED_Draw_WindowsDraw(&windemo);
				  POP = 1;
 	              OLED_Refresh_Gram();
				  */
				  EnDecode();
				  OLED_Clear();
				  OLED_ShowString(46,0,"DECODE",12,1);
				  OLED_Refresh_Gram();
				  InMode = 1;
				}
		  break;
		  case PR_MOD:
		  		Key_change=0; 
		        BeepFlag =1;
				OLED_Clear();
				Done = 1;
		  break;
		}
	  
	  }
	}
}

//void lookinfo(void)
//{
//
//}

u8 chek4052(void)
{
  u8 ret;
  ret = 0;
  OLED_Clear();
  if((RES.Pad0 <=Res_NG)||(RES.Pad1 <=Res_NG)||(RES.Pad2 <=Res_NG)||(RES.Pad3 <=Res_NG)||
     (RES.Pad4 <=Res_NG)||(RES.Pad5 <=Res_NG)||(RES.Pad6 <=Res_NG)||(RES.Pad7 <=Res_NG))
   {
      ret = 0x01;
	  send_string ("\n请更换U9 ：CD4052 \n");
	  OLED_ShowString(0,11,"Check U9  CD4052",12,1);
   }
  if((RES.Pad8 <=Res_NG)||(RES.Pad9 <=Res_NG)||(RES.Pad10 <=Res_NG)||(RES.Pad11 <=Res_NG)|| 
     (RES.Pad12 <=Res_NG)||(RES.Pad13 <=Res_NG)||(RES.Pad14 <=Res_NG)||(RES.Pad15 <=Res_NG))
   {
      ret = 0x11;
	  send_string ("\n请更换U12 ：CD4052 \n");
	  OLED_ShowString(0,22,"Check U12 CD4052",12,1);
   }
   if(ret)
   {
	OLED_ShowString(0,0,"Hardware error:",12,1);
	OLED_Refresh_Gram();
   }
  if((Key_back == PR_OK)&&(Key_change))
  {
   Key_change=0; 
   BeepFlag =1;
   OLED_Clear();
   ret = 0;
  }	 
   return ret;
}

void main(void)
{
    SPEAKER = 0;
    sysInit();
	SPEAKER = 1;
#ifdef PRINT_AUTHOR_INFO
	printInfo();
#endif
#ifdef PRINT_INFO
	send_string ("\n\n总烧录线【十进制】 : ");
    put_char((Maxline/10)+'0');
	put_char((Maxline%10)+'0');
	send_string ("  最大烧录地址【十六进制】:");
	printf_u8(MaxlineHex>>16);
    printf_u8(MaxlineHex>>8);
    printf_u8(MaxlineHex);
#endif
    GetPadRes();
	//判断4052是否损坏！
#ifdef PRINT_INFO
	send_string ("\n\nADC采集通道测试 :\n--------------------------------------------------------- ");
	send_string ("\n通道0：   ");
    printf_u8(RES.Pad0>>8);
    printf_u8(RES.Pad0);
	send_string ("\n通道1：   ");
    printf_u8(RES.Pad1>>8);
    printf_u8(RES.Pad1);
	send_string ("\n通道2：   ");
    printf_u8(RES.Pad2>>8);
    printf_u8(RES.Pad2);
	send_string ("\n通道3：   ");
    printf_u8(RES.Pad3>>8);
    printf_u8(RES.Pad3);
	send_string ("\n通道4：   ");
    printf_u8(RES.Pad4>>8);
    printf_u8(RES.Pad4);

	send_string ("\n通道5：   ");
    printf_u8(RES.Pad5>>8);
    printf_u8(RES.Pad5);
	send_string ("\n通道6：   ");
    printf_u8(RES.Pad6>>8);
    printf_u8(RES.Pad6);
	send_string ("\n通道7：   ");
    printf_u8(RES.Pad7>>8);
    printf_u8(RES.Pad7);
	send_string ("\n通道8：   ");
    printf_u8(RES.Pad8>>8);
    printf_u8(RES.Pad8);
	send_string ("\n通道9：   ");
    printf_u8(RES.Pad9>>8);
    printf_u8(RES.Pad9);
	send_string ("\n通道10：   ");
    printf_u8(RES.Pad10>>8);
    printf_u8(RES.Pad10);

	send_string ("\n通道11：   ");
    printf_u8(RES.Pad11>>8);
    printf_u8(RES.Pad11);
	send_string ("\n通道12：   ");
    printf_u8(RES.Pad12>>8);
    printf_u8(RES.Pad12);
	send_string ("\n通道13：   ");
    printf_u8(RES.Pad13>>8);
    printf_u8(RES.Pad13);
	send_string ("\n通道14：   ");
    printf_u8(RES.Pad14>>8);
    printf_u8(RES.Pad14);

	send_string ("\n通道15：   ");
    printf_u8(RES.Pad15>>8);
    printf_u8(RES.Pad15);
	send_string ("\n通道16：   ");
    printf_u8(RES.Pad16>>8);
    printf_u8(RES.Pad16);
	send_string ("\n通道17：   ");
    printf_u8(RES.Pad17>>8);
    printf_u8(RES.Pad17);
	send_string ("\n通道18：   ");
    printf_u8(RES.Pad18>>8);
    printf_u8(RES.Pad18);
	send_string ("\n通道19：   ");
    printf_u8(RES.Pad19>>8);
    printf_u8(RES.Pad19);
	send_string ("\n如果某个通道值为0或小于00 F0，请更换CD4052!\n");
#endif
    EA = 1;
	SPEAKER = 0;
	AddNo = 1;
	OLED_Clear();

	while(1)
	{
	   while(chek4052());
	   switch(MODE)
	   {
	   	 case setmode:
		   set_mode();
		 break;
	   	 case cutmode:
		   cut_mode();
		 break;
	   	 case testmode:
		   test_mode();
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
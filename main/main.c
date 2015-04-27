#include "main.h"
#include "uart.h"
#include "oled.h"
#include "24c02.h"
#include "cd4052.h"
#include "_595.h"
#include "isr.h"
#include "key.h"

u8 MODE;
u32 AddNo;    //�����ֵַ 

u8 Cutflag;
u8 BeepFlag;
u8 PKG;		   //��װ�ͺ�
u32 OK_COUNT;
u32 NG_COUNT;

u8 POP;      //������־
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

void initAddNo(void)	 //�жϵ�ֵַ�Ƿ�����
{
  u32 tmp;
  at24c02_rddat(0x10,(u8*)&tmp,4);
  if((tmp == 0xffffffff)||((tmp > 0xfffff))) //��ʼ״̬ || �ۼ������
  {
   AddNo = 0;
   at24c02_wrdat(0x10,(u8*)&AddNo,4);
  }
}
void oledInit(void)
{
	OLED_Init();  //��ʼ��OLED  
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
	  P1M0 &= ~0Xff; //��������	ADC����˿�����

	  P2M1 &= ~0XF8;
	  P2M0 |= 0XF8;	//����

	  P3M1 |= 0x04;
	  P3M1 &= ~0x04;  //	p32 ����

	  P5M1 &= ~0X20;
	  P5M0 |= 0X20;    //P55����
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
	NG_COUNT = 0; //��λ������
	DSENVMOS();		          // ��mos��Դ
	DSENABLE595();  		  // ��595����mos����
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
	u8 menucnt;
	u8 Eraseflag;
	u8 YesNo;
	YesNo = 0;
	Eraseflag = 0;    //ɾ���ڴ�Ķ���ȷ��
	menucnt = 0;
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
					  	    OLED_ShowString(2,0,"Do you want clear all memorry?",12,1);	
							OLED_ShowString(70,20,"YES",12,1);
							OLED_ShowString(110,20,"NO",12,0);				 
		 	                OLED_Refresh_Gram();						  
						  }
						  				    
					  }
/*					  else if(menucnt ==1)	 
					  {
						  memcpy(OLED_GRAM_TMP,OLED_GRAM,512);
				          init_windows(20,0,80,32,"Notice"," VT62538-B2",0);
				          OLED_Draw_WindowsDraw(&windemo);
						  POP = 1;
		 	              OLED_Refresh_Gram();					    
					  }
					  else if(menucnt ==2
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
				  MODE = testmode;
				  Done = 1;
				}
		  break;
		  case PR_OK:
		  		Key_change=0; 
		        BeepFlag =1;
				if(InMode)
				{
				  if(menucnt ==3)	 //����
				  {
				    OLED_Clear();	 //���� 
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
					  PKG = menucnt; //ע�������ͺ���Ҫ�ж��Ƿ��Ӧ
					  OK_COUNT = 0;
					  NG_COUNT = 0;
					  CutRun(PKG);	  
				      OLED_Clear();	  //�˳��˸������
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
#include "main.h"
#include "isr.h"
#include "key.h"
#include "uart.h"
#include "_595.h"
#include "oled.h"

//#define GET_FREQ
#ifdef GET_FREQ
u16 OscFreq;
#endif
#define D_TIMER0   40        //40us
#define SYNCMAX    750		 //30ms---->3000/4=750
#define SYNCMIN    125		 //5ms---->500/4=125
#define D_HIGHMAX  30    	 //970us ---->970/40 = 24 
#define D_IR_BIT_NUMBER   24	//◊∞‘ÿŒª¬Î ˝

u16 Decodecnt;
u32 ReIRcode;   //∂¡µΩµƒµÿ÷∑¬

/****************************************************************************************
//		   |````|
//SYNC: ___|	|_____________________________________________________________...
//          4clk                         124clk
//
//
//	     |````|			   |			  |````````````|	|		1clk = 8 osclk
//0:   __|    |____________|        1:  __|			   |____|
//        4clk    12clk                     12clk     4clk
//Datalength:
//Minlength:1.31ms @300k/13V    Maxlength:3.01ms @430k/4v
//bit:[0:19] addcode    bit:[20:23] keycode
****************************************************************************************/
void EnDecode(void)
{
   EX0 = 1;                 //ø™int0÷–∂œ
   Decodecnt = 0;			//∏¥Œªº∆ ˝∆˜
   ReIRcode = 0;
   Timer0_Run();	   		//ø™∂® ±∆˜
   Timer0_InterruptEnable();//ø™∂® ±∆˜1÷–∂œ
   ENVSOC();
  //ø™÷–∂œ ø™∂® ±∆˜ ≤∂ªÒIRcode
}
void DsDecode(void)
{
   EX0 = 0;
   Timer0_Stop();
   Decodecnt = 0;			//∏¥Œªº∆ ˝∆˜
   ReIRcode = 0;
   DSENVSOC();
   K1_OUT_LOW();
  //πÿ÷–∂œ πÿ∂® ±∆˜ Ω⁄ °◊ ‘¥
}
void Timer0Init(void)		//40us@11.0592MHz	//”√¿¥Ω‚¬Î
{
	Timer0_Stop();			//πÿ∂® ±∆˜0
	AUXR |= 0x80;		  	//∂® ±∆˜ ±÷”1Tƒ£ Ω
	TMOD &= 0xF0;			//…Ë÷√∂® ±∆˜ƒ£ Ω
	TL0 = 0x8B;				//…Ë÷√∂® ±≥ı÷µ
	TH0 = 0xFC;				//…Ë÷√∂® ±≥ı÷µ
	TF0 = 0;				//«Â≥˝TF0±Í÷æ
}

void Timer1Init(void)		//5∫¡√Î@22.1184MHz	 ”√¿¥…®√Ë∞¥º¸µ»
{
	Timer1_Stop();			//πÿ∂® ±∆˜1
	AUXR &= ~0x40;			//∂® ±∆˜ ±÷”1Tƒ£ Ω
	TMOD &= 0x0F;			//…Ë÷√∂® ±∆˜ƒ£ Ω
	TL1 = 0x00;				//…Ë÷√∂® ±≥ı÷µ
	TH1 = 0xDC;				//…Ë÷√∂® ±≥ı÷µ
	TF1 = 0;				//«Â≥˝TF1±Í÷æ
	Timer1_Run();			//ø™∂® ±∆˜1
}
void Int0Init(void)
{
    //≈‰÷√int0 ‰»Îƒ£ Ω  P3.2    XXXX,XHXX
	P3M1 |= 0x04;
	P3M0 &= ~0x04;
    INT0 = 0;
	IT0 = 0;                // 1£∫œ¬Ωµ—ÿ÷–∂œ    0£∫‘ –Ì…œ…˝—ÿ∫Õœ¬Ωµ—ÿ÷–∂œ
}

void Int3Init(void)
{
    //≈‰÷√int0 ‰»Îƒ£ Ω  P3.2    XXXX,XHXX
	P3M1 |= 0x80;
	P3M0 &= ~0x80;
    INT_CLKO |= 0x20;               //(EX3 = 1) πƒ‹INT3÷–∂œ
}

void timerInit(void)
{
	Timer0Init();          //º∆ ˝÷–∂œ
	Timer1Init();		   //º∆ ˝÷–∂œ
	Timer1_InterruptEnable();//ø™∂® ±∆˜1÷–∂œ
	Int0Init();
    Int3Init();
//	EnDecode();
}

void INT3_isr() interrupt INT3_VECTOR  //Õ‚≤ø÷–∂œ3 ”√¿¥ºÏ≤‚∆ º–≈∫≈
{
/////////////////////////////////////////////////////////
   if(Cutflag == 3)
   {
	  Cutflag = 2; 
   }
////////////////////////////////////////////////////////
}
void INT0_isr() interrupt INT0_VECTOR  //Õ‚≤ø÷–∂œ0 ”√¿¥ºÏ≤‚“£øÿ¬Î
{
    static u16 CountL , CountH;
	static u8 IR_BitCnt;	
	static u8 sync = 0;
	static u8 step = 0;
	static u32 IR_tmp=0;
	if(!sync)
	{
		if(INT0) //…œ…˝—ÿ
		{
          if(step==0)  //∆ ºŒª÷√£¨µ⁄“ª∏ˆ…œ…˝—ÿ∏¥Œªº∆ ˝∆˜
		  {
			   	Decodecnt = 0;
				step = 1;	   //step = 1;
		  }
		  else if(step==2)     //ºÏ≤‚µΩ¡Àµ⁄∂˛∏ˆ…œ…˝—ÿ£¨ø…“‘ªÒ»°µÕµÁ∆Ω≥÷–¯ ±º‰
		  {		  
			   CountL = Decodecnt;	 //ªÒ»°µΩµÕµÁ∆Ωµƒ ±º‰
			   Decodecnt = 0;
			   step = 0;
			   if(CountL > CountH)   //œ»≈–∂œµÕµÁ∆Ω ±º‰¥Û”⁄∏ﬂµÁ∆Ω ±º‰
			   {
			      if((CountL >= SYNCMIN)&&(CountL <= SYNCMAX))//ΩÿªÒÕ¨≤Ω¬Î
				  {
				    sync = 1;	
					IR_BitCnt = D_IR_BIT_NUMBER;	//◊∞‘ÿŒª¬Î ˝
#ifdef GET_FREQ
				    OscFreq =  1000/(CountL*40/124/8);       //kHz
#endif
                   return;
				  }

			   }

		  }

		} 
		if(!INT0) //œ¬Ωµ—ÿ
		{
		  if(step == 1)	     //Àµ√˜“—ºÏ≤‚µΩ¡À“ª∏ˆ…œ…˝—ÿ
		  {
		      
			  CountH = Decodecnt;	//ªÒ»°µΩ∏ﬂµÁ∆Ωµƒ ±º‰
			  Decodecnt = 0;
			  step = 2;
		  }
		  else			    //µ⁄“ª¥ŒºÏ≤‚µƒ «œ¬Ωµ—ÿ£¨∏¥Œªº∆ ˝∆˜£¨≤Ω ˝∏¥Œª
		  {		      
			  Decodecnt = 0;
			  step = 0;		  
		  }
		}
	}
	else //“—Õ¨≤Ω£¨∂¡»° ˝æ›
	{
//	  	SPEAKER=!SPEAKER;
	    if(!INT0) //œ¬Ωµ—ÿ
	    {
           CountH = Decodecnt;	 //ªÒ»°∏ﬂµÁ∆Ω ±º‰
		   Decodecnt = 0;
		   if(CountH > D_HIGHMAX) // ˝æ›≥¨ ±
		   sync = 0;
	    }
	    else if(INT0) //…œ…˝—ÿ
	    {
           CountL = Decodecnt;	 //ªÒ»°µÕµÁ∆Ω ±º‰
		   Decodecnt = 0;
		   IR_tmp <<= 1; //Ω” ’1bit
		   if(CountL < CountH)
		   {
		   	 IR_tmp |= 0X01;
		   } 
		   if(--IR_BitCnt == 0)				//≈–∂œΩ” ’ «∑ÒÕÍ≥…
		   {
		     ReIRcode = IR_tmp;
			 IR_tmp = 0;
			 sync= 0;                       //«Â≥˝Õ¨≤Ω¬Î		    
		   }		   		   	           
	    }
	}
		                 
}


void T0_isr() interrupt TIMER0_VECTOR //∂® ±∆˜0÷–∂œ	40us
{
   Decodecnt++;               
}


void T1_isr() interrupt TIMER1_VECTOR  //∂® ±∆˜1÷–∂œ”√¿¥◊ˆ µ ± ¬º˛¥¶¿Ì	5ms…®√Ë“ª¥Œ
{
	static u8 cnt = 0;
	static u8 keycnt=0,beepcnt=0;
	static u16 popcnt=0,cutcnt=0;
	keycnt++;
	cnt++;
	if(cnt >= 100)//∞Î√Î
	{
	  cnt =0;
	  LED0=!LED0;
	} 
	if(keycnt>=4) 
	{
	 KeyScan();
	 keycnt = 0;
	}
	if(BeepFlag == 1)
	{
	  SPEAKER = 1;
	  if(beepcnt++>=5)
	  {
	    SPEAKER = 0;
		beepcnt = 0;
		BeepFlag = 0;
	  }
	}
	if(Cutflag ==1)
	{
	  if(cutcnt++>=7)	   //40ms 
	  {
		DSENVMOS();		          // πÿmosµÁ‘¥
		DSENABLE595();  		  // πÿ595øÿ÷∆mosø™πÿ
		Cutflag = 0;
		cutcnt =0;
	  }
	} 
	if(POP != 0) 
	{
	   if(popcnt++>=400*POP)
	   {
	     popcnt = 0;
		 POP = 0;
         memcpy(OLED_GRAM,OLED_GRAM_TMP,512);
	   	 OLED_Refresh_Gram();
		 DsDecode();
	   }
	}           
}

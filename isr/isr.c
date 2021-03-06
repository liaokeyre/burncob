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
#define D_IR_BIT_NUMBER   24	//装载位码数

u16 Decodecnt;
u32 ReIRcode;   //读到的地址�

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
   EX0 = 1;                 //开int0中断
   Decodecnt = 0;			//复位计数器
   ReIRcode = 0;
   Timer0_Run();	   		//开定时器
   Timer0_InterruptEnable();//开定时器1中断
   ENVSOC();
  //开中断 开定时器 捕获IRcode
}
void DsDecode(void)
{
   EX0 = 0;
   Timer0_Stop();
   Decodecnt = 0;			//复位计数器
   ReIRcode = 0;
   DSENVSOC();
   K1_OUT_LOW();
  //关中断 关定时器 节省资源
}
void Timer0Init(void)		//40us@11.0592MHz	//用来解码
{
	Timer0_Stop();			//关定时器0
	AUXR |= 0x80;		  	//定时器时钟1T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x8B;				//设置定时初值
	TH0 = 0xFC;				//设置定时初值
	TF0 = 0;				//清除TF0标志
}

void Timer1Init(void)		//5毫秒@22.1184MHz	 用来扫描按键等
{
	Timer1_Stop();			//关定时器1
	AUXR &= ~0x40;			//定时器时钟1T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0x00;				//设置定时初值
	TH1 = 0xDC;				//设置定时初值
	TF1 = 0;				//清除TF1标志
	Timer1_Run();			//开定时器1
}
void Int0Init(void)
{
    //配置int0输入模式  P3.2    XXXX,XHXX
	P3M1 |= 0x04;
	P3M0 &= ~0x04;
    INT0 = 0;
	IT0 = 0;                // 1：下降沿中断    0：允许上升沿和下降沿中断
}

void Int3Init(void)
{
    //配置int0输入模式  P3.2    XXXX,XHXX
	P3M1 |= 0x80;
	P3M0 &= ~0x80;
    INT_CLKO |= 0x20;               //(EX3 = 1)使能INT3中断
}

void timerInit(void)
{
	Timer0Init();          //计数中断
	Timer1Init();		   //计数中断
	Timer1_InterruptEnable();//开定时器1中断
	Int0Init();
    Int3Init();
//	EnDecode();
}

void INT3_isr() interrupt INT3_VECTOR  //外部中断3 用来检测起始信号
{
/////////////////////////////////////////////////////////
   if(Cutflag == 3)
   {
	  Cutflag = 2; 
   }
////////////////////////////////////////////////////////
}
void INT0_isr() interrupt INT0_VECTOR  //外部中断0 用来检测遥控码
{
    static u16 CountL , CountH;
	static u8 IR_BitCnt;	
	static u8 sync = 0;
	static u8 step = 0;
	static u32 IR_tmp=0;
	if(!sync)
	{
		if(INT0) //上升沿
		{
          if(step==0)  //起始位置，第一个上升沿复位计数器
		  {
			   	Decodecnt = 0;
				step = 1;	   //step = 1;
		  }
		  else if(step==2)     //检测到了第二个上升沿，可以获取低电平持续时间
		  {		  
			   CountL = Decodecnt;	 //获取到低电平的时间
			   Decodecnt = 0;
			   step = 0;
			   if(CountL > CountH)   //先判断低电平时间大于高电平时间
			   {
			      if((CountL >= SYNCMIN)&&(CountL <= SYNCMAX))//截获同步码
				  {
				    sync = 1;	
					IR_BitCnt = D_IR_BIT_NUMBER;	//装载位码数
#ifdef GET_FREQ
				    OscFreq =  1000/(CountL*40/124/8);       //kHz
#endif
                   return;
				  }

			   }

		  }

		} 
		if(!INT0) //下降沿
		{
		  if(step == 1)	     //说明已检测到了一个上升沿
		  {
		      
			  CountH = Decodecnt;	//获取到高电平的时间
			  Decodecnt = 0;
			  step = 2;
		  }
		  else			    //第一次检测的是下降沿，复位计数器，步数复位
		  {		      
			  Decodecnt = 0;
			  step = 0;		  
		  }
		}
	}
	else //已同步，读取数据
	{
//	  	SPEAKER=!SPEAKER;
	    if(!INT0) //下降沿
	    {
           CountH = Decodecnt;	 //获取高电平时间
		   Decodecnt = 0;
		   if(CountH > D_HIGHMAX) //数据超时
		   sync = 0;
	    }
	    else if(INT0) //上升沿
	    {
           CountL = Decodecnt;	 //获取低电平时间
		   Decodecnt = 0;
		   IR_tmp <<= 1; //接收1bit
		   if(CountL < CountH)
		   {
		   	 IR_tmp |= 0X01;
		   } 
		   if(--IR_BitCnt == 0)				//判断接收是否完成
		   {
		     ReIRcode = IR_tmp;
			 IR_tmp = 0;
			 sync= 0;                       //清除同步码		    
		   }		   		   	           
	    }
	}
		                 
}


void T0_isr() interrupt TIMER0_VECTOR //定时器0中断	40us
{
   Decodecnt++;               
}


void T1_isr() interrupt TIMER1_VECTOR  //定时器1中断用来做实时事件处理	5ms扫描一次
{
	static u8 cnt = 0;
	static u8 keycnt=0,beepcnt=0;
	static u16 popcnt=0,cutcnt=0;
	keycnt++;
	cnt++;
	if(cnt >= 100)//半秒
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
		DSENVMOS();		          // 关mos电源
		DSENABLE595();  		  // 关595控制mos开关
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

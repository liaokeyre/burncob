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
#define D_IR_BIT_NUMBER   24	//װ��λ����

u16 Decodecnt;
u32 ReIRcode;   //�����ĵ�ַ�

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
   EX0 = 1;                 //��int0�ж�
   Decodecnt = 0;			//��λ������
   ReIRcode = 0;
   Timer0_Run();	   		//����ʱ��
   Timer0_InterruptEnable();//����ʱ��1�ж�
   ENVSOC();
  //���ж� ����ʱ�� ����IRcode
}
void DsDecode(void)
{
   EX0 = 0;
   Timer0_Stop();
   Decodecnt = 0;			//��λ������
   ReIRcode = 0;
   DSENVSOC();
   K1_OUT_LOW();
  //���ж� �ض�ʱ�� ��ʡ��Դ
}
void Timer0Init(void)		//40us@11.0592MHz	//��������
{
	Timer0_Stop();			//�ض�ʱ��0
	AUXR |= 0x80;		  	//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TL0 = 0x8B;				//���ö�ʱ��ֵ
	TH0 = 0xFC;				//���ö�ʱ��ֵ
	TF0 = 0;				//���TF0��־
}

void Timer1Init(void)		//5����@22.1184MHz	 ����ɨ�谴����
{
	Timer1_Stop();			//�ض�ʱ��1
	AUXR &= ~0x40;			//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;			//���ö�ʱ��ģʽ
	TL1 = 0x00;				//���ö�ʱ��ֵ
	TH1 = 0xDC;				//���ö�ʱ��ֵ
	TF1 = 0;				//���TF1��־
	Timer1_Run();			//����ʱ��1
}
void Int0Init(void)
{
    //����int0����ģʽ  P3.2    XXXX,XHXX
	P3M1 |= 0x04;
	P3M0 &= ~0x04;
    INT0 = 0;
	IT0 = 0;                // 1���½����ж�    0�����������غ��½����ж�
}

void Int3Init(void)
{
    //����int0����ģʽ  P3.2    XXXX,XHXX
	P3M1 |= 0x80;
	P3M0 &= ~0x80;
    INT_CLKO |= 0x20;               //(EX3 = 1)ʹ��INT3�ж�
}

void timerInit(void)
{
	Timer0Init();          //�����ж�
	Timer1Init();		   //�����ж�
	Timer1_InterruptEnable();//����ʱ��1�ж�
	Int0Init();
    Int3Init();
//	EnDecode();
}

void INT3_isr() interrupt INT3_VECTOR  //�ⲿ�ж�3 ���������ʼ�ź�
{
/////////////////////////////////////////////////////////
   if(Cutflag == 3)
   {
	  Cutflag = 2; 
   }
////////////////////////////////////////////////////////
}
void INT0_isr() interrupt INT0_VECTOR  //�ⲿ�ж�0 �������ң����
{
    static u16 CountL , CountH;
	static u8 IR_BitCnt;	
	static u8 sync = 0;
	static u8 step = 0;
	static u32 IR_tmp=0;
	if(!sync)
	{
		if(INT0) //������
		{
          if(step==0)  //��ʼλ�ã���һ�������ظ�λ������
		  {
			   	Decodecnt = 0;
				step = 1;	   //step = 1;
		  }
		  else if(step==2)     //��⵽�˵ڶ��������أ����Ի�ȡ�͵�ƽ����ʱ��
		  {		  
			   CountL = Decodecnt;	 //��ȡ���͵�ƽ��ʱ��
			   Decodecnt = 0;
			   step = 0;
			   if(CountL > CountH)   //���жϵ͵�ƽʱ����ڸߵ�ƽʱ��
			   {
			      if((CountL >= SYNCMIN)&&(CountL <= SYNCMAX))//�ػ�ͬ����
				  {
				    sync = 1;	
					IR_BitCnt = D_IR_BIT_NUMBER;	//װ��λ����
#ifdef GET_FREQ
				    OscFreq =  1000/(CountL*40/124/8);       //kHz
#endif
                   return;
				  }

			   }

		  }

		} 
		if(!INT0) //�½���
		{
		  if(step == 1)	     //˵���Ѽ�⵽��һ��������
		  {
		      
			  CountH = Decodecnt;	//��ȡ���ߵ�ƽ��ʱ��
			  Decodecnt = 0;
			  step = 2;
		  }
		  else			    //��һ�μ������½��أ���λ��������������λ
		  {		      
			  Decodecnt = 0;
			  step = 0;		  
		  }
		}
	}
	else //��ͬ������ȡ����
	{
//	  	SPEAKER=!SPEAKER;
	    if(!INT0) //�½���
	    {
           CountH = Decodecnt;	 //��ȡ�ߵ�ƽʱ��
		   Decodecnt = 0;
		   if(CountH > D_HIGHMAX) //���ݳ�ʱ
		   sync = 0;
	    }
	    else if(INT0) //������
	    {
           CountL = Decodecnt;	 //��ȡ�͵�ƽʱ��
		   Decodecnt = 0;
		   IR_tmp <<= 1; //����1bit
		   if(CountL < CountH)
		   {
		   	 IR_tmp |= 0X01;
		   } 
		   if(--IR_BitCnt == 0)				//�жϽ����Ƿ����
		   {
		     ReIRcode = IR_tmp;
			 IR_tmp = 0;
			 sync= 0;                       //���ͬ����		    
		   }		   		   	           
	    }
	}
		                 
}


void T0_isr() interrupt TIMER0_VECTOR //��ʱ��0�ж�	40us
{
   Decodecnt++;               
}


void T1_isr() interrupt TIMER1_VECTOR  //��ʱ��1�ж�������ʵʱ�¼�����	5msɨ��һ��
{
	static u8 cnt = 0;
	static u8 keycnt=0,beepcnt=0;
	static u16 popcnt=0,cutcnt=0;
	keycnt++;
	cnt++;
	if(cnt >= 100)//����
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
		DSENVMOS();		          // ��mos��Դ
		DSENABLE595();  		  // ��595����mos����
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

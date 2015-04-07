#include "main.h"
//#include "isr.h"

#define GET_FREQ
#ifdef GET_FREQ
u16 OscFreq;
#endif
#define D_TIMER0   40        //40us
#define SYNCMAX    750		 //30ms---->3000/4=750
#define SYNCMIN    125		 //5ms---->500/4=125
#define D_HIGHMAX  30    	 //970us ---->970/40 = 24 
#define D_IR_BIT_NUMBER   24	//װ��λ����

u16 Decodecnt;
u32 ReIRcode;   //�����ĵ�ַ��
void Timer0Init(void)		//40us@11.0592MHz	//��������
{
	Timer0_Stop();			//�ض�ʱ��0
	AUXR |= 0x80;		  	//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TL0 = 0x46;				//���ö�ʱ��ֵ
	TH0 = 0xFE;				//���ö�ʱ��ֵ
	TF0 = 0;				//���TF0��־
}

void Timer1Init(void)		//5����@11.0592MHz	 ����ɨ�谴����
{
	Timer1_Stop();			//�ض�ʱ��1
	AUXR |= 0x40;			//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;			//���ö�ʱ��ģʽ
	TL1 = 0x00;				//���ö�ʱ��ֵ
	TH1 = 0x28;				//���ö�ʱ��ֵ
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

void timerInit(void)
{
	Timer0Init();          //�������ж�
	Timer1Init();		   //�����ж�
	Timer1_InterruptEnable();//����ʱ��1�ж�
}
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
   Timer1_InterruptEnable();//����ʱ��1�ж�

  //���ж� ����ʱ�� ����IRcode
}
void DsDecode(void)
{
   EX0 = 0;
   Timer0_Stop();
   Decodecnt = 0;			//��λ������
   ReIRcode = 0;
  //���ж� �ض�ʱ�� ��ʡ��Դ
}

void INT0_isr() interrupt INT0_VECTOR  //�ⲿ�ж�0 �������ң����
{
    static u16 CountL , CountH;
	static u8 IR_BitCnt;	
	static u8 sync = 0;
	static u8 step = 0;
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
		   ReIRcode >>= 1; //����1bit
		   if(CountL < CountH)
		   {
		   	 ReIRcode |= 0x00800000;
		   } 
		   if(--IR_BitCnt == 0)				//�жϽ����Ƿ����
		   {
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
	static u8 cnt;
	cnt++;
	if(cnt >= 100)//����
	{
	  LED0^=LED0;
	}                  
}

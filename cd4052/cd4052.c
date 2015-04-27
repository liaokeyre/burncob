#include "main.h"
#include "cd4052.h"
#include "uart.h"
#include "_595.h"
#include "oled.h"
#include "24c02.h"
#include "key.h"
#include "isr.h"

CobPad PN;	  //20�ߵ�ַ��
CobPad PN_TMP;//��ʱ��Ų�ͬ��װ��ַ��
CobPad PN_CUT;//��������ĵ�ַ���˵�ַ����ʵ���ճ����ģ�����Ԥ���룻��Ҫ��Ԥ����Ա�
Pad_Res RES;
extern WINDOWS windemo;
void PN_Mirro(void)
{
  u32 tmp0,tmp2;
  u8 i;
  tmp0 = 0;
  tmp2 = 0;
  i = 0;
  for(i=0;i<24;i++)
  {
    tmp0 = (PN.PadByte>>i)&0x01;	   //��λ
	tmp2 |= (tmp0 << (23-i)) ;
  }
  PN.PadByte = tmp2;
}
void InitSign(void)
{
  BUS_6 = 0;//�������λ 
  BUS_5 = 0;//�������λ
  BUS_4 = 0;//����ڶ�λ
  BUS_3 = 0;//�����һλ
  BUS_2 = 0;//�ɹ�ʧ�ܱ�־��
  BUS_1 = 1;//��������
}
void SignOut(u8 status,u32 xornum)
{
//  u8 tmp,i;
//  tmp = 0;
//  for(i=0;i<24;i++)	//ת�ź���
//  {
//    if((xornum>>i)&0x01)
//	{
//	  tmp+=i;
//	}
//  }
  BUS_2 = status; //�ɹ�ʧ�ܱ�־��
  BUS_1 = 0; //�ɹ�ʧ�ܱ�־
//  BUS_3 = tmp&0x01;//�����һλ
//  BUS_4 = (tmp>>1)&0x01;//����ڶ�λ
//  BUS_5 = (tmp>>2)&0x01;//�������λ
//  BUS_6 = (tmp>>3)&0x01;//�������λ
}
/********************************************
�������ܣ� ADC��ʼ��
���� �� void
��ע  ����
*********************************************/
void InitADC(void)
{
    P1ASF = 0xff;                   //����P1��ΪAD��
    ADC_RES = 0;                    //�������Ĵ���
	ADC_RESL = 0; 
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
}

/********************************************
�������ܣ� ��ȡADCת��ֵ
���� �� ADCֵ��10λADC
��ע  ����
*********************************************/
u16 GetADCResult(u8 Chanel)
{
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | Chanel | ADC_START;
    _nop_();                        //�ȴ�4��NOP
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//�ȴ�ADCת�����
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC
    return ((ADC_RES<<2)+ADC_RESL);  //����ADC���  10bit
}
/******************************************************************************************
ADֵת��Ϊ����ֵ
R2=AD*R1/(1024-AD)	  R1 = 1M = 1000K
���ص���PAD�ŶԵ���ֵ��С����λΪKŷ
******************************************************************************************/
u16 Adc2Res(u16 adValue)
{
  return (((u32)adValue*1000/(1024-adValue)));
}
/*******************************************************************************************
ADCͨ���б���Ӧ��ͬPAD��
//A1		0		  0		   1		 1
//B1		0		  1		   0		 1
//A2		0		  0		   1		 1
//B2		0		  1		   0		 1
//--------------------------------------------------------------------------------------------
//AD0	   PAD0		 PAD1	  PAD2		PAD3
//AD1	   PAD4		 PAD5	  PAD6		PAD7
//AD2	   PAD8		 PAD9	  PAD10		PAD11
//AD3	   PAD12	 PAD13	  PAD14		PAD15
//-------------------------------------------------------------------------------------------
//AD4	   PAD16
//AD5	   PAD17
//AD6	   PAD18
//AD7	   PAD19
*******************************************************************************************/
void GetPadRes(void)
{
  u8 i;
  A1_4052 = 0;
  A2_4052 = 0;
  B1_4052 = 0;
  B2_4052 = 0;
  delay_10ms();
  delay_10ms();
  delay_10ms();	  //һ��Ҫ��ʱ�ȴ�4052�����л�ok
  RES.Pad0 = Adc2Res(GetADCResult(CH0));
  NOP4();
  RES.Pad4 = Adc2Res(GetADCResult(CH1));
//  printf_u8(RES.Pad4>>8);
//  printf_u8(RES.Pad4);
  NOP4();
  RES.Pad8 = Adc2Res(GetADCResult(CH2));
  NOP4();
  RES.Pad12 = Adc2Res(GetADCResult(CH3));
  NOP4();

  RES.Pad16 = Adc2Res(GetADCResult(CH4));
//  printf_u8(RES.Pad16>>8);
//  printf_u8(RES.Pad16);
  NOP4();
  RES.Pad17 = Adc2Res(GetADCResult(CH5));
  NOP4();
  RES.Pad18 = Adc2Res(GetADCResult(CH6));
  NOP4();
  RES.Pad19 = Adc2Res(GetADCResult(CH7));

//i=200;
//while(i--)
{
  A1_4052 = 0;
  A2_4052 = 0;
  B1_4052 = 1;
  B2_4052 = 1;
  delay_10ms();
  delay_10ms();
  delay_10ms();	  //һ��Ҫ��ʱ�ȴ�4052�����л�ok
  RES.Pad1 = Adc2Res(GetADCResult(CH0));
//  printf_u8(RES.Pad1>>8);
//  printf_u8(RES.Pad1);
  NOP4();
  RES.Pad5 = Adc2Res(GetADCResult(CH1));
  NOP4();
//  printf_u8(RES.Pad5>>8);
//  printf_u8(RES.Pad5);
  RES.Pad9 = Adc2Res(GetADCResult(CH2));
  NOP4();
  RES.Pad13 = Adc2Res(GetADCResult(CH3));
}
  A1_4052 = 1;
  A2_4052 = 1;
  B1_4052 = 0;
  B2_4052 = 0;
  delay_10ms();
  delay_10ms();
  delay_10ms();	  //һ��Ҫ��ʱ�ȴ�4052�����л�ok
  RES.Pad2 = Adc2Res(GetADCResult(CH0));
  NOP4();
  RES.Pad6 = Adc2Res(GetADCResult(CH1));
  NOP4();
  RES.Pad10 = Adc2Res(GetADCResult(CH2));
  NOP4();
  RES.Pad14 = Adc2Res(GetADCResult(CH3));

  A1_4052 = 1;
  A2_4052 = 1;
  B1_4052 = 1;
  B2_4052 = 1;
  delay_10ms();
  delay_10ms();
  delay_10ms();	  //һ��Ҫ��ʱ�ȴ�4052�����л�ok
  RES.Pad3 = Adc2Res(GetADCResult(CH0));
  NOP4();
  RES.Pad7 = Adc2Res(GetADCResult(CH1));
  NOP4();
  RES.Pad11 = Adc2Res(GetADCResult(CH2));
//  printf_u8(RES.Pad11>>8);
//  printf_u8(RES.Pad11);
  NOP4();
  RES.Pad15 = Adc2Res(GetADCResult(CH3));
}
/*****************************************************************************************
���ܣ�����ͬ��װ��ʱ��ַ��ת��Ϊ������оƬ��ַ��

******************************************************************************************/
void PnMask(u8 package)
{
  PN.PadByte = 0;   //��λ��ַ��,û����״̬
  if(package == Package_2)					 //��װ1
  {
 //    PN.Px.P_XXH.Pad0 = PN_TMP.Px.P_XXH.Pad0;
     PN.Px.P_XXH.Pad1 = PN_TMP.Px.P_XXH.Pad0;
     PN.Px.P_XXH.Pad3 = PN_TMP.Px.P_XXH.Pad1;
     PN.Px.P_XXH.Pad5 = PN_TMP.Px.P_XXH.Pad2;
     
     PN.Px.P_XXH.Pad7= PN_TMP.Px.P_XXH.Pad3;
     PN.Px.P_XHX.Pad1= PN_TMP.Px.P_XXH.Pad4;
     PN.Px.P_XHX.Pad3= PN_TMP.Px.P_XXH.Pad5;
     PN.Px.P_XHX.Pad4 = PN_TMP.Px.P_XXH.Pad6; 
/*
     PN.Px.P_XHX.Pad7 = PN_TMP.Px.P_XHX.Pad0;
     
     PN.Px.P_HXX.Pad1 = PN_TMP.Px.P_XHX.Pad1;
     PN.Px.P_HXX.Pad2 = PN_TMP.Px.P_XHX.Pad2;
     PN.Px.P_HXX.Pad3 = PN_TMP.Px.P_XHX.Pad3; 
*/	 		 	 
  }
  else if(package == Package_1)		            //��װ2
  {
     PN.Px.P_XXH.Pad0 = PN_TMP.Px.P_XXH.Pad0;
     PN.Px.P_XXH.Pad2 = PN_TMP.Px.P_XXH.Pad1;
     PN.Px.P_XXH.Pad4 = PN_TMP.Px.P_XXH.Pad2;
     PN.Px.P_XXH.Pad6 = PN_TMP.Px.P_XXH.Pad3;
     
     PN.Px.P_XHX.Pad0 = PN_TMP.Px.P_XXH.Pad4;
     PN.Px.P_XHX.Pad2 = PN_TMP.Px.P_XXH.Pad5;
     PN.Px.P_XHX.Pad3 = PN_TMP.Px.P_XXH.Pad6;
     PN.Px.P_XHX.Pad4 = PN_TMP.Px.P_XXH.Pad7;  
  }
  else
  {
   PN.PadByte = PN_TMP.PadByte;	              //�޴�λֱ��ת��
  }
}
/************************************************************************************************
ֻ�ж�Ҫ������PAD
�������� 1���ɹ�    ��   0��ʧ��
************************************************************************************************/
bool BurnCheck(void)
{
  bool ret;
  ret = 1;
  PN_CUT.PadByte = 0;                   //��λ�����

  if(PN_TMP.Px.P_XXH.Pad0== 1)
  {
    if(RES.Pad0 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XXH.Pad0= 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                      //��Ҫֱ�ӷ�����
	}
  }

  if(PN_TMP.Px.P_XXH.Pad1 == 1)
  {
    if(RES.Pad1 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XXH.Pad1= 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN_TMP.Px.P_XXH.Pad2== 1)
  {
    if(RES.Pad2 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XXH.Pad2 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN_TMP.Px.P_XXH.Pad3 == 1)
  {
    if(RES.Pad3 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XXH.Pad3 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN_TMP.Px.P_XXH.Pad4 == 1)
  {
    if(RES.Pad4 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XXH.Pad4 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN_TMP.Px.P_XXH.Pad5 == 1)
  {
    if(RES.Pad5 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XXH.Pad5 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN_TMP.Px.P_XXH.Pad6 == 1)
  {
    if(RES.Pad6 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XXH.Pad6 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN_TMP.Px.P_XXH.Pad7 == 1)
  {
    if(RES.Pad7 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XXH.Pad7 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN_TMP.Px.P_XHX.Pad0== 1)
  {
    if(RES.Pad8 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XHX.Pad0 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN_TMP.Px.P_XHX.Pad1 == 1)
  {
    if(RES.Pad9 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XHX.Pad1 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN_TMP.Px.P_XHX.Pad2 == 1)
  {
    if(RES.Pad10 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XHX.Pad2 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN_TMP.Px.P_XHX.Pad3 == 1)
  {
    if(RES.Pad11 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XHX.Pad3 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN_TMP.Px.P_XHX.Pad4 == 1)
  {
    if(RES.Pad12 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XHX.Pad4 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN_TMP.Px.P_XHX.Pad5 == 1)
  {
    if(RES.Pad13 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XHX.Pad5 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN_TMP.Px.P_XHX.Pad6 == 1)
  {
    if(RES.Pad14 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XHX.Pad6 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN_TMP.Px.P_XHX.Pad7 == 1)
  {
    if(RES.Pad15 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XHX.Pad7 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN_TMP.Px.P_HXX.Pad0== 1)
  {
    if(RES.Pad16 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_HXX.Pad0 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN_TMP.Px.P_HXX.Pad1 == 1)
  {
    if(RES.Pad17 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_HXX.Pad1 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN_TMP.Px.P_HXX.Pad2 == 1)
  {
    if(RES.Pad18 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_HXX.Pad2 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN_TMP.Px.P_HXX.Pad3 == 1)
  {
    if(RES.Pad19 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_HXX.Pad3 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }
  OLED_ShowAdd(0,0,PN_TMP.PadByte,PN_CUT.PadByte);
  OLED_Refresh_Gram();
  return   ret;
}

/*
bool BurnCheck(void)
{
  bool ret;
  ret = 1;
  PN_CUT.PadByte = 0;                   //��λ�����

  if(PN.Px.P_XXH.Pad0== 1)
  {
    if(RES.Pad0 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XXH.Pad0= 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                      //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.P_XXH.Pad1 == 1)
  {
    if(RES.Pad1 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XXH.Pad1= 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.P_XXH.Pad2== 1)
  {
    if(RES.Pad2 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XXH.Pad2 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.P_XXH.Pad3 == 1)
  {
    if(RES.Pad3 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XXH.Pad3 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.P_XXH.Pad4 == 1)
  {
    if(RES.Pad4 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XXH.Pad4 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.P_XXH.Pad5 == 1)
  {
    if(RES.Pad5 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XXH.Pad5 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.P_XXH.Pad6 == 1)
  {
    if(RES.Pad6 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XXH.Pad6 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.P_XXH.Pad7 == 1)
  {
    if(RES.Pad7 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XXH.Pad7 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.P_XHX.Pad0== 1)
  {
    if(RES.Pad8 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XHX.Pad0 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.P_XHX.Pad1 == 1)
  {
    if(RES.Pad9 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XHX.Pad1 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.P_XHX.Pad2 == 1)
  {
    if(RES.Pad10 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XHX.Pad2 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.P_XHX.Pad3 == 1)
  {
    if(RES.Pad11 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XHX.Pad3 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.P_XHX.Pad4 == 1)
  {
    if(RES.Pad12 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XHX.Pad4 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.P_XHX.Pad5 == 1)
  {
    if(RES.Pad13 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XHX.Pad5 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.P_XHX.Pad6 == 1)
  {
    if(RES.Pad14 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XHX.Pad6 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.P_XHX.Pad7 == 1)
  {
    if(RES.Pad15 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_XHX.Pad7 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.P_HXX.Pad0== 1)
  {
    if(RES.Pad16 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_HXX.Pad0 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.P_HXX.Pad1 == 1)
  {
    if(RES.Pad17 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_HXX.Pad1 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.P_HXX.Pad2 == 1)
  {
    if(RES.Pad18 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_HXX.Pad2 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.P_HXX.Pad3 == 1)
  {
    if(RES.Pad19 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.P_HXX.Pad3 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }
  OLED_ShowAdd(0,0,PN.PadByte,PN_CUT.PadByte);
  OLED_Refresh_Gram();
  return   ret;
}
 */
u8 Hex2Dat(u8 dat)
{
   u8 dat0;
   if(dat<0x0a)
   {
       dat0 = dat+'0';
   }
   else
   {
       dat0 = dat+'A'-10;
   }
   return dat0;
}  
/************************************************************************************************
�ȶ������ַ
************************************************************************************************/
void Decode(u8 status)
{
    u32 IRtmp[3];
	u32 Xornum;
	u8 i,j;
	ReIRcode = 0;
    EnDecode();
    delay_10ms(); 
    delay_10ms(); 
	K1_OUT = 1;
    delay_10ms(); 
    delay_10ms(); 
    delay_10ms(); 
    delay_10ms(); 
    delay_10ms(); 
    delay_10ms(); 
    delay_10ms(); 
 
    delay_10ms(); 
    delay_10ms(); 
    delay_10ms(); 
    delay_10ms(); 

	memcpy(OLED_GRAM_TMP,OLED_GRAM,512);
	init_windows(0,0,120,32,"Code No.:",0,0);
	OLED_Draw_WindowsDraw(&windemo);
	i = 200;
	j = 0;	
    while(i--)
	{
		if((ReIRcode & 0x0f) != 0)
		{
		  IRtmp[j++] = ReIRcode;
		  if(j>2) 
		  { 
		    j = 0;
		    if(IRtmp[1]==IRtmp[2])
		    break;
		  }
		}

	}
    OLED_ShowChar(22,16,Hex2Dat(((IRtmp[1] >>16)>>4)&0x0f),12,1);
    OLED_ShowChar(30,16,Hex2Dat(((IRtmp[1]>>16)&0x0f)),12,1);

	OLED_ShowChar(42,16,Hex2Dat(((IRtmp[1] >>8)>>4)&0x0f),12,1);
	OLED_ShowChar(50,16,Hex2Dat(((IRtmp[1] >>8)&0x0f)),12,1);

	OLED_ShowChar(62,16,Hex2Dat(((IRtmp[1])>>4)&0x0f),12,1);
 	OLED_ShowChar(70,16,Hex2Dat(((IRtmp[1])&0x0f)),12,1);

	POP = 1;
	OLED_Refresh_Gram();
	PN_Mirro();
	Xornum = (PN.PadByte |0x4)^IRtmp[1];
	SignOut(status,Xornum);

//  printf_u8((PN.PadByte )>>16);
//  printf_u8((PN.PadByte)>>8);
//  printf_u8(PN.PadByte );
//  put_char('\n');
//  printf_u8(((PN.PadByte )|0x4)>>16);
//  printf_u8(((PN.PadByte )|0x4)>>8);
//  printf_u8(((PN.PadByte )|0x4));
//  put_char('\n');
//  printf_u8(IRtmp>>16);
//  printf_u8(IRtmp>>8);
//  printf_u8(IRtmp);
//  put_char('\n');
//  printf_u8(Xornum>>16);
//  printf_u8(Xornum>>8);
//  printf_u8(Xornum);
//  put_char('\n');
}

void compareAdd(void)
{
  u32 tmp;
  u32 countmp;
  u8 status;
  if(BurnCheck() == 0)  //�ж����������ʧ��
  {
    status = NG;
	tmp = PN.PadByte^PN_CUT.PadByte;  //tmp ����쳣��ַ�� �����жϳ�����һλ�����쳣
    OLED_ShowChar(0,8,Hex2Dat(((tmp>>16)>>4)&0x0f),7,1);
    OLED_ShowChar(6,8,Hex2Dat(((tmp>>16)&0x0f)),7,1);

	OLED_ShowChar(12,8,Hex2Dat(((tmp>>8)>>4)&0x0f),7,1);
	OLED_ShowChar(18,8,Hex2Dat(((tmp>>8)&0x0f)),7,1);

	OLED_ShowChar(24,8,Hex2Dat(((tmp)>>4)&0x0f),7,1);
 	OLED_ShowChar(30,8,Hex2Dat(((tmp)&0x0f)),7,1);
    OLED_ShowString(0,24," NG!! ",7,1);
    OLED_Refresh_Gram();
	   //�����������ʧ�ܲ��� ��ʾ ���źŲ��Ի�   ��ַ�벻��
/**********************************************************************************************/
	NG_COUNT++;
#ifdef EEPROM_EN
	if(PKG == Package_1)
	{
	  at24c02_rddat(COUNT1_NG_ADD,&countmp,4);
	  countmp++;
      at24c02_wrdat(COUNT1_NG_ADD,&countmp,4);    //�ۼ� NG����
	}
	else if(PKG == Package_2)
	{
	  at24c02_rddat(COUNT2_NG_ADD,&countmp,4);
	  countmp++;
      at24c02_wrdat(COUNT2_NG_ADD,&countmp,4);    //�ۼ� NG����
	}
	else if(PKG == Package_3)
	{
	  at24c02_rddat(COUNT3_NG_ADD,&countmp,4);
	  countmp ++;
      at24c02_wrdat(COUNT3_NG_ADD,&countmp,4);    //�ۼ� NG����
	}
	at24c02_rddat(COUNTALL_NG_ADD,&countmp,4);
	countmp ++;
	at24c02_wrdat(COUNTALL_NG_ADD,&countmp,4);    //�ۼ� NG����  
#endif
/***********************************************************************************************/
  }
  else
  {
    status = PASS;
    OLED_ShowString(0,8,"      ",7,1);
    OLED_ShowString(0,24,"PASS!!",7,1);
    OLED_Refresh_Gram();
	   //�����ж���ֵ�͵�ַ��ƥ��
	   //�����������ɹ����� ��ʾ  ���źŲ��Ի�   ��ַ���Լ�1
	PN_TMP.PadByte ++;
	if((PKG == Package_1)||(PKG == Package_2))
	{
	  if(PN_TMP.PadByte > 0xff)	   //2^8 ���ܳ�����ַ�ռ�
	  PN_TMP.PadByte = 0;
#ifdef EEPROM_EN
	  if(PKG == Package_1)
	  {
	    at24c02_wrdat(PKG1_ADD,&PN_TMP.PadByte,4); 			 //��������ַ��2402
	  }
	  else
	  {
	    at24c02_wrdat(PKG2_ADD,&PN_TMP.PadByte,4); 	  		 //��������ַ��2402
	  }
#endif
	}
	else if(PKG == Package_3)
	{
	   if(PN_TMP.PadByte >= MaxlineHex)	  //2^20  ���ܳ�����ַ�ռ�
	   PN_TMP.PadByte = 0;
#ifdef EEPROM_EN
	   at24c02_wrdat(PKG3_ADD,&PN_TMP.PadByte,4);                    //��������ַ��2402
#endif	   	  	
	}

/*****************************************************************************************/
	OK_COUNT++;
#ifdef EEPROM_EN
	if(PKG == Package_1)
	{
	  at24c02_rddat(COUNT1_OK_ADD,&countmp,4);
	  countmp ++;
      at24c02_wrdat(COUNT1_OK_ADD,&countmp,4);    //�ۼ� OK����
	}
	else if(PKG == Package_2)
	{
	  at24c02_rddat(COUNT2_OK_ADD,&countmp,4);
	  countmp ++;
      at24c02_wrdat(COUNT2_OK_ADD,&countmp,4);    //�ۼ� OK����
	}
	else if(PKG == Package_3)
	{
	  at24c02_rddat(COUNT3_OK_ADD,&countmp,4);
	  countmp ++;
      at24c02_wrdat(COUNT3_OK_ADD,&countmp,4);    //�ۼ� OK����
	}
	at24c02_rddat(COUNTALL_OK_ADD,&countmp,4);
	countmp ++;
	at24c02_wrdat(COUNTALL_OK_ADD,&countmp,4);    //�ۼ� OK����  
#endif
/*****************************************************************************************************/
  }
   Decode(status); 
}
/************************************************************************************************
����
************************************************************************************************/
void CobCuting(u32 Add)
{
	storge595(&Add,4);
	ENVMOS();		      // ��mos��Դ
	ENABLE595();  		  // ��595����mos����
	Cutflag = 1;
}

void StartCut(u8 package)
{
  DSENVSOC();
  PnMask(package);                //ת��
  CobCuting(PN_TMP.PadByte);      //��ʼ����
  while(Cutflag)			  //�ȴ�������� �ſ���ȥ��������ֵ
  NOP1();
  GetPadRes();                //��������ֵ
  OLED_Clear();
  compareAdd();               //�Ƚϵ�ַ
//  ENVSOC();
}
void ShowInfo(void)
{
    u32 tmp;

	memcpy(OLED_GRAM_TMP,OLED_GRAM,512);
	init_windows(0,0,120,32,"Info",0,0);
	OLED_Draw_WindowsDraw(&windemo);
 	tmp = Get_Pkg_CNT(PKG,0);
	if(PKG == Package_1)	 	OLED_ShowString(30,2,"VT62538-B1",7,1);   
	else if(PKG == Package_2)	 	OLED_ShowString(30,2,"VT62538-B2",7,1);
	else  OLED_ShowString(30,2,"COB",7,1);
	OLED_ShowString(2,11,"OK:",7,1);   //Disp ok!
    OLED_ShowChar(22,11,Hex2Dat(((tmp>>16)>>4)&0x0f),7,1);
    OLED_ShowChar(28,11,Hex2Dat(((tmp>>16)&0x0f)),7,1);

	OLED_ShowChar(34,11,Hex2Dat(((tmp>>8)>>4)&0x0f),7,1);
	OLED_ShowChar(40,11,Hex2Dat(((tmp>>8)&0x0f)),7,1);

	OLED_ShowChar(46,11,Hex2Dat(((tmp)>>4)&0x0f),7,1);
 	OLED_ShowChar(52,11,Hex2Dat(((tmp)&0x0f)),7,1);

 	tmp = Get_Pkg_CNT(PKG,1);
	OLED_ShowString(62,11,"NG:",7,1);   //Disp ok!
    OLED_ShowChar(82,11,Hex2Dat(((tmp>>16)>>4)&0x0f),7,1);
    OLED_ShowChar(88,11,Hex2Dat(((tmp>>16)&0x0f)),7,1);

	OLED_ShowChar(94,11,Hex2Dat(((tmp>>8)>>4)&0x0f),7,1);
	OLED_ShowChar(100,11,Hex2Dat(((tmp>>8)&0x0f)),7,1);

	OLED_ShowChar(106,11,Hex2Dat(((tmp)>>4)&0x0f),7,1);
 	OLED_ShowChar(112,11,Hex2Dat(((tmp)&0x0f)),7,1);

 	tmp = Get_Pkg_Add(PKG);
	OLED_ShowString(2,20,"ADD:",7,1);   //Disp ok!
    OLED_ShowChar(28,20,Hex2Dat(((tmp>>16)>>4)&0x0f),7,1);
    OLED_ShowChar(34,20,Hex2Dat(((tmp>>16)&0x0f)),7,1);

	OLED_ShowChar(40,20,Hex2Dat(((tmp>>8)>>4)&0x0f),7,1);
	OLED_ShowChar(46,20,Hex2Dat(((tmp>>8)&0x0f)),7,1);

	OLED_ShowChar(52,20,Hex2Dat(((tmp)>>4)&0x0f),7,1);
 	OLED_ShowChar(58,20,Hex2Dat(((tmp)&0x0f)),7,1);

	POP = 2;
	OLED_Refresh_Gram();	  
}
void CutRun(u8 package)
{
  u8 Done;
  Done = 0;
  //��ȡ��װ��Ϣ
#ifdef EEPROM_EN
    PN_TMP.PadByte = Get_Pkg_Add(package); //��ȡ����Ӧ��װ�����ַ
#endif
	OLED_ShowString(30,0,"SADD-No.:",12,1); 
    OLED_ShowChar(0,15,Hex2Dat(((PN_TMP.PadByte>>16)>>4)&0x0f),12,1);
    OLED_ShowChar(8,15,Hex2Dat(((PN_TMP.PadByte>>16)&0x0f)),12,1);

	OLED_ShowChar(16,15,Hex2Dat(((PN_TMP.PadByte>>8)>>4)&0x0f),12,1);
	OLED_ShowChar(24,15,Hex2Dat(((PN_TMP.PadByte>>8)&0x0f)),12,1);

	OLED_ShowChar(32,15,Hex2Dat(((PN_TMP.PadByte)>>4)&0x0f),12,1);
 	OLED_ShowChar(40,15,Hex2Dat(((PN_TMP.PadByte)&0x0f)),12,1);
    OLED_Refresh_Gram();
	memcpy(OLED_GRAM_TMP,OLED_GRAM,512);
  while(!Done)
  {
	  if((Key_back == PR_START)&&(Key_change))
	  {
	   Key_change=0; 
	   BeepFlag =1;	
	   InitSign();
#ifdef PRINT_INFO
    send_string ("\n��ʼ����--->\n");
#endif
       StartCut(package);
#ifdef PRINT_INFO
    send_string ("\n��ɸ���\n");
#endif
	  }
	  if((Key_back == PR_OK)&&(Key_change))
	  {
	   Key_change=0; 
	   BeepFlag =1;
	   ShowInfo();
	  }	 
	  if((Key_back == PR_MOD)&&(Key_change))
	  {
	   Key_change=0; 
	   BeepFlag =1;
	   Done = 1;
	  }	
	  if((Key_back == PR_SUB)&&(Key_change))
	  {
	   Key_change=0; 
	   BeepFlag =1;
	   PN_TMP.PadByte ++;
	   OLED_Clear();
       OLED_ShowAdd(0,0,PN_TMP.PadByte,PN_TMP.PadByte);
       OLED_Refresh_Gram();
	  }	
	  if((Key_back == PR_ADD)&&(Key_change))
	  {
	   Key_change=0; 
	   BeepFlag =1;
	   PN_TMP.PadByte --;
	   OLED_Clear();
       OLED_ShowAdd(0,0,PN_TMP.PadByte,PN_TMP.PadByte);
       OLED_Refresh_Gram();
	  }		  	     
  }
}
/*
void TestPn(void)
{
  static u8 Done = 0;
  PN_TMP.PadByte = 0;
  PN.PadByte = 0;
  send_string("\nstart mask: package type 1 , PACK_ADD = 0 , IC_ADD = 0\n\n");
//  PN_TMP.Px.Pad0 = 1;
//  PN.Px.Pad1 = 1;
//  printf_u8(PN_TMP.PadByte);
//  printf_u8(PN.PadByte);
//	send_string("   ===>masking...\n");
  while(!Done)
  {
	PnMask(0);
//	send_string("start mask: ");
//	send_string("PACK_ADD=:");
//	printf_u8(PN_TMP.PadByte>>24);
//	printf_u8(PN_TMP.PadByte>>16);
//	printf_u8(PN_TMP.PadByte>>8);
//	printf_u8(PN_TMP.PadByte);
//	send_string("   IC_ADD=:");
//	printf_u8(PN.PadByte>>24);
//	printf_u8(PN.PadByte>>16);
//	printf_u8(PN.PadByte>>8);
//	printf_u8(PN.PadByte);
//	send_string("   ===>masking...\n");
//	send_string("PACK_ADD.BIT0 = \n");
//	printf_u8(PN_TMP.Px.Pad0);
	send_string("\n");
	send_string("PN_TMP BIT 15:0\n");
	printf_u8(PN_TMP.Px.P_XHX.Pad7);
	printf_u8(PN_TMP.Px.P_XHX.Pad6);
	printf_u8(PN_TMP.Px.P_XHX.Pad5);
	printf_u8(PN_TMP.Px.P_XHX.Pad4);
	printf_u8(PN_TMP.Px.P_XHX.Pad3);
	printf_u8(PN_TMP.Px.P_XHX.Pad2);
	printf_u8(PN_TMP.Px.P_XHX.Pad1);
	printf_u8(PN_TMP.Px.P_XHX.Pad0);	
	
	printf_u8(PN_TMP.Px.P_XXH.Pad7);
	printf_u8(PN_TMP.Px.P_XXH.Pad6);
	printf_u8(PN_TMP.Px.P_XXH.Pad5);
	printf_u8(PN_TMP.Px.P_XXH.Pad4);
	printf_u8(PN_TMP.Px.P_XXH.Pad3);
	printf_u8(PN_TMP.Px.P_XXH.Pad2);
	printf_u8(PN_TMP.Px.P_XXH.Pad1);
	printf_u8(PN_TMP.Px.P_XXH.Pad0);
//	send_string("\n");
	send_string("PN_TMP BIT 15:0\n");
	printf_u8(PN.Px.P_XHX.Pad7);
	printf_u8(PN.Px.P_XHX.Pad6);
	printf_u8(PN.Px.P_XHX.Pad5);
	printf_u8(PN.Px.P_XHX.Pad4);
	printf_u8(PN.Px.P_XHX.Pad3);
	printf_u8(PN.Px.P_XHX.Pad2);
	printf_u8(PN.Px.P_XHX.Pad1);
	printf_u8(PN.Px.P_XHX.Pad0);	
	
	printf_u8(PN.Px.P_XXH.Pad7);
	printf_u8(PN.Px.P_XXH.Pad6);
	printf_u8(PN.Px.P_XXH.Pad5);
	printf_u8(PN.Px.P_XXH.Pad4);
	printf_u8(PN.Px.P_XXH.Pad3);
	printf_u8(PN.Px.P_XXH.Pad2);
	printf_u8(PN.Px.P_XXH.Pad1);
	printf_u8(PN.Px.P_XXH.Pad0);
	send_string("\n");		
	if(PN_TMP.PadByte ++ >=0xfff)
	Done = 1;
  }
	send_string("mask over!\n");
}
*/
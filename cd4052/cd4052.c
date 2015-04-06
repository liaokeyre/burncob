#include "main.h"
#include "cd4052.h"
#include "uart.h"

#define Res_OK  2000        //���õ����ж�ֵ2000K=2M
CobPad PN;	  //20�ߵ�ַ��
CobPad PN_TMP;//��ʱ��Ų�ͬ��װ��ַ��
CobPad PN_CUT;//��������ĵ�ַ���˵�ַ����ʵ���ճ����ģ�����Ԥ���룻��Ҫ��Ԥ����Ա�
Pad_Res RES;
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
  return ((adValue/(1024-adValue))*1000);
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
  A1_4052 = 0;
  A2_4052 = 0;
  B1_4052 = 0;
  B2_4052 = 0;
  NOP10();
  RES.Pad0 = Adc2Res(GetADCResult(CH0));
  NOP4();
  RES.Pad4 = Adc2Res(GetADCResult(CH1));
  NOP4();
  RES.Pad8 = Adc2Res(GetADCResult(CH2));
  NOP4();
  RES.Pad12 = Adc2Res(GetADCResult(CH3));

  NOP4();
  RES.Pad16 = Adc2Res(GetADCResult(CH4));
  NOP4();
  RES.Pad17 = Adc2Res(GetADCResult(CH5));
  NOP4();
  RES.Pad18 = Adc2Res(GetADCResult(CH6));
  NOP4();
  RES.Pad19 = Adc2Res(GetADCResult(CH7));

  A1_4052 = 0;
  A2_4052 = 0;
  B1_4052 = 1;
  B2_4052 = 1;
  NOP10();
  RES.Pad1 = Adc2Res(GetADCResult(CH0));
  NOP4();
  RES.Pad5 = Adc2Res(GetADCResult(CH1));
  NOP4();
  RES.Pad9 = Adc2Res(GetADCResult(CH2));
  NOP4();
  RES.Pad13 = Adc2Res(GetADCResult(CH3));

  A1_4052 = 1;
  A2_4052 = 1;
  B1_4052 = 0;
  B2_4052 = 0;
  NOP10();
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
  NOP10();
  RES.Pad3 = Adc2Res(GetADCResult(CH0));
  NOP4();
  RES.Pad7 = Adc2Res(GetADCResult(CH1));
  NOP4();
  RES.Pad11 = Adc2Res(GetADCResult(CH2));
  NOP4();
  RES.Pad15 = Adc2Res(GetADCResult(CH3));
}
/*****************************************************************************************
���ܣ�����ͬ��װ��ʱ��ַ��ת��Ϊ������оƬ��ַ��

******************************************************************************************/
void PnMask(u8 package)
{
  PN.PadByte = 0;   //��λ��ַ��,û����״̬
  if(package == 0)					 //��װ1
  {
     PN.Px.P_XXH.Pad0 = PN_TMP.Px.P_XXH.Pad0;
     PN.Px.P_XXH.Pad2 = PN_TMP.Px.P_XXH.Pad1;
     PN.Px.P_XXH.Pad4 = PN_TMP.Px.P_XXH.Pad2;
     PN.Px.P_XXH.Pad6 = PN_TMP.Px.P_XXH.Pad3;
     
     PN.Px.P_XHX.Pad0 = PN_TMP.Px.P_XXH.Pad4;
     PN.Px.P_XHX.Pad3 = PN_TMP.Px.P_XXH.Pad5;
     PN.Px.P_XHX.Pad5 = PN_TMP.Px.P_XXH.Pad6;
     PN.Px.P_XHX.Pad6 = PN_TMP.Px.P_XXH.Pad7; 
     PN.Px.P_XHX.Pad7 = PN_TMP.Px.P_XHX.Pad0;
     
     PN.Px.P_HXX.Pad1 = PN_TMP.Px.P_XHX.Pad1;
     PN.Px.P_HXX.Pad2 = PN_TMP.Px.P_XHX.Pad2;
     PN.Px.P_HXX.Pad3 = PN_TMP.Px.P_XHX.Pad3; 
     

//  	PN.Px.Pad0 = PN_TMP.Px.Pad0;
//  	PN.Px.Pad2 = PN_TMP.Px.Pad1;
//  	PN.Px.Pad4 = PN_TMP.Px.Pad2; 
//  	PN.Px.Pad6 = PN_TMP.Px.Pad3;
//  	PN.Px.Pad8 = PN_TMP.Px.Pad4;
//  	PN.Px.Pad10 = PN_TMP.Px.Pad5; 
//  	PN.Px.Pad13 = PN_TMP.Px.Pad6;
//  	PN.Px.Pad14 = PN_TMP.Px.Pad7;
//  	PN.Px.Pad15 = PN_TMP.Px.Pad8; 
//  	PN.Px.Pad17 = PN_TMP.Px.Pad9;
//  	PN.Px.Pad18 = PN_TMP.Px.Pad10;
//  	PN.Px.Pad19 = PN_TMP.Px.Pad11; 			 	 
  }
  else if(package == 1)		            //��װ2
  {
//  	PN.Px.Pad1 = PN_TMP.Px.Pad0;
//  	PN.Px.Pad3 = PN_TMP.Px.Pad1;
//  	PN.Px.Pad5 = PN_TMP.Px.Pad2; 
//  	PN.Px.Pad7 = PN_TMP.Px.Pad3;
//  	PN.Px.Pad9 = PN_TMP.Px.Pad4;
//  	PN.Px.Pad11 = PN_TMP.Px.Pad5; 
//  	PN.Px.Pad12 = PN_TMP.Px.Pad6;
//  	PN.Px.Pad16 = PN_TMP.Px.Pad7;
//  	PN.Px.Pad17 = PN_TMP.Px.Pad8; 
//  	PN.Px.Pad18 = PN_TMP.Px.Pad9;
//  	PN.Px.Pad19 = PN_TMP.Px.Pad10;  
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
/*
  if(PN.Px.Pad0 == 0)
  {
    if(RES.Pad0 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.Pad0 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                      //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.Pad1 == 0)
  {
    if(RES.Pad1 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.Pad1 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.Pad2 == 0)
  {
    if(RES.Pad2 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.Pad2 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.Pad3 == 0)
  {
    if(RES.Pad3 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.Pad3 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.Pad4 == 0)
  {
    if(RES.Pad4 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.Pad4 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.Pad5 == 0)
  {
    if(RES.Pad5 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.Pad5 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.Pad6 == 0)
  {
    if(RES.Pad6 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.Pad6 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.Pad7 == 0)
  {
    if(RES.Pad7 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.Pad7 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.Pad8 == 0)
  {
    if(RES.Pad8 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.Pad8 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.Pad9 == 0)
  {
    if(RES.Pad9 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.Pad9 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.Pad10 == 0)
  {
    if(RES.Pad10 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.Pad10 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.Pad11 == 0)
  {
    if(RES.Pad11 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.Pad11 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.Pad12 == 0)
  {
    if(RES.Pad12 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.Pad12 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.Pad13 == 0)
  {
    if(RES.Pad13 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.Pad13 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.Pad14 == 0)
  {
    if(RES.Pad14 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.Pad14 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.Pad15 == 0)
  {
    if(RES.Pad15 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.Pad15 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.Pad16 == 0)
  {
    if(RES.Pad16 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.Pad16 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.Pad17 == 0)
  {
    if(RES.Pad17 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.Pad17 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.Pad18 == 0)
  {
    if(RES.Pad18 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.Pad18 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }

  if(PN.Px.Pad19 == 0)
  {
    if(RES.Pad19 >= Res_OK)                    //����ɹ�
    {
     PN_CUT.Px.Pad19 = 1;
	}
	else                                     //����ʧ��
	{
     ret = 0;                                       //��Ҫֱ�ӷ�����
	}
  }
  */
  return   ret;
}
/************************************************************************************************
�ȶ������ַ
************************************************************************************************/
void compareAdd(void)
{
  u32 tmp;
  if(BurnCheck() == 0)  //�ж����������ʧ��
  {
	tmp = PN.PadByte^PN_CUT.PadByte;  //tmp ����쳣��ַ�� �����жϳ�����һλ�����쳣

	   //�����������ʧ�ܲ��� ��ʾ ���źŲ��Ի�   ��ַ�벻��

  }
  else
  {
	   //�����ж���ֵ�͵�ַ��ƥ��
	   //�����������ɹ����� ��ʾ  ���źŲ��Ի�   ��ַ���Լ�1

  }
}
void TestPn(void)
{
  u8 i;
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
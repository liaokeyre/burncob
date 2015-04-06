#include "main.h"
#include "cd4052.h"
#include "uart.h"

#define Res_OK  2000        //设置电阻判定值2000K=2M
CobPad PN;	  //20线地址码
CobPad PN_TMP;//临时存放不同封装地址码
CobPad PN_CUT;//存放烧码后的地址，此地址码是实际烧出来的，不是预设码；需要和预设码对比
Pad_Res RES;
/********************************************
函数功能： ADC初始化
返回 ： void
备注  ：无
*********************************************/
void InitADC(void)
{
    P1ASF = 0xff;                   //设置P1口为AD口
    ADC_RES = 0;                    //清除结果寄存器
	ADC_RESL = 0; 
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
}

/********************************************
函数功能： 获取ADC转换值
返回 ： ADC值，10位ADC
备注  ：无
*********************************************/
u16 GetADCResult(u8 Chanel)
{
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | Chanel | ADC_START;
    _nop_();                        //等待4个NOP
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//等待ADC转换完成
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC
    return ((ADC_RES<<2)+ADC_RESL);  //返回ADC结果  10bit
}
/******************************************************************************************
AD值转换为电阻值
R2=AD*R1/(1024-AD)	  R1 = 1M = 1000K
返回的是PAD脚对地阻值大小，单位为K欧
******************************************************************************************/
u16 Adc2Res(u16 adValue)
{
  return ((adValue/(1024-adValue))*1000);
}
/*******************************************************************************************
ADC通道列表，对应不同PAD脚
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
功能：将不同封装临时地址码转换为真正的芯片地址码

******************************************************************************************/
void PnMask(u8 package)
{
  PN.PadByte = 0;   //复位地址码,没割码状态
  if(package == 0)					 //封装1
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
  else if(package == 1)		            //封装2
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
   PN.PadByte = PN_TMP.PadByte;	              //无错位直接转换
  }
}
/************************************************************************************************
只判断要求被烧码PAD
烧码结果： 1：成功    ；   0：失败
************************************************************************************************/
bool BurnCheck(void)
{
  bool ret;
  ret = 1;
  PN_CUT.PadByte = 0;                   //复位检测码
/*
  if(PN.Px.Pad0 == 0)
  {
    if(RES.Pad0 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.Pad0 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                      //需要直接返回吗
	}
  }

  if(PN.Px.Pad1 == 0)
  {
    if(RES.Pad1 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.Pad1 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.Pad2 == 0)
  {
    if(RES.Pad2 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.Pad2 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.Pad3 == 0)
  {
    if(RES.Pad3 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.Pad3 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.Pad4 == 0)
  {
    if(RES.Pad4 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.Pad4 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.Pad5 == 0)
  {
    if(RES.Pad5 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.Pad5 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.Pad6 == 0)
  {
    if(RES.Pad6 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.Pad6 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.Pad7 == 0)
  {
    if(RES.Pad7 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.Pad7 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.Pad8 == 0)
  {
    if(RES.Pad8 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.Pad8 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.Pad9 == 0)
  {
    if(RES.Pad9 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.Pad9 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.Pad10 == 0)
  {
    if(RES.Pad10 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.Pad10 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.Pad11 == 0)
  {
    if(RES.Pad11 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.Pad11 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.Pad12 == 0)
  {
    if(RES.Pad12 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.Pad12 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.Pad13 == 0)
  {
    if(RES.Pad13 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.Pad13 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.Pad14 == 0)
  {
    if(RES.Pad14 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.Pad14 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.Pad15 == 0)
  {
    if(RES.Pad15 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.Pad15 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.Pad16 == 0)
  {
    if(RES.Pad16 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.Pad16 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.Pad17 == 0)
  {
    if(RES.Pad17 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.Pad17 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.Pad18 == 0)
  {
    if(RES.Pad18 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.Pad18 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.Pad19 == 0)
  {
    if(RES.Pad19 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.Pad19 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }
  */
  return   ret;
}
/************************************************************************************************
比对烧码地址
************************************************************************************************/
void compareAdd(void)
{
  u32 tmp;
  if(BurnCheck() == 0)  //判断烧码脚烧码失败
  {
	tmp = PN.PadByte^PN_CUT.PadByte;  //tmp 存放异常地址码 可以判断出在哪一位出现异常

	   //这里添加烧码失败操作 显示 给信号测试机   地址码不变

  }
  else
  {
	   //解码判断码值和地址码匹配
	   //这里添加烧码成功操作 显示  给信号测试机   地址码自加1

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
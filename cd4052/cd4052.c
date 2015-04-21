#include "main.h"
#include "cd4052.h"
#include "uart.h"
#include "_595.h"
#include "oled.h"

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
  return (((u32)adValue*1000/(1024-adValue)));
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
  u8 i;
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
//  printf_u8(RES.Pad16>>8);
//  printf_u8(RES.Pad16);
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
//  printf_u8(RES.Pad1>>8);
//  printf_u8(RES.Pad1);
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
  printf_u8(RES.Pad11>>8);
  printf_u8(RES.Pad11);
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
  else if(package == 1)		            //封装2
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

  if(PN.Px.P_XXH.Pad0== 1)
  {
    if(RES.Pad0 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.P_XXH.Pad0= 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                      //需要直接返回吗
	}
  }

  if(PN.Px.P_XXH.Pad1 == 1)
  {
    if(RES.Pad1 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.P_XXH.Pad1= 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.P_XXH.Pad2== 1)
  {
    if(RES.Pad2 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.P_XXH.Pad2 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.P_XXH.Pad3 == 1)
  {
    if(RES.Pad3 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.P_XXH.Pad3 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.P_XXH.Pad4 == 1)
  {
    if(RES.Pad4 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.P_XXH.Pad4 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.P_XXH.Pad5 == 1)
  {
    if(RES.Pad5 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.P_XXH.Pad5 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.P_XXH.Pad6 == 1)
  {
    if(RES.Pad6 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.P_XXH.Pad6 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.P_XXH.Pad7 == 1)
  {
    if(RES.Pad7 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.P_XXH.Pad7 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.P_XHX.Pad0== 1)
  {
    if(RES.Pad8 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.P_XHX.Pad0 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.P_XHX.Pad1 == 1)
  {
    if(RES.Pad9 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.P_XHX.Pad1 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.P_XHX.Pad2 == 1)
  {
    if(RES.Pad10 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.P_XHX.Pad2 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.P_XHX.Pad3 == 1)
  {
    if(RES.Pad11 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.P_XHX.Pad3 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.P_XHX.Pad4 == 1)
  {
    if(RES.Pad12 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.P_XHX.Pad4 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.P_XHX.Pad5 == 1)
  {
    if(RES.Pad13 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.P_XHX.Pad5 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.P_XHX.Pad6 == 1)
  {
    if(RES.Pad14 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.P_XHX.Pad6 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.P_XHX.Pad7 == 1)
  {
    if(RES.Pad15 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.P_XHX.Pad7 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.P_HXX.Pad0== 1)
  {
    if(RES.Pad16 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.P_HXX.Pad0 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.P_HXX.Pad1 == 1)
  {
    if(RES.Pad17 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.P_HXX.Pad1 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.P_HXX.Pad2 == 1)
  {
    if(RES.Pad18 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.P_HXX.Pad2 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }

  if(PN.Px.P_HXX.Pad3 == 1)
  {
    if(RES.Pad19 >= Res_OK)                    //烧码成功
    {
     PN_CUT.Px.P_HXX.Pad3 = 1;
	}
	else                                     //烧码失败
	{
     ret = 0;                                       //需要直接返回吗
	}
  }
  OLED_ShowAdd(0,0,PN.PadByte,PN_CUT.PadByte);
  OLED_Refresh_Gram();
  return   ret;
}

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
比对烧码地址
************************************************************************************************/
void compareAdd(void)
{
  u32 tmp;
  if(BurnCheck() == 0)  //判断烧码脚烧码失败
  {
	tmp = PN.PadByte^PN_CUT.PadByte;  //tmp 存放异常地址码 可以判断出在哪一位出现异常
    OLED_ShowChar(0,8,Hex2Dat(((tmp>>16)>>4)&0x0f),7,1);
    OLED_ShowChar(6,8,Hex2Dat(((tmp>>16)&0x0f)),7,1);

	OLED_ShowChar(12,8,Hex2Dat(((tmp>>8)>>4)&0x0f),7,1);
	OLED_ShowChar(18,8,Hex2Dat(((tmp>>8)&0x0f)),7,1);

	OLED_ShowChar(24,8,Hex2Dat(((tmp)>>4)&0x0f),7,1);
 	OLED_ShowChar(30,8,Hex2Dat(((tmp)&0x0f)),7,1);
    OLED_ShowString(0,24," NG!! ",7);
    OLED_Refresh_Gram();
	   //这里添加烧码失败操作 显示 给信号测试机   地址码不变
  }
  else
  {
    OLED_ShowString(0,8,"      ",7);
    OLED_ShowString(0,24,"PASS!!",7);
    OLED_Refresh_Gram();
	   //解码判断码值和地址码匹配
	   //这里添加烧码成功操作 显示  给信号测试机   地址码自加1
//	     PN_TMP.PadByte ++;
  }
   PN_TMP.PadByte ++;
}
/************************************************************************************************
烧码
************************************************************************************************/
void CobCuting(u32 Add)
{
	storge595(&Add,4);
	ENVMOS();		      // 开mos电源
	ENABLE595();  		  // 开595控制mos开关
	Cutflag = 1;
}

void StartCut(u8 package)
{
  PnMask(package);            //转码
  CobCuting(PN.PadByte);      //开始割码
  while(Cutflag)			  //等待割码完成 才可以去读引脚阻值
  NOP1();
//  GetPadRes();                //读引脚阻值
  compareAdd();               //比较地址
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
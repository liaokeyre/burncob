#ifndef __MAIN_H__
#define __MAIN_H__
#include "STC15xxxxxx.H"
#include "TYPEDEF.H"

/*******************************************************/
#define I2C_SDA         P26
#define I2C_SCL         P27
/*******************************************************///EEPROM I2C DEFINE
#define EN_595          P20		   //OE
#define SH_595          P21		   //SHcp
#define ST_595          P22		   //STcp
#define DS_595          P23		   //Data 
#define ENABLE595()   	EN_595 = 0
#define DSENABLE595()   EN_595 = 1
/*******************************************************///74HC595 IO DEFINE
#define A1_4052          P11		   
#define B1_4052          P10
#define A2_4052          P46		   
#define B2_4052          P45		   
/*******************************************************///CD4052 IO DEFINE

//#define MF522_NSS   P12 //RC500CS
//#define MF522_SCK   P15	//RC500CLK
//#define MF522_SI    P14	//RC500SI
//#define MF522_SO    P13	//RC500SO
//#define MF522_RST   P04	//RC500RST
//#define MF522_EA    P03	//RC500EA
//
///*******************************************************///RC522 SPI IO DEFINE
#define OLED_CS   P37  //片选
#define OLED_RST  P36  //复位
#define OLED_DC   P34  //数据/命令控制
#define OLED_SCL  P33  //时钟 D0（SCLK)
#define OLED_SDIN P32  //D1（MOSI） 数据
/*******************************************************///OLED SPI IO DEFINE
#define SPEAKER   P02
#define LED0      P24
#define VMOS_EN   P25
/*******************************************************///LED SPK IO DEFINE
#define OLED_CS_Clr()  OLED_CS=0
#define OLED_CS_Set()  OLED_CS=1

#define OLED_RST_Clr() OLED_RST=0
#define OLED_RST_Set() OLED_RST=1

#define OLED_DC_Clr() OLED_DC=0
#define OLED_DC_Set() OLED_DC=1

#define OLED_SCLK_Clr() OLED_SCL=0
#define OLED_SCLK_Set() OLED_SCL=1

#define OLED_SDIN_Clr() OLED_SDIN=0
#define OLED_SDIN_Set() OLED_SDIN=1  

/****************位域定义*********************///注意大小端
/*
typedef struct PadNo
{
   u8        : 8 ;
   u8        : 4 ;
   u8   Pad19 : 1 ,
        Pad18 : 1 ,
		Pad17 : 1 ,
		Pad16 : 1 ,
		Pad15 : 1 ,
		Pad14 : 1 ,
		Pad13 : 1 ,
		Pad12 : 1 ,
        Pad11 : 1 ,
		Pad10: 1 ,
		Pad9 : 1 ,
		Pad8 : 1 ,
		Pad7 : 1 ,
		Pad6 : 1 ,
		Pad5 : 1 ,
        Pad4 : 1 ,
		Pad3 : 1 ,
		Pad2 : 1 ,
		Pad1 : 1 ,
		Pad0 : 1 ;	  //20bit
}Pad;
*/
typedef struct PadBit
{
  u8    	Pad0 : 1 ;
  u8	    Pad1 : 1 ;
  u8		Pad2 : 1 ;
  u8		Pad3 : 1 ;
  u8		Pad4 : 1 ;
  u8		Pad5 : 1 ;
  u8		Pad6 : 1 ;
  u8		Pad7 : 1 ;	  //8bit
}PB;

typedef struct PadNo
{
  u8 NC;
  PB P_HXX ;
  PB P_XHX ;
  PB P_XXH ;
//  PB P_XXH ;
//  PB P_XHX ;
//  PB P_HXX ;
//  u8 NC;
}Pad;

typedef union	 //联合体
{
 Pad Px;
 u32 PadByte;
}CobPad;

typedef struct PadRes
{
  u16 Pad0;
  u16 Pad1;
  u16 Pad2;
  u16 Pad3;
  u16 Pad4;
  u16 Pad5;
  u16 Pad6;
  u16 Pad7;
  u16 Pad8;
  u16 Pad9;
  u16 Pad10;
  u16 Pad11;
  u16 Pad12;
  u16 Pad13;
  u16 Pad14;
  u16 Pad15;
  u16 Pad16;
  u16 Pad17;
  u16 Pad18;
  u16 Pad19;
}Pad_Res;

enum ADC_CH
{
  CH0,
  CH1,
  CH2,
  CH3,
  CH4,
  CH5,
  CH6,
  CH7
};
enum IC_PAC
{
  Package_1,
  Package_2
};
extern u32 AddNo;  //割码地址值            
#endif
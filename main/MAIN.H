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
#define A1_4052          P01		   
#define B1_4052          P00
#define A2_4052          P46		   
#define B2_4052          P45		   
/*******************************************************///CD4052 IO DEFINE
#define BUS_1            P34
#define BUS_2            P35
#define BUS_3            P36
#define BUS_4            P37
#define BUS_5            P40
#define BUS_6            P47
#define START            P33
/*******************************************************/// TEST MACHINE BUS
#define IR_IN            P32	//检测遥控码
#define VMOS_EN          P25	//控制割码电源
#define K1_OUT           P55	//控制模拟测试芯片按键1被按下
#define EN_VSOC          P54    //控制给测试芯片供电
/*******************************************************/// TESTER CONTROL
#define KEY1             P41	//按键1
#define KEY2             P42	//按键2
#define KEY3             P43	//按键3
#define KEY4             P44	//按键4
/*******************************************************/// IO KEY DEFINE
#define OLED_CS  		 P37    //片选
#define OLED_RST 		 P36    //复位
#define OLED_DC   	  	 P34    //数据/命令控制
#define OLED_SCL  		 P33    //时钟 D0（SCLK)
#define OLED_SDIN 		 P32    //D1（MOSI） 数据
/*******************************************************///OLED SPI IO DEFINE
#define SPEAKER  		 P02	//异常报警蜂鸣器
#define LED0      		 P22//P24	//工作指示灯
/*******************************************************///LED SPK IO DEFINE  <---------------------TEST LED
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

typedef struct PadNo  //结构体
{
  u8 NC;
  PB P_HXX ;
  PB P_XHX ;
  PB P_XXH ;
}Pad;

typedef union	 //联合体
{
 Pad Px;
 u32 PadByte;
}CobPad;

typedef struct PadRes  //结构体，连续的存储单元来存储阻值大小
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

enum ADC_CH	  //枚举0-7CH
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
enum IC_PAC	 //枚举 2封装
{
  Package_1,
  Package_2
};
extern u32 AddNo;  //割码地址值            
#endif
#ifndef __OLEDFONT_H
#define __OLEDFONT_H 	   
//����ASCII��
//ƫ����32
//ASCII�ַ���
//ƫ����32
//��С:12*6
/************************************6*8�ĵ���************************************/
const unsigned char code F7x5[][6] =		
{
 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00} // 20
,{0x00, 0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
,{0x00, 0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
,{0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
,{0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
,{0x00, 0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
,{0x00, 0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
,{0x00, 0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
,{0x00, 0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
,{0x00, 0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
,{0x00, 0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
,{0x00, 0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
,{0x00, 0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
,{0x00, 0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
,{0x00, 0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
,{0x00, 0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
,{0x00, 0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
,{0x00, 0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
,{0x00, 0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
,{0x00, 0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
,{0x00, 0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
,{0x00, 0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
,{0x00, 0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
,{0x00, 0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
,{0x00, 0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
,{0x00, 0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
,{0x00, 0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
,{0x00, 0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
,{0x00, 0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
,{0x00, 0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
,{0x00, 0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
,{0x00, 0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
,{0x00, 0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
,{0x00, 0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
,{0x00, 0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
,{0x00, 0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
,{0x00, 0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
,{0x00, 0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
,{0x00, 0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
,{0x00, 0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
,{0x00, 0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
,{0x00, 0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
,{0x00, 0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
,{0x00, 0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
,{0x00, 0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
,{0x00, 0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
,{0x00, 0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
,{0x00, 0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
,{0x00, 0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
,{0x00, 0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
,{0x00, 0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
,{0x00, 0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
,{0x00, 0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
,{0x00, 0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
,{0x00, 0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
,{0x00, 0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
,{0x00, 0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
,{0x00, 0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
,{0x00, 0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
,{0x00, 0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
,{0x00, 0x02, 0x04, 0x08, 0x10, 0x20} // 5c \ //
,{0x00, 0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
,{0x00, 0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
,{0x00, 0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
,{0x00, 0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
,{0x00, 0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
,{0x00, 0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
,{0x00, 0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
,{0x00, 0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
,{0x00, 0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
,{0x00, 0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
,{0x00, 0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
,{0x00, 0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
,{0x00, 0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
,{0x00, 0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
,{0x00, 0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
,{0x00, 0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
,{0x00, 0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
,{0x00, 0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
,{0x00, 0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
,{0x00, 0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
,{0x00, 0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
,{0x00, 0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
,{0x00, 0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
,{0x00, 0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
,{0x00, 0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
,{0x00, 0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
,{0x00, 0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
,{0x00, 0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
,{0x00, 0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
,{0x00, 0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
,{0x00, 0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
,{0x00, 0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
,{0x00, 0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
,{0x00, 0x10, 0x08, 0x08, 0x10, 0x08} // 7e ~
};
/************************************12*6�ĵ���************************************/
const unsigned char code asc2_1206[95][12]={
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",0*/
{0x00,0x00,0x00,0x00,0x3F,0x40,0x00,0x00,0x00,0x00,0x00,0x00},/*"!",1*/
{0x00,0x00,0x30,0x00,0x40,0x00,0x30,0x00,0x40,0x00,0x00,0x00},/*""",2*/
{0x09,0x00,0x0B,0xC0,0x3D,0x00,0x0B,0xC0,0x3D,0x00,0x09,0x00},/*"#",3*/
{0x18,0xC0,0x24,0x40,0x7F,0xE0,0x22,0x40,0x31,0x80,0x00,0x00},/*"$",4*/
{0x18,0x00,0x24,0xC0,0x1B,0x00,0x0D,0x80,0x32,0x40,0x01,0x80},/*"%",5*/
{0x03,0x80,0x1C,0x40,0x27,0x40,0x1C,0x80,0x07,0x40,0x00,0x40},/*"&",6*/
{0x10,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"'",7*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x80,0x20,0x40,0x40,0x20},/*"(",8*/
{0x00,0x00,0x40,0x20,0x20,0x40,0x1F,0x80,0x00,0x00,0x00,0x00},/*")",9*/
{0x09,0x00,0x06,0x00,0x1F,0x80,0x06,0x00,0x09,0x00,0x00,0x00},/*"*",10*/
{0x04,0x00,0x04,0x00,0x3F,0x80,0x04,0x00,0x04,0x00,0x00,0x00},/*"+",11*/
{0x00,0x10,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*",",12*/
{0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0x00,0x00},/*"-",13*/
{0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*".",14*/
{0x00,0x20,0x01,0xC0,0x06,0x00,0x38,0x00,0x40,0x00,0x00,0x00},/*"/",15*/
{0x1F,0x80,0x20,0x40,0x20,0x40,0x20,0x40,0x1F,0x80,0x00,0x00},/*"0",16*/
{0x00,0x00,0x10,0x40,0x3F,0xC0,0x00,0x40,0x00,0x00,0x00,0x00},/*"1",17*/
{0x18,0xC0,0x21,0x40,0x22,0x40,0x24,0x40,0x18,0x40,0x00,0x00},/*"2",18*/
{0x10,0x80,0x20,0x40,0x24,0x40,0x24,0x40,0x1B,0x80,0x00,0x00},/*"3",19*/
{0x02,0x00,0x0D,0x00,0x11,0x00,0x3F,0xC0,0x01,0x40,0x00,0x00},/*"4",20*/
{0x3C,0x80,0x24,0x40,0x24,0x40,0x24,0x40,0x23,0x80,0x00,0x00},/*"5",21*/
{0x1F,0x80,0x24,0x40,0x24,0x40,0x34,0x40,0x03,0x80,0x00,0x00},/*"6",22*/
{0x30,0x00,0x20,0x00,0x27,0xC0,0x38,0x00,0x20,0x00,0x00,0x00},/*"7",23*/
{0x1B,0x80,0x24,0x40,0x24,0x40,0x24,0x40,0x1B,0x80,0x00,0x00},/*"8",24*/
{0x1C,0x00,0x22,0xC0,0x22,0x40,0x22,0x40,0x1F,0x80,0x00,0x00},/*"9",25*/
{0x00,0x00,0x00,0x00,0x08,0x40,0x00,0x00,0x00,0x00,0x00,0x00},/*":",26*/
{0x00,0x00,0x00,0x00,0x04,0x60,0x00,0x00,0x00,0x00,0x00,0x00},/*";",27*/
{0x00,0x00,0x04,0x00,0x0A,0x00,0x11,0x00,0x20,0x80,0x40,0x40},/*"<",28*/
{0x09,0x00,0x09,0x00,0x09,0x00,0x09,0x00,0x09,0x00,0x00,0x00},/*"=",29*/
{0x00,0x00,0x40,0x40,0x20,0x80,0x11,0x00,0x0A,0x00,0x04,0x00},/*">",30*/
{0x18,0x00,0x20,0x00,0x23,0x40,0x24,0x00,0x18,0x00,0x00,0x00},/*"?",31*/
{0x1F,0x80,0x20,0x40,0x27,0x40,0x29,0x40,0x1F,0x40,0x00,0x00},/*"@",32*/
{0x00,0x40,0x07,0xC0,0x39,0x00,0x0F,0x00,0x01,0xC0,0x00,0x40},/*"A",33*/
{0x20,0x40,0x3F,0xC0,0x24,0x40,0x24,0x40,0x1B,0x80,0x00,0x00},/*"B",34*/
{0x1F,0x80,0x20,0x40,0x20,0x40,0x20,0x40,0x30,0x80,0x00,0x00},/*"C",35*/
{0x20,0x40,0x3F,0xC0,0x20,0x40,0x20,0x40,0x1F,0x80,0x00,0x00},/*"D",36*/
{0x20,0x40,0x3F,0xC0,0x24,0x40,0x2E,0x40,0x30,0xC0,0x00,0x00},/*"E",37*/
{0x20,0x40,0x3F,0xC0,0x24,0x40,0x2E,0x00,0x30,0x00,0x00,0x00},/*"F",38*/
{0x0F,0x00,0x10,0x80,0x20,0x40,0x22,0x40,0x33,0x80,0x02,0x00},/*"G",39*/
{0x20,0x40,0x3F,0xC0,0x04,0x00,0x04,0x00,0x3F,0xC0,0x20,0x40},/*"H",40*/
{0x20,0x40,0x20,0x40,0x3F,0xC0,0x20,0x40,0x20,0x40,0x00,0x00},/*"I",41*/
{0x00,0x60,0x20,0x20,0x20,0x20,0x3F,0xC0,0x20,0x00,0x20,0x00},/*"J",42*/
{0x20,0x40,0x3F,0xC0,0x24,0x40,0x0B,0x00,0x30,0xC0,0x20,0x40},/*"K",43*/
{0x20,0x40,0x3F,0xC0,0x20,0x40,0x00,0x40,0x00,0x40,0x00,0xC0},/*"L",44*/
{0x3F,0xC0,0x3C,0x00,0x03,0xC0,0x3C,0x00,0x3F,0xC0,0x00,0x00},/*"M",45*/
{0x20,0x40,0x3F,0xC0,0x0C,0x40,0x23,0x00,0x3F,0xC0,0x20,0x00},/*"N",46*/
{0x1F,0x80,0x20,0x40,0x20,0x40,0x20,0x40,0x1F,0x80,0x00,0x00},/*"O",47*/
{0x20,0x40,0x3F,0xC0,0x24,0x40,0x24,0x00,0x18,0x00,0x00,0x00},/*"P",48*/
{0x1F,0x80,0x21,0x40,0x21,0x40,0x20,0xE0,0x1F,0xA0,0x00,0x00},/*"Q",49*/
{0x20,0x40,0x3F,0xC0,0x24,0x40,0x26,0x00,0x19,0xC0,0x00,0x40},/*"R",50*/
{0x18,0xC0,0x24,0x40,0x24,0x40,0x22,0x40,0x31,0x80,0x00,0x00},/*"S",51*/
{0x30,0x00,0x20,0x40,0x3F,0xC0,0x20,0x40,0x30,0x00,0x00,0x00},/*"T",52*/
{0x20,0x00,0x3F,0x80,0x00,0x40,0x00,0x40,0x3F,0x80,0x20,0x00},/*"U",53*/
{0x20,0x00,0x3E,0x00,0x01,0xC0,0x07,0x00,0x38,0x00,0x20,0x00},/*"V",54*/
{0x38,0x00,0x07,0xC0,0x3C,0x00,0x07,0xC0,0x38,0x00,0x00,0x00},/*"W",55*/
{0x20,0x40,0x39,0xC0,0x06,0x00,0x39,0xC0,0x20,0x40,0x00,0x00},/*"X",56*/
{0x20,0x00,0x38,0x40,0x07,0xC0,0x38,0x40,0x20,0x00,0x00,0x00},/*"Y",57*/
{0x30,0x40,0x21,0xC0,0x26,0x40,0x38,0x40,0x20,0xC0,0x00,0x00},/*"Z",58*/
{0x00,0x00,0x00,0x00,0x7F,0xE0,0x40,0x20,0x40,0x20,0x00,0x00},/*"[",59*/
{0x00,0x00,0x70,0x00,0x0C,0x00,0x03,0x80,0x00,0x40,0x00,0x00},/*"\",60*/
{0x00,0x00,0x40,0x20,0x40,0x20,0x7F,0xE0,0x00,0x00,0x00,0x00},/*"]",61*/
{0x00,0x00,0x20,0x00,0x40,0x00,0x20,0x00,0x00,0x00,0x00,0x00},/*"^",62*/
{0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10},/*"_",63*/
{0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"`",64*/
{0x00,0x00,0x02,0x80,0x05,0x40,0x05,0x40,0x03,0xC0,0x00,0x40},/*"a",65*/
{0x20,0x00,0x3F,0xC0,0x04,0x40,0x04,0x40,0x03,0x80,0x00,0x00},/*"b",66*/
{0x00,0x00,0x03,0x80,0x04,0x40,0x04,0x40,0x06,0x40,0x00,0x00},/*"c",67*/
{0x00,0x00,0x03,0x80,0x04,0x40,0x24,0x40,0x3F,0xC0,0x00,0x40},/*"d",68*/
{0x00,0x00,0x03,0x80,0x05,0x40,0x05,0x40,0x03,0x40,0x00,0x00},/*"e",69*/
{0x00,0x00,0x04,0x40,0x1F,0xC0,0x24,0x40,0x24,0x40,0x20,0x00},/*"f",70*/
{0x00,0x00,0x02,0xE0,0x05,0x50,0x05,0x50,0x06,0x50,0x04,0x20},/*"g",71*/
{0x20,0x40,0x3F,0xC0,0x04,0x40,0x04,0x00,0x03,0xC0,0x00,0x40},/*"h",72*/
{0x00,0x00,0x04,0x40,0x27,0xC0,0x00,0x40,0x00,0x00,0x00,0x00},/*"i",73*/
{0x00,0x10,0x00,0x10,0x04,0x10,0x27,0xE0,0x00,0x00,0x00,0x00},/*"j",74*/
{0x20,0x40,0x3F,0xC0,0x01,0x40,0x07,0x00,0x04,0xC0,0x04,0x40},/*"k",75*/
{0x20,0x40,0x20,0x40,0x3F,0xC0,0x00,0x40,0x00,0x40,0x00,0x00},/*"l",76*/
{0x07,0xC0,0x04,0x00,0x07,0xC0,0x04,0x00,0x03,0xC0,0x00,0x00},/*"m",77*/
{0x04,0x40,0x07,0xC0,0x04,0x40,0x04,0x00,0x03,0xC0,0x00,0x40},/*"n",78*/
{0x00,0x00,0x03,0x80,0x04,0x40,0x04,0x40,0x03,0x80,0x00,0x00},/*"o",79*/
{0x04,0x10,0x07,0xF0,0x04,0x50,0x04,0x40,0x03,0x80,0x00,0x00},/*"p",80*/
{0x00,0x00,0x03,0x80,0x04,0x40,0x04,0x50,0x07,0xF0,0x00,0x10},/*"q",81*/
{0x04,0x40,0x07,0xC0,0x02,0x40,0x04,0x00,0x04,0x00,0x00,0x00},/*"r",82*/
{0x00,0x00,0x06,0x40,0x05,0x40,0x05,0x40,0x04,0xC0,0x00,0x00},/*"s",83*/
{0x00,0x00,0x04,0x00,0x1F,0x80,0x04,0x40,0x00,0x40,0x00,0x00},/*"t",84*/
{0x04,0x00,0x07,0x80,0x00,0x40,0x04,0x40,0x07,0xC0,0x00,0x40},/*"u",85*/
{0x04,0x00,0x07,0x00,0x04,0xC0,0x01,0x80,0x06,0x00,0x04,0x00},/*"v",86*/
{0x06,0x00,0x01,0xC0,0x07,0x00,0x01,0xC0,0x06,0x00,0x00,0x00},/*"w",87*/
{0x04,0x40,0x06,0xC0,0x01,0x00,0x06,0xC0,0x04,0x40,0x00,0x00},/*"x",88*/
{0x04,0x10,0x07,0x10,0x04,0xE0,0x01,0x80,0x06,0x00,0x04,0x00},/*"y",89*/
{0x00,0x00,0x04,0x40,0x05,0xC0,0x06,0x40,0x04,0x40,0x00,0x00},/*"z",90*/
{0x00,0x00,0x00,0x00,0x04,0x00,0x7B,0xE0,0x40,0x20,0x00,0x00},/*"{",91*/
{0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xF0,0x00,0x00,0x00,0x00},/*"|",92*/
{0x00,0x00,0x40,0x20,0x7B,0xE0,0x04,0x00,0x00,0x00,0x00,0x00},/*"}",93*/
{0x40,0x00,0x80,0x00,0x40,0x00,0x20,0x00,0x20,0x00,0x40,0x00},/*"~",94*/
}; 
/****************************************8*16�ĵ���************************************/
const unsigned char code asc2_1608[95][16]={	  
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",0*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xCC,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x00},/*"!",1*/
{0x00,0x00,0x08,0x00,0x30,0x00,0x60,0x00,0x08,0x00,0x30,0x00,0x60,0x00,0x00,0x00},/*""",2*/
{0x02,0x20,0x03,0xFC,0x1E,0x20,0x02,0x20,0x03,0xFC,0x1E,0x20,0x02,0x20,0x00,0x00},/*"#",3*/
{0x00,0x00,0x0E,0x18,0x11,0x04,0x3F,0xFF,0x10,0x84,0x0C,0x78,0x00,0x00,0x00,0x00},/*"$",4*/
{0x0F,0x00,0x10,0x84,0x0F,0x38,0x00,0xC0,0x07,0x78,0x18,0x84,0x00,0x78,0x00,0x00},/*"%",5*/
{0x00,0x78,0x0F,0x84,0x10,0xC4,0x11,0x24,0x0E,0x98,0x00,0xE4,0x00,0x84,0x00,0x08},/*"&",6*/
{0x08,0x00,0x68,0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"'",7*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xE0,0x18,0x18,0x20,0x04,0x40,0x02,0x00,0x00},/*"(",8*/
{0x00,0x00,0x40,0x02,0x20,0x04,0x18,0x18,0x07,0xE0,0x00,0x00,0x00,0x00,0x00,0x00},/*")",9*/
{0x02,0x40,0x02,0x40,0x01,0x80,0x0F,0xF0,0x01,0x80,0x02,0x40,0x02,0x40,0x00,0x00},/*"*",10*/
{0x00,0x80,0x00,0x80,0x00,0x80,0x0F,0xF8,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x00},/*"+",11*/
{0x00,0x01,0x00,0x0D,0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*",",12*/
{0x00,0x00,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80},/*"-",13*/
{0x00,0x00,0x00,0x0C,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*".",14*/
{0x00,0x00,0x00,0x06,0x00,0x18,0x00,0x60,0x01,0x80,0x06,0x00,0x18,0x00,0x20,0x00},/*"/",15*/
{0x00,0x00,0x07,0xF0,0x08,0x08,0x10,0x04,0x10,0x04,0x08,0x08,0x07,0xF0,0x00,0x00},/*"0",16*/
{0x00,0x00,0x08,0x04,0x08,0x04,0x1F,0xFC,0x00,0x04,0x00,0x04,0x00,0x00,0x00,0x00},/*"1",17*/
{0x00,0x00,0x0E,0x0C,0x10,0x14,0x10,0x24,0x10,0x44,0x11,0x84,0x0E,0x0C,0x00,0x00},/*"2",18*/
{0x00,0x00,0x0C,0x18,0x10,0x04,0x11,0x04,0x11,0x04,0x12,0x88,0x0C,0x70,0x00,0x00},/*"3",19*/
{0x00,0x00,0x00,0xE0,0x03,0x20,0x04,0x24,0x08,0x24,0x1F,0xFC,0x00,0x24,0x00,0x00},/*"4",20*/
{0x00,0x00,0x1F,0x98,0x10,0x84,0x11,0x04,0x11,0x04,0x10,0x88,0x10,0x70,0x00,0x00},/*"5",21*/
{0x00,0x00,0x07,0xF0,0x08,0x88,0x11,0x04,0x11,0x04,0x18,0x88,0x00,0x70,0x00,0x00},/*"6",22*/
{0x00,0x00,0x1C,0x00,0x10,0x00,0x10,0xFC,0x13,0x00,0x1C,0x00,0x10,0x00,0x00,0x00},/*"7",23*/
{0x00,0x00,0x0E,0x38,0x11,0x44,0x10,0x84,0x10,0x84,0x11,0x44,0x0E,0x38,0x00,0x00},/*"8",24*/
{0x00,0x00,0x07,0x00,0x08,0x8C,0x10,0x44,0x10,0x44,0x08,0x88,0x07,0xF0,0x00,0x00},/*"9",25*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x0C,0x03,0x0C,0x00,0x00,0x00,0x00,0x00,0x00},/*":",26*/
{0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*";",27*/
{0x00,0x00,0x00,0x80,0x01,0x40,0x02,0x20,0x04,0x10,0x08,0x08,0x10,0x04,0x00,0x00},/*"<",28*/
{0x02,0x20,0x02,0x20,0x02,0x20,0x02,0x20,0x02,0x20,0x02,0x20,0x02,0x20,0x00,0x00},/*"=",29*/
{0x00,0x00,0x10,0x04,0x08,0x08,0x04,0x10,0x02,0x20,0x01,0x40,0x00,0x80,0x00,0x00},/*">",30*/
{0x00,0x00,0x0E,0x00,0x12,0x00,0x10,0x0C,0x10,0x6C,0x10,0x80,0x0F,0x00,0x00,0x00},/*"?",31*/
{0x03,0xE0,0x0C,0x18,0x13,0xE4,0x14,0x24,0x17,0xC4,0x08,0x28,0x07,0xD0,0x00,0x00},/*"@",32*/
{0x00,0x04,0x00,0x3C,0x03,0xC4,0x1C,0x40,0x07,0x40,0x00,0xE4,0x00,0x1C,0x00,0x04},/*"A",33*/
{0x10,0x04,0x1F,0xFC,0x11,0x04,0x11,0x04,0x11,0x04,0x0E,0x88,0x00,0x70,0x00,0x00},/*"B",34*/
{0x03,0xE0,0x0C,0x18,0x10,0x04,0x10,0x04,0x10,0x04,0x10,0x08,0x1C,0x10,0x00,0x00},/*"C",35*/
{0x10,0x04,0x1F,0xFC,0x10,0x04,0x10,0x04,0x10,0x04,0x08,0x08,0x07,0xF0,0x00,0x00},/*"D",36*/
{0x10,0x04,0x1F,0xFC,0x11,0x04,0x11,0x04,0x17,0xC4,0x10,0x04,0x08,0x18,0x00,0x00},/*"E",37*/
{0x10,0x04,0x1F,0xFC,0x11,0x04,0x11,0x00,0x17,0xC0,0x10,0x00,0x08,0x00,0x00,0x00},/*"F",38*/
{0x03,0xE0,0x0C,0x18,0x10,0x04,0x10,0x04,0x10,0x44,0x1C,0x78,0x00,0x40,0x00,0x00},/*"G",39*/
{0x10,0x04,0x1F,0xFC,0x10,0x84,0x00,0x80,0x00,0x80,0x10,0x84,0x1F,0xFC,0x10,0x04},/*"H",40*/
{0x00,0x00,0x10,0x04,0x10,0x04,0x1F,0xFC,0x10,0x04,0x10,0x04,0x00,0x00,0x00,0x00},/*"I",41*/
{0x00,0x03,0x00,0x01,0x10,0x01,0x10,0x01,0x1F,0xFE,0x10,0x00,0x10,0x00,0x00,0x00},/*"J",42*/
{0x10,0x04,0x1F,0xFC,0x11,0x04,0x03,0x80,0x14,0x64,0x18,0x1C,0x10,0x04,0x00,0x00},/*"K",43*/
{0x10,0x04,0x1F,0xFC,0x10,0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0x0C,0x00,0x00},/*"L",44*/
{0x10,0x04,0x1F,0xFC,0x1F,0x00,0x00,0xFC,0x1F,0x00,0x1F,0xFC,0x10,0x04,0x00,0x00},/*"M",45*/
{0x10,0x04,0x1F,0xFC,0x0C,0x04,0x03,0x00,0x00,0xE0,0x10,0x18,0x1F,0xFC,0x10,0x00},/*"N",46*/
{0x07,0xF0,0x08,0x08,0x10,0x04,0x10,0x04,0x10,0x04,0x08,0x08,0x07,0xF0,0x00,0x00},/*"O",47*/
{0x10,0x04,0x1F,0xFC,0x10,0x84,0x10,0x80,0x10,0x80,0x10,0x80,0x0F,0x00,0x00,0x00},/*"P",48*/
{0x07,0xF0,0x08,0x18,0x10,0x24,0x10,0x24,0x10,0x1C,0x08,0x0A,0x07,0xF2,0x00,0x00},/*"Q",49*/
{0x10,0x04,0x1F,0xFC,0x11,0x04,0x11,0x00,0x11,0xC0,0x11,0x30,0x0E,0x0C,0x00,0x04},/*"R",50*/
{0x00,0x00,0x0E,0x1C,0x11,0x04,0x10,0x84,0x10,0x84,0x10,0x44,0x1C,0x38,0x00,0x00},/*"S",51*/
{0x18,0x00,0x10,0x00,0x10,0x04,0x1F,0xFC,0x10,0x04,0x10,0x00,0x18,0x00,0x00,0x00},/*"T",52*/
{0x10,0x00,0x1F,0xF8,0x10,0x04,0x00,0x04,0x00,0x04,0x10,0x04,0x1F,0xF8,0x10,0x00},/*"U",53*/
{0x10,0x00,0x1E,0x00,0x11,0xE0,0x00,0x1C,0x00,0x70,0x13,0x80,0x1C,0x00,0x10,0x00},/*"V",54*/
{0x1F,0xC0,0x10,0x3C,0x00,0xE0,0x1F,0x00,0x00,0xE0,0x10,0x3C,0x1F,0xC0,0x00,0x00},/*"W",55*/
{0x10,0x04,0x18,0x0C,0x16,0x34,0x01,0xC0,0x01,0xC0,0x16,0x34,0x18,0x0C,0x10,0x04},/*"X",56*/
{0x10,0x00,0x1C,0x00,0x13,0x04,0x00,0xFC,0x13,0x04,0x1C,0x00,0x10,0x00,0x00,0x00},/*"Y",57*/
{0x08,0x04,0x10,0x1C,0x10,0x64,0x10,0x84,0x13,0x04,0x1C,0x04,0x10,0x18,0x00,0x00},/*"Z",58*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFE,0x40,0x02,0x40,0x02,0x40,0x02,0x00,0x00},/*"[",59*/
{0x00,0x00,0x30,0x00,0x0C,0x00,0x03,0x80,0x00,0x60,0x00,0x1C,0x00,0x03,0x00,0x00},/*"\",60*/
{0x00,0x00,0x40,0x02,0x40,0x02,0x40,0x02,0x7F,0xFE,0x00,0x00,0x00,0x00,0x00,0x00},/*"]",61*/
{0x00,0x00,0x00,0x00,0x20,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x20,0x00,0x00,0x00},/*"^",62*/
{0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01},/*"_",63*/
{0x00,0x00,0x40,0x00,0x40,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"`",64*/
{0x00,0x00,0x00,0x98,0x01,0x24,0x01,0x44,0x01,0x44,0x01,0x44,0x00,0xFC,0x00,0x04},/*"a",65*/
{0x10,0x00,0x1F,0xFC,0x00,0x88,0x01,0x04,0x01,0x04,0x00,0x88,0x00,0x70,0x00,0x00},/*"b",66*/
{0x00,0x00,0x00,0x70,0x00,0x88,0x01,0x04,0x01,0x04,0x01,0x04,0x00,0x88,0x00,0x00},/*"c",67*/
{0x00,0x00,0x00,0x70,0x00,0x88,0x01,0x04,0x01,0x04,0x11,0x08,0x1F,0xFC,0x00,0x04},/*"d",68*/
{0x00,0x00,0x00,0xF8,0x01,0x44,0x01,0x44,0x01,0x44,0x01,0x44,0x00,0xC8,0x00,0x00},/*"e",69*/
{0x00,0x00,0x01,0x04,0x01,0x04,0x0F,0xFC,0x11,0x04,0x11,0x04,0x11,0x00,0x18,0x00},/*"f",70*/
{0x00,0x00,0x00,0xD6,0x01,0x29,0x01,0x29,0x01,0x29,0x01,0xC9,0x01,0x06,0x00,0x00},/*"g",71*/
{0x10,0x04,0x1F,0xFC,0x00,0x84,0x01,0x00,0x01,0x00,0x01,0x04,0x00,0xFC,0x00,0x04},/*"h",72*/
{0x00,0x00,0x01,0x04,0x19,0x04,0x19,0xFC,0x00,0x04,0x00,0x04,0x00,0x00,0x00,0x00},/*"i",73*/
{0x00,0x00,0x00,0x03,0x00,0x01,0x01,0x01,0x19,0x01,0x19,0xFE,0x00,0x00,0x00,0x00},/*"j",74*/
{0x10,0x04,0x1F,0xFC,0x00,0x24,0x00,0x40,0x01,0xB4,0x01,0x0C,0x01,0x04,0x00,0x00},/*"k",75*/
{0x00,0x00,0x10,0x04,0x10,0x04,0x1F,0xFC,0x00,0x04,0x00,0x04,0x00,0x00,0x00,0x00},/*"l",76*/
{0x01,0x04,0x01,0xFC,0x01,0x04,0x01,0x00,0x01,0xFC,0x01,0x04,0x01,0x00,0x00,0xFC},/*"m",77*/
{0x01,0x04,0x01,0xFC,0x00,0x84,0x01,0x00,0x01,0x00,0x01,0x04,0x00,0xFC,0x00,0x04},/*"n",78*/
{0x00,0x00,0x00,0xF8,0x01,0x04,0x01,0x04,0x01,0x04,0x01,0x04,0x00,0xF8,0x00,0x00},/*"o",79*/
{0x01,0x01,0x01,0xFF,0x00,0x85,0x01,0x04,0x01,0x04,0x00,0x88,0x00,0x70,0x00,0x00},/*"p",80*/
{0x00,0x00,0x00,0x70,0x00,0x88,0x01,0x04,0x01,0x04,0x01,0x05,0x01,0xFF,0x00,0x01},/*"q",81*/
{0x01,0x04,0x01,0x04,0x01,0xFC,0x00,0x84,0x01,0x04,0x01,0x00,0x01,0x80,0x00,0x00},/*"r",82*/
{0x00,0x00,0x00,0xCC,0x01,0x24,0x01,0x24,0x01,0x24,0x01,0x24,0x01,0x98,0x00,0x00},/*"s",83*/
{0x00,0x00,0x01,0x00,0x01,0x00,0x07,0xF8,0x01,0x04,0x01,0x04,0x00,0x00,0x00,0x00},/*"t",84*/
{0x01,0x00,0x01,0xF8,0x00,0x04,0x00,0x04,0x00,0x04,0x01,0x08,0x01,0xFC,0x00,0x04},/*"u",85*/
{0x01,0x00,0x01,0x80,0x01,0x70,0x00,0x0C,0x00,0x10,0x01,0x60,0x01,0x80,0x01,0x00},/*"v",86*/
{0x01,0xF0,0x01,0x0C,0x00,0x30,0x01,0xC0,0x00,0x30,0x01,0x0C,0x01,0xF0,0x01,0x00},/*"w",87*/
{0x00,0x00,0x01,0x04,0x01,0x8C,0x00,0x74,0x01,0x70,0x01,0x8C,0x01,0x04,0x00,0x00},/*"x",88*/
{0x01,0x01,0x01,0x81,0x01,0x71,0x00,0x0E,0x00,0x18,0x01,0x60,0x01,0x80,0x01,0x00},/*"y",89*/
{0x00,0x00,0x01,0x84,0x01,0x0C,0x01,0x34,0x01,0x44,0x01,0x84,0x01,0x0C,0x00,0x00},/*"z",90*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x3E,0xFC,0x40,0x02,0x40,0x02},/*"{",91*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00},/*"|",92*/
{0x00,0x00,0x40,0x02,0x40,0x02,0x3E,0xFC,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"}",93*/
{0x00,0x00,0x60,0x00,0x80,0x00,0x80,0x00,0x40,0x00,0x40,0x00,0x20,0x00,0x20,0x00},/*"~",94*/
};  
//˳��  ����ɨ��  ���� 16x16    
unsigned char code Hzk[][16]={

{0x40,0x10,0x7F,0xF8,0x49,0x10,0x49,0x10,0x49,0x20,0x7F,0xFF,0x48,0xA0,0x88,0x81},
{0x48,0x82,0x38,0x8C,0x0F,0xF0,0x18,0x88,0xE8,0x84,0x48,0x83,0x08,0x82,0x00,0x00},/*"��",0*/

{0x00,0x80,0x41,0x00,0x4F,0xFC,0x72,0x10,0x42,0x10,0x43,0xF9,0x00,0x01,0x4F,0xF2},
{0x48,0x04,0x58,0x18,0x6B,0xE0,0x48,0x10,0x48,0x08,0x4F,0xE6,0x40,0x03,0x00,0x00},/*"˶",1*/

{0x00,0x80,0x01,0x00,0x06,0x00,0x1F,0xFF,0xE0,0x00,0x50,0x80,0x14,0x80,0x14,0x80},
{0x14,0x80,0xFF,0xFF,0x14,0x80,0x14,0x84,0x14,0x82,0x14,0xFC,0x00,0x00,0x00,0x00},/*"ΰ",2*/

{0x00,0x04,0x08,0x04,0x06,0x04,0x01,0xC4,0x00,0x84,0xFF,0xFC,0x00,0x04,0x00,0x04},
{0x00,0x04,0xFF,0xFC,0x00,0x44,0x01,0x84,0x06,0x04,0x1C,0x0C,0x08,0x04,0x00,0x00},/*"ҵ",3*/

{0x00,0x00,0x00,0x00,0x1F,0xF0,0x12,0x20,0x12,0x20,0x12,0x20,0x12,0x20,0xFF,0xFC},
{0x12,0x22,0x12,0x22,0x12,0x22,0x12,0x22,0x1F,0xF2,0x00,0x02,0x00,0x0E,0x00,0x00},/*"��",4*/

{0x00,0x80,0x00,0x80,0x40,0x80,0x40,0x80,0x40,0x80,0x40,0x82,0x40,0x81,0x47,0xFE},
{0x48,0x80,0x50,0x80,0x60,0x80,0x40,0x80,0x00,0x80,0x01,0x80,0x00,0x80,0x00,0x00},/*"��",5*/

{0x08,0x20,0x48,0x40,0x49,0x80,0x4E,0x00,0x7F,0xFF,0x8A,0x00,0x89,0x20,0x00,0x20},
{0x44,0x20,0x33,0x40,0x00,0x40,0x00,0x40,0xFF,0xFF,0x00,0x80,0x00,0x80,0x00,0x00},/*"��",6*/

{0x10,0x80,0x10,0x82,0x11,0x01,0xFF,0xFE,0x12,0x00,0x14,0x02,0x00,0x02,0x13,0x04},
{0x12,0xC8,0x12,0x30,0xFE,0x30,0x12,0x48,0x13,0x84,0x12,0x06,0x10,0x04,0x00,0x00},/*"��",7*/

{0x00,0x00,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x7F,0xFE},
{0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x00,0x06,0x00,0x02,0x00,0x00},/*"��",8*/

{0x00,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x7F,0xFE},
{0x44,0x00,0x46,0x00,0x43,0x80,0x41,0x00,0x40,0x00,0xC0,0x00,0x40,0x00,0x00,0x00},/*"��",9*/

{0x42,0x08,0x42,0x08,0x7F,0xF0,0x42,0x11,0x42,0x92,0x07,0x04,0x00,0x18,0xFF,0xE0},
{0x00,0x04,0x42,0x04,0x42,0x04,0x7F,0xFC,0x42,0x04,0x42,0x04,0x42,0x04,0x00,0x00},/*"��",10*/

{0x02,0x00,0x42,0x00,0x32,0x00,0x23,0xFE,0x00,0x04,0x44,0x08,0x55,0xFF,0x55,0x50},
{0x55,0x50,0xFD,0x50,0x55,0x52,0x55,0x51,0x55,0xFE,0x44,0x00,0x04,0x00,0x00,0x00},/*"��",11*/

{0x00,0x00,0x01,0x01,0x07,0x02,0x79,0x04,0x11,0x18,0x11,0xE0,0x11,0x00,0xFF,0x00},
{0x11,0x00,0x11,0xFC,0x11,0x02,0x11,0x02,0x11,0x02,0x01,0x02,0x00,0x0E,0x00,0x00},/*"��",12*/

{0x00,0x20,0x80,0x20,0x80,0x20,0x9F,0xFF,0x92,0x20,0x92,0x20,0x92,0x20,0xFF,0xE0},
{0x92,0x20,0x92,0x22,0x92,0x21,0x9F,0xFE,0x80,0x20,0x80,0x20,0x00,0x20,0x00,0x00},/*"��",13*/

{0x00,0x40,0x00,0x81,0x51,0x01,0x4A,0x01,0x46,0xF5,0x5A,0x93,0x62,0x91,0x02,0x91},
{0xE2,0x91,0x1A,0x93,0x26,0xF5,0x4A,0x01,0x13,0x01,0x21,0x81,0x01,0x00,0x00,0x00},/*"��",14*/

{0x7F,0xFF,0x40,0x00,0x48,0x40,0x54,0x20,0x63,0xC0,0x11,0x3E,0x11,0x04,0x11,0x04},
{0x11,0x04,0xFF,0xFC,0x11,0x04,0x11,0x04,0x11,0x04,0x11,0x3E,0x01,0x00,0x00,0x00},/*"½",15*/

{0x00,0x10,0x11,0x10,0x11,0x10,0x11,0x10,0x11,0x10,0xFF,0xFF,0x00,0x00,0x00,0x00},
{0x00,0x00,0xFF,0xFF,0x11,0x10,0x11,0x10,0x11,0x10,0x11,0x10,0x10,0x10,0x00,0x00},/*"��",16*/

{0x08,0x20,0x06,0x20,0x80,0x7F,0x61,0x80,0x06,0x02,0x10,0x02,0x10,0x82,0x10,0x82},
{0x90,0x82,0x7F,0xFE,0x10,0x82,0x10,0x82,0x10,0x82,0x10,0x82,0x10,0x02,0x00,0x00},/*"ע",17*/

{0x02,0x02,0x02,0x0C,0xFF,0xF0,0x82,0x04,0x82,0x02,0xFF,0xFC,0x02,0x02,0x02,0x04},
{0x02,0x18,0xFF,0xE0,0x82,0x04,0x82,0x02,0xFF,0xFC,0x02,0x00,0x02,0x00,0x00,0x00},/*"��",18*/

{0x00,0x01,0x00,0x02,0x00,0x0C,0x7F,0xF0,0x44,0x40,0x44,0x40,0x44,0x40,0x44,0x40},
{0x7F,0xFF,0x44,0x40,0x44,0x40,0x44,0x42,0x44,0x41,0x7F,0xFE,0x00,0x00,0x00,0x00},/*"��",19*/

{0x00,0x01,0x00,0x02,0x00,0x0C,0x1F,0xF0,0x11,0x00,0x11,0x00,0x11,0x00,0x91,0x00},
{0x51,0x00,0x71,0x00,0x11,0x00,0x11,0x00,0x11,0x00,0x1F,0x00,0x00,0x00,0x00,0x00},/*"��",20*/

};

const unsigned char code logo[]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x03,0xE0,0x00,0x00,0x1F,0xF8,0x00,0x00,0x7F,0x80,0x00,
0x01,0xFE,0x00,0x00,0x03,0xE0,0x00,0x00,0x03,0xE0,0x00,0x00,0x07,0x80,0x00,0x00,
0x0F,0x80,0x00,0x00,0x0F,0x00,0x00,0x00,0x1E,0x07,0x80,0x00,0x1E,0x1F,0x00,0x00,
0x1C,0x32,0x00,0x00,0x1C,0x72,0xC0,0x00,0x1C,0x7E,0xC7,0x00,0x1E,0x7E,0x1F,0x80,
0x1E,0xFE,0x3F,0xC0,0x1E,0xFF,0x3F,0xC0,0x1F,0xFF,0xFF,0xE0,0x0F,0xFF,0xFF,0xE0,
0x0F,0xFF,0xFF,0xE0,0x07,0xFF,0xFF,0xC0,0x07,0xFF,0xFF,0xC0,0x03,0xFF,0xFF,0x80,
0x01,0xFF,0xFF,0x00,0x00,0xFF,0xFE,0x00,0x00,0x3F,0xFC,0x00,0x00,0x0F,0xF0,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x80,0x00,0x1F,0xFE,0xBE,
0x00,0x3F,0xFE,0x82,0x00,0x3F,0xFE,0x82,0x00,0x30,0x82,0x80,0x00,0x30,0x82,0xAE,
0x00,0x30,0x82,0x80,0x00,0x31,0xC6,0x9E,0x00,0x1F,0xFE,0xA8,0x00,0x1F,0x7C,0xA8,
0x00,0x0E,0x38,0x9E,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x9C,0x00,0x37,0xFE,0xA2,
0x00,0x37,0xFE,0xA2,0x00,0x00,0x00,0x9C,0x00,0x00,0x00,0x80,0x00,0x01,0xF8,0xBE,
0x00,0x03,0xFC,0x88,0x00,0x07,0x0E,0x94,0x00,0x06,0x06,0xA2,0x00,0x04,0x02,0x80,
0x00,0x04,0x02,0xBE,0x00,0x04,0x02,0xAA,0x00,0x06,0x06,0xAA,0x00,0x07,0x0E,0xA2,
0x00,0x03,0xFC,0x80,0x00,0x01,0xF8,0xA2,0x00,0x00,0x00,0x94,0x00,0x06,0x00,0x88,
0x00,0x1F,0xFC,0xB0,0x00,0x1F,0xFE,0x80,0x00,0x1F,0xFE,0xBE,0x00,0x06,0x02,0xA8,
0x00,0x00,0x00,0xAC,0x00,0x01,0xF8,0x92,0x00,0x03,0xFC,0x80,0x00,0x07,0x7E,0xBE,
0x00,0x06,0x4E,0xAA,0x00,0x04,0x46,0xAA,0x00,0x04,0x46,0xA2,0x00,0x04,0x46,0x80,
0x00,0x06,0x46,0xBE,0x00,0x03,0x46,0xA2,0x00,0x01,0xC4,0xB6,0x00,0x00,0x80,0x9C,
0x00,0x00,0x00,0x80,0x00,0x00,0xF0,0xBE,0x00,0x03,0xFC,0xAA,0x00,0x03,0xFC,0xAA,
0x00,0x06,0x06,0xA2,0x00,0x04,0x02,0x80,0x00,0x04,0x02,0xBA,0x00,0x04,0x02,0xAA,
0x00,0x04,0x06,0xAA,0x00,0x00,0x04,0xAE,0x00,0x00,0x00,0x80,0x00,0x3F,0xFE,0x80,
0x00,0x3F,0xFE,0xAE,0x00,0x06,0x00,0x80,0x00,0x06,0x00,0xBE,0x00,0x06,0x00,0xA2,
0x00,0x06,0x00,0xAA,0x00,0x06,0x00,0xAE,0x00,0x07,0xFE,0x80,0x00,0x07,0xFE,0xBE,
0x00,0x03,0xFE,0xA0,0x00,0x00,0x00,0xA0,0x00,0x00,0x00,0xBE,0x00,0x00,0x00,0x80,
0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x80,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

#endif


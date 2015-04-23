#ifndef _24C02_H
#define _24C02_H
/*******************************************************************
24C02容量2Kbits  2048bit，256Byte；100万次编程周期
地址（0x00~0xFF）
*******************************************************************/
void i2c_init(void);
void at24c02_wr(u8 address, u8 dat);
u8 at24c02_rd(u8 address);
void delay_10ms();
void at24c02_wrdat(u8 address, void *dat, u8 bytenum);
void at24c02_rddat(u8 address, void *dat, u8 bytenum);

void Flush_24c02(void);
u32 Get_Pkg_Add(u8 package);
u32 Get_Pkg_CNT(u8 package,u8 mode);
/***********************************************************************/
#define PKG1_ADD  0X00
#define PKG2_ADD  0X08
#define PKG3_ADD  0X16

#define COUNT1_OK_ADD 0X24
#define COUNT1_NG_ADD 0X32

#define COUNT2_OK_ADD 0X40
#define COUNT2_NG_ADD 0X48

#define COUNT3_OK_ADD 0X56
#define COUNT3_NG_ADD 0X64

#define COUNTALL_OK_ADD 0X72
#define COUNTALL_NG_ADD 0X80
/************************************************************************///存储信息地址
#endif
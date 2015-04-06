#ifndef _24C02_H
#define _24C02_H
/*******************************************************************
24C02容量2Kbits  2048bit，256Byte；100万次编程周期
地址（0x00~0xFF）
*******************************************************************/
void i2c_init(void);
void at24c02_wr(u8 address, u8 dat);
u8 at24c02_rd(u8 address);

void at24c02_wrdat(u8 address, void *dat, u8 bytenum);
void at24c02_rddat(u8 address, void *dat, u8 bytenum);

#endif
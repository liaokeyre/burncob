#ifndef __UART_H__
#define __UART_H__

#define BAUD9600	  9600
#define BAUD115200	  115200
#define Uart_IP_En    ES = 1         //串口1,2中断允许
#define Uart_IP_Uen   ES = 0 		  //串口1,2中断禁止
void Uart_Baudset(u32 num);
void send_string(char *s);
char put_char (char c);
void get_hex_data(u8 dat);
void printf_u8(u8 dat);
void put_byte(u8 tmp);
void UartInit(void);
void get_hex_data(u8 dat);
#endif
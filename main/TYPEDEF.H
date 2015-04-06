/*--------------------------------------------------------------------------
   @file    TYPEDEF.H
   @brief   数据类型定义
   @details	
   @author	liaokeyre <Liaokeyre@foxmail.com>
   @date   	2013.10.05
   @note	Copyright:(c) 2013 , All Rights Reserved.

----------------------------------------------------------------------------*/
#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

//#define	uint unsigned int
//#define	uchar unsigned char
//#define ulong unsigned long

typedef unsigned char	u8, uint8,U8,uchar;
typedef char			s8, int8,S8;
typedef unsigned int	u16, uint16,func,U16,uint;
typedef signed int		s16, int16,S16;
typedef unsigned long	u32, uint32,U32,ulong;
typedef signed long		s32, int32,S32;

#define bool		bit
#define _bool		bool
#define _xdata		xdata
#define _data		data
#define _idata		idata
#define _pdata		pdata
#define _bdata		bdata
#define _bit		bit
#define _sbit		sbit
#define _code		code
#define _small		small
#define _large      large
#endif
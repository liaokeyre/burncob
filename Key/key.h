#ifndef __KEY_H__
#define __KEY_H__
#include "main.h"

#define   KEY_SCAN_TIMES		(5/2)    //按键防抖的扫描次数
#define   KEY_SHORT_TIMES		(4)
#define   KEY_LONG_TIMES		(0x10)	 //长按键的次数
#define   KEY_HOLD_TIMES        (0x03)   //连按的频率次数
							 
#define   KEY_LONG		0x10
#define   KEY_HOLD		0x20
#define   KEY_SHORT_UP  0x30 
#define   KEY_LONG_UP   0x40

#define   KEY_1		0x01
#define   KEY_2		0x02
#define   KEY_3		0x03
#define   KEY_4		0x04
#define   KEY_5		0x05
#define   KEY_6     0x06
#define   KEY_7		0x07
#define   KEY_8		0x08

#define   KEY_MOD   KEY_1	 //菜单
#define   KEY_ADD   KEY_2	 //加
#define   KEY_SUB   KEY_3	 //减
#define   KEY_OK    KEY_4 	 //确定
#define   KEY_START   KEY_5 	 //确定
//*********************************************************//
#define PR_MOD			 KEY_MOD     //MOD
#define PS_MOD			(KEY_MOD|KEY_SHORT_UP)
#define PU_MOD			(KEY_MOD|KEY_LONG_UP)
#define PL_MOD			(KEY_MOD|KEY_LONG)
#define PH_MOD			(KEY_MOD|KEY_HOLD)

#define PR_ADD			 KEY_ADD     //ADD
#define PS_ADD			(KEY_ADD|KEY_SHORT_UP)
#define PU_ADD			(KEY_ADD|KEY_LONG_UP)
#define PL_ADD			(KEY_ADD|KEY_LONG)
#define PH_ADD			(KEY_ADD|KEY_HOLD)

#define PR_SUB			 KEY_SUB     //SUB
#define PS_SUB			(KEY_SUB|KEY_SHORT_UP)
#define PU_SUB			(KEY_SUB|KEY_LONG_UP)
#define PL_SUB			(KEY_SUB|KEY_LONG)
#define PH_SUB			(KEY_SUB|KEY_HOLD)

#define PR_OK			 KEY_OK     //OK
#define PS_OK			(KEY_OK|KEY_SHORT_UP)
#define PU_OK			(KEY_OK|KEY_LONG_UP)
#define PL_OK			(KEY_OK|KEY_LONG)
#define PH_OK			(KEY_OK|KEY_HOLD)

#define PR_START			 KEY_START     //START
#define PS_START			(KEY_START|KEY_SHORT_UP)
#define PU_START			(KEY_START|KEY_LONG_UP)
#define PL_START			(KEY_START|KEY_LONG)
#define PH_START			(KEY_START|KEY_HOLD)
//****************************************************************//

#define NO_KEY              0xff
#define KEY_BASE_CNT        (8)
#define KEY_LONG_CNT        (75)
#define KEY_HOLD_CNT        (15)

extern u8 Key_back;
void KeyScan(void);  
#endif

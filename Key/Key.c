#include "MAIN.H"
#include "key.h"
#include "uart.h"

u8 keyCounter ;		                //按键扫描计数
u8 Key_back;		                //键值返回值
u8 Key_change = 0;	                //键值改变标志


void CheckKey(u8 *flag)
{
   
	 if(KEY1 == 0)
	 {
	  	  *flag = KEY_MOD;	//1
	 }
	 else if(KEY2 == 0)
	 {
	  	  *flag = KEY_ADD;	//2
	 }
	 else if(KEY3 == 0)
	 {
	  	  *flag = KEY_SUB;	//3
	 }
	 else if(KEY4 == 0)
	 {
	  	  *flag = KEY_OK;	//4
	 }
	 else if(START == 0)
	 {
	  	  *flag = KEY_START; //5	
	 }
	 else
		 *flag = NO_KEY;
}

/*----------------------------------------------------------------------------*/
/**@brief 按键检测，每10ms进行一次
@return 无
*/
/*----------------------------------------------------------------------------*/
void KeyScan(void)
{
    static  u8 keyValue = 0;
    u8  keyTmp = NO_KEY;
	u8  key_return = NO_KEY;	
	CheckKey(&keyTmp); 	     //获取键值
    if (keyTmp == NO_KEY)
    {
        if (keyCounter >= KEY_LONG_TIMES)  //长按抬键
        {
            key_return = keyValue | KEY_LONG_UP;
        }
        else if (keyCounter >= KEY_SCAN_TIMES)   //短按抬键
        {
            key_return = keyValue | KEY_SHORT_UP;
        }

        keyValue = keyTmp;   
        keyCounter = 0;
    }
	else if(keyTmp != keyValue)
	{
        keyValue = keyTmp;
        keyCounter = 0;
		Key_change = 1;
	}	
    else
    {
        keyCounter++;
        if (keyCounter == KEY_SCAN_TIMES)			//去抖
        {  
            key_return = keyValue;
        }
        else if (keyCounter == KEY_LONG_TIMES)				//长按
        {
            key_return = keyValue | KEY_LONG;
        }
        else if (keyCounter == (KEY_LONG_TIMES + KEY_HOLD_TIMES) )			//连按
        {
            key_return = keyValue | KEY_HOLD;
            keyCounter = KEY_LONG_TIMES;
        }

    }
//	    if (key_return != NO_KEY)
//	    {	   
           
	    Key_back =  key_return;
//	    }

}
#include	"main.h"
#include    "24c02.h"
#include    "oled.h"
bool ACK = 1;  
/*******************************************************************************************
    延时 10us 		  @11.0592MHz
*********************************************************************************************/
void delay_10us()
{
	 u8 i;
	_nop_();
	_nop_();
	_nop_();
	i = 24;
	while (--i);
}    
/*******************************************************************************************
    延时 5us 		   @11.0592MHz
*********************************************************************************************/
void delay_5us()
{
	 u8 i;
	_nop_();
	_nop_();
	_nop_();
	i = 10;
	while (--i);
}
/*******************************************************************************************
    延时 10ms 			@11.0592MHz
*********************************************************************************************/
void delay_10ms()
{

	u8 i, j;
	_nop_();
	_nop_();
	i = 108;
	j = 144;
	do
	{
	 while (--j);
	}
	while (--i);
}
/*******************************************************************************************
函数名: bit i2c_start()
说明: 启动成功返回1, 启动失败还回0

	````````````
SCL 

	 `````|
SDA 	  |__
********************************************************************************************/
bit i2c_start()
{
    delay_10us();
    I2C_SCL = 1;
    delay_10us();
    I2C_SDA = 1;
    delay_10us();
    if (I2C_SCL & I2C_SDA) // 确保为高电平 
    {
        I2C_SDA = 0;       // 启动I2C总线 
       delay_10us();
        I2C_SCL = 0;       // 拉低时钟线以备用 
       delay_10us();
        return 1;
    }
    else
    {
        return 0;
    }
}

/*******************************************************************************************
函数名: void i2c_stop()
说明: I2C停止信号

	````````````
SCL 

	 	|`````
SDA ____|
********************************************************************************************/
void i2c_stop()
{
    delay_10us();
    I2C_SDA = 0;
    delay_10us();
    I2C_SCL = 1;
    delay_10us();
    I2C_SDA = 1;
}

 

/*******************************************************************************************
函数名: bit i2c_sentbyte(u8 byt)
说明: 发送数据,还回 1: 发送成功 0;发送失败
********************************************************************************************/
bit i2c_sentbyte(u8 byt)
{
    u8 i;
    
    for (i = 0; i < 8; i++)
    {
        delay_5us();   
        if (byt & 0x80)
        {
            I2C_SDA = 1;
        }
        else
        {
            I2C_SDA = 0;
        }
        
        delay_5us();
        I2C_SCL = 1;
        delay_5us();
        I2C_SCL = 0;    
        
        byt <<= 1;
    }
    delay_5us();    // 在第9 个周期响应ACK,读24c64发送出的应答信号ACK拉SDA 
    I2C_SDA = 1;
    delay_5us();
    I2C_SCL = 1;
    delay_5us();
    ACK = I2C_SDA; // 读应答信号 
    I2C_SCL = 0;
    
    return ACK;
}

/*******************************************************************************************
函数名: u8 i2c_rcvbyte(u8 ack_chack)
说明: 接收数据 ack_chack= 1: 发送应答信号, 0: 不发送应答信号
    ** 发送主机发送应答信号后 SDA为低电平,此时接收数据要重新拉高SDA电平
********************************************************************************************/
u8 i2c_rcvbyte(u8 ack_chack)
{
    u8 i, temp = 0;
    
    delay_5us();    
    I2C_SDA = 1;    // 读数据时候先拉高SDA电平 
    
    for (i = 0; i < 8; i++)
    {
        temp <<= 1;
        delay_5us();
        I2C_SCL = 1;
        delay_5us();
        if (I2C_SDA)
		temp++;
        I2C_SCL = 0;    
    }
    
    delay_5us();    // 发送应答信号
    I2C_SDA = 1;
    if (ack_chack) // 如果为 1 发送应答信号
    {   
        I2C_SDA = 0;
    }

    delay_5us();
    I2C_SCL = 1;
    delay_5us();
    I2C_SCL = 0;
//////////////////////////////////////////是否发送NACK?*******?????????????????????????
    return (temp);
}
/****************************************************************************
* 功能：写一个字节到24C02指定的地址
* 入口参数：address，地址（0x00~0xFF）；dat，数据
* 出口参数：无
* 说明：
****************************************************************************/
void at24c02_wr(u8 address, u8 dat)
{ 
    i2c_start();   
    i2c_sentbyte(0xa0); 
    i2c_sentbyte(address); 
    i2c_sentbyte(dat); 
    i2c_stop(); 
//    delay_10ms();  //增加了进度条，删除死延时！
}

void at24c02_wrdat(u8 address, void *dat, u8 bytenum)
{ 
    i2c_start();   
    i2c_sentbyte(0xa0); 
    i2c_sentbyte(address);
	while(bytenum--)
	{ 
      i2c_sentbyte(*(u8*)dat ); 
	  ((u8*)dat)++;
	  address++;
	  if(address>=0xff)
	  return;
	}
    i2c_stop(); 
    delay_10ms(); 
}
/****************************************************************************
* 功能：从24C02指定的地址读出一个字节
* 入口参数：address，地址（0x00~0xFF）。
* 出口参数：读到的数据
* 说明：
****************************************************************************/
u8 at24c02_rd(u8 address)
{ 
    u8 temp;      
    i2c_start();    
    i2c_sentbyte(0xa0); 
    i2c_sentbyte(address);      
    i2c_start();   
    i2c_sentbyte(0xa1); 
    temp = i2c_rcvbyte(0);
    i2c_stop();
	return temp;
}
void at24c02_rddat(u8 address, void *dat, u8 bytenum)
{ 
    if(bytenum)
	{  
		i2c_start();    
		i2c_sentbyte(0xa0); 
		i2c_sentbyte(address);      
		i2c_start();   
		i2c_sentbyte(0xa1); 
		bytenum--;
        while (bytenum--)
        {
            *(u8*)dat = i2c_rcvbyte(1); // 发送ACK进行连续读 
            ((u8*)dat)++;               // 主机接收存储单元加一           
        }
        *(u8*)dat = i2c_rcvbyte(0);     // 读最后一个数据,不发送ACK
		i2c_stop();
	}
}

void Flush_24c02(void)
{
  u8 i;
  OLED_DrawRectangle(0, 26, 126, 30, 1);
  for(i=0;i<0xff;i++)
  {
    at24c02_wr(i,0);  //全部填充0
	OLED_Draw_RectangleFill(1, 27, 1+i/2, 29, 1); 
	OLED_Refresh_Gram();
  } 
}
/****************************************************************/
//获取地址码
/***************************************************************/
u32 Get_Pkg_Add(u8 package)
{
   u32 ret;
   if(package == Package_1)
   {
    at24c02_rddat(PKG1_ADD, &ret, 4);
   } 
   else	if(package == Package_2)
   {
    at24c02_rddat(PKG2_ADD, &ret, 4);
   } 
   else
   {
    at24c02_rddat(PKG3_ADD, &ret, 4);
   } 
   return ret;
}

/****************************************************************/
//获取地址码	mode 0=OK  1=NG
/***************************************************************/
u32 Get_Pkg_CNT(u8 package,u8 mode)
{
   u32 ret;
   if(mode == 0)
   {
	   if(package == Package_1)
	   {
	    at24c02_rddat(COUNT1_OK_ADD, &ret, 4);
	   } 
	   else	if(package == Package_2)
	   {
	    at24c02_rddat(COUNT2_OK_ADD, &ret, 4);
	   } 
	   else
	   {
	    at24c02_rddat(COUNT3_OK_ADD, &ret, 4);
	   } 
   }
   else
   {
 	   if(package == Package_1)
	   {
	    at24c02_rddat(COUNT1_NG_ADD, &ret, 4);
	   } 
	   else	if(package == Package_2)
	   {
	    at24c02_rddat(COUNT2_NG_ADD, &ret, 4);
	   } 
	   else
	   {
	    at24c02_rddat(COUNT3_NG_ADD, &ret, 4);
	   }   
   }
   return ret;
}




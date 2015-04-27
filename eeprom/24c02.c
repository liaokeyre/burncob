#include	"main.h"
#include    "24c02.h"
#include    "oled.h"
bool ACK = 1;  
/*******************************************************************************************
    ��ʱ 10us 		  @11.0592MHz
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
    ��ʱ 5us 		   @11.0592MHz
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
    ��ʱ 10ms 			@11.0592MHz
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
������: bit i2c_start()
˵��: �����ɹ�����1, ����ʧ�ܻ���0

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
    if (I2C_SCL & I2C_SDA) // ȷ��Ϊ�ߵ�ƽ 
    {
        I2C_SDA = 0;       // ����I2C���� 
       delay_10us();
        I2C_SCL = 0;       // ����ʱ�����Ա��� 
       delay_10us();
        return 1;
    }
    else
    {
        return 0;
    }
}

/*******************************************************************************************
������: void i2c_stop()
˵��: I2Cֹͣ�ź�

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
������: bit i2c_sentbyte(u8 byt)
˵��: ��������,���� 1: ���ͳɹ� 0;����ʧ��
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
    delay_5us();    // �ڵ�9 ��������ӦACK,��24c64���ͳ���Ӧ���ź�ACK��SDA 
    I2C_SDA = 1;
    delay_5us();
    I2C_SCL = 1;
    delay_5us();
    ACK = I2C_SDA; // ��Ӧ���ź� 
    I2C_SCL = 0;
    
    return ACK;
}

/*******************************************************************************************
������: u8 i2c_rcvbyte(u8 ack_chack)
˵��: �������� ack_chack= 1: ����Ӧ���ź�, 0: ������Ӧ���ź�
    ** ������������Ӧ���źź� SDAΪ�͵�ƽ,��ʱ��������Ҫ��������SDA��ƽ
********************************************************************************************/
u8 i2c_rcvbyte(u8 ack_chack)
{
    u8 i, temp = 0;
    
    delay_5us();    
    I2C_SDA = 1;    // ������ʱ��������SDA��ƽ 
    
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
    
    delay_5us();    // ����Ӧ���ź�
    I2C_SDA = 1;
    if (ack_chack) // ���Ϊ 1 ����Ӧ���ź�
    {   
        I2C_SDA = 0;
    }

    delay_5us();
    I2C_SCL = 1;
    delay_5us();
    I2C_SCL = 0;
//////////////////////////////////////////�Ƿ���NACK?*******?????????????????????????
    return (temp);
}
/****************************************************************************
* ���ܣ�дһ���ֽڵ�24C02ָ���ĵ�ַ
* ��ڲ�����address����ַ��0x00~0xFF����dat������
* ���ڲ�������
* ˵����
****************************************************************************/
void at24c02_wr(u8 address, u8 dat)
{ 
    i2c_start();   
    i2c_sentbyte(0xa0); 
    i2c_sentbyte(address); 
    i2c_sentbyte(dat); 
    i2c_stop(); 
//    delay_10ms();  //�����˽�������ɾ������ʱ��
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
* ���ܣ���24C02ָ���ĵ�ַ����һ���ֽ�
* ��ڲ�����address����ַ��0x00~0xFF����
* ���ڲ���������������
* ˵����
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
            *(u8*)dat = i2c_rcvbyte(1); // ����ACK���������� 
            ((u8*)dat)++;               // �������մ洢��Ԫ��һ           
        }
        *(u8*)dat = i2c_rcvbyte(0);     // �����һ������,������ACK
		i2c_stop();
	}
}

void Flush_24c02(void)
{
  u8 i;
  OLED_DrawRectangle(0, 26, 126, 30, 1);
  for(i=0;i<0xff;i++)
  {
    at24c02_wr(i,0);  //ȫ�����0
	OLED_Draw_RectangleFill(1, 27, 1+i/2, 29, 1); 
	OLED_Refresh_Gram();
  } 
}
/****************************************************************/
//��ȡ��ַ��
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
//��ȡ��ַ��	mode 0=OK  1=NG
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




#ifndef __cd4052_H__
#define __cd4052_H__

extern CobPad PN;	  //20�ߵ�ַ��
extern CobPad PN_TMP;//��ʱ��Ų�ͬ��װ��ַ��
extern CobPad PN_CUT;//��������ĵ�ַ���˵�ַ����ʵ���ճ����ģ�����Ԥ���룻��Ҫ��Ԥ����Ա�

void InitADC(void);
u16 GetADCResult(u8 Chanel);
u16 Adc2Res(u16 adValue);
void GetPadRes(void);
void PnMask(u8 package);
bool BurnCheck(void);
void compareAdd(void);

void TestPn(void);
#endif
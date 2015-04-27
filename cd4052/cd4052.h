#ifndef __cd4052_H__
#define __cd4052_H__

extern CobPad PN;	  //20线地址码
extern CobPad PN_TMP;//临时存放不同封装地址码
extern CobPad PN_CUT;//存放烧码后的地址，此地址码是实际烧出来的，不是预设码；需要和预设码对比

void InitADC(void);
void InitSign(void);
u16 GetADCResult(u8 Chanel);
u16 Adc2Res(u16 adValue);
void GetPadRes(void);
void PnMask(u8 package);
bool BurnCheck(void);
void compareAdd(void);
u8 Hex2Dat(u8 dat);
void CutRun(u8 package);
void TestPn(void);
#endif
#ifndef __MYTOUCH_H__
#define __MYTOUCH_H__



void XPT2046_Init(void);
void XPT2046_Write_Byte(uint8_t data);
uint8_t XPT2046_Read_Byte(void);
int16_t XPT2046_Read_ADC(uint8_t cmd);
void XPT2046_Get_TouchXY(int16_t *x, int16_t *y);
uint8_t XPT2046_Is_Touched(void);


#endif

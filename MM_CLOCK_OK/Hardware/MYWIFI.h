#ifndef __MYWIFI_H__
#define __MYWIFI_H__
#include "stm32f10x.h"                  // Device header
#include "Delay.h"



void MYUSART1_INIT(void);
//接收中断
void USART1_IRQHandler(void);
//USART3(WIFI)
void MYUSART3_INIT(void);
void WIFI_SendString(char *str);
void USART3_IRQHandler(void);
void USART3_TEST(void);
void WIFI_INIT(void);
void WIFI_GETTIME(void);
void wifi_get_time(int *hour, int *min, int *sec);





#endif

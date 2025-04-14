#include "stm32f10x.h"                  // Device header

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	//PB5 LED1 PE5 LED0 PE4 KEY0 PA0 KEYWKUP
	GPIO_InitTypeDef PB5;
	PB5.GPIO_Pin=GPIO_Pin_5;
	PB5.GPIO_Mode=GPIO_Mode_Out_PP;
	PB5.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitTypeDef PE5;
	PE5.GPIO_Pin=GPIO_Pin_5;
	PE5.GPIO_Mode=GPIO_Mode_Out_PP;
	PE5.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&PB5);
	GPIO_Init(GPIOE,&PE5);
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	GPIO_ResetBits(GPIOE,GPIO_Pin_5);
	while(1)
	{

	
	
	}


}


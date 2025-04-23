#include "stm32f10x.h"                  // Device header
#include "lvgl.h"
#include "TIMER3.h"
int myupflag=0;

void TIMER3_INT_INIT(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

    TIM_TimeBaseInitTypeDef TIMER3;
    TIMER3.TIM_ClockDivision=TIM_CKD_DIV1;// 时钟分频
    TIMER3.TIM_CounterMode=TIM_CounterMode_Up;// 递增计数模式
    TIMER3.TIM_Period=1000-1;// 自动重装值
    TIMER3.TIM_Prescaler=72-1;// 预分频
    TIM_TimeBaseInit(TIM3,&TIMER3);
    
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

    NVIC_InitTypeDef MYNVIC;
    MYNVIC.NVIC_IRQChannel=TIM3_IRQn;
    MYNVIC.NVIC_IRQChannelCmd=ENABLE;
    MYNVIC.NVIC_IRQChannelPreemptionPriority=0;
    MYNVIC.NVIC_IRQChannelSubPriority=0;
    NVIC_Init(&MYNVIC);

    TIM_Cmd(TIM3,ENABLE);

}
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // 清除中断标志位
        lv_tick_inc(1); // LVGL 心跳加 1ms
    }

}
void TIMER2_INT_INIT(void) 
{
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    TIM_InitStruct.TIM_Prescaler = 7200 - 1; //  72MHz/7200 = 10kHz
    TIM_InitStruct.TIM_Period = 1000 - 1;    // 10kHz/1000 = 10Hz
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
				static int time2count=0;
				time2count++;
				if(time2count>=300)
				{
					myupflag=1;
					time2count=0;
				}
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除中断标志位

  }

}

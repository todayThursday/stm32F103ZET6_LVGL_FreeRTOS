/*代码功能：让任务交替运行*/
#include "stm32f10x.h"					// Device header
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
 
 
/*函数声明*/
void task0(void *param);
void task1(void *param);

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
    GPIO_InitTypeDef PB5,PE5;
    PB5.GPIO_Mode=GPIO_Mode_Out_PP;
    PB5.GPIO_Pin=GPIO_Pin_5;
    PB5.GPIO_Speed=GPIO_Speed_50MHz;

    PE5.GPIO_Mode=GPIO_Mode_Out_PP;
    PE5.GPIO_Pin=GPIO_Pin_5;
    PE5.GPIO_Speed=GPIO_Speed_50MHz;

    GPIO_Init(GPIOB,&PB5);
    GPIO_Init(GPIOE,&PE5);

    GPIO_SetBits(GPIOB,GPIO_Pin_5);
    GPIO_SetBits(GPIOE,GPIO_Pin_5);

}



int main(){

    LED_Init();
    xTaskCreate(task0,"Task0",128,"Task 0 is running",1,NULL);
    xTaskCreate(task1,"Task1",128,"Task 0 is running",1,NULL);
 
    vTaskStartScheduler();
 
    while(1);
}
 
/*任务0*/
void task0(void *param){
    while(1)
    {
      GPIO_ResetBits(GPIOB,GPIO_Pin_5);
      vTaskDelay(500);
      GPIO_SetBits(GPIOB,GPIO_Pin_5);
      vTaskDelay(500);
        
    }
}
 
/*任务1*/
void task1(void *param){
    while(1)
    {
      GPIO_ResetBits(GPIOE,GPIO_Pin_5);
      vTaskDelay(500);
      GPIO_SetBits(GPIOE,GPIO_Pin_5);
      vTaskDelay(500);
    }
}
 

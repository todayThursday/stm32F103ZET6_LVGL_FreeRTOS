/*代码功能：让任务交替运行*/
#include "stm32f10x.h"					// Device header
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
 
/*初始化*/
unsigned char j = 0;
unsigned char k = 0;
 
/*函数声明*/
void task0(void *param);
void task1(void *param);
 
int main(){
    xTaskCreate(task0,"Task0",10,"Task 0 is running",1,NULL);
    xTaskCreate(task1,"Task1",10,"Task 0 is running",1,NULL);
 
    vTaskStartScheduler();
 
    while(1);
}
 
/*任务0*/
void task0(void *param){
    while(1)
    {
        j = 1;
        k = 0;
        
    }
}
 
/*任务1*/
void task1(void *param){
    while(1)
    {
        j = 0;
        k = 1;
        
    }
}


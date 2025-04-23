#include "stm32f10x.h"                  // Device header

#include "lvgl.h"
#include "FreeRTOS.h"
#include "task.h"
#include "gui_guider.h"
#include "events_init.h"
#include "MYWIFI.h"
/*********************
 *  GUI创建
 *********************/
lv_ui guider_ui;
//extern uint8_t Net_Hour, Net_Minute, Net_Second;
//extern int screen_digital_clock_1_hour_value;
//extern int screen_digital_clock_1_min_value;
//extern int screen_digital_clock_1_sec_value;


static void CLOCK_gui(void)
{
    setup_ui(&guider_ui);
   	events_init(&guider_ui);
}
static void lvgl_task(void *pvParameters)
{
	  extern int screen_digital_clock_1_min_value;
    extern int screen_digital_clock_1_hour_value;
    extern int screen_digital_clock_1_sec_value;
		int HOUR=5,MIN=5,SEC=55;
		WIFI_INIT();
		wifi_get_time(&HOUR,&MIN,&SEC);
		screen_digital_clock_1_min_value=MIN;
		screen_digital_clock_1_hour_value=HOUR;
		screen_digital_clock_1_sec_value=SEC;
		CLOCK_gui();
		
    while(1) {
        lv_task_handler(); // 处理LVGL任务
        vTaskDelay(pdMS_TO_TICKS(5)); // 延迟5ms
    }
}


static void led_blink_task(void *pvParameters)
{
    const TickType_t delay_500ms = pdMS_TO_TICKS(500);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		//PB5 LED1 PE5 LED0 PE4 KEY0 P 
		GPIO_InitTypeDef PB5;
		PB5.GPIO_Pin=GPIO_Pin_5;
		PB5.GPIO_Mode=GPIO_Mode_Out_PP;
		PB5.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_InitTypeDef PA0;
		PA0.GPIO_Pin=GPIO_Pin_0;
		PA0.GPIO_Mode=GPIO_Mode_IPU;
		PA0.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOB,&PB5);
		GPIO_Init(GPIOB,&PA0);
		static uint8_t flag=0;
    
    while(1) {
			if(flag==0)
			{
				GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_RESET);
				GPIO_WriteBit(GPIOE,GPIO_Pin_5,Bit_SET);
				flag=1;
			}
			else
			{
				GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_SET);
				GPIO_WriteBit(GPIOE,GPIO_Pin_5,Bit_RESET);
				flag=0;
			}
      vTaskDelay(delay_500ms); // 延迟500ms
    }
}
// 创建所有任务
void MyTask_Create(void)
{
    // 创建LVGL任务
    xTaskCreate(lvgl_task,       // 任务函数
                "LVGL Task",     // 任务名称
                512,            // 堆栈大小(字)（32：字*4=字节）
                NULL,           // 参数
                2,             // 优先级(数字越大优先级越高)
                NULL);          // 任务句柄
    
//    // 创建LED闪烁任务
//    xTaskCreate(led_blink_task,  // 任务函数
//                "LED Blink",     // 任务名称
//                128,            // 堆栈大小(字)
//                NULL,           // 参数
//                1,             // 优先级
//                NULL);          // 任务句柄
}



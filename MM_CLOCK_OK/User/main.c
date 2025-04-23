#include "stm32f10x.h"                  // Device header
#include "MYLCD.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "Delay.h"
#include "TIMER3.h"
//#include "MYTASKS.h"
//#include "FreeRTOS.h"
//#include "task.h"
#include "MYSRAM.h"
#include "MYWIFI.h"
#include "gui_guider.h"
#include "events_init.h"
lv_ui guider_ui;
extern int myupflag;
extern int screen_digital_clock_1_hour_value;
extern int screen_digital_clock_1_min_value;
extern int screen_digital_clock_1_sec_value;
extern char USART3_RX_BUFFER[256];
extern int getokflag;
int HOUR=5,MIN=5,SEC=55;
int main (void)
{
	WIFI_INIT();
	screen_digital_clock_1_hour_value=HOUR;
	screen_digital_clock_1_min_value=MIN;
	screen_digital_clock_1_sec_value=SEC+3;
	lv_init();
	lv_port_disp_init();  // 显示初始化
  lv_port_indev_init(); // 输入设备初始化
	
	setup_ui(&guider_ui);
  events_init(&guider_ui);

	while(1)
	{
		 // 1. 每小时同步检测
    if(myupflag==1) 
		{
        WIFI_SendString("AT\r\n");
				Delay_ms(500);
				WIFI_SendString("AT+CWJAP=\"FA iPhone Macintosh SE\",\"1234577890\"\r\n");
				Delay_s(8);
				WIFI_SendString("AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\",\"ntp2.aliyun.com\",\"ntp3.aliyun.com\"\r\n");
				Delay_ms(3000);
				WIFI_SendString("AT+CIPSNTPTIME?\r\n");
				Delay_s(1);
				if (getokflag==1)
				{
					wifi_get_time(&HOUR,&MIN,&SEC);
					screen_digital_clock_1_hour_value=HOUR;
					screen_digital_clock_1_min_value=MIN;
					screen_digital_clock_1_sec_value=SEC+3;
					getokflag=0;
					myupflag=0;
				}
				else
				{
					getokflag=0;
					myupflag=0;
				
				}
    }
		lv_task_handler();
		Delay_ms(5);
		
	}
}


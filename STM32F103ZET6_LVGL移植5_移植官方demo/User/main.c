#include "stm32f10x.h"                  // Device header
#include "MYPRINTF.h"
#include "MYLCD.h"
#include "TIMER3.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "Delay.h"
#include "lv_demo_benchmark.h"

// 触摸事件处理函数
static void btn_event_handler(lv_event_t * e)
{
    lv_obj_t * btn = lv_event_get_target(e);  // 获取按钮对象
    lv_label_set_text(lv_obj_get_child(btn, NULL), "Clicked!");  // 修改按钮文本为 "Clicked!"
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x00000), LV_PART_MAIN);  // 更改按钮背景色为红色
}

int main(void)
{
	
	
		lv_init();
		lv_port_disp_init();
		lv_port_indev_init();
	

//		lv_obj_t *label = lv_label_create(lv_scr_act());
//		lv_label_set_text(label, "Hello,This is ");
//		lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

			
    // 创建按钮
    lv_obj_t * btn = lv_btn_create(lv_scr_act());  // 创建一个按钮，父对象为屏幕
    lv_obj_set_size(btn, 200, 50);  // 设置按钮大小
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);  // 按钮居中显示

    // 设置按钮上的文本
    lv_obj_t * label = lv_label_create(btn);  // 在按钮上创建一个标签
    lv_label_set_text(label, "Click Me!");  // 设置标签文本为 "Click Me!"

    // 为按钮设置事件回调函数
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_CLICKED, NULL);

//			lv_demo_benchmark();
	while(1)
	{
		Delay_ms(1);
		lv_timer_handler();

	}


}


#include "stm32f10x.h"                  // Device header
#include "MYPRINTF.h"
#include "Delay.h"


#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"

static void btn_event_cb(lv_event_t* e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* btn = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;
        lv_obj_t* label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}

void lv_example(void) {
    lv_obj_t* btn = lv_btn_create(lv_scr_act());
    lv_obj_set_pos(btn, 10, 10);
    lv_obj_set_size(btn, 120, 50);
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_t* label = lv_label_create(btn);
    lv_label_set_text(label, "Button");
    lv_obj_center(label);
}

int main(void)
{
		lv_init();
		lv_port_disp_init();
		lv_port_indev_init();
	

		lv_obj_t *label = lv_label_create(lv_scr_act());
		lv_label_set_text(label, "Zb&Msy");
		lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
	
		lv_example();
	
	while(1)
	{
		Delay_ms(5);
		lv_task_handler();
	
	
	}


}


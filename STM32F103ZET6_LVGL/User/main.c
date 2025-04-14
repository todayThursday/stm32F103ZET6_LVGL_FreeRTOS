#include "stm32f10x.h"                  // Device header
#include "MYLCD.h"
#include "MYPRINTF.h"
#include "Delay.h"
#include "TIMER3.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"

void lv_ex_label(void)
{
		char* github_addr = "https://gitee.com/W23";
		lv_obj_t * label = lv_label_create(lv_scr_act());
    lv_label_set_recolor(label, true);
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR); /*Circular scroll*/
    lv_obj_set_width(label, 120);
    lv_label_set_text_fmt(label, "#ff0000 Gitee: %s#", github_addr);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 10);
	
    lv_obj_t * label2 = lv_label_create(lv_scr_act());
    lv_label_set_recolor(label2, true);
    lv_label_set_long_mode(label2, LV_LABEL_LONG_SCROLL_CIRCULAR); /*Circular scroll*/
    lv_obj_set_width(label2, 120);
    lv_label_set_text_fmt(label2, "#ff0000 Hello# #0000ff world !123456789#");
    lv_obj_align(label2, LV_ALIGN_CENTER, 0, -10);
}

int main(void)
{
    MYPRINTF_INIT();
    TIMER3_INT_INIT();
    lv_init();
    lv_port_disp_init();
		lv_ex_label();

    while(1)
    {
        lv_timer_handler();
        Delay_ms(5);

    }
}

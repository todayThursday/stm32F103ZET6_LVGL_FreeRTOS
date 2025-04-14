#include "stm32f10x.h"                  // Device header
#include "MYPRINTF.h"
#include "MYLCD.h"
#include "TIMER3.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "Delay.h"

int main(void)
{
	MYLCD_INIT();
	TIMER3_INT_INIT();
	
	lv_init();
  lv_port_disp_init();

  lv_obj_t *label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "Zb&Msy");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
	while(1)
	{
		Delay_ms(5);
		lv_task_handler();
	
	
	}


}


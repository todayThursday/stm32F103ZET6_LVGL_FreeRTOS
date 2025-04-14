#include "stm32f10x.h"                  // Device header
#include "MYPRINTF.h"
#include "MYLCD.h"
#include "MYTOUCH.h"


int main(void)
{
	//x330-3900
	//y200-3860
	int16_t x=0,y=0;
	MYPRINTF_INIT();
	MYLCD_INIT();
	XPT2046_Init();
	MYLCD_FillScreen(0xFFFF);
//	MYLCD_FillArea(0,0,40,140,0xF800);
//	MYLCD_DrawPixel(20,80,0xF800);
	while(1)
	{
        // 判断是否触摸
        if (XPT2046_Is_Touched()==1)
				{
            // 打印触摸坐标
						XPT2046_Get_TouchXY(&x,&y);
            printf("Touched at X: %d, Y: %d\n", x, y);
					  MYLCD_DrawPixel((uint16_t)(x-330)/(3900.0-330.0)*320.0,(uint16_t)(y-200)/3660.0*240,0xF800);
        } else {
            // 如果没有触摸
            printf("No touch detected\n");
        }
	
	}


}


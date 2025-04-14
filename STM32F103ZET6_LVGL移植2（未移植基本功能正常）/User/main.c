#include "stm32f10x.h"                  // Device header
#include "MYPRINTF.h"
#include "MYLCD.h"
#include "TIMER3.h"
extern uint8_t flag;

int main(void)
{
	MYPRINTF_INIT();
	MYLCD_INIT();
	MYLCD_FillScreen(0x0000);
	MYLCD_FillArea(0,0,40,140,0xF800);
	TIMER3_INT_INIT();
	while(1)
	{
		if(flag==1)
		{
			printf("ok\n");
			flag=0;
		}
	
	
	}


}


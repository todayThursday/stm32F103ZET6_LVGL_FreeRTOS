#include "stm32f10x.h"                  // Device header
#include "MYPRINTF.h"

void MYPRINTF_INIT(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitTypeDef MYUSARTPIN;//配置 PA9 (USART1_TX) 作为复用推挽输出
	MYUSARTPIN.GPIO_Mode=GPIO_Mode_AF_PP;
	MYUSARTPIN.GPIO_Pin=GPIO_Pin_9;
	MYUSARTPIN.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&MYUSARTPIN);

	MYUSARTPIN.GPIO_Mode=GPIO_Mode_IPU;//配置 PA10 (USART1_RX) 作为浮空输入
	MYUSARTPIN.GPIO_Pin=GPIO_Pin_10;
	MYUSARTPIN.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&MYUSARTPIN);


	USART_InitTypeDef MYUSART1;
	MYUSART1.USART_BaudRate=115200;
	MYUSART1.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	MYUSART1.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	MYUSART1.USART_Parity=USART_Parity_No;
	MYUSART1.USART_StopBits=USART_StopBits_1;
	MYUSART1.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1, &MYUSART1);

	// //开中断
	// USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	// //NVIC
	// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	// NVIC_InitTypeDef NVICINIT;
	// NVICINIT.NVIC_IRQChannel=USART1_IRQn;
	// NVICINIT.NVIC_IRQChannelCmd=ENABLE;
	// NVICINIT.NVIC_IRQChannelPreemptionPriority=1;
	// NVICINIT.NVIC_IRQChannelSubPriority=1;
	// NVIC_Init(&NVICINIT);
	
	USART_Cmd(USART1,ENABLE);

}

int fputc(int ch, FILE *f) {
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); // 等待发送完成
    USART_SendData(USART1, (uint8_t) ch);
    return ch;
}

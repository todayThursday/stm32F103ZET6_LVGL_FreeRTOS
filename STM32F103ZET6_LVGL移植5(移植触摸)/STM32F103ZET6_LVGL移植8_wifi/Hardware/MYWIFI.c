#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include <stdio.h>
#include <string.h>
#include "MYWIFI.h"


// volatile uint8_t CMD_OK = 0;  // 标志位
volatile uint16_t USART1_RX_index=0;
volatile uint16_t USART3_RX_index = 0;
char USART1_RX_BUFFER[256];
char USART3_RX_BUFFER[256];

void MYUSART1_INIT(void)
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

	//开中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	//NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVICINIT;
	NVICINIT.NVIC_IRQChannel=USART1_IRQn;
	NVICINIT.NVIC_IRQChannelCmd=ENABLE;
	NVICINIT.NVIC_IRQChannelPreemptionPriority=1;
	NVICINIT.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVICINIT);
	
	USART_Cmd(USART1,ENABLE);

}
//重定向
int fputc(int ch, FILE *f) {
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); // 等待发送完成
    USART_SendData(USART1, (uint8_t) ch);
    return ch;
}
//接收中断
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        char ch = USART_ReceiveData(USART1);
		USART1_RX_BUFFER[USART1_RX_index] = ch;  // 先存入
        if (USART1_RX_index >= 2 && USART1_RX_BUFFER[USART1_RX_index-1] == '\r' && USART1_RX_BUFFER[USART1_RX_index] == '\n')
        {
            USART1_RX_BUFFER[USART1_RX_index+1] = '\0';  // 结尾加\0
            USART1_RX_index = 0;
            printf("\r\nCMD_RECIEVED: %s\r\n", USART1_RX_BUFFER);
            USART3_SendString(USART1_RX_BUFFER);
			memset(USART1_RX_BUFFER, 0, sizeof(USART1_RX_BUFFER));
		}
		else
		{
			USART1_RX_index++;
		}	
	}
	
}
//USART3(WIFI)
void MYUSART3_INIT(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//PB10_TX//PB11_RX

	GPIO_InitTypeDef MYUSARTPIN;//配置 PB10 (USART3_TX) 作为复用推挽输出
	MYUSARTPIN.GPIO_Mode=GPIO_Mode_AF_PP;
	MYUSARTPIN.GPIO_Pin=GPIO_Pin_10;
	MYUSARTPIN.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&MYUSARTPIN);

	MYUSARTPIN.GPIO_Mode=GPIO_Mode_IPU;//配置 PB11 (USART3_RX) 作为浮空输入
	MYUSARTPIN.GPIO_Pin=GPIO_Pin_11;
	MYUSARTPIN.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&MYUSARTPIN);


	USART_InitTypeDef MYUSART3;
	MYUSART3.USART_BaudRate=115200;
	MYUSART3.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	MYUSART3.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	MYUSART3.USART_Parity=USART_Parity_No;
	MYUSART3.USART_StopBits=USART_StopBits_1;
	MYUSART3.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART3, &MYUSART3);

    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=USART3_IRQn;   //选择USART3的中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;		//中断使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruct);

	USART_Cmd(USART3,ENABLE);
}
void USART3_SendString(char *str)
{
    while (*str)
    {
        USART_SendData(USART3, *str++);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    }
}
void USART3_IRQHandler(void)
{
    if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
    {
        char ch = USART_ReceiveData(USART3);
		printf("%c",ch);
		
    }
}
void USART3_TEST(void)
{
	USART3_SendString("AT\r\n");  // 测试连通性
	Delay_ms(1000);
	// USART3_SendString("AT+CWMODE=1\r\n"); // 设置为 station 模式
    printf("USART3_INIT OK!");
}


void WIFI_INIT(void)
{
	MYUSART1_INIT();
	MYUSART3_INIT();
}

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include <stdio.h>
#include <string.h>
#include "MYWIFI.h"

int getokflag=0;
// volatile uint8_t CMD_OK = 0;  // 标志位
volatile uint16_t USART1_RX_index=0;
volatile uint16_t USART3_RX_index = 0;
char USART1_RX_BUFFER[256];
char USART3_RX_BUFFER[256];
// 当前时间存储变量
static uint16_t Net_Year;
static uint8_t  Net_Month, Net_Day;
static uint8_t Net_Hour, Net_Minute, Net_Second;
char Net_weekdayStr[4] = {0};
char Net_monthStr[4] = {0};
extern int HOUR,MIN,SEC;

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
            
            printf("\r\nCMD_RECIEVED: %s\r\n", USART1_RX_BUFFER);
						USART1_RX_index = 0;
            WIFI_SendString(USART1_RX_BUFFER);
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

void USART3_IRQHandler(void)
{
    if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
    {
        char ch = USART_ReceiveData(USART3);
				printf("%c",ch);
			// 超过长度则清零
        if (USART3_RX_index < sizeof(USART3_RX_BUFFER)-1) 
				{
						USART3_RX_BUFFER[USART3_RX_index++] = ch;
        }
				else
				{
					USART3_RX_index = 0;
				}
				 // 接收到结束符判断是否处理
         if (ch == '\n')
        {
            USART3_RX_BUFFER[USART3_RX_index] = '\0';

            if (strstr(USART3_RX_BUFFER, "+CIPSNTPTIME:")!=NULL)
            {
								WIFI_GETTIME();
                memset(USART3_RX_BUFFER, 0, sizeof(USART3_RX_BUFFER));
                USART3_RX_index = 0;
								getokflag=1;
            }
        }
		
    }
}

void WIFI_SendString(char *str)
{
    while (*str)
    {
        USART_SendData(USART3, *str++);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    }
}
void WIFI_GETTIME(void)
{
    char *start = strstr(USART3_RX_BUFFER, "+CIPSNTPTIME:");
    if (start!=NULL)
    {
        start += strlen("+CIPSNTPTIME:");
        
        if (sscanf(start, "%3s %3s %hhu %hhu:%hhu:%hhu %hu",
                   Net_weekdayStr, Net_monthStr, &Net_Day,
                   &Net_Hour, &Net_Minute, &Net_Second,&Net_Year) == 7)
        {
						printf("%d,  %d,  %d\r\n",Net_Hour,Net_Minute,Net_Second);
            printf("[WIFI TIME] %04d-%s-%02d %02d:%02d:%02d\r\n",
                   Net_Year, Net_monthStr, Net_Day,
                   Net_Hour, Net_Minute, Net_Second);
						
				}
        else
        {
            printf("[ERROR] Failed to parse datetime.\r\n");
        }
    }
}
void wifi_get_time(int *hour, int *min, int *sec) {
    *hour = Net_Hour;
    *min = Net_Minute;
    *sec = Net_Second;
}
void USART3_TEST(void)
{
	WIFI_SendString("AT\r\n");  // 测试连通性
	Delay_ms(1000);
  printf("USART3_INIT OK!");
}


void WIFI_INIT(void)
{
	MYUSART1_INIT();
	MYUSART3_INIT();
	Delay_ms(1000);
  printf("WIFI Init OK\r\n");
	WIFI_SendString("AT\r\n");
	Delay_ms(500);
	WIFI_SendString("AT+CWJAP=\"FA iPhone Macintosh SE\",\"1234577890\"\r\n");
	Delay_s(10);
	WIFI_SendString("AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\",\"ntp2.aliyun.com\",\"ntp3.aliyun.com\"\r\n");
	Delay_ms(5000);
	WIFI_SendString("AT+CIPSNTPTIME?\r\n");
	Delay_s(1);
	wifi_get_time(&HOUR,&MIN,&SEC);
}

#include "stm32f10x.h"                  // Device header
#define MYIIC_SCL(x) GPIO_WriteBit(GPIOB,GPIO_Pin_8,(bitaction)(x))



void MYIIC_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_8,(BitAction) BitValue);
}
void MYIIC_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_9,(BitAction) BitValue);
}
uint8_t MYIIC_R_SDA(void)
{
	uint8_t Bitvalue;
	Bitvalue=GPIO_ReadInputDataBit (GPIOB,GPIO_Pin_9);
	return Bitvalue;
}


void MYIIC_INIT(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef PB8;//SCL
	PB8.GPIO_Pin=GPIO_Pin_8;
	PB8.GPIO_Mode=GPIO_Mode_Out_OD;
	PB8.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_InitTypeDef PB9;//SDA
	PB9.GPIO_Pin=GPIO_Pin_9;
	PB9.GPIO_Mode=GPIO_Mode_Out_OD;
	PB9.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&PB8);
	GPIO_Init(GPIOB,&PB9);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	GPIO_SetBits(GPIOB,GPIO_Pin_9);
}
void MYIIC_START(void)
{
	MYIIC_W_SDA(1);
	MYIIC_W_SCL(1);
	MYIIC_W_SDA(0);
	MYIIC_W_SCL(0);
}

void MYIIC_STOP(void)
{
	MYIIC_W_SDA(0);
	MYIIC_W_SCL(1);
	MYIIC_W_SDA(1);
}
void MYIIC_SENDBYTE(uint8_t BYTE)
{
	uint8_t i=0;
	for(i=0;i<8;i++)
	{
		MYIIC_W_SDA(BYTE & (0x80>>i));
		MYIIC_W_SCL(1);
		MYIIC_W_SCL(0);
	}
}
uint8_t MYIIC_RECEIVEBYTE(void)
{
	uint8_t i=0;
	uint8_t BYTE=0x00;
	MYIIC_W_SDA(1);
	for(i=0;i<8;i++)
	{
		MYIIC_W_SCL(1);
		if(MYIIC_R_SDA()==1)
		{
			BYTE |= (0x80>>i);
		}
		MYIIC_W_SCL(0);
	}
	return BYTE;
}
void MYIIC_SENDACK(uint8_t ACKBIT)
{
	MYIIC_W_SDA(ACKBIT);
	MYIIC_W_SCL(1);
	MYIIC_W_SCL(0);
}
uint8_t MYIIC_RECEIVEACK(void)
{
	uint8_t ACKBIT;
  MYIIC_W_SDA(1);
	MYIIC_W_SCL(1);
	ACKBIT=MYIIC_R_SDA();
	MYIIC_W_SCL(0);
	return ACKBIT;
}


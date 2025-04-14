#include "stm32f10x.h"                  // Device header


void MYSPI_W_CS(uint8_t Bitvalue)
{
    GPIO_WriteBit(GPIOB,GPIO_Pin_12,(BitAction) Bitvalue);
}
void MYSPI_W_SCK(uint8_t Bitvalue)
{
    GPIO_WriteBit(GPIOB,GPIO_Pin_13,(BitAction) Bitvalue);
}
void MYSPI_W_MOSI(uint8_t Bitvalue)
{
    GPIO_WriteBit(GPIOB,GPIO_Pin_15,(BitAction) Bitvalue);
}
uint8_t MYSPI_R_MISO(void)
{
    return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14);
}



//引脚定义SPI2:SCK-PB13 MISO-PB14(浮空或者上拉) MOSI-PB15 CS片选-PB12；
void MYSPI_INIT(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO_InitTypeDef GPIO_INITPIN;
    GPIO_INITPIN.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_INITPIN.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13 |GPIO_Pin_15;
    GPIO_INITPIN.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_INITPIN);

    GPIO_INITPIN.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_INITPIN.GPIO_Pin=GPIO_Pin_14;
    GPIO_INITPIN.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_INITPIN);

    MYSPI_W_CS(1);
    MYSPI_W_SCK(0);
}
void MYSPI_START(void)
{
    MYSPI_W_CS(0);
}
void MYSPI_STOP(void)
{
    MYSPI_W_CS(1);
}
uint8_t MYSPI_SWAPBYTE(uint8_t BYTESEND)
{
    uint8_t i=0;
    uint8_t BYTERECIEVE;
    for (i=0;i<8;i++)
    {
        MYSPI_W_MOSI(BYTESEND&(0x80>>i));
        MYSPI_W_SCK(1);
        if (MYSPI_R_MISO()==1)
        {
            BYTERECIEVE|=(0x80>>i);
        }
        MYSPI_W_SCK(0);
    }
    return BYTERECIEVE;
   
}

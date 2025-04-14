#include "stm32f10x.h"

#define ILI9341_CS_LOW()     GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define ILI9341_CS_HIGH()    GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define ILI9341_DC_LOW()     GPIO_ResetBits(GPIOA, GPIO_Pin_6)
#define ILI9341_DC_HIGH()    GPIO_SetBits(GPIOA, GPIO_Pin_6)
#define ILI9341_RST_LOW()    GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define ILI9341_RST_HIGH()   GPIO_SetBits(GPIOA, GPIO_Pin_5)

void SPI1_Init(void) {
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);
    
    // SPI1 SCK, MOSI, MISO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7; // SCK, MOSI
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; // MISO
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入，提高稳定性
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // CS, DC, RST
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);
    
    SPI_Cmd(SPI1, ENABLE);
}

uint8_t SPI1_Transfer(uint8_t data) {
    while (!(SPI1->SR & SPI_I2S_FLAG_TXE)); // 等待发送缓冲区空
    SPI_I2S_SendData(SPI1, data);
    while (SPI1->SR & SPI_I2S_FLAG_BSY); // 等待传输完成
    return SPI_I2S_ReceiveData(SPI1);
}

void ILI9341_WriteCommand(uint8_t cmd) {
    ILI9341_DC_LOW();
    ILI9341_CS_LOW();
    SPI1_Transfer(cmd);
    ILI9341_CS_HIGH();
}

void ILI9341_WriteData(uint8_t data) {
    ILI9341_DC_HIGH();
    ILI9341_CS_LOW();
    SPI1_Transfer(data);
    ILI9341_CS_HIGH();
}

void ILI9341_WriteData16(uint16_t data) {
    ILI9341_WriteData(data >> 8);
    ILI9341_WriteData(data & 0xFF);
}

void ILI9341_Reset(void) {
    ILI9341_RST_LOW();
    HAL_Delay(10); // 使用 HAL_Delay 代替 for 循环
    ILI9341_RST_HIGH();
    HAL_Delay(10);
}

void ILI9341_Init(void) {
    ILI9341_Reset();
    ILI9341_WriteCommand(0x01); // 软件复位
    HAL_Delay(5);
    ILI9341_WriteCommand(0x28); // 关闭显示
    ILI9341_WriteCommand(0x3A);
    ILI9341_WriteData(0x55); // 16-bit 颜色格式
    ILI9341_WriteCommand(0x11); // 退出睡眠模式
    HAL_Delay(120);
    ILI9341_WriteCommand(0x29); // 打开显示
}

void ILI9341_Clear(uint16_t color) {
    ILI9341_WriteCommand(0x2A);
    ILI9341_WriteData16(0);
    ILI9341_WriteData16(239);

    ILI9341_WriteCommand(0x2B);
    ILI9341_WriteData16(0);
    ILI9341_WriteData16(319);

    ILI9341_WriteCommand(0x2C);
    
    ILI9341_DC_HIGH();
    ILI9341_CS_LOW();
    
    for (uint32_t i = 0; i < 76800; i++) {
        SPI1_Transfer(color >> 8);
        SPI1_Transfer(color & 0xFF);
    }

    ILI9341_CS_HIGH();
}

int main(void) {
    SPI1_Init();
    ILI9341_Init();
    ILI9341_Clear(0xF800); // 清屏为红色
    while (1);
}

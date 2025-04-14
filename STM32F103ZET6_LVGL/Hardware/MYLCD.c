#include "stm32f10x.h"                  // Device header
#include "MYLCD.h"

#define ILI9341_CS GPIO_Pin_4
#define ILI9341_SCK GPIO_Pin_5
#define ILI9341_MISO GPIO_Pin_6
#define ILI9341_MOSI GPIO_Pin_7

#define ILI9341_DC GPIO_Pin_1
#define ILI9341_RESET GPIO_Pin_2
#define ILI9341_LED GPIO_Pin_3

#define ILI9341_CS_LOW() GPIO_ResetBits(GPIOA,ILI9341_CS)
#define ILI9341_CS_HIGH() GPIO_SetBits(GPIOA,ILI9341_CS)
#define ILI9341_DC_LOW() GPIO_ResetBits(GPIOA,ILI9341_DC)
#define ILI9341_DC_HIGH() GPIO_SetBits(GPIOA,ILI9341_DC)
#define ILI9341_LED_OFF() GPIO_ResetBits(GPIOA,ILI9341_LED)
#define ILI9341_LED_ON() GPIO_SetBits(GPIOA,ILI9341_LED)
#define ILI9341_RESET_LOW() GPIO_ResetBits(GPIOA,ILI9341_RESET)
#define ILI9341_RESET_HIGH() GPIO_SetBits(GPIOA,ILI9341_RESET)

void MYLCD_INIT(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//LED
	GPIO_InitTypeDef MYGPIOINIT;
	MYGPIOINIT.GPIO_Mode=GPIO_Mode_Out_PP;
	MYGPIOINIT.GPIO_Pin=ILI9341_LED;
	MYGPIOINIT.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&MYGPIOINIT);
	ILI9341_LED_ON();//LED显示
	
	//SPI1初始化
	//SCK MOSI
	MYGPIOINIT.GPIO_Mode=GPIO_Mode_AF_PP;
	MYGPIOINIT.GPIO_Pin=ILI9341_SCK | ILI9341_MOSI;
	MYGPIOINIT.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&MYGPIOINIT);
	//MISO
	MYGPIOINIT.GPIO_Mode=GPIO_Mode_IPU;
	MYGPIOINIT.GPIO_Pin=ILI9341_MISO;
	MYGPIOINIT.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&MYGPIOINIT);
	//CS RESET DC
	MYGPIOINIT.GPIO_Mode=GPIO_Mode_Out_PP;
	MYGPIOINIT.GPIO_Pin=ILI9341_CS | ILI9341_RESET |ILI9341_DC;
	MYGPIOINIT.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&MYGPIOINIT);
	
	//SPI1
	SPI_InitTypeDef SPI1INIT;
	SPI1INIT.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;
	SPI1INIT.SPI_Mode=SPI_Mode_Master;
	SPI1INIT.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	SPI1INIT.SPI_DataSize=SPI_DataSize_8b;
	SPI1INIT.SPI_CPHA=SPI_CPHA_1Edge;
	SPI1INIT.SPI_CPOL=SPI_CPOL_Low;
	SPI1INIT.SPI_FirstBit=SPI_FirstBit_MSB;
	SPI1INIT.SPI_NSS=SPI_NSS_Soft;

	SPI1INIT.SPI_CRCPolynomial=7;
	SPI_Init(SPI1,&SPI1INIT);
	SPI_Cmd(SPI1,ENABLE);
	
	 // **执行复位**
    ILI9341_RESET_LOW();
    for (volatile int i = 0; i < 500000; i++);
    ILI9341_RESET_HIGH();
    for (volatile int i = 0; i < 500000; i++);

    // **初始化命令**
    MYLCD_SENDCOMMAND(0x36); // 设置扫描方向
    MYLCD_SENDDATA(0x48); 

    MYLCD_SENDCOMMAND(0x3A); // 设置像素格式
    MYLCD_SENDDATA(0x55); // 16-bit 颜色

    MYLCD_SENDCOMMAND(0x11); // 退出休眠
    for (volatile int i = 0; i < 200000; i++);

    MYLCD_SENDCOMMAND(0x29); // 开启显示

}

uint8_t SPI1_SWAP(uint8_t BYTE)
{
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET){}
	SPI_I2S_SendData(SPI1,BYTE);
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET){}
	return SPI_I2S_ReceiveData(SPI1);


}
void MYLCD_SENDCOMMAND(uint8_t CMD)
{
	ILI9341_CS_LOW();
	ILI9341_DC_LOW();
	SPI1_SWAP(CMD);
	ILI9341_CS_HIGH();
}
void MYLCD_SENDDATA(uint8_t DATA)
{
	ILI9341_CS_LOW();
	ILI9341_DC_HIGH();
	SPI1_SWAP(DATA);
	ILI9341_CS_HIGH();
}
uint8_t MYLCD_READDATA(void)
{
	uint8_t DATA;
	ILI9341_CS_LOW();
	ILI9341_DC_HIGH();
	DATA=SPI1_SWAP(0xFF);
	ILI9341_CS_HIGH();
	return DATA;

}
// **设置窗口地址**
void MYLCD_SetAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    MYLCD_SENDCOMMAND(0x2A);
    MYLCD_SENDDATA(x1 >> 8);
    MYLCD_SENDDATA(x1 & 0xFF);
    MYLCD_SENDDATA(x2 >> 8);
    MYLCD_SENDDATA(x2 & 0xFF);

    MYLCD_SENDCOMMAND(0x2B);
    MYLCD_SENDDATA(y1 >> 8);
    MYLCD_SENDDATA(y1 & 0xFF);
    MYLCD_SENDDATA(y2 >> 8);
    MYLCD_SENDDATA(y2 & 0xFF);

    MYLCD_SENDCOMMAND(0x2C);
}

// **画点**
void MYLCD_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    MYLCD_SetAddress(x, y, x, y);
    MYLCD_SENDDATA(color >> 8);
    MYLCD_SENDDATA(color & 0xFF);
}

// **填充屏幕**
void MYLCD_FillScreen(uint16_t color) {
    MYLCD_SetAddress(0, 0, 239, 319);
    for (uint32_t i = 0; i < 240 * 320; i++) {
        MYLCD_SENDDATA(color >> 8);
        MYLCD_SENDDATA(color & 0xFF);
    }
}
// **填充区域**
void MYLCD_FillArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) 
{
    MYLCD_SetAddress(x1, y1, x2, y2);
    for (uint32_t i = 0; i < (x2-x1) * (y2-y1); i++) {
        MYLCD_SENDDATA(color >> 8);
        MYLCD_SENDDATA(color & 0xFF);
    }
}


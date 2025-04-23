#include "stm32f10x.h"                  // Device header
#include "MYLCD.h"
#include "lv_color.h"

#define ILI9341_CS GPIO_Pin_4
#define ILI9341_SCK GPIO_Pin_5
#define ILI9341_MISO GPIO_Pin_6
#define ILI9341_MOSI GPIO_Pin_7

#define ILI9341_DC GPIO_Pin_6//pc6
#define ILI9341_RESET GPIO_Pin_5//pc5
#define ILI9341_LED GPIO_Pin_7//pc7

#define ILI9341_CS_LOW() GPIO_ResetBits(GPIOA,ILI9341_CS)
#define ILI9341_CS_HIGH() GPIO_SetBits(GPIOA,ILI9341_CS)
#define ILI9341_DC_LOW() GPIO_ResetBits(GPIOC,ILI9341_DC)
#define ILI9341_DC_HIGH() GPIO_SetBits(GPIOC,ILI9341_DC)
#define ILI9341_LED_OFF() GPIO_ResetBits(GPIOC,ILI9341_LED)
#define ILI9341_LED_ON() GPIO_SetBits(GPIOC,ILI9341_LED)
#define ILI9341_RESET_LOW() GPIO_ResetBits(GPIOC,ILI9341_RESET)
#define ILI9341_RESET_HIGH() GPIO_SetBits(GPIOC,ILI9341_RESET)

void MYLCD_INIT(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//LED
	GPIO_InitTypeDef MYGPIOINIT;
	MYGPIOINIT.GPIO_Mode=GPIO_Mode_Out_PP;
	MYGPIOINIT.GPIO_Pin=ILI9341_LED;
	MYGPIOINIT.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC,&MYGPIOINIT);
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
	//CS 
	MYGPIOINIT.GPIO_Mode=GPIO_Mode_Out_PP;
	MYGPIOINIT.GPIO_Pin=ILI9341_CS;
	MYGPIOINIT.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&MYGPIOINIT);
	//RESET DC
	MYGPIOINIT.GPIO_Mode=GPIO_Mode_Out_PP;
	MYGPIOINIT.GPIO_Pin=ILI9341_RESET |ILI9341_DC;
	MYGPIOINIT.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOC,&MYGPIOINIT);
	
	
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
    MYLCD_SENDDATA(0xE8); 

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
    MYLCD_SetAddress(0, 0, 319, 239);
    for (uint32_t i = 0; i < 240 * 320; i++) {
        MYLCD_SENDDATA(color >> 8);
        MYLCD_SENDDATA(color & 0xFF);
    }
}
// **填充区域**
//void lvgl_LCD_Color_Fill(u16 sx, u16 sy, u16 ex, u16 ey, lv_color_t *color)
//{

//	uint32_t y=0; 
//	u16 height, width;
//	width = ex - sx + 1;            //得到填充的宽度
//  height = ey - sy + 1;           //高度
//	
//	LCD_SetWindows(sx,sy,ex,ey);
//	
//	for(y = 0; y <width*height; y++) 
//	{
//		Lcd_WriteData_16Bit(color->full);
//		color++;
//  }
//}


void MYLCD_FillArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, lv_color_t *color) 
{
	  uint32_t i=0; 
		u16 height, width;
		width = x2-x1+1;            //得到填充的宽度
		height = y2-y1+1;           //高度
    MYLCD_SetAddress(x1, y1, x2, y2);
    for (i = 0; i < width*height; i++) {
        MYLCD_SENDDATA((color->full) >> 8);
        MYLCD_SENDDATA((color->full) & 0xFF);
			  color++;
				
    }
}
///////////////////////////////////////////////////////触摸屏
#define XPT2046_CS GPIO_Pin_1
#define XPT2046_SCK GPIO_Pin_0
#define XPT2046_MISO GPIO_Pin_3
#define XPT2046_MOSI GPIO_Pin_2
#define XPT2046_IRQ GPIO_Pin_4

#define CMD_X 0x90  // 读取 X 轴
#define CMD_Y 0xD0  // 读取 Y 轴

#define XPT2046_PORT  GPIOC  // 触摸屏使用 GPIOC 端口

// 片选控制
#define XPT2046_CS_LOW()  GPIO_ResetBits(XPT2046_PORT, XPT2046_CS)
#define XPT2046_CS_HIGH() GPIO_SetBits(XPT2046_PORT, XPT2046_CS)

void XPT2046_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // 使能 GPIOF 时钟

    // 配置 CS, SCK, MOSI 为推挽输出
    GPIO_InitStruct.GPIO_Pin = XPT2046_CS | XPT2046_SCK | XPT2046_MOSI;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(XPT2046_PORT, &GPIO_InitStruct);

    // 配置 MISO 为输入
    GPIO_InitStruct.GPIO_Pin = XPT2046_MISO;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(XPT2046_PORT, &GPIO_InitStruct);
		// 配置 IRQ 为输入
		GPIO_InitStruct.GPIO_Pin = XPT2046_IRQ;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(XPT2046_PORT, &GPIO_InitStruct);

    XPT2046_CS_HIGH();  // 默认不选中
}
// 发送 1 字节数据
void XPT2046_Write_Byte(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++) {
        if (data & 0x80) {
            GPIO_SetBits(XPT2046_PORT, XPT2046_MOSI);
        } else {
            GPIO_ResetBits(XPT2046_PORT, XPT2046_MOSI);
        }
        data <<= 1;

        GPIO_SetBits(XPT2046_PORT, XPT2046_SCK);
        GPIO_ResetBits(XPT2046_PORT, XPT2046_SCK);
    }
}

// 读取 1 字节数据
uint8_t XPT2046_Read_Byte(void) {
    uint8_t data = 0;
    for (uint8_t i = 0; i < 8; i++) {
        data <<= 1;
        GPIO_SetBits(XPT2046_PORT, XPT2046_SCK);
        if (GPIO_ReadInputDataBit(XPT2046_PORT, XPT2046_MISO)) {
            data |= 0x01;
        }
        GPIO_ResetBits(XPT2046_PORT, XPT2046_SCK);
    }
    return data;
}


int16_t XPT2046_Read_ADC(uint8_t cmd) {
    uint16_t data = 0;

    XPT2046_CS_LOW();  // 选中触摸屏
    XPT2046_Write_Byte(cmd);  // 发送命令

    // 读取 12-bit 数据（高 8 位 + 低 4 位）
    data = XPT2046_Read_Byte();
    data <<= 8;
    data |= XPT2046_Read_Byte();
    data >>= 3;  // 右移 3 位，去掉无效位(0+data+000)

    XPT2046_CS_HIGH();  // 释放触摸屏
    return data;
}

// 读取 X, Y 坐标
void XPT2046_Get_TouchXY(int16_t *x, int16_t *y) {
    *x = (XPT2046_Read_ADC(CMD_X)-330)/(3900.0-330.0)*320.0;
    *y = (XPT2046_Read_ADC(CMD_Y)-200)/3660.0*240;
}
// 判断是否触摸
uint8_t XPT2046_Is_Touched(void) 
{
    if (GPIO_ReadInputDataBit(XPT2046_PORT,XPT2046_IRQ)==1)
		{  
        return 0;  // 没有触摸
    }
    return 1;  // 有触摸
}

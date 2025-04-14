#ifndef __ILI9341_H__
#define __ILI9341_H__



// **软件 SPI 初始化**
void ILI9341_SPI1Hard_INIT(void);

// **SPI 传输 8bit**
uint8_t ILI9341_SPI_Transfer(uint8_t byte) ;

// **发送命令**
void ILI9341_SendCommand(uint8_t cmd) ;

// **发送数据**
void ILI9341_SendData(uint8_t data) ;
// **ILI9341 初始化**
void ILI9341_Init(void) ;
// **设置窗口地址**
void ILI9341_SetAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) ;
// **画点**
void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
// **填充屏幕**
void ILI9341_FillScreen(uint16_t color) ;




#endif

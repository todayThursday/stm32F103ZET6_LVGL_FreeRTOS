/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"                  // Device header
#include "ST_ILI9341.h"

void ILI9341_Init(void)
{
  /* Initialize ILI9341 low level bus layer ----------------------------------*/
   LCD_IO_Init();
	 ILI9341_RESET_LOW();
	 LCD_Delay(100);
	 ILI9341_RESET_HIGH();
   LCD_Delay(100);

   // 初始化显示屏相关设置
   ILI9341_WriteReg(0x36); // 设置扫描方向
   ILI9341_WriteData(0x48); 

   ILI9341_WriteReg(0x3A); // 设置像素格式
   ILI9341_WriteData(0x55); // 16-bit 颜色

   ILI9341_WriteReg(0x11); // 退出休眠
   LCD_Delay(200);

   ILI9341_WriteReg(0x29); // 开启显示
}
// 读取显示屏 ID
uint16_t ILI9341_ReadID(void)
{
  return ((uint16_t)ILI9341_ReadData(LCD_READ_ID4, LCD_READ_ID4_SIZE));
}
// 开启显示
void ILI9341_DisplayOn(void)
{
  ILI9341_WriteReg(LCD_DISPLAY_ON);
}
// 关闭显示
void ILI9341_DisplayOff(void)
{
  ILI9341_WriteReg(LCD_DISPLAY_OFF);
}
// 向 LCD 寄存器写入数据
void ILI9341_WriteReg(uint8_t LCD_Reg)
{
  LCD_IO_WriteReg(LCD_Reg);
}
// 向 LCD 写入数据
void ILI9341_WriteData(uint16_t RegValue)
{
  LCD_IO_WriteData(RegValue);
}
// 从 LCD 读取数据
uint32_t ILI9341_ReadData(uint16_t RegValue, uint8_t ReadSize)
{
  return (LCD_IO_ReadData(RegValue, ReadSize));
}
// 获取 LCD 屏幕宽度
uint16_t ILI9341_GetLcdPixelWidth(void)
{
  return ILI9341_LCD_PIXEL_WIDTH;
}
// 获取 LCD 屏幕高度
uint16_t ILI9341_GetLcdPixelHeight(void)
{
  return ILI9341_LCD_PIXEL_HEIGHT;
}
/* SPI functions */
uint8_t SPI_SendByte(uint8_t data)
{
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);  // 等待发送缓冲区空
    SPI_I2S_SendData(SPI1, data);  // 发送数据
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);	// 等待接收完成
		return SPI_I2S_ReceiveData(SPI1);
}
uint8_t SPI_ReceiveByte(void)
{
    return SPI_SendByte(0xFF);  // 发送 0xFF 触发时钟，同时接收数据
}

/* LCD IO functions */
void LCD_IO_Init(void)
{
	 GPIO_InitTypeDef GPIO_InitStruct;

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

   // 配置 CS、RESET、DC、LED 为推挽输出
   GPIO_InitStruct.GPIO_Pin = ILI9341_CS_PIN | ILI9341_RESET_PIN | ILI9341_DC_PIN | ILI9341_LED_PIN;
   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(ILI9341_GPIO_PORT, &GPIO_InitStruct);
	 // 配置 SCK (PA5)、MOSI (PA7) 为 复用推挽输出
   GPIO_InitStruct.GPIO_Pin = ILI9341_SCK_PIN | ILI9341_MOSI_PIN;
   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;  // 复用推挽输出
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(ILI9341_GPIO_PORT, &GPIO_InitStruct);
   // 配置 MISO
   GPIO_InitStruct.GPIO_Pin = ILI9341_MISO_PIN;
   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(ILI9341_GPIO_PORT, &GPIO_InitStruct);
		
	 SPI_InitTypeDef SPI_InitStruct;

	 SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  // 全双工
	 SPI_InitStruct.SPI_Mode = SPI_Mode_Master;  // 主机模式
	 SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;  // 8 位数据
	 SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;  // 空闲时SCK低电平
	 SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;  // 第一个边沿采样
	 SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;  // 软控制 CS
	 SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  // 4 分频
	 SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;  // 最高位先传输
   
	 SPI_Init(SPI1, &SPI_InitStruct);  // 初始化 SPI1
	 SPI_Cmd(SPI1, ENABLE);  // 使能 SPI1

   // **默认状态**
   ILI9341_CS_HIGH();
   ILI9341_DC_HIGH();
   ILI9341_RESET_HIGH();
   ILI9341_LED_ON();  // 开启 LED 背光


}
// **发送命令**
void ILI9341_SendCommand(uint8_t cmd) {
  ILI9341_DC_LOW();
  ILI9341_CS_LOW();
  SPI_SendByte(cmd);
  ILI9341_CS_HIGH();
}

// **发送数据**
void ILI9341_SendData(uint8_t data) {
  ILI9341_DC_HIGH();
  ILI9341_CS_LOW();
  SPI_SendByte(data);
  ILI9341_CS_HIGH();
}
void LCD_IO_WriteData(uint16_t RegValue)
{
	ILI9341_SendData(RegValue>>8);//高八位
	ILI9341_SendData(RegValue&0xFF);//低八位
}
void LCD_IO_WriteReg(uint8_t Reg)
{
	ILI9341_SendCommand(Reg);
}
uint32_t LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize)
{
	uint32_t result = 0;
	LCD_IO_WriteReg(RegValue);  // 先写寄存器地址
	for (uint8_t i=0;i<ReadSize;i++)
	{
		result=(result<<8)|SPI_ReceiveByte();//读取数据
	}
  return result;


}
void LCD_Delay (uint32_t delay)
{
	for (uint32_t i = 0; i < delay * 1000; i++)
    {
        __NOP();  // 空操作，延时
    }
}

// **设置显示区域**
void ILI9341_SetWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    ILI9341_WriteReg(0x2A);  // 列地址设置
    ILI9341_WriteData(x1 >> 8);  // 起始X高8位
    ILI9341_WriteData(x1 & 0xFF);// 起始X低8位
    ILI9341_WriteData(x2 >> 8);  // 结束X高8位
    ILI9341_WriteData(x2 & 0xFF);// 结束X低8位

    ILI9341_WriteReg(0x2B);  // 行地址设置
    ILI9341_WriteData(y1 >> 8);  // 起始Y高8位
    ILI9341_WriteData(y1 & 0xFF);// 起始Y低8位
    ILI9341_WriteData(y2 >> 8);  // 结束Y高8位
    ILI9341_WriteData(y2 & 0xFF);// 结束Y低8位

    ILI9341_WriteReg(0x2C);  // 准备写入GRAM
	
}

// **画点**
void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    ILI9341_SetWindow(x, y, x, y);
    ILI9341_WriteData(color >> 8);// 发送高字节
    ILI9341_WriteData(color & 0xFF);// 发送低字节
}

// **填充屏幕**
void ILI9341_FillScreen(uint16_t color) {
    ILI9341_SetWindow(0, 0, 239, 319);
    for (uint32_t i = 0; i < 240 * 320; i++) {
        ILI9341_WriteData(color >> 8);
        ILI9341_WriteData(color & 0xFF);
    }
}



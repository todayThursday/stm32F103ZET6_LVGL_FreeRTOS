#include "stm32f10x.h"
#include "myILI9341.h"

// **引脚定义**
#define ILI9341_CS_PIN     GPIO_Pin_4  // 片选
#define ILI9341_RESET_PIN  GPIO_Pin_2  // 复位
#define ILI9341_DC_PIN     GPIO_Pin_3  // 数据/命令选择
#define ILI9341_SCK_PIN    GPIO_Pin_5  // SPI 时钟
#define ILI9341_MISO_PIN   GPIO_Pin_6  // SPI 读取
#define ILI9341_MOSI_PIN   GPIO_Pin_7  // SPI 数据
#define ILI9341_LED_PIN    GPIO_Pin_1  // LED 背光
#define ILI9341_GPIO_PORT  GPIOA       // GPIO 端口

// **控制信号操作**
#define ILI9341_CS_LOW()      GPIO_ResetBits(ILI9341_GPIO_PORT, ILI9341_CS_PIN)
#define ILI9341_CS_HIGH()     GPIO_SetBits(ILI9341_GPIO_PORT, ILI9341_CS_PIN)

#define ILI9341_DC_LOW()      GPIO_ResetBits(ILI9341_GPIO_PORT, ILI9341_DC_PIN)
#define ILI9341_DC_HIGH()     GPIO_SetBits(ILI9341_GPIO_PORT, ILI9341_DC_PIN)

#define ILI9341_RESET_LOW()   GPIO_ResetBits(ILI9341_GPIO_PORT, ILI9341_RESET_PIN)
#define ILI9341_RESET_HIGH()  GPIO_SetBits(ILI9341_GPIO_PORT, ILI9341_RESET_PIN)

#define ILI9341_SCK_LOW()     GPIO_ResetBits(ILI9341_GPIO_PORT, ILI9341_SCK_PIN)
#define ILI9341_SCK_HIGH()    GPIO_SetBits(ILI9341_GPIO_PORT, ILI9341_SCK_PIN)

#define ILI9341_MOSI_LOW()    GPIO_ResetBits(ILI9341_GPIO_PORT, ILI9341_MOSI_PIN)
#define ILI9341_MOSI_HIGH()   GPIO_SetBits(ILI9341_GPIO_PORT, ILI9341_MOSI_PIN)

#define ILI9341_LED_ON()      GPIO_SetBits(GPIOA, ILI9341_LED_PIN)   // LED 亮
#define ILI9341_LED_OFF()     GPIO_ResetBits(GPIOA, ILI9341_LED_PIN) // LED 关

// **SPI 传输 8bit**
uint8_t ILI9341_SPI_Transfer(uint8_t byte) {
    uint8_t received = 0;
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)!=SET){}
		SPI_I2S_SendData(SPI1,byte);
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)!=SET){}
    received=SPI_I2S_ReceiveData(SPI1);
    return received;
}
// **硬件 SPI 初始化**
void ILI9341_SPI1Hard_INIT(void)
{
	ILI9341_LED_ON();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	
	SPI_InitTypeDef SPI_INITStructure;
	SPI_INITStructure.SPI_Mode=SPI_Mode_Master;
	SPI_INITStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	SPI_INITStructure.SPI_DataSize=SPI_DataSize_8b;
	SPI_INITStructure.SPI_FirstBit=SPI_FirstBit_MSB;
	SPI_INITStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_128;
	SPI_INITStructure.SPI_CPOL=SPI_CPOL_Low;
	SPI_INITStructure.SPI_CPHA=SPI_CPHA_1Edge;
	SPI_INITStructure.SPI_NSS=SPI_NSS_Soft;
	SPI_INITStructure.SPI_CRCPolynomial=7;
	SPI_Init(SPI1,&SPI_INITStructure);
  
	SPI_Cmd(SPI1,ENABLE);
	ILI9341_CS_HIGH();
  ILI9341_SCK_LOW();
  ILI9341_RESET_HIGH();
  ILI9341_LED_ON();  // **点亮背光**


}
// **发送命令**
void ILI9341_SendCommand(uint8_t cmd) {
    ILI9341_DC_LOW();
    ILI9341_CS_LOW();
    ILI9341_SPI_Transfer(cmd);
    ILI9341_CS_HIGH();
}

// **发送数据**
void ILI9341_SendData(uint8_t data) {
    ILI9341_DC_HIGH();
    ILI9341_CS_LOW();
    ILI9341_SPI_Transfer(data);
    ILI9341_CS_HIGH();
}

// **ILI9341 初始化**
void ILI9341_Init(void) {
    ILI9341_SPI1Hard_INIT();

    // **执行复位**
    ILI9341_RESET_LOW();
    for (volatile int i = 0; i < 100000; i++);
    ILI9341_RESET_HIGH();
    for (volatile int i = 0; i < 100000; i++);

    // **初始化命令**
    ILI9341_SendCommand(0x36); // 设置扫描方向
    ILI9341_SendData(0x48); 

    ILI9341_SendCommand(0x3A); // 设置像素格式
    ILI9341_SendData(0x55); // 16-bit 颜色

    ILI9341_SendCommand(0x11); // 退出休眠
    for (volatile int i = 0; i < 200000; i++);

    ILI9341_SendCommand(0x29); // 开启显示
}

// **设置窗口地址**
void ILI9341_SetAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    ILI9341_SendCommand(0x2A);
    ILI9341_SendData(x1 >> 8);
    ILI9341_SendData(x1 & 0xFF);
    ILI9341_SendData(x2 >> 8);
    ILI9341_SendData(x2 & 0xFF);

    ILI9341_SendCommand(0x2B);
    ILI9341_SendData(y1 >> 8);
    ILI9341_SendData(y1 & 0xFF);
    ILI9341_SendData(y2 >> 8);
    ILI9341_SendData(y2 & 0xFF);

    ILI9341_SendCommand(0x2C);
}

// **画点**
void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    ILI9341_SetAddress(x, y, x, y);
    ILI9341_SendData(color >> 8);
    ILI9341_SendData(color & 0xFF);
}

// **填充屏幕**
void ILI9341_FillScreen(uint16_t color) {
    ILI9341_SetAddress(0, 0, 239, 319);
    for (uint32_t i = 0; i < 240 * 320; i++) {
        ILI9341_SendData(color >> 8);
        ILI9341_SendData(color & 0xFF);
    }
}


#include "stm32f10x.h"                  // Device header
#include "My_ILI9341.h"

#define ILI9341_CS_PIN     GPIO_Pin_4  // 片选
#define ILI9341_RESET_PIN  GPIO_Pin_2  // 复位
#define ILI9341_DC_PIN     GPIO_Pin_3  // 数据/命令选择
#define ILI9341_SCK_PIN    GPIO_Pin_5  // SPI 时钟
#define ILI9341_MISO_PIN   GPIO_Pin_6  // SPI 读取
#define ILI9341_MOSI_PIN   GPIO_Pin_7  // SPI 数据
#define ILI9341_LED_PIN    GPIO_Pin_1  // LED 背光
#define ILI9341_GPIO_PORT  GPIOA       // GPIO 端口

#define ILI9341_CS_LOW()     GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define ILI9341_CS_HIGH()    GPIO_SetBits(GPIOA, GPIO_Pin_1)
#define ILI9341_DC_LOW()     GPIO_ResetBits(GPIOA, GPIO_Pin_3)
#define ILI9341_DC_HIGH()    GPIO_SetBits(GPIOA, GPIO_Pin_3)
#define ILI9341_RST_LOW()    GPIO_ResetBits(GPIOA, GPIO_Pin_2)
#define ILI9341_RST_HIGH()   GPIO_SetBits(GPIOA, GPIO_Pin_2)

void SPI1_INIT(void)
{
	GPIO_InitTypeDef MYGPIO_INIT;
	SPI_InitTypeDef MYSPI1_INIT;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1,ENABLE);
	
	// 配置CS, RESET, DC,LED引脚为普通输出
	MYGPIO_INIT.GPIO_Pin=ILI9341_CS_PIN | ILI9341_RESET_PIN| ILI9341_DC_PIN | ILI9341_LED_PIN;
	MYGPIO_INIT.GPIO_Mode=GPIO_Mode_Out_PP;
	MYGPIO_INIT.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ILI9341_GPIO_PORT,&MYGPIO_INIT);

	// 配置SCK, MOSI引脚为复用推挽输出（SPI）
	MYGPIO_INIT.GPIO_Pin=ILI9341_SCK_PIN | ILI9341_MOSI_PIN;
	MYGPIO_INIT.GPIO_Mode=GPIO_Mode_AF_PP;
	MYGPIO_INIT.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ILI9341_GPIO_PORT,&MYGPIO_INIT);
	// MISO (PA6) 配置为上拉输入
	MYGPIO_INIT.GPIO_Pin=ILI9341_MISO_PIN;
	MYGPIO_INIT.GPIO_Mode=GPIO_Mode_IPU;
	MYGPIO_INIT.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ILI9341_GPIO_PORT,&MYGPIO_INIT);
	// 配置SPI1
	MYSPI1_INIT.SPI_Direction=SPI_Direction_2Lines_FullDuplex;// 双向全双工
	MYSPI1_INIT.SPI_Mode=SPI_Mode_Master;//主机模式
	MYSPI1_INIT.SPI_DataSize=SPI_DataSize_8b;//8位数据
	MYSPI1_INIT.SPI_CPOL=SPI_CPOL_Low;//SCK空闲时低电平
	MYSPI1_INIT.SPI_CPHA=SPI_CPHA_1Edge;//第一个边沿采样
	MYSPI1_INIT.SPI_NSS=SPI_NSS_Soft;//软件控制NSS(CS)
	MYSPI1_INIT.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_16;//SPI时钟频率
	MYSPI1_INIT.SPI_FirstBit=SPI_FirstBit_MSB;//高位先行
	MYSPI1_INIT.SPI_CRCPolynomial=7;//CRC多项式（一般不用）
	
	SPI_Init(SPI1, &MYSPI1_INIT);  // 初始化SPI1
  SPI_Cmd(SPI1, ENABLE);  // 使能SPI1
	
}

uint8_t MYSPI_SendByte(uint8_t BYTE)
{
	 while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET)// 等待发送就绪
	 SPI_I2S_SendData(SPI1,BYTE);
	
	 while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==RESET)// 等待接收完成
   return ((uint8_t)SPI_I2S_ReceiveData(SPI1));
}

uint8_t MYSPI_ReceiveByte(void) {
    return MYSPI_SendByte(0xFF);  // 发送0xFF触发时钟，同时接收数据
}
//初始化LCD
void ILI9341_INIT(void)
{
	  SPI1_INIT();
	
	 // **执行复位**
		GPIO_SetBits(ILI9341_GPIO_PORT,ILI9341_LED_PIN);
	  GPIO_ResetBits(ILI9341_GPIO_PORT,ILI9341_RESET_PIN);
    for (volatile int i = 0; i < 100000; i++);
    GPIO_SetBits(ILI9341_GPIO_PORT,ILI9341_RESET_PIN);
    for (volatile int i = 0; i < 100000; i++);

    // **初始化命令**
	  ILI9341_WriteCommand(0x01); // 软件复位
    ILI9341_WriteCommand(0x36); // 设置扫描方向
    ILI9341_WriteData(0x48); 

    ILI9341_WriteCommand(0x3A); // 设置像素格式
    ILI9341_WriteData(0x55); // 16-bit 颜色

    ILI9341_WriteCommand(0x11); // 退出休眠
    for (volatile int i = 0; i < 200000; i++);

    ILI9341_WriteCommand(0x29); // 开启显示


}


// 发送命令和数据
void ILI9341_WriteCommand(uint8_t cmd) 
{
    GPIO_ResetBits(ILI9341_GPIO_PORT, ILI9341_DC_PIN);  // DC=0（命令）
    GPIO_ResetBits(ILI9341_GPIO_PORT, ILI9341_CS_PIN);  // CS=0（选中）
    MYSPI_SendByte(cmd);
    GPIO_SetBits(ILI9341_GPIO_PORT, ILI9341_CS_PIN);    // CS=1（取消选中）
}

void ILI9341_WriteData(uint8_t DATA) 
{
    GPIO_SetBits(ILI9341_GPIO_PORT, ILI9341_DC_PIN);    // DC=1（数据）
    GPIO_ResetBits(ILI9341_GPIO_PORT, ILI9341_CS_PIN);  // CS=0（选中）
    MYSPI_SendByte(DATA);
    GPIO_SetBits(ILI9341_GPIO_PORT, ILI9341_CS_PIN);    // CS=1（取消选中）
}
uint8_t ILI9341_ReceiveByte(void) 
{
	  uint8_t DATA;
	  GPIO_ResetBits(ILI9341_GPIO_PORT, ILI9341_DC_PIN);  // DC=0（命令）
    GPIO_ResetBits(ILI9341_GPIO_PORT, ILI9341_CS_PIN);  // CS=0（选中）
	  DATA=MYSPI_SendByte(0xFF);
	  GPIO_SetBits(ILI9341_GPIO_PORT, ILI9341_CS_PIN);    // CS=1（取消选中）
    return DATA;  // 发送0xFF触发时钟，同时接收数据
}
// 从 LCD 读取数据
uint32_t ILI9341_ReadData(uint16_t RegValue, uint8_t ReadSize)
{
	uint32_t result = 0;
	ILI9341_WriteCommand(RegValue);  // 先写寄存器地址
	for (uint8_t i=0;i<ReadSize;i++)
	{
		result=(result<<8)|ILI9341_ReceiveByte();//读取数据
	}
  return result;
}
// 读取显示屏 ID
uint16_t ILI9341_ReadID(void)
{
	// 1. 先发送读取ID命令
    ILI9341_WriteCommand(0xD3);
    
    // 2. 读取3字节数据（ID存储在最后2字节）
    uint8_t data1 = ILI9341_ReceiveByte();
    uint8_t data2 = ILI9341_ReceiveByte();
    uint8_t data3 = ILI9341_ReceiveByte();
    
    // 3. 组合有效数据（通常ID在data2和data3）
    return (data2 << 8) | data3;
}

// 简单延时函数（需要根据系统时钟调整）
void LCD_Delay(uint32_t delay)
{
    for(uint32_t i = 0; i < delay * 1000; i++)
    {
        __NOP();
    }
}

void ILI9341_FillScreen(uint16_t color)
{
    ILI9341_SetWindow(0, 0, 239, 319);
    
    GPIO_SetBits(ILI9341_GPIO_PORT, ILI9341_DC_PIN);  // 数据模式
    GPIO_ResetBits(ILI9341_GPIO_PORT, ILI9341_CS_PIN); // 选中
    
    for(uint32_t i=0; i<240*320; i++) {
        ILI9341_WriteData(color >> 8);    // 高字节
        ILI9341_WriteData(color & 0xFF);  // 低字节
    }
    
    GPIO_SetBits(ILI9341_GPIO_PORT, ILI9341_CS_PIN); // 取消选中
}

// **设置显示区域**
void ILI9341_SetWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    ILI9341_WriteCommand(0x2A);  // 列地址设置
    ILI9341_WriteData(x1 >> 8);  // 起始X高8位
    ILI9341_WriteData(x1 & 0xFF);// 起始X低8位
    ILI9341_WriteData(x2 >> 8);  // 结束X高8位
    ILI9341_WriteData(x2 & 0xFF);// 结束X低8位

    ILI9341_WriteCommand(0x2B);  // 行地址设置
    ILI9341_WriteData(y1 >> 8);  // 起始Y高8位
    ILI9341_WriteData(y1 & 0xFF);// 起始Y低8位
    ILI9341_WriteData(y2 >> 8);  // 结束Y高8位
    ILI9341_WriteData(y2 & 0xFF);// 结束Y低8位

    ILI9341_WriteCommand(0x2C);  // 准备写入GRAM
	
}
















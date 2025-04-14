#include "stm32f10x.h"                  // Device header

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
    *x = XPT2046_Read_ADC(CMD_X);
    *y = XPT2046_Read_ADC(CMD_Y);
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



#include "stm32f10x.h"                  // Device header
#include "MYIIC.h"


#define AS5600_I2C_ADDR  0x36   // 7-bit 地址
#define AS5600_WRITE     (AS5600_I2C_ADDR << 1 | 0)  // 0x6C
#define AS5600_READ      (AS5600_I2C_ADDR << 1 | 1)  // 0x6D

void AS5600_INIT(void)
{
	MYIIC_INIT();
}

void AS5600_WriteReg(uint8_t reg)
{
    MYIIC_START();
    MYIIC_SENDBYTE(AS5600_WRITE);
    MYIIC_RECEIVEACK();
    MYIIC_SENDBYTE(reg);
    MYIIC_RECEIVEACK();
    MYIIC_STOP();
}

uint16_t AS5600_ReadAngle(void)
{
    uint16_t angle = 0;
    AS5600_WriteReg(0x0C); // 发送寄存器地址

    MYIIC_START();
    MYIIC_SENDBYTE(AS5600_READ);
    MYIIC_RECEIVEACK();

    uint8_t highByte = MYIIC_RECEIVEBYTE();
    MYIIC_SENDACK(0); // 发送 ACK
    uint8_t lowByte = MYIIC_RECEIVEBYTE();
    MYIIC_SENDACK(1); // 发送 NACK
    MYIIC_STOP();

    angle = ((uint16_t)highByte << 8) | lowByte;
    angle = angle & 0x0FFF; // 12-bit 角度数据
    return angle;
}


float AS5600_GetActualAngle(void)
{
    uint16_t angle = AS5600_ReadAngle();
    return (angle / 4095.0f) * 360.0f; // 返回实际角度值，范围是 0 到 360°
}

//uint8_t AS5600_ReadMagnetStatus(void)//读取磁场强度
//{
//    AS5600_WriteReg(0x0B); // 发送寄存器地址

//    MYIIC_START();
//    MYIIC_SENDBYTE(AS5600_READ);
//    MYIIC_RECIEVEACK();
//    uint8_t status = MYIIC_RECIEVEBYTE();
//    MYIIC_SENDACK(1);
//    MYIIC_STOP();

//    return status;
//}

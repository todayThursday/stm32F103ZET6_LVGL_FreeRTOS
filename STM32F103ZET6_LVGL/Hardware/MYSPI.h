#ifndef __MYSPI_H__
#define __MYSPI_H__

void MYSPI_W_CS(uint8_t Bitvalue);
void MYSPI_W_SCK(uint8_t Bitvalue);
void MYSPI_W_MOSI(uint8_t Bitvalue);
uint8_t MYSPI_R_MISO(void);
//引脚定义SPI2:SCK-PB13 MISO-PB14(浮空或者上拉) MOSI-PB15 CS片选-PB12；
void MYSPI_INIT(void);
void MYSPI_START(void);
void MYSPI_STOP(void);
uint8_t MYSPI_SWAPBYTE(uint8_t BYTESEND);

#endif

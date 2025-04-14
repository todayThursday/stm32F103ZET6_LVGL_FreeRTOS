#include "stm32f10x.h"
#include "MYSPI.h"

void W25Q128_INIT(void)
{
    MYSPI_INIT();
}
void W25Q128_READID(uint8_t *MID,uint16_t *DID)
{
    MYSPI_START();
    MYSPI_SWAPBYTE(0x9F);
    *MID=MYSPI_SWAPBYTE(0xFF);
    *DID=MYSPI_SWAPBYTE(0xFF);
    *DID<<=8;
    *DID|=MYSPI_SWAPBYTE(0xFF);
    MYSPI_STOP();
}



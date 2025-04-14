#include "stm32f10x.h"                  // Device header
#include "MYPRINTF.h"
#include "MYSRAM.h"
#include "Delay.h"


#define TEST_SIZE      100



// 主测试函数
void SRAM_Test(void)
{
    volatile uint16_t* sram = (volatile uint16_t*)0x68000000;
    
    // 写入已知模式
    sram[0] = 0x55AA;  // 0101 0101 1010 1010
    sram[1] = 0xAA55;  // 1010 1010 0101 0101
    sram[2] = 0x0000;
    sram[3] = 0xFFFF;
    
    // 读取并打印
    printf("SRAM[0]: 0x%04X\n", sram[0]);
    printf("SRAM[1]: 0x%04X\n", sram[1]);
    printf("SRAM[2]: 0x%04X\n", sram[2]);
    printf("SRAM[3]: 0x%04X\n", sram[3]);
    
    // 检查相邻地址是否被污染
    printf("SRAM[4]: 0x%04X\n", sram[4]);
}

void SRAM_AddressLineTest(void)
{
    volatile uint16_t* sram = (volatile uint16_t*)0x68000000;
    
    // 测试地址线A0-A18
    for(uint32_t i = 0; i < 19; i++) {
        uint32_t addr = 1 << i;  // 依次测试每条地址线
        sram[addr] = 0xA55A;     // 写入特定模式
        printf("Addr 0x%08X: Wrote 0xA55A, Read 0x%04X\n", 
              addr, sram[addr]);
        
        // 检查相邻地址是否被污染
        printf("Addr-1: 0x%04X, Addr+1: 0x%04X\n",
              sram[addr-1], sram[addr+1]);
    }
}
int main (void)
{
	MYFSMC_INIT();
	MYPRINTF_INIT();
	
	SRAM_AddressLineTest();


	while(1)
	{


	}
}

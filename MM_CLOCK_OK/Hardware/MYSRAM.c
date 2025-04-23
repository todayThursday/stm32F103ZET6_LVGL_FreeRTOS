#include "stm32f10x.h"                  // Device header
#include "MYSRAM.h"


void MYFSMC_INIT(void)
{
	GPIO_InitTypeDef GPIO_INITSTRUCTURE;

	//开启 FSMC 和 GPIO 时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);
	RCC_APB2PeriphClockCmd(
		RCC_APB2Periph_GPIOD | 
		RCC_APB2Periph_GPIOE |
		RCC_APB2Periph_GPIOF | 
		RCC_APB2Periph_GPIOG,
		ENABLE);

	// D0-D3, D13-D15
	GPIO_INITSTRUCTURE.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_INITSTRUCTURE.GPIO_Pin=
	GPIO_Pin_0 | GPIO_Pin_1  |	GPIO_Pin_8 | 
	GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 |
	GPIO_Pin_15; 
	GPIO_INITSTRUCTURE.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_INITSTRUCTURE);

	 // D4-D12
	GPIO_INITSTRUCTURE.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_INITSTRUCTURE.GPIO_Pin =
	GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9  |
	GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 |
	GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_INITSTRUCTURE.GPIO_Speed=GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_INITSTRUCTURE);
	
	 //A0 - A9
	GPIO_INITSTRUCTURE.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_INITSTRUCTURE.GPIO_Pin =
	GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
	GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |
	GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 |
	GPIO_Pin_9;
    GPIO_Init(GPIOF, &GPIO_INITSTRUCTURE);
	//A0-A15
	GPIO_INITSTRUCTURE.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_INITSTRUCTURE.GPIO_Pin =
	GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
	GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOG, &GPIO_INITSTRUCTURE);
	//A16-A18
	GPIO_INITSTRUCTURE.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_INITSTRUCTURE.GPIO_Pin =
	GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_Init(GPIOD, &GPIO_INITSTRUCTURE);
	//NOE, NWE;
	GPIO_INITSTRUCTURE.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_INITSTRUCTURE.GPIO_Pin =
	GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOD, &GPIO_INITSTRUCTURE);
	//NBL0, NBL1 (LB, UB)
	GPIO_INITSTRUCTURE.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_INITSTRUCTURE.GPIO_Pin =
	GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOE, &GPIO_INITSTRUCTURE);
	//NE3
	GPIO_INITSTRUCTURE.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_INITSTRUCTURE.GPIO_Pin =
	GPIO_Pin_10;
	GPIO_Init(GPIOG, &GPIO_INITSTRUCTURE);


	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  ReadWriteTiming;

    // 配置读/写时序
	/* 地址建立时间，1个HCLK周期, 1/36M = 27ns */
    ReadWriteTiming.FSMC_AddressSetupTime = 1;       // 地址建立时间 1~3，越大越慢，越安全
    /* 地址保持时间，1个HCLK周期 */
	ReadWriteTiming.FSMC_AddressHoldTime = 0;        // 地址保持时间
    /* 数据建立时间，63个HCLK周期 4/72 = 55ns */
	ReadWriteTiming.FSMC_DataSetupTime = 4;          // 数据建立时间	1~4，根据芯片要求
    /* 总线恢复时间设置 */
	ReadWriteTiming.FSMC_BusTurnAroundDuration = 0;	 //用不到时设为 0
    /* 时钟分频设置 */
	ReadWriteTiming.FSMC_CLKDivision = 0;
	/* 数据保持时间，1个HCLK周期 */
    ReadWriteTiming.FSMC_DataLatency = 0;
	/* 设置模式，如果在地址/数据不复用时，ABCD模式都区别不大 */
    ReadWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A; // 访问模式 A~D	SRAM 一般选 Mode A，不要搞复杂了。通常选 A，最通用访问模式 A

    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;   // 使用 NE3
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // 地址数据分离（SRAM 必须关）
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;  // 使用 SRAM
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;  // 16 位数据线
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable; // 不使用突发模式
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable; // 允许写
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable; // 不区分读写时序
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &ReadWriteTiming;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &ReadWriteTiming; // 若启用 ExtendedMode 则用不同的

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); // 初始化 FSMC 控制器

    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);  // 使能 FSMC 的 Bank3
}



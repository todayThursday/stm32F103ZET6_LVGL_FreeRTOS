/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ST_ILI9341_H
#define __ST_ILI9341_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/


#define ILI9341_ID                  0x9341

/** 
  * @brief  ILI9341 Size  
  */  
#define  ILI9341_LCD_PIXEL_WIDTH    ((uint16_t)240)
#define  ILI9341_LCD_PIXEL_HEIGHT   ((uint16_t)320)

/** 
  * @brief  ILI9341 Timing  
  */     
/* Timing configuration  (Typical configuration from ILI9341 datasheet)
  HSYNC=10 (9+1)
  HBP=20 (29-10+1)
  ActiveW=240 (269-20-10+1)
  HFP=10 (279-240-20-10+1)

  VSYNC=2 (1+1)
  VBP=2 (3-2+1)
  ActiveH=320 (323-2-2+1)
  VFP=4 (327-320-2-2+1)
*/
/*IO口初始化定义*/
// **引脚定义**
#define ILI9341_CS_PIN     GPIO_Pin_1  // 片选
#define ILI9341_RESET_PIN  GPIO_Pin_2  // 复位
#define ILI9341_DC_PIN     GPIO_Pin_3  // 数据/命令选择
#define ILI9341_SCK_PIN    GPIO_Pin_5  // SPI 时钟
#define ILI9341_MISO_PIN   GPIO_Pin_6  // SPI 读取
#define ILI9341_MOSI_PIN   GPIO_Pin_7  // SPI 数据
#define ILI9341_LED_PIN    GPIO_Pin_4  // **LED 背光**
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

#define ILI9341_LED_ON()      GPIO_SetBits(ILI9341_GPIO_PORT, ILI9341_LED_PIN)   // LED 亮
#define ILI9341_LED_OFF()     GPIO_ResetBits(ILI9341_GPIO_PORT, ILI9341_LED_PIN) // LED 关

#define ILI9341_W_MOSI(Bitvalue) GPIO_WriteBit(GPIOB,GPIO_Pin_15,(BitAction) (Bitvalue))
#define ILI9341_R_MOSI()      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)



#define  ILI9341_HSYNC            ((uint32_t)9)   /* Horizontal synchronization */
#define  ILI9341_HBP              ((uint32_t)29)    /* Horizontal back porch      */ 
#define  ILI9341_HFP              ((uint32_t)2)    /* Horizontal front porch     */
#define  ILI9341_VSYNC            ((uint32_t)1)   /* Vertical synchronization   */
#define  ILI9341_VBP              ((uint32_t)3)    /* Vertical back porch        */
#define  ILI9341_VFP              ((uint32_t)2)    /* Vertical front porch       */

/** 
  * @brief  ILI9341 Registers  
  */

/* Level 1 Commands */
#define LCD_SWRESET             0x01   /* Software Reset */
#define LCD_READ_DISPLAY_ID     0x04   /* Read display identification information */
#define LCD_RDDST               0x09   /* Read Display Status */
#define LCD_RDDPM               0x0A   /* Read Display Power Mode */
#define LCD_RDDMADCTL           0x0B   /* Read Display MADCTL */
#define LCD_RDDCOLMOD           0x0C   /* Read Display Pixel Format */
#define LCD_RDDIM               0x0D   /* Read Display Image Format */
#define LCD_RDDSM               0x0E   /* Read Display Signal Mode */
#define LCD_RDDSDR              0x0F   /* Read Display Self-Diagnostic Result */
#define LCD_SPLIN               0x10   /* Enter Sleep Mode */
#define LCD_SLEEP_OUT           0x11   /* Sleep out register */
#define LCD_PTLON               0x12   /* Partial Mode ON */
#define LCD_NORMAL_MODE_ON      0x13   /* Normal Display Mode ON */
#define LCD_DINVOFF             0x20   /* Display Inversion OFF */
#define LCD_DINVON              0x21   /* Display Inversion ON */
#define LCD_GAMMA               0x26   /* Gamma register */
#define LCD_DISPLAY_OFF         0x28   /* Display off register */
#define LCD_DISPLAY_ON          0x29   /* Display on register */
#define LCD_COLUMN_ADDR         0x2A   /* Colomn address register */ 
#define LCD_PAGE_ADDR           0x2B   /* Page address register */ 
#define LCD_GRAM                0x2C   /* GRAM register */   
#define LCD_RGBSET              0x2D   /* Color SET */   
#define LCD_RAMRD               0x2E   /* Memory Read */   
#define LCD_PLTAR               0x30   /* Partial Area */   
#define LCD_VSCRDEF             0x33   /* Vertical Scrolling Definition */   
#define LCD_TEOFF               0x34   /* Tearing Effect Line OFF */   
#define LCD_TEON                0x35   /* Tearing Effect Line ON */   
#define LCD_MAC                 0x36   /* Memory Access Control register*/
#define LCD_VSCRSADD            0x37   /* Vertical Scrolling Start Address */   
#define LCD_IDMOFF              0x38   /* Idle Mode OFF */   
#define LCD_IDMON               0x39   /* Idle Mode ON */   
#define LCD_PIXEL_FORMAT        0x3A   /* Pixel Format register */
#define LCD_WRITE_MEM_CONTINUE  0x3C   /* Write Memory Continue */   
#define LCD_READ_MEM_CONTINUE   0x3E   /* Read Memory Continue */   
#define LCD_SET_TEAR_SCANLINE   0x44   /* Set Tear Scanline */   
#define LCD_GET_SCANLINE        0x45   /* Get Scanline */   
#define LCD_WDB                 0x51   /* Write Brightness Display register */
#define LCD_RDDISBV             0x52   /* Read Display Brightness */   
#define LCD_WCD                 0x53   /* Write Control Display register*/
#define LCD_RDCTRLD             0x54   /* Read CTRL Display */   
#define LCD_WRCABC              0x55   /* Write Content Adaptive Brightness Control */   
#define LCD_RDCABC              0x56   /* Read Content Adaptive Brightness Control */   
#define LCD_WRITE_CABC          0x5E   /* Write CABC Minimum Brightness */   
#define LCD_READ_CABC           0x5F   /* Read CABC Minimum Brightness */   
#define LCD_READ_ID1            0xDA   /* Read ID1 */
#define LCD_READ_ID2            0xDB   /* Read ID2 */
#define LCD_READ_ID3            0xDC   /* Read ID3 */

/* Level 2 Commands */
#define LCD_RGB_INTERFACE       0xB0   /* RGB Interface Signal Control */
#define LCD_FRMCTR1             0xB1   /* Frame Rate Control (In Normal Mode) */
#define LCD_FRMCTR2             0xB2   /* Frame Rate Control (In Idle Mode) */
#define LCD_FRMCTR3             0xB3   /* Frame Rate Control (In Partial Mode) */
#define LCD_INVTR               0xB4   /* Display Inversion Control */
#define LCD_BPC                 0xB5   /* Blanking Porch Control register */
#define LCD_DFC                 0xB6   /* Display Function Control register */
#define LCD_ETMOD               0xB7   /* Entry Mode Set */
#define LCD_BACKLIGHT1          0xB8   /* Backlight Control 1 */
#define LCD_BACKLIGHT2          0xB9   /* Backlight Control 2 */
#define LCD_BACKLIGHT3          0xBA   /* Backlight Control 3 */
#define LCD_BACKLIGHT4          0xBB   /* Backlight Control 4 */
#define LCD_BACKLIGHT5          0xBC   /* Backlight Control 5 */
#define LCD_BACKLIGHT7          0xBE   /* Backlight Control 7 */
#define LCD_BACKLIGHT8          0xBF   /* Backlight Control 8 */
#define LCD_POWER1              0xC0   /* Power Control 1 register */
#define LCD_POWER2              0xC1   /* Power Control 2 register */
#define LCD_VCOM1               0xC5   /* VCOM Control 1 register */
#define LCD_VCOM2               0xC7   /* VCOM Control 2 register */
#define LCD_NVMWR               0xD0   /* NV Memory Write */
#define LCD_NVMPKEY             0xD1   /* NV Memory Protection Key */
#define LCD_RDNVM               0xD2   /* NV Memory Status Read */
#define LCD_READ_ID4            0xD3   /* Read ID4 */
#define LCD_PGAMMA              0xE0   /* Positive Gamma Correction register */
#define LCD_NGAMMA              0xE1   /* Negative Gamma Correction register */
#define LCD_DGAMCTRL1           0xE2   /* Digital Gamma Control 1 */
#define LCD_DGAMCTRL2           0xE3   /* Digital Gamma Control 2 */
#define LCD_INTERFACE           0xF6   /* Interface control register */

/* Extend register commands */
#define LCD_POWERA               0xCB   /* Power control A register */
#define LCD_POWERB               0xCF   /* Power control B register */
#define LCD_DTCA                 0xE8   /* Driver timing control A */
#define LCD_DTCB                 0xEA   /* Driver timing control B */
#define LCD_POWER_SEQ            0xED   /* Power on sequence register */
#define LCD_3GAMMA_EN            0xF2   /* 3 Gamma enable register */
#define LCD_PRC                  0xF7   /* Pump ratio control register */


/* Size of read registers */
#define LCD_READ_ID4_SIZE        3      /* Size of Read ID4 */


/** @defgroup ILI9341_Exported_Functions
  * @{
  */ 
void     ILI9341_Init(void);
uint16_t ILI9341_ReadID(void);
void     ILI9341_WriteReg(uint8_t LCD_Reg);
void     ILI9341_WriteData(uint16_t RegValue);
uint32_t ILI9341_ReadData(uint16_t RegValue, uint8_t ReadSize);
void     ILI9341_DisplayOn(void);
void     ILI9341_DisplayOff(void);
uint16_t ILI9341_GetLcdPixelWidth(void);
uint16_t ILI9341_GetLcdPixelHeight(void);


/* LCD IO functions */
void     LCD_IO_Init(void);
void     LCD_IO_WriteData(uint16_t RegValue);
void     LCD_IO_WriteReg(uint8_t Reg);
uint32_t LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize);
void     LCD_Delay (uint32_t delay);
// **设置显示区域**
void ILI9341_SetWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
// **画点**
void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
// **填充屏幕**
void ILI9341_FillScreen(uint16_t color);
void ILI9341_SendCommand(uint8_t cmd);

// **发送数据**
void ILI9341_SendData(uint8_t data);  
#ifdef __cplusplus
}
#endif

#endif 


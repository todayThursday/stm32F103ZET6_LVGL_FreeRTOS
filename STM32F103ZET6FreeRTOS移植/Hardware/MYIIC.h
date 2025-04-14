#ifndef __MYIIC_H__
#define __MYIIC_H__

void MYIIC_W_SCL(uint8_t BitValue);
void MYIIC_W_SDA(uint8_t BitValue);
uint8_t MYIIC_R_SDA(void);
void MYIIC_INIT(void);
void MYIIC_START(void);
void MYIIC_STOP(void);
void MYIIC_SENDBYTE(uint8_t BYTE);
uint8_t MYIIC_RECEIVEBYTE(void);
void MYIIC_SENDACK(uint8_t ACKBIT);
uint8_t MYIIC_RECEIVEACK(void);

#endif

#ifndef __AS5600_H__
#define __AS5600_H__


void AS5600_INIT(void);
void AS5600_WriteReg(uint8_t reg);
uint16_t AS5600_ReadAngle(void);

//uint8_t AS5600_ReadMagnetStatus(void);



#endif

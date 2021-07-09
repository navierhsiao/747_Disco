#ifndef __I2C_H
#define __I2C_H

void I2C_Init(void);
void I2C_WriteReg(uint16_t deviceAddr,uint16_t reg,uint8_t *pData,uint16_t length);
void I2C_ReadReg(uint16_t deviceAddr,uint16_t reg,uint8_t *pData,uint16_t length);
void I2C_WriteReg16(uint16_t deviceAddr, uint16_t reg, uint8_t *pData, uint16_t length);
void I2C_ReadReg16(uint16_t deviceAddr, uint16_t reg, uint8_t *pData, uint16_t length);
void I2C_IsReady(uint16_t deviceAddr, uint32_t trials);

#endif
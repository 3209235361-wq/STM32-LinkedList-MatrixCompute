#ifndef __SERIAL_H__
#define __SERIAL_H__
#include"stm32f10x.h"
extern char Serial_RxPacket[];
extern uint8_t Serial_RxFlag;
void SerialInit(void);
void SerialSendByte(uint8_t byte);
void SerialSendString(uint8_t *str);
void SerialSendNumber(uint32_t num,uint16_t len);
void SerialSendFloat(float num, uint16_t decimal_places);
uint8_t GetNumberDigits(uint32_t num);
void Serial_SendArray(uint8_t*str,int len);
#endif
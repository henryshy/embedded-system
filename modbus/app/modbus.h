#ifndef _MODBUS_H
#define _MODBUS_H

#include "main.h"
uint16_t crc_check(uint8_t Rx[],uint8_t length);

void modbus(uint8_t Rx[],uint8_t length);
void func_error(uint8_t Rx[],uint8_t length);
void addr_amt_error(uint8_t Rx[],uint8_t length);
void amt_error(uint8_t Rx[],uint8_t length);
void sw_flag_error(uint8_t Rx[],uint8_t length);
void value_error(uint8_t Rx[],uint8_t length);

void modbus_0x01(uint8_t Rx[],uint8_t length);
void modbus_0x02(uint8_t Rx[],uint8_t length);
void modbus_0x03(uint8_t Rx[],uint8_t length);
void modbus_0x04(uint8_t Rx[],uint8_t length);
void modbus_0x05(uint8_t Rx[],uint8_t length);
void modbus_0x06(uint8_t Rx[],uint8_t length);
void modbus_0x0f(uint8_t Rx[],uint8_t length);

#endif

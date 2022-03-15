#pragma once

#include "twi.h"

uint8_t getRegister ( uint8_t i2cAddress, uint8_t registerAddress );
void setRegister (uint8_t i2cAddress, uint8_t registerAddress, uint8_t valueToSet);
uint8_t displayRegister (uint8_t i2cAddress, char * label, uint8_t registerAddress);
void byteToBinaryString ( char * buf, byte b );

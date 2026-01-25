#ifndef _BSP_EEPROM_H
#define _BSP_EEPROM_H

#include "stdint.h"

uint8_t read_eeprom(uint8_t *_ucpBuf, uint16_t _usAddress, uint16_t _usSize);

uint8_t write_eeprom(uint8_t *_ucpBuf, uint16_t _usAddress, uint16_t _usSize);

#endif


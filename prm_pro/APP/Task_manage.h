#ifndef _TASK_MANAGE_H
#define _TASK_MANAGE_H

#include "stdint.h"

enum EEPROM_SET_TYPE
{
    EEPROM_SET_BASIC_PARAM = 1,
    EEPROM_SET_WARN_RANGE = 0x02,
    EEPROM_SET_DISPLAY_UNIT = 0x04,
    EEPROM_SET_INT_DOESE_PARAM = 0x08,
    EEPROM_SET_DETECTOR_TYPE = 0x10,
    EEPROM_SET_DETECTOR_PARAM = 0x20,
    EEPROM_SET_PASSWORD         = 0x40,
};

#define EEPROM_ADDR_FLAG            1
#define EEPROM_ADDR_BASIC_PARAM     10
#define EEPROM_ADDR_WARN_RANGE      20
#define EEPROM_ADDR_DISPLATY_UNIT   30
#define EEPROM_ADDR_INT_DOSE_PARAM  40
#define EEPROM_ADDR_DETECTOR_TYPE   80
#define EEPROM_ADDR_DETECTOR_PARAM  90
#define EEPROM_ADDR_PASSWORD        120
#define EEPROM_ADDR                 130


void eepromSetStatus(uint32_t setType);
void Task_manage(void);

void SYS_Param_Init(void);

#endif


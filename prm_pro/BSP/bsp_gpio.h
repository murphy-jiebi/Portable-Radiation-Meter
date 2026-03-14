#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H

#include <stdint.h>

void BSP_GPIO_Init(void);

void PWR_shutdown(void);

void BEEP_SetState(uint8_t state);

void LED_SetState(uint8_t state);

uint8_t CHG_plugCheck(void);

uint8_t CHG_GetChgState(void);

void CHG_SetChg(uint8_t state);

#endif



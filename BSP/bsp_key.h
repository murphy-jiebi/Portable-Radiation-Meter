#ifndef _BSP_KEY_H
#define _BSP_KEY_H

#include <stdint.h>

typedef enum
{
	KEY_NONE = 0,
	
	KEY_ENTER_CLICK,
	KEY_BACK_CLICK,
	KEY_UP_CLICK,
	KEY_DOWN_CLICK,
	KEY_ENTER_LONG,
	KEY_BACK_LONG,
	KEY_UP_LONG,
	KEY_DOWN_LONG,
	KEY_ENTER_DC,  //DC:double click
	KEY_BACK_DC,
	KEY_UP_DC,
	KEY_DOWN_DC,
	
}KEY_STATUS_E;


void BSP_Key_Init(void);

uint8_t bsp_GetKey(void);

void bsp_KeyScan(void);

#endif

#ifndef _BSP_LCD_H
#define _BSP_LCD_H

#include <stdint.h>

void SetLcdBacklight(uint8_t _ucRate);

void LCD_Init(void);

void LCD_clear(uint8_t color);

void LCD_Fill(uint8_t color,uint8_t x_start, uint8_t x_end, uint8_t y_start, uint8_t y_end);

void LCD_set_window(uint8_t x_start, uint8_t x_end, uint8_t y_start, uint8_t y_end);

void LCD_FillData(uint8_t *data);

#endif

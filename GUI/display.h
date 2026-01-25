#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <stdint.h>

void Display_bat(uint8_t x,uint8_t y);
void display_time(uint8_t x,uint8_t y);
void Display_Chg(uint8_t x,uint8_t y);

void Display_doseVal(uint8_t x, uint8_t y,float val, uint8_t unit);
void Display_dose(uint8_t x, uint8_t y,float val,uint8_t unit);

void Display_statusBar(void);

void Display_setWarnVal(uint8_t x, uint8_t y, uint8_t val[6], uint8_t index);

uint8_t GetDoseUnit(float dose);

void Display_unitParam(uint8_t x, uint8_t y, uint8_t unit);
void Display_brightness(uint8_t x, uint8_t y, uint8_t brightness, uint8_t mode);

    void Display_scaleBar(uint8_t x, uint8_t y, uint16_t cur);

void Display_ProgressBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t bar);

#endif


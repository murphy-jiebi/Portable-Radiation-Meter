#ifndef _GUI_H
#define _GUI_H

#include <stdint.h>

void GUI_Refresh(void);

void GUI_SetPixel(uint8_t x, uint8_t y);
void GUI_SetLine(uint8_t x, uint8_t y, uint8_t length, uint8_t dir);
void GUI_SetRect(uint8_t x, uint8_t y, uint8_t length, uint8_t height);
void GUI_SetFillRect(uint8_t x, uint8_t y, uint8_t length, uint8_t height);

void GUI_SetString1608(uint8_t x,uint8_t y,uint8_t *str,uint8_t mode);

void GUI_SetChar2412(uint8_t x,uint8_t y,uint8_t c,uint8_t mode);
void GUI_SetString2412(uint8_t x,uint8_t y,uint8_t *str,uint8_t mode);

void GUI_SetChar3216(uint8_t x,uint8_t y,uint8_t c, uint8_t mode);
void GUI_SetString3216(uint8_t x,uint8_t y,uint8_t *str, uint8_t mode);

void GUI_SetChar4824(uint8_t x,uint8_t y,uint8_t c, uint8_t mode);
void GUI_SetString4824(uint8_t x,uint8_t y,uint8_t *str, uint8_t mode);

void GUI_SetChar1616(uint8_t x, uint8_t y, char *c, uint8_t mode);
void GUI_SetString1616(uint8_t x, uint8_t y, char *str, uint8_t mode);

void GUI_SetChar2424(uint8_t x, uint8_t y, char *c, uint8_t mode);
void GUI_SetString2424(uint8_t x, uint8_t y, char *str, uint8_t mode);


void GUI_SetString5628(uint8_t x,uint8_t y,uint8_t *str,uint8_t mode);

void GUI_SetBat(uint8_t x, uint8_t y, uint8_t per, uint8_t charge);

void GUI_SetChgPic(uint8_t x, uint8_t y, uint8_t mode);

#endif


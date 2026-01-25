#include "gui.h"
#include "bsp_lcd.h"
#include "string.h"
#include "font.h"
#include "font1.h"

#define LCD_WIDTH 		240
#define LCD_HEIGHT 		160

uint8_t ramBuf[LCD_WIDTH*LCD_HEIGHT/8] = {0};


void GUI_Refresh(void)
{
	LCD_FillData(ramBuf);
	memset(ramBuf,0,sizeof(ramBuf));
}

void GUI_SetPixel(uint8_t x, uint8_t y)
{
	uint8_t page = y/8;
	uint8_t offset = y%8;
	
	ramBuf[x+page*LCD_WIDTH] |= (0x01 << offset);
}

/* 
dir 0:horizontal
	1:vertical
*/
void GUI_SetLine(uint8_t x, uint8_t y, uint8_t length, uint8_t dir)
{
	uint8_t page = y/8;
	uint8_t offset = y%8;
	uint8_t offset1 = (y+length)%8;
	uint8_t i;
	
	if(dir==0)
	{
		for(i=0;i<length;i++)
		{
			ramBuf[x+i+page*LCD_WIDTH] |= (0x01 << offset);
		}
	}else{
		for(i=offset;(i<8)&&(i<length);i++)
		{
			ramBuf[x+page*LCD_WIDTH] |= (0x01 << i);
		}
		
		for(i=0;(i<((length-8+offset)/8))&&(length>8);i++)
		{
			ramBuf[x+(page+i+1)*LCD_WIDTH] |= 0xFF;
		}
		
		for(i=0;i<offset1;i++)
		{
			ramBuf[x+((y+length)/8)*LCD_WIDTH] |= (0x01 << i);
		}
	}
}

void GUI_SetRect(uint8_t x, uint8_t y, uint8_t length, uint8_t height)
{
	GUI_SetLine(x,y,length,0);
	GUI_SetLine(x,y,height,1);
	
	GUI_SetLine(x,y+height,length,0);
	GUI_SetLine(x+length ,y,height+1,1);
}

void GUI_SetFillRect(uint8_t x, uint8_t y, uint8_t length, uint8_t height)
{
	for(uint8_t i=0;i<height;i++)
	{
		GUI_SetLine(x,y+i,length,0);
	}
}

/*only support y is mul 8,*/
void GUI_SetChar1608(uint8_t x,uint8_t y,uint8_t c,uint8_t mode)
{
	uint8_t height = 2;
	uint8_t width = 8;
	
	uint8_t page = y/8;
	
	uint8_t i,j;
	// if((x+width) >= LCD_WIDTH)
	// {
	// 	return ;
	// }
	if(c < 10)
	{
		c +='0';
	}
	for(i=0;i<height;i++)
	{
		for (j = 0; (j < width) && ((x + j) < (LCD_WIDTH - 1)); j++)
		{
			if(mode == 0)
			{
				ramBuf[x+j+(page+i)*LCD_WIDTH] |= ascii1608[(c-0x20)*(height*width)+width*i+j];
			}else{
				ramBuf[x+j+(page+i)*LCD_WIDTH] |= ~ascii1608[(c-0x20)*(height*width)+width*i+j];
			}
		}
	}
}

void GUI_SetString1608(uint8_t x,uint8_t y,uint8_t *str,uint8_t mode)
{
	uint8_t height = 2;
	uint8_t width = 8;
	
	while(*str != '\0')
	{
		GUI_SetChar1608(x,y,*str++,mode);
		x += width;
	}
}

void GUI_SetChar2412(uint8_t x,uint8_t y,uint8_t c,uint8_t mode)
{
	uint8_t height = 3;
	uint8_t width = 12;
	
	uint8_t page = y/8;
	uint8_t i,j;
	// if((x+width) >= LCD_WIDTH)
	// {
	// 	return ;
	// }
	if(c < 10)
	{
		c +='0';
	}
	for(i=0;i<height;i++)
	{
		for (j = 0; (j < width) && ((x + j) < (LCD_WIDTH - 1)); j++)
		{
			if(mode == 0)
			{
				ramBuf[x+j+(page+i)*LCD_WIDTH] |= ascii2412[(c-0x20)*(height*width)+width*i+j];
			}else{
				ramBuf[x+j+(page+i)*LCD_WIDTH] |= ~ascii2412[(c-0x20)*(height*width)+width*i+j];
			}
		}
	}
}

void GUI_SetString2412(uint8_t x,uint8_t y,uint8_t *str,uint8_t mode)
{
	uint8_t height = 3;
	uint8_t width = 12;
	
	while(*str != '\0')
	{
		GUI_SetChar2412(x,y,*str++,mode);
		x += width;
	}
}

void GUI_SetChar3216(uint8_t x,uint8_t y,uint8_t c,uint8_t mode)
{
	uint8_t height = 4;
	uint8_t width = 16;
	
	uint8_t page = y/8;
	uint8_t i,j;
	// if((x+width) >= LCD_WIDTH)
	// {
	// 	return ;
	// }
	if(c < 10)
	{
		c +='0';
	}
	for(i=0;i<height;i++)
	{
		for (j = 0; (j < width) && ((x + j) < (LCD_WIDTH - 1)); j++)
		{
			if(mode == 0)
			{
				ramBuf[x+j+(page+i)*LCD_WIDTH] |= ascii3216[(c-0x20)*(height*width)+width*i+j];
			}else{
				ramBuf[x+j+(page+i)*LCD_WIDTH] |= ~ascii3216[(c-0x20)*(height*width)+width*i+j];
			}
		}
	}
}

void GUI_SetString3216(uint8_t x,uint8_t y,uint8_t *str,uint8_t mode)
{
	uint8_t height = 4;
	uint8_t width = 16;
	
	while(*str != '\0')
	{
		GUI_SetChar3216(x,y,*str++,mode);
		x += width;
	}
}

void GUI_SetChar4824(uint8_t x,uint8_t y,uint8_t c,uint8_t mode)
{
	uint8_t height = 6;
	uint8_t width = 24;
	
	uint8_t page = y/8;
	uint8_t i,j;
	
	// if((x+width) >= LCD_WIDTH)
	// {
	// 	return ;
	// }
	
	if(c == 0x20)
	{
		c = 11;
	}
	if(c>0x20)
	{
		c -=0x30;
	}
	
	for(i=0;i<height;i++)
	{
		for (j = 0; (j < width) && ((x + j) < (LCD_WIDTH - 1)); j++)
		{
			if(mode == 0)
			{
				ramBuf[x+j+(page+i)*LCD_WIDTH] |= ascii4824[(c)*(height*width)+width*i+j];
			}else{
				ramBuf[x+j+(page+i)*LCD_WIDTH] |= ~ascii4824[(c)*(height*width)+width*i+j];
			}
		}
	}
}

void GUI_SetString4824(uint8_t x,uint8_t y,uint8_t *str,uint8_t mode)
{
	uint8_t height =6;
	uint8_t width = 24;
	
	while(*str != '\0')
	{
		GUI_SetChar4824(x,y,*str++,mode);
		x += width;
	}
}

void GUI_SetBat(uint8_t x, uint8_t y, uint8_t per,uint8_t charge)
{
	uint8_t height =2;
	uint8_t width = 26;
	
	uint8_t page = y/8;
	uint8_t i,j;

	if ((x + width) >= LCD_WIDTH)
	{
		return;
	}

	if(per > 10)
	{
		per = 10;
		
	}

	if(charge)
	{
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				ramBuf[x + j + (page + i) * LCD_WIDTH] |= batpic[(0) * (height * width) + width * i + j];
			}
			for (j = 0; j < 16; j++)
			{
				ramBuf[x + 5 + j + (page + i) * LCD_WIDTH] |= chargePic[(1) * (height * 16) + 16 * i + j];
			}
		}
		return;
	}
	for(i=0;i<height;i++)
	{
		memcpy(&ramBuf[x+(page+i)*LCD_WIDTH],&batpic[(per)*(height*width)+width*i],width);
	}

}

void GUI_SetChgPic(uint8_t x, uint8_t y, uint8_t mode)
{
	uint8_t height =2;
	uint8_t width = 16;
	
	uint8_t page = y/8;
	uint8_t i,j;
	
	if(mode > 1)
	{
		mode = 1;
	}
	if((x+width) >= LCD_WIDTH)
	{
		return ;
	}
	for(i=0;i<height;i++)
	{
		memcpy(&ramBuf[x+(page+i)*LCD_WIDTH],&chargePic[(mode)*(height*width)+width*i],width);
	}
}

uint8_t HZ16_to_index(char *hz)
{
	uint16_t hz_code=0;
	hz_code=(hz[0]<<8)|hz[1];
	switch(hz_code)
	{
		case 0xA6C3:
			return 0;
		case 0xC0DB:
			return 1;
		case 0xBCC6:
			return 2;
		case 0xBCC1:
			return 3;
		case 0xC1BF:
			return 4;
		case 0xC2CA:
			return 5;
		case 0xC4DA:
			return 6;
		case 0xCDE2:
			return 7;
		case 0xB2BF:
			return 8;
		case 0xC9E8:
			return 9;
		case 0xD6C3:
			return 10;
		case 0xB2E2:
			return 11;
		case 0xCDA8:
			return 12;
		case '信':
			return 13;
		case '系':
			return 14;
		case '统':
			return 15;
		case '历':
			return 16;
		case '史':
			return 17;
		case '数':
			return 18;
		case '据':
			return 19;
		case '用':
			return 20;
		case '户':
			return 21;
		case '报':
			return 22;
		case '警':
			return 23;
		case '确':
			return 24;
		case '认':
			return 25;
		case '返':
			return 26;
		case '回':
			return 27;
		case '按':
			return 28;
		case '键':
			return 29;
		case '音':
			return 30;
		case '日':
			return 31;
		case '期':
			return 32;
		case '时':
			return 33;
		case '间':
			return 34;
		case '背':
			return 35;
		case '光':
			return 36;
		case '亮':
			return 37;
		case '度':
			return 38;
		case '开':
			return 39;
		case '关':
			return 40;
		case '单':
			return 41;
		case '位':
			return 42;
		case '探':
			return 43;
		case '型':
			return 44;
		case '器':
			return 45;
		case '类':
			return 46;
		case '重':
			return 47;
		case '阈':
			return 48;
		case '值':
			return 49;
		case '存':
			return 50;
		case '储':
			return 51;
		case '查':
			return 52;
		case '询':
			return 53;
		case '清':
			return 54;
		case '除':
			return 55;
		case '空':
			return 56;
		case '修':
			return 57;
		case '改':
			return 58;
		case '密':
			return 59;	
		case '码':
			return 60;
		default:return 0;
	}
}

uint8_t HZ24_to_index(char *hz)
{
	uint16_t hz_code=0;
	hz_code=(hz[0]<<8)|hz[1];
	switch(hz_code)
	{
		case 0xA6C3:
			return 0;
		case 0xC0DB:
			return 1;
		case 0xBCC6:
			return 2;
		case 0xBCC1:
			return 3;
		case 0xC1BF:
			return 4;
		case 0xC2CA:
			return 5;
		case 0xE3D0:
			return 6;
		case 0xD6B5:
			return 7;
		case 0xC8B7:
			return 8;
		case 0xC8CF:
			return 9;
		case 0xB7B5:
			return 10;
		case 0xBBD8:
			return 11;
		case 0xB5A5:
			return 12;
		case 0xCEBB:
			return 13;
		case 0xCAB2:
			return 14;
		case 0xBCE4:
			return 15;
		case 0xB1B3:
			return 16;
		case 0xB9E2:
			return 17;
		case 0xC7E5:
			return 18;
		case 0xB3FD:
			return 19;
		case 0xB4E6:
			return 20;
		case 0xB4A2:
			return 21;
		case 0xC4DA:
			return 22;
		case 0xB2BF:
			return 23;
		case 0xCDE2:
			return 24;
		case 0xC1C1:
			return 25;
		case 0xB6C8:
			return 26;
		case 0xBBFA:
			return 27;
		case 0xD0C5:
			return 28;
		case 0xCFA2:
			return 29;
		case 0xB1BE:
			return 30;
		case 0xC9E8:
			return 31;
		case 0xD6C3:
			return 32;
		case 0xCDB3:
			return 33;
		case 0xCFB5:
			return 34;
		case 0xB1E0:
			return 35;
		case 0xBAC5:
			return 36;
		case 0xB0E6:
			return 37;
		case 0xB3CC:
			return 38;
		case 0xA3BF:
			return 39;
		case 0xD2D1:
			return 40;
		case '请':
			return 41;
		case '输':
			return 42;
		case '入':
			return 43;
		case '密':
			return 44;
		case '码':
			return 45;
		case '基':
			return 46;
		case '高':	
			return 47;
		case '级':
			return 48;
		default:return 0;
	}
}

void GUI_SetChar1616(uint8_t x,uint8_t y,char *c,uint8_t mode)
{
	uint8_t height = 2;
	uint8_t width = 16;
	
	uint8_t page = y/8;
	
	uint8_t i,j;

	for(i=0;i<height;i++)
	{
		for (j = 0; (j < width) && ((x + j) < (LCD_WIDTH - 1)); j++)
		{
			if(mode == 0)
			{
				ramBuf[x+j+(page+i)*LCD_WIDTH] |= hanzi16[(HZ16_to_index(c))*(height*width)+width*i+j];
			}else{
				ramBuf[x+j+(page+i)*LCD_WIDTH] |= ~hanzi16[(HZ16_to_index(c))*(height*width)+width*i+j];
			}
		}
	}
}

void GUI_SetString1616(uint8_t x, uint8_t y, char *str,uint8_t mode)
{
	uint8_t height = 2;
	uint8_t width = 16;
	while(*str != '\0')
	{
		if((*(str+0) < 0x80))
		{
			GUI_SetChar1608(x, y, *str, mode);
			str += 1;
			x += width/2;
		}else{
			GUI_SetChar1616(x, y, str, mode);
			str += 2;
			x += width;
		}
	}
}

void GUI_SetChar2424(uint8_t x,uint8_t y,char *c, uint8_t mode)
{
	uint8_t height = 3;
	uint8_t width = 24;
	
	uint8_t page = y/8;
	
	uint8_t i,j;

	for(i=0;i<height;i++)
	{
		for (j = 0; (j < width) && ((x + j) < (LCD_WIDTH - 1)); j++)
		{
			if(mode == 0)
			{
				ramBuf[x+j+(page+i)*LCD_WIDTH] |= hanzi24[(HZ24_to_index(c))*(height*width)+width*i+j];
			}else{
				ramBuf[x+j+(page+i)*LCD_WIDTH] |= ~hanzi24[(HZ24_to_index(c))*(height*width)+width*i+j];
			}
		}
	}
}

void GUI_SetString2424(uint8_t x, uint8_t y, char *str, uint8_t mode)
{
	uint8_t height = 3;
	uint8_t width = 24;
	while(*str != '\0')
	{
		if((*(str+0) < 0x80))
		{
			GUI_SetChar2412(x, y, *str, mode);
			str += 1;
			x += width/2;
		}else{
			GUI_SetChar2424(x,y,str,mode);
			str += 2;
			x += width;
		}
		
	}
}

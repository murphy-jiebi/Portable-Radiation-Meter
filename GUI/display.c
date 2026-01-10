#include "display.h"
#include "gui.h"
#include "string.h"
#include "bsp_rtc.h"

extern float warnRangeRate ;
extern float warnRangeDose ;
extern uint8_t batVoltPerSmooth;
uint8_t CHG_plugCheck(void);

void Display_bat(uint8_t x,uint8_t y)
{
	char arry[10]={0};
	
	GUI_SetBat(213,0,batVoltPerSmooth/10);
	sprintf(arry,"%3d%%",batVoltPerSmooth);
	GUI_SetString1608(181,0,arry);
}

void Display_Chg(uint8_t x,uint8_t y)
{
	GUI_SetChgPic(x,y,CHG_plugCheck());
}

void display_time(uint8_t x,uint8_t y)
{
	char arry[10]={0};
	uint8_t time[7] = {0};

	Get_RTC_Time(time);
	
	sprintf(arry,"%02d:%02d:%02d",time[2],time[1],time[0]);
	GUI_SetString1608(x,y,arry);
	
	sprintf(arry,"%02d-%02d-%02d",time[5],time[4],time[3]);
	GUI_SetString1608(x,y+24,arry);
}

void Display_dose(uint8_t x, uint8_t y,float val,uint8_t unit)
{
	uint8_t valInt = 0;
	uint8_t valFra = 0;
	uint8_t offset = 0;
	char arry[10]={0};
	
	if(val > 999.99f)
	{
		val /= 1000.0f;
	}
	
	sprintf(arry,"%3.2f",val);
	
	GUI_SetString2412(x+(6-strlen(arry))*6,y,arry);
	
	switch(unit)
	{
		case 0:
			GUI_SetString2412(x+24,y+24,"uSv/h");
			break;
		case 1:
			GUI_SetString2412(x+24,y+24,"mSv/h");
			break;
		case 2:
			GUI_SetString2412(x+24,y+24,"uSv");
			break;
		case 3:
			GUI_SetString2412(x+24,y+24,"mSv");
			break;
		default :break;
	}
}

void Display_doseVal(uint8_t x, uint8_t y,float val, uint8_t unit)
{
	uint16_t valInt = 0;
	uint8_t valFra = 0;
	uint8_t offset = 0;
	
	char arry[10]={0};
	
	if(val > 999.99f)
	{
		val /= 1000.0f;
	}
	
	valInt = (uint32_t)(val*1);
	valFra = (uint32_t)(val*100)%100;
	
	switch(unit)
	{
		case 0:
			GUI_SetString2412(x+70,y+60,"uSv/h");
			break;
		case 1:
			GUI_SetString2412(x+70,y+60,"mSv/h");
			break;
		case 2:
			GUI_SetString2412(x+94,y+60,"uSv");
			break;
		case 3:
			GUI_SetString2412(x+94,y+60,"mSv");
			break;
		default :break;
	}
	
	if(valInt > 99)
	{
		offset = 0;
	}else if(valInt > 9)
	{
		offset = 1;
	}else{
		offset = 2;
	}
	
	sprintf(arry,"%3d",valInt);
	GUI_SetString4824(x+12*offset,y,&arry[offset]);
	
	sprintf(arry,"%02d",valFra);
	GUI_SetString4824(x+84-12*offset,y,arry);
	
	GUI_SetChar2412(x+72-12*offset,y+16,'.');
}


void Display_statusBar(void)
{
	
	Display_bat(0,0);
	Display_Chg(164,0);
	
	char arry[10]={0};
	uint8_t time[7] = {0};

	Get_RTC_Time(time);
	
	sprintf(arry,"%02d:%02d:%02d",time[2],time[1],time[0]);
	GUI_SetString1608(0,0,arry);
	
}


void Display_setWarnVal(uint8_t x, uint8_t y, uint8_t val[6], uint8_t index)
{
	uint8_t width = 0;
	
	GUI_SetChar4824(x+24*0,y,val[0]);
	GUI_SetChar4824(x+24*1,y,val[1]);
	GUI_SetChar4824(x+24*2,y,val[2]);
	GUI_SetChar2412(x+72,y+16,'.');
	GUI_SetChar4824(x+84,y,val[3]);
	GUI_SetChar4824(x+108,y,val[4]);
	
	switch(val[5])
	{
		case 0:
			GUI_SetString3216(x+26,y+60,"uSv/h");
			width = 26;
			break;
		case 1:
			GUI_SetString3216(x+26,y+60,"mSv/h");
			width = 26;
			break;
		case 2:
			GUI_SetString3216(x+36,y+60,"uSv");
			width = 36;
			break;
		case 3:
			GUI_SetString3216(x+36,y+60,"uSv");
			width = 36;
			break;
		default:
			break;
	}
	
	switch(index)
	{
		case 0:
			GUI_SetLine(x,y+48,24,0);
			break;
		case 1:
			GUI_SetLine(x+24,y+48,24,0);
			break;
		case 2:
			GUI_SetLine(x+48,y+48,24,0);
			break;
		case 3:
			GUI_SetLine(x+84,y+48,24,0);
			break;
		case 4:
			GUI_SetLine(x+108,y+48,24,0);
			break;
		case 5:
			if(width == 26)
			{
				GUI_SetLine(x+width,y+92,16*5,0);
			}else{
				GUI_SetLine(x+width,y+92,16*3,0);
			}
			break;
		default:
			break;
	}
}

uint8_t GetDoseUnit(float dose)
{
	if(dose > 999.99f)
	{
		return 1;
	}else{
		return 0;
	}
}

void Display_unitParam(uint8_t x, uint8_t y, uint8_t unit)
{
	switch(unit)
	{
		case 0:
			GUI_SetRect(x-1,y-1,62,26);
			break;
		case 1:
			GUI_SetRect(x-1,y-1+32,62,26);
			break;
		case 2:
			GUI_SetRect(x-1,y-1+64,62,26);
			break;
		default:
			break;
	}
	
	GUI_SetString2412(x+12,y,"cps");
	GUI_SetString2412(x,y+32,"uSv/h");
	GUI_SetString2412(x,y+64,"uGy/h");
	
	
}

void Display_brightness(uint8_t x, uint8_t y, uint8_t brightness)
{
	uint8_t i = 0;
	char arry[10]={0};
	
	GUI_SetRect(x,y,20,101);
	
	for(i = 0; i< brightness; i++)
	{
		
		GUI_SetLine(x,y+100-i,20,0);
	}
	
	sprintf(arry,"%3d",brightness);
	GUI_SetString2412(x+30,y+40,arry);
	
//	GUI_SetRect(x+30-1,y+40-1,38,28);
}

void Display_scaleBar(uint8_t x, uint8_t y,uint16_t cur)
{
	uint8_t len =0;
	
	GUI_SetLine(x+4,y+10,152,0);
	GUI_SetLine(x+4,y+4,14,1);
	GUI_SetLine(x+4+38,y+4,14,1);
	GUI_SetLine(x+4+38*2,y+4,14,1);
	GUI_SetLine(x+4+38*3,y+4,14,1);
	GUI_SetLine(x+4+38*4,y+4,14,1);
	
	GUI_SetLine(x+4+19,y+10,4,1);
	GUI_SetLine(x+4+19+38,y+10,4,1);
	GUI_SetLine(x+4+19+38*2,y+10,4,1);
	GUI_SetLine(x+4+19+38*3,y+10,4,1);
	
	GUI_SetString1608(x,y+24,"0");
	GUI_SetString1608(x+38-8,y+24,"10");
	GUI_SetString1608(x+38*2-12,y+24,"100");
	GUI_SetString1608(x+38*3-8,y+24,"1K");
	GUI_SetString1608(x+38*4-20,y+24,"10K");
	
	if(cur > 10000)
	{
		cur = 10000;
	}
	
	if(cur<10)
	{
		len = cur*38/10;
	}else if(cur<100)
	{
		len = 38+(cur-10)*38/100;
	}else if(cur<1000)
	{
		len = 38*2+(cur-100)*38/1000;
	}else{
		len = 38*3+(cur-1000)*38/10000;
	}
	
	GUI_SetFillRect(x+4,y+5,len,5);
}


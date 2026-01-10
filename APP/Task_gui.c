#include "Task_gui.h"
#include "bsp_lcd.h"
#include "bsp_key.h"
#include "gui.h"
#include "user.h"
#include "display.h"
#include "Task_dose.h"

extern uint8_t batVoltPerSmooth;
extern float warnRangeRate ;
extern float warnRangeDose ;
extern uint8_t displayUnit;
extern uint8_t brightness;

void PWR_shutdown(void);

static uint8_t menuIndex = 1;
static uint8_t preMenuIndex = 0;


void menu_shutdown(uint8_t key);
void menu_main1(uint8_t key);
void menu_main2(uint8_t key);
void menu_main3(uint8_t key);
void menu1(uint8_t key);
void menu2(uint8_t key);
void menu3(uint8_t key);
void menu4(uint8_t key);
void menu41(uint8_t key);
void menu11(uint8_t key);
void menu12(uint8_t key);
void menu111(uint8_t key);
void menu121(uint8_t key);
void menu21(uint8_t key);
void menu211(uint8_t key);
void menu31(uint8_t key);
void menu32(uint8_t key);
void menu311(uint8_t key);
void menu321(uint8_t key);

typedef struct
{
	uint8_t Cur_Index;					 //当前索引项
	uint8_t enter;						 //确认
	uint8_t back;
	uint8_t up;							 //上面
	uint8_t down;						 //下面
	uint8_t long_enter;
	uint8_t long_back;
	uint8_t long_up;
	uint8_t long_down;
	void (*current_operation)(uint8_t); //当前索引执行的函数(界面)
} Main_Menu;

Main_Menu menuList[]=
{	
/*index	 1  2  3  4  5  6  7  8*/	
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0,menu_shutdown},
	{ 1, 4, 1, 1, 1, 0, 2, 3, 1,menu_main1},
	{ 2, 4, 1, 1, 1, 0, 3, 1, 1,menu_main2},
	{ 3, 4, 1, 1, 1, 0, 1, 2, 1,menu_main3},
	{ 4, 9, 1, 7, 5, 4, 4, 4, 4,menu1     },
	{ 5,13, 1, 4, 6, 4, 4, 4, 4,menu2     },
	{ 6,15, 1, 5, 7, 4, 4, 4, 4,menu3     },
	{ 7, 8, 1, 6, 4, 4, 4, 4, 4,menu4     },
	{ 8, 7, 7, 8, 8, 8, 8, 8, 8,menu41    },
	{ 9,11, 4,10,10,11, 9, 9, 9,menu11    },
	{10,12, 4, 9, 9,10,10,10,10,menu12    },
	{11,11,11,11,11,11, 9,11,11,menu111   },
	{12,12,10,12,12,12,10,12,12,menu121   },
	{13,14, 5,13,13,13,13,13,13,menu21    },
	{14,14,13,13,13,13,13,13,13,menu211   },
	{15,17, 6,16,16,15,15,15,15,menu31    },
	{16,18, 6,15,15,16,16,16,16,menu32    },
	{17,17,15,17,17,17,17,17,17,menu311   },
	{18,18,16,18,18,18,18,18,18,menu321   },
};


void Task_HMI(void)
{
	uint8_t action = 0;
	action = bsp_GetKey();
	
	
	switch(action)
	{
		case KEY_ENTER_CLICK:
			menuIndex = menuList[menuIndex].enter;
			break;
		case KEY_BACK_CLICK:
			menuIndex = menuList[menuIndex].back;
			break;
		case KEY_UP_CLICK:
			menuIndex = menuList[menuIndex].up;
			break;
		case KEY_DOWN_CLICK:
			menuIndex = menuList[menuIndex].down;
			break;
		case KEY_ENTER_LONG:
			menuIndex = menuList[menuIndex].long_enter;
			break;
		case KEY_BACK_LONG:
			menuIndex = menuList[menuIndex].long_back;
			break;
		case KEY_UP_LONG:
			menuIndex = menuList[menuIndex].long_up;
			break;
		case KEY_DOWN_LONG:
			menuIndex = menuList[menuIndex].long_down;
			break;
		default:break;
	}
	
	if(menuIndex != 18)
	{
		SetLcdBacklight(brightness);
	}
	menuList[menuIndex].current_operation(action);
	GUI_Refresh();
	
	preMenuIndex = menuIndex;
}

void HMI_start(uint8_t step)
{
	uint8_t str[11]={0};
	
	for(uint8_t i=0;i<step && i<10;i++)
	{
		str[i] = '>';
	}
	
	GUI_SetString2412(60,64,str);
	GUI_Refresh();
}

void menu_main1(uint8_t key)
{
	GUI_SetLine(160,0,160,1);
	GUI_SetLine(160,78,79,0);
	GUI_SetLine(160,98,79,0);
	
	GUI_Setstring1616(4,8,"内部剂量率");
	GUI_Setstring1616(184,80,"剂量");
	
	Display_bat(0,0);
	display_time(168,25);
	Display_Chg(164,0);
	
	Display_doseVal(0,24,intDoseParam.doserate_uSv_h,GetDoseUnit(intDoseParam.doserate_uSv_h));
	Display_dose(164,104,intDoseParam.dose_uSv,GetDoseUnit(intDoseParam.dose_uSv)+2);
	
	Display_scaleBar(0,112,intDoseParam.doserate_uSv_h);
}
void menu_main2(uint8_t key)
{
	
}
void menu_main3(uint8_t key)
{
	
}
void menu1(uint8_t key)
{
	
	Display_statusBar();
	
	GUI_SetString2412(0,24,"=>");
	GUI_SetString2412(215,24,"<=");
	GUI_Setstring2424(72,24,"阈值设置");
	GUI_Setstring2424(72,48,"累计剂量");
	GUI_Setstring2424(72,72,"系统设置");
	GUI_Setstring2424(72,96,"本机信息");
	
	GUI_Setstring2424(0,136,"确认");
	GUI_Setstring2424(191,136,"返回");
	GUI_SetString2412(108,136,"{}");
	
	GUI_SetLine(0,18,240,0);
	GUI_SetLine(0,134,240,0);
	
}
void menu2(uint8_t key)
{
	Display_statusBar();
	
	GUI_SetString2412(0,48,"=>");
	GUI_SetString2412(215,48,"<=");
	
	GUI_Setstring2424(72,24,"阈值设置");
	GUI_Setstring2424(72,48,"累计剂量");
	GUI_Setstring2424(72,72,"系统设置");
	GUI_Setstring2424(72,96,"本机信息");
	
	GUI_Setstring2424(0,136,"确认");
	GUI_Setstring2424(191,136,"返回");
	GUI_SetString2412(108,136,"{}");
	
	GUI_SetLine(0,18,240,0);
	GUI_SetLine(0,134,240,0);
}
void menu3(uint8_t key)
{
	Display_statusBar();
	
	GUI_SetString2412(0,72,"=>");
	GUI_SetString2412(215,72,"<=");
	
	GUI_Setstring2424(72,24,"阈值设置");
	GUI_Setstring2424(72,48,"累计剂量");
	GUI_Setstring2424(72,72,"系统设置");
	GUI_Setstring2424(72,96,"本机信息");
	
	GUI_Setstring2424(0,136,"确认");
	GUI_Setstring2424(191,136,"返回");
	GUI_SetString2412(108,136,"{}");
	
	GUI_SetLine(0,18,240,0);
	GUI_SetLine(0,134,240,0);
}
void menu4(uint8_t key)
{
	Display_statusBar();
	
	GUI_SetString2412(0,96,"=>");
	GUI_SetString2412(215,96,"<=");
	
	GUI_Setstring2424(72,24,"阈值设置");
	GUI_Setstring2424(72,48,"累计剂量");
	GUI_Setstring2424(72,72,"系统设置");
	GUI_Setstring2424(72,96,"本机信息");
	
	GUI_Setstring2424(0,136,"确认");
	GUI_Setstring2424(191,136,"返回");
	GUI_SetString2412(108,136,"{}");
	
	GUI_SetLine(0,18,240,0);
	GUI_SetLine(0,134,240,0);
}

void menu41(uint8_t key)
{
	Display_statusBar();
	GUI_SetLine(0,18,240,0);
	GUI_SetLine(0,134,240,0);
	GUI_Setstring2424(0,136,"确认");
	GUI_Setstring2424(191,136,"返回");
	
	GUI_Setstring2424(0,24,"编号");
	GUI_Setstring2424(0,48,"版本");
	GUI_Setstring2424(0,72,"量程");
	
	GUI_SetString2412(49,24,":2026010101");
	GUI_SetString2412(49,48,":V01.01.01");
	GUI_SetString2412(49,72,":0.1uSv/h~");
	GUI_SetString2412(62,96,"10mSv/h");
}

void menu11(uint8_t key)
{
	Display_statusBar();
	GUI_SetLine(0,18,240,0);
	GUI_SetLine(0,134,240,0);
	GUI_Setstring2424(0,136,"确认");
	GUI_Setstring2424(191,136,"返回");
	
	GUI_Setstring2424(0,40,"剂量率");
	
	GUI_Setstring2424(12,88,"剂量");
	
	GUI_SetLine(80,79,56,1);
	GUI_SetLine(0,79,80,0);
	
	Display_doseVal(94,32,warnRangeRate,0);
}

void menu12(uint8_t key)
{
	Display_statusBar();
	GUI_SetLine(0,18,240,0);
	GUI_SetLine(0,134,240,0);
	GUI_Setstring2424(0,136,"确认");
	GUI_Setstring2424(191,136,"返回");
	
	GUI_Setstring2424(0,40,"剂量率");
	
	GUI_Setstring2424(12,88,"剂量");
	
	GUI_SetLine(80,18,62,1);
	GUI_SetLine(0,79,80,0);
	
	Display_doseVal(94,32,warnRangeDose,0);
}

void menu111(uint8_t key)
{
	static uint8_t str[6] = {1,2,3,4,5,1};
	static uint8_t index = 0;
	uint8_t i;
	uint32_t range = 0;
	
	switch(key)
	{
		case KEY_ENTER_CLICK:
			if(preMenuIndex == menuIndex)
			{
				if(index == 0)
				{
					index = 5;
				}else{
					index--;
				}
			}else{
				str[5] = GetDoseUnit(warnRangeRate);
				if(str[5] == 1)
				{
					range = (uint32_t)(warnRangeRate*100/1000);
				}else{
					range = (uint32_t)(warnRangeRate*100);
				}
				str[4] = range%10;
				range /= 10;
				str[3] = range%10;
				range /= 10;
				str[2] = range%10;
				range /= 10;
				str[1] = range%10;
				range /= 10;
				str[0] = range%10;
				
				
			}
			break;
		case KEY_BACK_CLICK:
			index++;
			if(index > 5)
			{
				index = 0;
			}
			break;
		case KEY_UP_CLICK:
			if(index >= 5)
			{
				if(str[index] == 0)
				{
					str[index] = 1;
				}else{
					str[index] = 0;
				}
			}else{
				str[index]++;
				if(str[index]>9)
				{
					str[index] = 0;
				}
			}
			break;
		case KEY_DOWN_CLICK:
			if(index >= 5)
			{
				if(str[index] == 0)
				{
					str[index] = 1;
				}else{
					str[index] = 0;
				}
			}else{
				if(str[index] == 0)
				{
					str[index] = 9;
				}else{
					str[index]--;
				}
			}
			break;
		case KEY_ENTER_LONG:
			range = ((((str[0]*10)+str[1])*10+str[2])*10+str[3])*10+str[4];
			if(str[5] == 1)
			{
				range *= 1000;
			}
			warnRangeRate = (float)(range/100.00f);
			break;
		default:break;
	}
	Display_statusBar();
	GUI_SetLine(0,18,240,0);
	GUI_SetLine(0,134,240,0);
	GUI_Setstring2424(0,136,"确认");
	GUI_Setstring2424(191,136,"返回");
	
	GUI_SetLine(80,79,56,1);
	GUI_SetLine(0,79,80,0);
	
	GUI_Setstring2424(0,40,"剂量率");
	
	GUI_Setstring2424(12,88,"剂量");
	
	Display_setWarnVal(94,32,str,index);
}

void menu121(uint8_t key)
{
	static uint8_t str[6] = {1,2,3,4,5,1};
	static uint8_t index = 0;
	uint8_t i;
	uint32_t range = 0;
	
	switch(key)
	{
		case KEY_ENTER_CLICK:
			if(preMenuIndex == menuIndex)
			{
				if(index == 0)
				{
					index = 5;
				}else{
					index--;
				}
			}else{
				str[5] = GetDoseUnit(warnRangeRate) + 2;
				if(str[5] == 3)
				{
					range = (uint32_t)(warnRangeRate*100/1000);
				}else{
					range = (uint32_t)(warnRangeRate*100);
				}
				
				range = (uint32_t)(warnRangeDose*100);
				str[4] = range%10;
				range /= 10;
				str[3] = range%10;
				range /= 10;
				str[2] = range%10;
				range /= 10;
				str[1] = range%10;
				range /= 10;
				str[0] = range%10;
			}
			break;
		case KEY_BACK_CLICK:
			index++;
			if(index > 5)
			{
				index = 0;
			}
			break;
		case KEY_UP_CLICK:
			if(index >= 5)
			{
				if(str[index] == 2)
				{
					str[index] = 3;
				}else{
					str[index] = 2;
				}
			}else{
				str[index]++;
				if(str[index]>9)
				{
					str[index] = 0;
				}
			}
			break;
		case KEY_DOWN_CLICK:
			if(index >= 5)
			{
				if(str[index] == 2)
				{
					str[index] = 3;
				}else{
					str[index] = 2;
				}
			}else{
				if(str[index] == 0)
				{
					str[index] = 9;
				}else{
					str[index]--;
				}
			}
			break;
		case KEY_ENTER_LONG:
			range = ((((str[0]*10)+str[1])*10+str[2])*10+str[3])*10+str[4];
			if(str[5] == 3)
			{
				range *= 1000;
			}
			warnRangeDose = (float)(range/100.00f);
			break;
		default:break;
	}
	Display_statusBar();
	GUI_SetLine(0,18,240,0);
	GUI_SetLine(0,134,240,0);
	GUI_Setstring2424(0,136,"确认");
	GUI_Setstring2424(191,136,"返回");
	
	GUI_SetLine(80,18,62,1);
	GUI_SetLine(0,79,80,0);
	
	GUI_Setstring2424(0,40,"剂量率");
	
	GUI_Setstring2424(12,88,"剂量");
	
	Display_setWarnVal(94,32,str,index);
}

void menu21(uint8_t key)
{
	
	Display_statusBar();
	GUI_SetLine(0,18,240,0);
	GUI_SetLine(0,134,240,0);
	GUI_Setstring2424(0,136,"清除");
	GUI_Setstring2424(191,136,"返回");
	

	Display_doseVal(50,32,intDoseParam.dose_uSv,GetDoseUnit(intDoseParam.dose_uSv) + 2);
	
}

void menu211(uint8_t key)
{
	static uint8_t cnt =0;
	static uint8_t back =0;
	
	Display_statusBar();
	GUI_SetLine(0,18,240,0);
	GUI_SetLine(0,134,240,0);
	GUI_Setstring2424(0,136,"确认");
	GUI_Setstring2424(191,136,"返回");
	
	
	
	if(key == KEY_ENTER_CLICK)
	{
		if(preMenuIndex != menuIndex)
		{
			//clear dose
			GUI_Setstring2424(60,64,"确认清除？");
		}else{
			back = 1;
			cnt = 0;
			GUI_Setstring2424(60+24,64,"已清除");
		}
	}
	
	if(back)
	{
		cnt ++;
		GUI_Setstring2424(60+24,64,"已清除");
		if(cnt > 5)
		{
			back = 0;
			menuIndex = menuList[menuIndex].back;
		}
		intDoseParam.dose_uSv = 0;
		intDoseParam.dose_uSv = 0;
	}else{
		GUI_Setstring2424(60,64,"确认清除？");
	}
}

void menu31(uint8_t key)
{
	Display_statusBar();
	GUI_SetLine(0,18,240,0);
	GUI_SetLine(0,134,240,0);
	GUI_Setstring2424(0,136,"确认");
	GUI_Setstring2424(191,136,"返回");
	GUI_SetString2412(108,136,"{}");
	
	GUI_Setstring2424(5,40,"单位");
	
	GUI_Setstring2424(5,88,"亮度");
	
	GUI_SetLine(80,79,56,1);
	GUI_SetLine(0,79,80,0);
	
	
	Display_unitParam(130,32,displayUnit);
	
}

void menu32(uint8_t key)
{
	Display_statusBar();
	GUI_SetLine(0,18,240,0);
	GUI_SetLine(0,134,240,0);
	GUI_Setstring2424(0,136,"确认");
	GUI_Setstring2424(191,136,"返回");
	GUI_SetString2412(108,136,"{}");
	
	GUI_SetLine(80,18,62,1);
	GUI_SetLine(0,79,80,0);
	
	GUI_Setstring2424(5,40,"单位");
	GUI_Setstring2424(5,88,"亮度");
	
	Display_brightness(120,28,brightness);
}

void menu311(uint8_t key)
{
	static uint8_t unit = 0;
	switch(key)
	{
		case KEY_ENTER_CLICK:
			if(preMenuIndex == menuIndex)
			{
				displayUnit = unit;
				menuIndex = menuList[menuIndex].back;
			}else{
				unit = displayUnit;
			}
			break;
		case KEY_BACK_CLICK:
			break;
		case KEY_UP_CLICK:
			if(unit == 0)
			{
				unit = 2;
			}else{
				unit --;
			}
			break;
		case KEY_DOWN_CLICK:
			unit++;
			if(unit > 2)
			{
				unit = 0;
			}
			break;
		default:break;
	}
	Display_statusBar();
	GUI_SetLine(0,18,240,0);
	GUI_SetLine(0,134,240,0);
	GUI_Setstring2424(0,136,"确认");
	GUI_Setstring2424(191,136,"返回");
	GUI_SetString2412(108,136,"{}");
	
	GUI_Setstring2424(5,40,"单位");
	GUI_Setstring2424(5,88,"亮度");
	
	GUI_SetLine(80,79,56,1);
	GUI_SetLine(0,79,80,0);
	
	Display_unitParam(130,32,unit);
	
}

void menu321(uint8_t key)
{
	static uint8_t bn = 50;
	switch(key)
	{
		case KEY_ENTER_CLICK:
			if(preMenuIndex == menuIndex)
			{
				brightness = bn;
				menuIndex = menuList[menuIndex].back;
			}else{
				bn = brightness;
			}
			break;
		case KEY_BACK_CLICK:
			break;
		case KEY_UP_CLICK:
			bn += 10;
			if(bn > 100)
			{
				bn = 100;
			}
			break;
		case KEY_DOWN_CLICK:
			if(bn<10)
			{
				bn = 0;
			}else{
				bn -= 10;
			}
			break;
		default:break;
	}
	
	Display_statusBar();
	GUI_SetLine(0,18,240,0);
	GUI_SetLine(0,134,240,0);
	GUI_Setstring2424(0,136,"确认");
	GUI_Setstring2424(191,136,"返回");
	GUI_SetString2412(108,136,"{}");
	
	GUI_SetLine(80,18,62,1);
	GUI_SetLine(0,79,80,0);
	
	GUI_Setstring2424(5,40,"单位");
	GUI_Setstring2424(5,88,"亮度");
	
	Display_brightness(120,24,bn);
	
	SetLcdBacklight(bn);
}


void menu_shutdown(uint8_t key)
{
	uint8_t str[11]={0};
	
	for(uint8_t i=0; i<10;i++)
	{
		for(uint8_t j=0;j<(10-i);j++)
		{
			str[j] = '<';
		}
		GUI_SetString2412(60,64,str);
		GUI_Refresh();
		delay_ms(100);
	}
	SetLcdBacklight(0);
	PWR_shutdown();
	while(1);
}
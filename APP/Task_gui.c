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
extern uint8_t passWord[4];

static uint8_t menuIndex = 1;
static uint8_t preMenuIndex = 0;

void PWR_shutdown(void);
void menu_shutdown(uint8_t key);
void menu_main1(uint8_t key);
void menu_main2(uint8_t key);
void menu_main3(uint8_t key);
void menu01(uint8_t key);
void menu02(uint8_t key);
void menu03(uint8_t key);
void menu011(uint8_t key);
void menu012(uint8_t key);
void menu013(uint8_t key);
void menu014(uint8_t key);
void menu015(uint8_t key);
void menu0111(uint8_t key);
void menu0121(uint8_t key);
void menu0131(uint8_t key);
void menu0141(uint8_t key);
void menu0151(uint8_t key);

void menu1(uint8_t key);
void menu2(uint8_t key);
void menu3(uint8_t key);
void menu4(uint8_t key);
void menu5(uint8_t key);
void menu41(uint8_t key);
void menu11(uint8_t key);	
void menu12(uint8_t key);
void menu111(uint8_t key);
void menu121(uint8_t key);
void menu31(uint8_t key);
void menu311(uint8_t key);
void menu21(uint8_t key);
void menu22(uint8_t key);
void menu211(uint8_t key);
void menu51(uint8_t key);
void menu511(uint8_t key);
void menu221(uint8_t key);
void menu222(uint8_t key);
void menu223(uint8_t key);
void menu41(uint8_t key);
void menu42(uint8_t key);
void menu43(uint8_t key);
void menu44(uint8_t key);

#define ADVANCE_MENU_START	17

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
	{ 4, 7, 1, 5, 5, 4, 4, 4, 4,menu01     },
	{ 5, 6, 1, 4, 4, 5, 5, 5, 5,menu02     },
	{ 6, 6, 5, 6, 6, 6, 6, 6, 6,menu03     },
	{ 7,12, 4,11, 8, 7, 7, 7, 7,menu011    },
	{ 8,13, 4, 7, 9, 8, 8, 8, 8,menu012    },
	{ 9,14, 4, 8,10, 9, 9, 9, 9,menu013    },
	{10,15, 4, 9,11,10,10,10,10,menu014    },
	{11,16, 4,10, 7,11,11,11,11,menu015    },
	{12,12, 7,12,12,12,12,12,12,menu0111   },
	{13,13, 8,13,13,13,13,13,13,menu0121   },
	{14,14, 9,14,14,14,14,14,14,menu0131   },
	{15,15,10,15,15,15,15,15,15,menu0141   },
	{16,16,11,16,16,16,16,16,16,menu0151   },
	{17,21, 5,30,18,17,17,17,17,menu1      },
	{18,27, 5,17,19,18,18,18,18,menu2      },
	{19,25, 5,18,20,19,19,19,19,menu3      },
	{20,36, 5,19,30,20,20,20,20,menu4      },
	{21,23,17,22,22,21,21,21,21,menu11     },
	{22,24,17,21,21,22,22,22,22,menu12     },
	{23,23,21,23,23,23,23,23,23,menu111    },
	{24,24,22,24,24,24,24,24,24,menu121    },
	{25,26,19,25,25,25,25,25,25,menu31     },
	{26,26,25,26,26,26,26,26,26,menu311    },
	{27,29,18,28,28,27,27,27,27,menu21     },
	{28,33,18,27,27,28,28,28,28,menu22     },
	{29,29,27,29,29,29,29,29,29,menu211    },
	{30,31, 5,20,17,30,30,30,30,menu5	   },
	{31,32,30,31,31,31,31,31,31,menu51     },
	{32,32,31,32,32,32,32,32,32,menu511    },
	{33,33,28,35,34,33,33,33,33,menu221    },
	{34,34,28,33,35,34,34,34,34,menu222    },
	{35,35,28,34,33,35,35,35,35,menu223    },
	{36,36,20,39,37,36,36,36,36,menu41     },
	{37,37,20,36,38,37,37,37,37,menu42     },
	{38,38,20,37,39,38,38,38,38,menu43     },
	{39,39,20,38,36,39,39,39,39,menu44     },
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
	
	if(menuIndex != 15)
	{
		SetLcdBacklight(basicParam.backLight);
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
	
	GUI_SetString2412(60,64,str,0);
	GUI_Refresh();
}

void menu_shutdown(uint8_t key)
{
	uint8_t str[11] = {0};

	for (uint8_t i = 0; i < 10; i++)
	{
		for (uint8_t j = 0; j < (10 - i); j++)
		{
			str[j] = '<';
		}
		GUI_SetString2412(60, 64, str, 0);
		GUI_Refresh();
		delay_ms(100);
	}
	SetLcdBacklight(0);
	PWR_shutdown();
	while (1)
		;
}

void menu_main1(uint8_t key)
{
	GUI_SetLine(160,0,160,1);
	GUI_SetLine(160,78,79,0);
	GUI_SetLine(160,98,79,0);
	
	GUI_SetString1616(4,8,"内部剂量率",1);
	GUI_SetString1616(168,80,"累计剂量",0);
	
	Display_bat(0,0);
	display_time(168,25);
	// Display_Chg(164,0);
	
	Display_doseVal(0,24,intDoseParam.doserate_uSv_h,GetDoseUnit(intDoseParam.doserate_uSv_h));
	Display_dose(164,104,intDoseParam.dose_uSv,GetDoseUnit(intDoseParam.dose_uSv)+2);
	
	// Display_scaleBar(0,112,intDoseParam.doserate_uSv_h);
	Display_ProgressBar(4, 112, 152, 16, (uint8_t)(intDoseParam.doserate_uSv_h / warnRangeRate * 100));
	// Display_ProgressBar(4, 112, 152, 16, test3);
}
void menu_main2(uint8_t key)
{
	
}
void menu_main3(uint8_t key)
{
	
}

void menu01(uint8_t key)
{
	Display_statusBar();

	GUI_SetString2424(0, 136, "确认", 0);
	GUI_SetString2424(191, 136, "返回", 0);
	GUI_SetString2424(108, 136, "{}", 0);

	GUI_SetString2424(72, 32, "基本设置", 1);
	GUI_SetString2424(72, 72, "高级设置", 0);

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 134, 240, 0);
}

void menu02(uint8_t key)
{
	Display_statusBar();

	GUI_SetString2424(0, 136, "确认", 0);
	GUI_SetString2424(191, 136, "返回", 0);
	GUI_SetString2424(108, 136, "{}", 0);

	GUI_SetString2424(72, 32, "基本设置", 0);
	GUI_SetString2424(72, 72, "高级设置", 2);

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 134, 240, 0);
}

void menu03(uint8_t key)
{
	static uint8_t ps[4] = {0};
	static uint8_t index = 0;

	switch(key)
	{
		case KEY_ENTER_CLICK:
			if (preMenuIndex == menuIndex)
			{
				if(index < 3)
				{
					index++;
				}else{
					if((ps[0] == passWord[0]) && (ps[1] == passWord[1]) && (ps[2] == passWord[2]) && (ps[3] == passWord[3]))
					{
						menuIndex = ADVANCE_MENU_START;
					}else{
						index = 0;
						ps[0] = 0;
						ps[1] = 0;
						ps[2] = 0;
						ps[3] = 0;
					}
				}
			}else{
				index = 0;
				ps[0] = 0;
				ps[1] = 0;
				ps[2] = 0;
				ps[3] = 0;
			}
				break;
		case KEY_BACK_CLICK:
			menuIndex = menuList[menuIndex].back;
			break;
		case KEY_UP_CLICK:
			if(ps[index] < 9)
			{
				ps[index]++;
			}else{
				ps[index] = 0;
			}
			break;
		case KEY_DOWN_CLICK:
			if(ps[index] > 0)
			{
				ps[index]--;
			}else{
				ps[index] = 9;
			}
			break;
		default:
			break;
	}

	Display_statusBar();

	GUI_SetString2424(0, 136, "确认", 0);
	GUI_SetString2424(191, 136, "返回", 0);
	GUI_SetString2424(108, 136, "{}", 0);

	GUI_SetString2424(64, 32, "请输入密码", 0);

	GUI_SetChar2412(88, 72, ps[0], index==0?1:0);
	GUI_SetChar2412(108, 72, ps[1], index==1?1:0);
	GUI_SetChar2412(128, 72, ps[2], index==2?1:0);
	GUI_SetChar2412(148, 72, ps[3], index==3?1:0);

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 134, 240, 0);
}

void menu011(uint8_t key)
{
	uint8_t height = 24;

	Display_statusBar();
	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);

	GUI_SetString1616(4, 24 + height * 0, "按键音", 1);
	GUI_SetString1616(4, 24 + height * 1, "日期设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "时间设置", 0);
	GUI_SetString1616(4, 24 + height * 3, "背光亮度", 0);
	GUI_SetString1616(4, 24 + height * 4, "背光时间", 0);

	GUI_SetString1616(180, 24 + height * 2, basicParam.keyBeep ? "开" : "关", 0);
}

void menu012(uint8_t key)
{
	uint8_t height = 24;
	uint8_t str[10] = {0};
	uint8_t time[7] = {0};

	Get_RTC_Time(time);

	Display_statusBar();
	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);

	GUI_SetString1616(4, 24 + height * 0, "按键音", 0);
	GUI_SetString1616(4, 24 + height * 1, "日期设置", 1);
	GUI_SetString1616(4, 24 + height * 2, "时间设置", 0);
	GUI_SetString1616(4, 24 + height * 3, "背光亮度", 0);
	GUI_SetString1616(4, 24 + height * 4, "背光时间", 0);

	sprintf(str, "20%02d", time[5]);
	GUI_SetString2412(156, 24 + height * 1, str, 0);
	sprintf(str, "%02d-%02d", time[4], time[3]);
	GUI_SetString2412(150, 24 + height * 3, str, 0);
}

void menu013(uint8_t key)
{
	uint8_t height = 24;
	uint8_t str[10] = {0};
	uint8_t time[7] = {0};

	Get_RTC_Time(time);

	Display_statusBar();
	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);

	GUI_SetString1616(4, 24 + height * 0, "按键音", 0);
	GUI_SetString1616(4, 24 + height * 1, "日期设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "时间设置", 1);
	GUI_SetString1616(4, 24 + height * 3, "背光亮度", 0);
	GUI_SetString1616(4, 24 + height * 4, "背光时间", 0);

	sprintf(str, "%02d:%02d:%02d", time[2], time[1], time[0]);
	GUI_SetString2412(136, 24 + height * 2, str, 0);
}

void menu014(uint8_t key)
{
	uint8_t height = 24;

	Display_statusBar();
	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);

	GUI_SetString1616(4, 24 + height * 0, "按键音", 0);
	GUI_SetString1616(4, 24 + height * 1, "日期设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "时间设置", 0);
	GUI_SetString1616(4, 24 + height * 3, "背光亮度", 1);
	GUI_SetString1616(4, 24 + height * 4, "背光时间", 0);

	Display_brightness(160, 28, basicParam.backLight, 0);
}

void menu015(uint8_t key)
{
	uint8_t height = 24;

	Display_statusBar();
	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);

	GUI_SetString1616(4, 24 + height * 0, "按键音", 0);
	GUI_SetString1616(4, 24 + height * 1, "日期设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "时间设置", 0);
	GUI_SetString1616(4, 24 + height * 3, "背光亮度", 0);
	GUI_SetString1616(4, 24 + height * 4, "背光时间", 1);

	char str[10] = {0};
	sprintf(str, "%dS", basicParam.blTime);
	GUI_SetString2412(160, 24 + height * 2, str, 0);
}

void menu0111(uint8_t key)
{
	uint8_t height = 24;
	static uint8_t beep = 0;
	switch(key)
	{
		case KEY_ENTER_CLICK:
			if(preMenuIndex == menuIndex)
			{
				basicParam.keyBeep = beep;
				menuIndex = menuList[menuIndex].back;
			}else{
				beep = basicParam.keyBeep;
			}
			break;
		case KEY_BACK_CLICK:
			break;
		case KEY_UP_CLICK:
			beep ^= 1;
			break;
		case KEY_DOWN_CLICK:
			beep ^= 1;
			break;
		default:
			break;
	}
	Display_statusBar();
	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);

	GUI_SetString1616(4, 24 + height * 0, "按键音", 0);
	GUI_SetString1616(4, 24 + height * 1, "日期设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "时间设置", 0);
	GUI_SetString1616(4, 24 + height * 3, "背光亮度", 0);
	GUI_SetString1616(4, 24 + height * 4, "背光时间", 0);

	GUI_SetString1616(180, 24 + height * 2, beep ? "开" : "关", 1);
}

void menu0121(uint8_t key)
{
	static uint8_t tim[3] = {0};
	static uint8_t index = 0;
	uint8_t height = 24;
	uint8_t str[10] = {0};
	uint8_t time[7] = {0};

	Get_RTC_Time(time);

	switch (key)
	{
		case KEY_ENTER_CLICK:
			if (preMenuIndex == menuIndex)
			{
				if(index < 2)
				{
					index++;
				} 
				else
				{
					time[5] = tim[0];
					time[4] = tim[1];
					time[3] = tim[2];
					Set_RTC_Time(time);
					menuIndex = menuList[menuIndex].back;
				}
			}
			else
			{
				index = 0;
				tim[0] = time[5];
				tim[1] = time[4];
				tim[2] = time[3];
			}
			break;
		case KEY_UP_CLICK:
			tim[index]++;
			if(index == 0)
			{
				if (tim[index] > 99)
				{
					tim[index] = 0;
				}	
			}else if(index == 1)
			{
				if (tim[index] > 12)
				{
					tim[index] = 1;
				}
			}else if(index == 2)
			{
				if (tim[index] > 31)
				{
					tim[index] = 1;
				}
			}
			break;
		case KEY_DOWN_CLICK:
			if(tim[index] > 0)
			{
				tim[index]--;
			}else{
				if(index == 0)
				{
					tim[index] = 99;
				}else if(index == 1)
				{
					tim[index] = 12;
				}else if(index == 2)
				{
					tim[index] = 31;
				}
			}
			break;
		case KEY_UP_LONG:
			tim[index] += 10;
			if (index == 0)
			{
				if (tim[index] > 99)
				{
					tim[index] = 0;
				}
			}
			else if (index == 1)
			{
				if (tim[index] > 12)
				{
					tim[index] = 1;
				}
			}
			else if (index == 2)
			{
				if (tim[index] > 31)
				{
					tim[index] = 1;
				}
			}
			break;
		case KEY_DOWN_LONG:
			if (tim[index] > 10)
			{
				tim[index]-=10;
			}
			else
			{
				if (index == 0)
				{
					tim[index] = 99;
				}
				else if (index == 1)
				{
					tim[index] = 12;
				}
				else if (index == 2)
				{
					tim[index] = 31;
				}
			}
			break;
		default:
			break;
	}


	Display_statusBar();
	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);

	GUI_SetString1616(4, 24 + height * 0, "按键音", 0);
	GUI_SetString1616(4, 24 + height * 1, "日期设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "时间设置", 0);
	GUI_SetString1616(4, 24 + height * 3, "背光亮度", 0);
	GUI_SetString1616(4, 24 + height * 4, "背光时间", 0);

	sprintf(str, "20%02d", tim[0]);
	GUI_SetString2412(156, 24 + height * 1, str, index==0?1:0);
	sprintf(str, "%02d", tim[1]);
	GUI_SetString2412(150, 24 + height * 3, str, index==1?1:0);
	GUI_SetString2412(150 + 24, 24 + height * 3, "-", 0);
	sprintf(str, "%02d", tim[2]);
	GUI_SetString2412(150+36, 24 + height * 3, str, index==2?1:0);
}

void menu0131(uint8_t key)
{
	static uint8_t tim[3] = {0};
	static uint8_t index = 0;
	uint8_t height = 24;
	uint8_t str[10] = {0};
	uint8_t time[7] = {0};

	Get_RTC_Time(time);

	switch (key)
	{
	case KEY_ENTER_CLICK:
		if (preMenuIndex == menuIndex)
		{
			if (index < 2)
			{
				index++;
			}
			else
			{
				time[2] = tim[0];
				time[1] = tim[1];
				time[0] = tim[2];
				Set_RTC_Time(time);
				menuIndex = menuList[menuIndex].back;
			}
		}
		else
		{
			index = 0;
			tim[0] = time[2];
			tim[1] = time[1];
			tim[2] = time[0];
		}
		break;
	case KEY_UP_CLICK:
		tim[index]++;
		if (index == 0)
		{
			if (tim[index] > 23)
			{
				tim[index] = 0;
			}
		}
		else if (index == 1)
		{
			if (tim[index] > 59)
			{
				tim[index] = 0;
			}
		}
		else if (index == 2)
		{
			if (tim[index] > 59)
			{
				tim[index] = 0;
			}
		}
		break;
	case KEY_DOWN_CLICK:
		if (tim[index] > 0)
		{
			tim[index]--;
		}
		else
		{
			if (index == 0)
			{
				tim[index] = 23;
			}
			else if (index == 1)
			{
				tim[index] = 59;
			}
			else if (index == 2)
			{
				tim[index] = 59;
			}
		}
		break;
	case KEY_UP_LONG:
		tim[index] += 10;
		if (index == 0)
		{
			if (tim[index] > 23)
			{
				tim[index] = 0;
			}
		}
		else if (index == 1)
		{
			if (tim[index] > 59)
			{
				tim[index] = 0;
			}
		}
		else if (index == 2)
		{
			if (tim[index] > 59)
			{
				tim[index] = 0;
			}
		}
		break;
	case KEY_DOWN_LONG:
		if (tim[index] > 9)
		{
			tim[index] -= 10;
		}
		else
		{
			if (index == 0)
			{
				tim[index] = 23;
			}
			else if (index == 1)
			{
				tim[index] = 59;
			}
			else if (index == 2)
			{
				tim[index] = 59;
			}
		}
		break;
	default:
		break;
	}

	Display_statusBar();
	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);

	GUI_SetString1616(4, 24 + height * 0, "按键音", 0);
	GUI_SetString1616(4, 24 + height * 1, "日期设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "时间设置", 0);
	GUI_SetString1616(4, 24 + height * 3, "背光亮度", 0);
	GUI_SetString1616(4, 24 + height * 4, "背光时间", 0);

	sprintf(str, "%02d", tim[0]);
	GUI_SetString2412(136, 24 + height * 2, str, index == 0 ? 1 : 0);
	GUI_SetString2412(136 + 24, 24 + height * 2, ":", 0);
	sprintf(str, "%02d", tim[1]);
	GUI_SetString2412(136 + 36, 24 + height * 2, str, index == 1 ? 1 : 0);
	GUI_SetString2412(136 + 60, 24 + height * 2, ":", 0);
	sprintf(str, "%02d", tim[2]);
	GUI_SetString2412(136 + 72, 24 + height * 2, str, index == 2 ? 1 : 0);
}

void menu0141(uint8_t key)
{
	static uint8_t bright = 0;
	uint8_t height = 24;

	switch(key)
	{
		case KEY_ENTER_CLICK:
			if(preMenuIndex == menuIndex)
			{
				basicParam.backLight = bright;
				menuIndex = menuList[menuIndex].back;
			}else{
				bright = basicParam.backLight;
			}
			break;
		case KEY_BACK_CLICK:
			break;
		case KEY_UP_CLICK:
			if (bright < 100)
			{
				bright ++;
			}
			break;
		case KEY_DOWN_CLICK:
			if (bright > 0)
			{
				bright --;
			}
			break;
		case KEY_UP_LONG:
			if (bright < 100)
			{
				bright += 10;
			}
			if(bright > 100)
			{
				bright = 100;
			}
			break;
		case KEY_DOWN_LONG:
			if (bright > 9)
			{
				bright -= 10;
			}else{
				bright = 0;
			}
			break;
		default:
			break;
	}
	Display_statusBar();
	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);

	GUI_SetString1616(4, 24 + height * 0, "按键音", 0);
	GUI_SetString1616(4, 24 + height * 1, "日期设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "时间设置", 0);
	GUI_SetString1616(4, 24 + height * 3, "背光亮度", 0);
	GUI_SetString1616(4, 24 + height * 4, "背光时间", 0);

	Display_brightness(160, 28, bright, 1);
	SetLcdBacklight(bright);
}

void menu0151(uint8_t key)
{
	static uint16_t blTime = 0;
	uint8_t height = 24;

	switch(key)
	{
		case KEY_ENTER_CLICK:
			if(preMenuIndex == menuIndex)
			{
				basicParam.blTime = blTime;
				menuIndex = menuList[menuIndex].back;
			}else{
				blTime = basicParam.blTime;
			}
			break;
		case KEY_BACK_CLICK:
			break;
		case KEY_UP_CLICK:
			if(blTime < 300)
			{
				blTime++;
			}
			break;
		case KEY_DOWN_CLICK:
			if(blTime > 0)
			{
				blTime--;
			}
			break;
		case KEY_UP_LONG:
			if (blTime < 300)
			{
				blTime += 10;
			}
			if(blTime > 300)
			{
				blTime = 300;
			}
			break;
		case KEY_DOWN_LONG:
			if (blTime > 9)
			{
				blTime -= 10;
			}else{
				blTime = 0;
			}
			break;
		default:
			break;
	}

	Display_statusBar();
	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);

	GUI_SetString1616(4, 24 + height * 0, "按键音", 0);
	GUI_SetString1616(4, 24 + height * 1, "日期设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "时间设置", 0);
	GUI_SetString1616(4, 24 + height * 3, "背光亮度", 0);
	GUI_SetString1616(4, 24 + height * 4, "背光时间", 0);

	char str[10] = {0};
	sprintf(str, "%dS", blTime);
	GUI_SetString2412(160, 24 + height * 2, str, 1);
}


void menu1(uint8_t key)
{
	uint8_t height = 24;

	Display_statusBar();

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);
	// GUI_SetLine(160, 18, 124, 1);

	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetString1616(4, 24 + height * 0, "报警设置", 1);
	GUI_SetString1616(4, 24 + height * 1, "测量设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "累计剂量", 0);
	GUI_SetString1616(4, 24 + height * 3, "历史数据", 0);
	GUI_SetString1616(4, 24 + height * 4, "密码设置", 0);

	GUI_SetString1616(120 + 4, 24 + height * 0, "剂量率", 0);
	GUI_SetString1616(120 + 4, 24 + height * 1, "累计剂量", 0);

}

void menu2(uint8_t key)
{
	uint8_t height = 24;

	Display_statusBar();

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);
	// GUI_SetLine(160, 18, 124, 1);

	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetString1616(4, 24 + height * 0, "报警设置", 0);
	GUI_SetString1616(4, 24 + height * 1, "测量设置", 1);
	GUI_SetString1616(4, 24 + height * 2, "累计剂量", 0);
	GUI_SetString1616(4, 24 + height * 3, "历史数据", 0);
	GUI_SetString1616(4, 24 + height * 4, "密码设置", 0);

	GUI_SetString1616(120 + 4, 24 + height * 0, "单位设置", 0);
	GUI_SetString1616(120 + 4, 24 + height * 1, "探测器类型", 0);
}

void menu3(uint8_t key)
{
	uint8_t height = 24;

	Display_statusBar();

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);
	// GUI_SetLine(160, 18, 124, 1);

	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetString1616(4, 24 + height * 0, "报警设置", 0);
	GUI_SetString1616(4, 24 + height * 1, "测量设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "累计剂量", 1);
	GUI_SetString1616(4, 24 + height * 3, "历史数据", 0);
	GUI_SetString1616(4, 24 + height * 4, "密码设置", 0);

	GUI_SetString1616(120 + 4, 24 + height * 0, "累计剂量重置", 0);
}

void menu4(uint8_t key)
{
	uint8_t height = 24;

	Display_statusBar();

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);
	// GUI_SetLine(160, 18, 124, 1);

	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetString1616(4, 24 + height * 0, "报警设置", 0);
	GUI_SetString1616(4, 24 + height * 1, "测量设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "累计剂量", 0);
	GUI_SetString1616(4, 24 + height * 3, "历史数据", 1);
	GUI_SetString1616(4, 24 + height * 4, "密码设置", 0);

	GUI_SetString1616(120 + 4, 24 + height * 0, "存储时间", 0);
	GUI_SetString1616(120 + 4, 24 + height * 1, "数据查询", 0);
	GUI_SetString1616(120 + 4, 24 + height * 2, "数据清除", 0);
	GUI_SetString1616(120 + 4, 24 + height * 3, "存储空间", 0);
}

void menu5(uint8_t key)
{
	uint8_t height = 24;

	Display_statusBar();

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);
	// GUI_SetLine(160, 18, 124, 1);

	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetString1616(4, 24 + height * 0, "报警设置", 0);
	GUI_SetString1616(4, 24 + height * 1, "测量设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "累计剂量", 0);
	GUI_SetString1616(4, 24 + height * 3, "历史数据", 0);
	GUI_SetString1616(4, 24 + height * 4, "密码设置", 1);

	GUI_SetString1616(120 + 4, 24 + height * 0, "修改密码", 0);
}

void menu51(uint8_t key)
{
	uint8_t height = 24;

	Display_statusBar();

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);
	// GUI_SetLine(160, 18, 124, 1);

	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetString1616(4, 24 + height * 0, "报警设置", 0);
	GUI_SetString1616(4, 24 + height * 1, "测量设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "累计剂量", 0);
	GUI_SetString1616(4, 24 + height * 3, "历史数据", 0);
	GUI_SetString1616(4, 24 + height * 4, "密码设置", 0);

	GUI_SetString1616(120 + 4, 24 + height * 0, "修改密码", 1);
}

void menu11(uint8_t key)
{
	uint8_t height = 24;

	Display_statusBar();

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);
	// GUI_SetLine(160, 18, 124, 1);

	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetString1616(4, 24 + height * 0, "报警设置", 0);
	GUI_SetString1616(4, 24 + height * 1, "测量设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "累计剂量", 0);
	GUI_SetString1616(4, 24 + height * 3, "历史数据", 0);
	GUI_SetString1616(4, 24 + height * 4, "密码设置", 0);

	GUI_SetString1616(120 + 4, 24 + height * 0, "剂量率", 1);
	GUI_SetString1616(120 + 4, 24 + height * 1, "累计剂量", 0);
}

void menu12(uint8_t key)
{
	uint8_t height = 24;

	Display_statusBar();

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);
	// GUI_SetLine(160, 18, 124, 1);

	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetString1616(4, 24 + height * 0, "报警设置", 0);
	GUI_SetString1616(4, 24 + height * 1, "测量设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "累计剂量", 0);
	GUI_SetString1616(4, 24 + height * 3, "历史数据", 0);
	GUI_SetString1616(4, 24 + height * 4, "密码设置", 0);

	GUI_SetString1616(120 + 4, 24 + height * 0, "剂量率", 0);
	GUI_SetString1616(120 + 4, 24 + height * 1, "累计剂量", 1);
}

void menu111(uint8_t key)
{
	static uint8_t str[6] = {1, 2, 3, 4, 5, 1};
	static uint8_t index = 0;
	uint32_t range = 0;

	switch (key)
	{
		case KEY_ENTER_CLICK:
			if (preMenuIndex == menuIndex)
			{
				if(index < 5)
				{	
					index ++;
				}else{
					range = ((((str[0] * 10) + str[1]) * 10 + str[2]) * 10 + str[3]) * 10 + str[4];
					if (str[5] == 1)
					{
						range *= 1000;
					}
					warnRangeRate = (float)(range / 100.00f);
					menuIndex = menuList[menuIndex].back;
				}
			}
			else
			{
				str[5] = GetDoseUnit(warnRangeRate);
				if (str[5] == 1)
				{
					range = (uint32_t)(warnRangeRate * 100 / 1000);
				}
				else
				{
					range = (uint32_t)(warnRangeRate * 100);
				}
				str[4] = range % 10;
				range /= 10;
				str[3] = range % 10;
				range /= 10;
				str[2] = range % 10;
				range /= 10;
				str[1] = range % 10;
				range /= 10;
				str[0] = range % 10;
			}
			break;
		case KEY_BACK_CLICK:
			index++;
			if (index > 5)
			{
				index = 0;
			}
			break;
		case KEY_UP_CLICK:
			if (index >= 5)
			{
				if (str[index] == 0)
				{
					str[index] = 1;
				}
				else
				{
					str[index] = 0;
				}
			}
			else
			{
				str[index]++;
				if (str[index] > 9)
				{
					str[index] = 0;
				}
			}
			break;
		case KEY_DOWN_CLICK:
			if (index >= 5)
			{
				if (str[index] == 0)
				{
					str[index] = 1;
				}
				else
				{
					str[index] = 0;
				}
			}
			else
			{
				if (str[index] == 0)
				{
					str[index] = 9;
				}
				else
				{
					str[index]--;
				}
			}
			break;
		case KEY_ENTER_LONG:
			
			break;
		default:
			break;
	}
	Display_statusBar();
	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	// GUI_SetLine(120, 18, 124, 1);

	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);

	GUI_SetString1616(64, 24, "剂量率报警阈值", 0);

	Display_setWarnVal(60, 48, str, index);
}

void menu121(uint8_t key)
{
	static uint8_t str[6] = {1, 2, 3, 4, 5, 1};
	static uint8_t index = 0;
	uint8_t i;
	uint32_t range = 0;

	switch (key)
	{
	case KEY_ENTER_CLICK:
		if (preMenuIndex == menuIndex)
		{
			if(index < 5)
			{	
				index++;
			}else{
				range = ((((str[0] * 10) + str[1]) * 10 + str[2]) * 10 + str[3]) * 10 + str[4];
				if (str[5] == 3)
				{
					range *= 1000;
				}
				warnRangeDose = (float)(range / 100.00f);
				menuIndex = menuList[menuIndex].back;
			}
		}
		else
		{
			index = 0;
			str[5] = GetDoseUnit(warnRangeRate) + 2;
			if (str[5] == 3)
			{
				range = (uint32_t)(warnRangeRate * 100 / 1000);
			}
			else
			{
				range = (uint32_t)(warnRangeRate * 100);
			}

			range = (uint32_t)(warnRangeDose * 100);
			str[4] = range % 10;
			range /= 10;
			str[3] = range % 10;
			range /= 10;
			str[2] = range % 10;
			range /= 10;
			str[1] = range % 10;
			range /= 10;
			str[0] = range % 10;
		}
		break;
	case KEY_BACK_CLICK:
		break;
	case KEY_UP_CLICK:
		if (index >= 5)
		{
			if (str[index] == 2)
			{
				str[index] = 3;
			}
			else
			{
				str[index] = 2;
			}
		}
		else
		{
			str[index]++;
			if (str[index] > 9)
			{
				str[index] = 0;
			}
		}
		break;
	case KEY_DOWN_CLICK:
		if (index >= 5)
		{
			if (str[index] == 2)
			{
				str[index] = 3;
			}
			else
			{
				str[index] = 2;
			}
		}
		else
		{
			if (str[index] == 0)
			{
				str[index] = 9;
			}
			else
			{
				str[index]--;
			}
		}
		break;
	default:
		break;
	}
	Display_statusBar();
	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	// GUI_SetLine(120, 18, 124, 1);
	// GUI_SetLine(160, 18, 124, 1);

	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);

	GUI_SetString1616(56, 24, "累计剂量报警阈值", 0);

	Display_setWarnVal(60, 48, str, index);
}

void menu31(uint8_t key)
{
	uint8_t height = 24;

	Display_statusBar();

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);
	// GUI_SetLine(160, 18, 124, 1);

	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);

	GUI_SetString1616(4, 24 + height * 0, "报警设置", 0);
	GUI_SetString1616(4, 24 + height * 1, "测量设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "累计剂量", 0);
	GUI_SetString1616(4, 24 + height * 3, "历史数据", 0);
	GUI_SetString1616(4, 24 + height * 4, "密码设置", 0);

	GUI_SetString1616(120 + 4, 24 + height * 0, "累计剂量重置", 1);


}

void menu311(uint8_t key)
{
	static uint8_t cnt = 0;
	static uint8_t back = 0;

	Display_statusBar();
	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);

	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);

	if (key == KEY_ENTER_CLICK)
	{
		if (preMenuIndex != menuIndex)
		{
			// clear dose
			GUI_SetString2424(60, 64, "确认清除？", 0);
		}
		else
		{
			back = 1;
			cnt = 0;
			GUI_SetString2424(60 + 24, 64, "已清除", 0);
		}
	}

	if (back)
	{
		cnt++;
		GUI_SetString2424(60 + 24, 64, "已清除", 0);
		if (cnt > 5)
		{
			back = 0;
			menuIndex = menuList[menuIndex].back;
		}
		intDoseParam.dose_uSv = 0;
		intDoseParam.dose_uGy = 0;
	}
	else
	{
		GUI_SetString2424(60, 64, "确认清除？", 0);
	}
}

void menu21(uint8_t key)
{
	uint8_t height = 24;

	Display_statusBar();

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);
	// GUI_SetLine(160, 18, 124, 1);

	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetString1616(4, 24 + height * 0, "报警设置", 0);
	GUI_SetString1616(4, 24 + height * 1, "测量设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "累计剂量", 0);
	GUI_SetString1616(4, 24 + height * 3, "历史数据", 0);
	GUI_SetString1616(4, 24 + height * 4, "密码设置", 0);

	GUI_SetString1616(120 + 4, 24 + height * 0, "单位设置", 1);
	GUI_SetString1616(120 + 4, 24 + height * 1, "探测器类型", 0);

}

void menu22(uint8_t key)
{
	uint8_t height = 24;

	Display_statusBar();

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);
	// GUI_SetLine(160, 18, 124, 1);

	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetString1616(4, 24 + height * 0, "报警设置", 0);
	GUI_SetString1616(4, 24 + height * 1, "测量设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "累计剂量", 0);
	GUI_SetString1616(4, 24 + height * 3, "历史数据", 0);
	GUI_SetString1616(4, 24 + height * 4, "密码设置", 0);

	GUI_SetString1616(120 + 4, 24 + height * 0, "单位设置", 0);
	GUI_SetString1616(120 + 4, 24 + height * 1, "探测器类型", 1);
}

void menu211(uint8_t key)
{
	static uint8_t unit = 0;
	uint8_t height = 24;

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
			if (unit > 0)
			{
				unit--;
			}else{
				unit = 2;
			}
			
			break;
		case KEY_DOWN_CLICK:
			if (unit < 2)
			{
				unit++;
			}else{
				unit = 0;
			}
			break;
		default:
			break;
	}
	Display_statusBar();
	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	// GUI_SetLine(120, 18, 124, 1);
	Display_unitParam(90, 30, unit);
}

void menu221(uint8_t key)
{
	uint8_t height = 24;

	Display_statusBar();
	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);
	GUI_SetLine(120, 18, 124, 1);

	GUI_SetString1616(4, 24 + height * 0, "探测器1", 1);
	GUI_SetString1616(4, 24 + height * 1, "探测器2", 0);
	GUI_SetString1616(4, 24 + height * 2, "探测器3", 0);

}

void menu222(uint8_t key)
{
	uint8_t height = 24;

	Display_statusBar();
	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);
	// GUI_SetLine(120, 18, 124, 1);

	GUI_SetString1616(4, 24 + height * 0, "探测器A", 0);
	GUI_SetString1616(4, 24 + height * 1, "探测器B", 1);
	GUI_SetString1616(4, 24 + height * 2, "探测器C", 0);

}

void menu223(uint8_t key)
{
	uint8_t height = 24;

	Display_statusBar();
	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);
	// GUI_SetLine(120, 18, 124, 1);

	GUI_SetString1616(4, 24 + height * 0, "探测器A", 0);
	GUI_SetString1616(4, 24 + height * 1, "探测器B", 0);
	GUI_SetString1616(4, 24 + height * 2, "探测器C", 1);
}


void menu511(uint8_t key)
{

	static uint8_t ps[4] = {0};
	static uint8_t index = 0;

	switch (key)
	{
		case KEY_ENTER_CLICK:
			if (preMenuIndex == menuIndex)
			{
				if (index < 3)
				{
					index++;
				}
				else
				{
					passWord[0] = ps[0];
					passWord[1] = ps[1];
					passWord[2] = ps[2];
					passWord[3] = ps[3];
					menuIndex = menuList[menuIndex].back;
				}
			}
			else
			{
				index = 0;
				ps[0] = 0;
				ps[1] = 0;
				ps[2] = 0;
				ps[3] = 0;
			}
			break;
		case KEY_UP_CLICK:
			if (ps[index] < 9)
			{
				ps[index]++;
			}
			else
			{
				ps[index] = 0;
			}
			break;
		case KEY_DOWN_CLICK:
			if (ps[index] > 0)
			{
				ps[index]--;
			}
			else
			{
				ps[index] = 9;
			}
			break;
		default:
			break;
	}

	Display_statusBar();

	GUI_SetString2424(0, 136, "确认", 0);
	GUI_SetString2424(191, 136, "返回", 0);
	GUI_SetString2424(108, 136, "{}", 0);

	GUI_SetString2424(64, 32, "请输入密码", 0);

	GUI_SetChar2412(88, 72, ps[0], index == 0 ? 1 : 0);
	GUI_SetChar2412(108, 72, ps[1], index == 1 ? 1 : 0);
	GUI_SetChar2412(128, 72, ps[2], index == 2 ? 1 : 0);
	GUI_SetChar2412(148, 72, ps[3], index == 3 ? 1 : 0);

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 134, 240, 0);
}


void menu41(uint8_t key)
{
	uint8_t height = 24;

	Display_statusBar();

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);
	// GUI_SetLine(160, 18, 124, 1);

	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetString1616(4, 24 + height * 0, "报警设置", 0);
	GUI_SetString1616(4, 24 + height * 1, "测量设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "累计剂量", 0);
	GUI_SetString1616(4, 24 + height * 3, "历史数据", 0);
	GUI_SetString1616(4, 24 + height * 4, "密码设置", 0);

	GUI_SetString1616(120 + 4, 24 + height * 0, "存储时间", 1);
	GUI_SetString1616(120 + 4, 24 + height * 1, "数据查询", 0);
	GUI_SetString1616(120 + 4, 24 + height * 2, "数据清除", 0);
	GUI_SetString1616(120 + 4, 24 + height * 3, "存储空间", 0);
}

void menu42(uint8_t key)
{
	uint8_t height = 24;

	Display_statusBar();

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);
	// GUI_SetLine(160, 18, 124, 1);

	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetString1616(4, 24 + height * 0, "报警设置", 0);
	GUI_SetString1616(4, 24 + height * 1, "测量设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "累计剂量", 0);
	GUI_SetString1616(4, 24 + height * 3, "历史数据", 0);
	GUI_SetString1616(4, 24 + height * 4, "密码设置", 0);

	GUI_SetString1616(120 + 4, 24 + height * 0, "存储时间", 0);
	GUI_SetString1616(120 + 4, 24 + height * 1, "数据查询", 1);
	GUI_SetString1616(120 + 4, 24 + height * 2, "数据清除", 0);
	GUI_SetString1616(120 + 4, 24 + height * 3, "存储空间", 0);
}

void menu43(uint8_t key)
{
	uint8_t height = 24;

	Display_statusBar();

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);
	// GUI_SetLine(160, 18, 124, 1);

	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetString1616(4, 24 + height * 0, "报警设置", 0);
	GUI_SetString1616(4, 24 + height * 1, "测量设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "累计剂量", 0);
	GUI_SetString1616(4, 24 + height * 3, "历史数据", 0);
	GUI_SetString1616(4, 24 + height * 4, "密码设置", 0);

	GUI_SetString1616(120 + 4, 24 + height * 0, "存储时间", 0);
	GUI_SetString1616(120 + 4, 24 + height * 1, "数据查询", 0);
	GUI_SetString1616(120 + 4, 24 + height * 2, "数据清除", 1);
	GUI_SetString1616(120 + 4, 24 + height * 3, "存储空间", 0);
}

void menu44(uint8_t key)
{
	uint8_t height = 24;

	Display_statusBar();

	GUI_SetLine(0, 18, 240, 0);
	GUI_SetLine(0, 142, 240, 0);
	GUI_SetLine(120, 18, 124, 1);
	// GUI_SetLine(160, 18, 124, 1);

	GUI_SetString1616(0, 144, "确认", 0);
	GUI_SetString1616(191, 144, "返回", 0);
	GUI_SetString1616(108, 144, "{}", 0);

	GUI_SetString1616(4, 24 + height * 0, "报警设置", 0);
	GUI_SetString1616(4, 24 + height * 1, "测量设置", 0);
	GUI_SetString1616(4, 24 + height * 2, "累计剂量", 0);
	GUI_SetString1616(4, 24 + height * 3, "历史数据", 0);
	GUI_SetString1616(4, 24 + height * 4, "密码设置", 0);

	GUI_SetString1616(120 + 4, 24 + height * 0, "存储时间", 0);
	GUI_SetString1616(120 + 4, 24 + height * 1, "数据查询", 0);
	GUI_SetString1616(120 + 4, 24 + height * 2, "数据清除", 0);
	GUI_SetString1616(120 + 4, 24 + height * 3, "存储空间", 1);
}


/**********************************************************************************************************
 *
 *	ģ������ : ������
 *	�ļ����� : main.c
 *	��    �� : V1.0
 *	˵    �� : ������
 *
 *	�޸ļ�¼ :
 *		�汾��  ����             ����              ˵��
 *		V1.0    2022-04-08
 *
 **********************************************************************************************************/
#include "gd32f10x.h"
#include "bsp.h"
#include "user.h"
#include <math.h>
#include "bsp_uart.h"
#include "bsp_lcd.h"
#include "bsp_key.h"
#include "Task_gui.h"
#include "Task_dose.h"
#include "TASK_status.h"
/*********************************************************************************************************
 *                                            ��������
 *********************************************************************************************************/

#define  SOFT_RESET        0xAA

struct tag_sys_param sys_param;
struct tag_uwb_param tag_dis; // ����Դ
uint8 Voltage = 0u;

struct basic_param_typedef basicParam=
{
	1,      // keyBeep
	50,     // backLight
	10      // blTime
};

uint8_t passWord[4] = {0, 0, 0, 0};

uint32_t pulseCntInt = 0;
uint32_t pulseCntExt = 0;
uint8_t batVoltPerSmooth = 0;
float volt;
uint8_t detectorType = DETECTOR_NULL;

float warnRangeRate = 2.5;
float warnRangeDose = 50;
uint8_t displayUnit = 0;
uint8_t brightness = 50;

uint8_t test = 0;
uint8_t x1,x2,y1,y2;
uint32_t vad = 0;

void Task_manage(void);
void Task_com(uint8_t tick);
/*********************************************************************************************************
 *	�� �� ��: Bsp_Init
 *	����˵��: �ײ�������ʼ��
 *	��    ��: ��
 *	�� �� ֵ: ��
 *********************************************************************************************************/
void BSP_Init(void)
{
    BSP_GPIO_Init();
    BSP_EEPROM_Init();
    BSP_ADC_Init();
    BSP_Key_Init();
    BSP_PWM_Init();
    BSP_RTC_Init();
    BSP_TIM_Init();
    BSP_Uart_Init();
	
	SetLcdBacklight(50);
    LCD_Init();
}


/*********************************************************************************************************
 *	�� �� ��: main
 *	����˵��: ������
 *	��    ��: ��
 *	�� �� ֵ: ��
 **********************************************************************************************************/
int main(void)
{
	static uint32_t task1Cnt = 0;
	static uint32_t task2Cnt = 0;
	static uint32_t task3Cnt = 0;
	static uint32_t task4Cnt = 0;
	uint8_t tick = 0;
	uint8_t i = 0;
    setSystemTick(10); 

    BSP_Init(); 
    
//    SYS_Param_Init(); 
	for(i=0;i<10;i++)
	{
		HMI_start(i);
		delay_ms(100);
	}
    while (1)
    {
		if((TimerGetCnt() - task1Cnt) >= 10)
		{
			task1Cnt = TimerGetCnt();
			bsp_KeyScan();
		}
		
		if((TimerGetCnt() - task2Cnt) >= 100)
		{
			task2Cnt = TimerGetCnt();
			Task_status();
//			Task_manage();
		}
		
		if((TimerGetCnt() - task3Cnt) >= 1000)
		{
			task3Cnt = TimerGetCnt();
			task4Cnt++;
			Task_dose();
			Task_bat();
			tick = 1;
		}
		if(task4Cnt>60)
		{
			task4Cnt = 0;
			
		}
		Task_HMI();
		Task_com(tick);
		tick = 0;
		// switch(test)
		// {
		// 	case 1:
		// 		GUI_SetLine(10,x2,y1,1);
		// 		break;
		// 	default:
		// 		break;
		// }
//		test = 0;
    }

}

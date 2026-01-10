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

uint32_t pulseCntInt = 0;
uint32_t pulseCntExt = 0;
uint8_t batVoltPerSmooth = 0;
float volt;

float warnRangeRate = 2.5;
float warnRangeDose = 50;
uint8_t displayUnit = 0;
uint8_t brightness = 50;

uint8_t test = 0;
uint8_t x1,x2,y1,y2;
uint32_t vad = 0;
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

/***********************************************************************************
 * Function	    : SYS_Param_Init
 * Description	: ϵͳ�����ϵ��ʼ��
 * Input Para	:
 * Output Para   :
 * Return Value  :
 ***********************************************************************************/
void SYS_Param_Init(void)
{
	extern struct tag_chn_dat Chn_Data;
	
	uint8_t result;
	
	float sensi_value = 500.0f;
	
	float fact = 0.00067f;
	
	float back_ground = 0.0f;
	
	uint8_t unit = 0x01;
	

	read_eeprom(&result, e2rom_addr(_E2rom_Flag), 1);				/* 读取系统参数标志位 */
	if(result != SOFT_RESET)
	{
		delay_ms(50);
		read_eeprom(&result, e2rom_addr(_E2rom_Flag), 1);  
	}
	if(result != SOFT_RESET)
	{
		result = SOFT_RESET;
		
		write_eeprom((uint8_t*)&sensi_value, e2rom_addr(_Sensi_Value), 4);
		write_eeprom((uint8_t*)&fact, e2rom_addr(_Fact), 4);
		write_eeprom((uint8_t*)&back_ground, e2rom_addr(_BackGroung), 4);
		write_eeprom(&unit, e2rom_addr(_Unit), 1);
		
		
		write_eeprom(&result, e2rom_addr(_E2rom_Flag), 1);
		
	}
	
	delay_ms(50);
	read_eeprom((uint8_t*)&Chn_Data.sensi_value, e2rom_addr(_Sensi_Value), 4);
	read_eeprom((uint8_t*)&Chn_Data.fact, e2rom_addr(_Fact), 4);
	read_eeprom((uint8_t*)&Chn_Data.back_ground, e2rom_addr(_BackGroung), 4);
	read_eeprom(&Chn_Data.unit, e2rom_addr(_Unit), 1);
	
	Chn_Data.cps = 0.0f;
	Chn_Data.value = 0.0f;
	
}

void GUI_SetLine(uint8_t x, uint8_t y, uint8_t length, uint8_t dir);
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
	
	uint8_t i = 0;
    setSystemTick(10); 

    BSP_Init(); 
    
    SYS_Param_Init(); 
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
		}
		
		if((TimerGetCnt() - task3Cnt) >= 1000)
		{
			task3Cnt = TimerGetCnt();
			task4Cnt++;
//			Task_dose();
			Task_bat();
		}
		if(task4Cnt>60)
		{
			task4Cnt = 0;
			
		}
		Task_HMI();
		
		switch(test)
		{
			case 1:
				GUI_SetLine(10,x2,y1,1);
				break;
			default:
				break;
		}
//		test = 0;
    }

}

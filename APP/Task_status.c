#include "TASK_status.h"
#include "Task_dose.h"
#include "bsp_gpio.h"

extern float warnRangeRate ;
extern float warnRangeDose ;
extern uint8_t batVoltPerSmooth;

uint8_t ADC_GetVolPer(void);

const uint8_t beepOnTickTable[10] = {0,1,1,1,1,1,2,3,4,1}; 
const uint8_t beepOffTickTable[10] = {1,5,4,3,2,1,1,1,1,0};
	
uint8_t warnLevel = 0;

uint8_t chgState = 0;

uint8_t GetChgState(void)
{
	return chgState;
}

uint8_t BeepStateProcess(uint8_t warnLevel)
{
	static uint8_t step = 1; 
	static uint8_t preLevel = 0; 
	uint8_t beepState = 0;
	static uint8_t onTicks = 0;
	static uint8_t offTicks = 1;

	if(preLevel != warnLevel)
	{
		preLevel = warnLevel;
		step = 0;
	}
	
	switch(step)
	{
		case 0:
			onTicks = beepOnTickTable[warnLevel];
			offTicks = beepOffTickTable[warnLevel];
			step = 1;
		case 1://ready
			if(onTicks)
			{
				beepState = 1;
				onTicks--;
			}
			else
			{
				step = 2;
			}
			break;
		case 2:
			if(offTicks)
			{
				beepState = 0;
				offTicks--;
			}
			else
			{
				step = 0;
			}
			break;
		default:
			beepState = 0;
		break;
	}

	return beepState;	
}

uint8_t LedStateProcess(uint8_t warnLevel)
{
	uint8_t ledState = 0;
	static uint8_t step = 0;
	
	step++;
	
	if(warnLevel == 0)
	{
		if(GetChgState() == 2)
		{
			ledState |= 0x02;
		}else if(GetChgState() == 1)
		{
			if((step%20)<10)
			{
				ledState |= 0x02;
			}
		}else{
			if(step%20<10)
			{
				ledState |= 0x01;
			}
		}
		
	}else{
		ledState |=0x04;
	}
	
	return ledState;
}

void ChgStateProcess(void)
{
	if(CHG_plugCheck() == 0)
	{
		CHG_SetChg(0);
		chgState = 0;
	}else{
		if(ADC_GetVolPer() >99)
		{
			CHG_SetChg(0);
			chgState = 2;
		}else{
			if(CHG_GetChgState() == 1)
			{
				CHG_SetChg(1);
				chgState = 1;
			}else{
				CHG_SetChg(0);
				chgState = 2;
			}
		}
	}
}

void Task_status(void)
{
	uint8_t beepState = 0;
	uint8_t ledState = 0;
	
	uint8_t warnLevel1 = 0;
	uint8_t warnLevel2 = 0;
	
	warnLevel1  = intDoseParam.doserate_uSv_h/warnRangeRate;
	warnLevel2  = intDoseParam.dose_uSv/warnRangeDose;
	
	warnLevel=(warnLevel1>warnLevel2)?warnLevel1:warnLevel2;
	
	beepState = BeepStateProcess(warnLevel);
	BEEP_SetState(beepState);
	
	ledState = LedStateProcess(warnLevel);
	LED_SetState(ledState);
	
	ChgStateProcess();
}

void Task_bat(void)
{
	static uint8_t batper[6] = {0};
	static uint8_t step = 0;
	static uint8_t firstIn = 0;
	
	uint32_t sum =0;
	
	if(firstIn == 0)
	{
		firstIn = 1;
		
		batper[0] = ADC_GetVolPer();
		batper[1] = ADC_GetVolPer();
		batper[2] = ADC_GetVolPer();
		batper[3] = ADC_GetVolPer();
		batper[4] = ADC_GetVolPer();
		batper[5] = ADC_GetVolPer();
	}else{
		batper[step%6] = ADC_GetVolPer();
		step ++;
		
		for(uint8_t i=0;i<6;i++)
		{
			sum+=batper[i];
		}
		batVoltPerSmooth = sum/6;
	}
}

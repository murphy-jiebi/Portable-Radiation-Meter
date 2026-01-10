#include "Task_com.h"
#include "bsp_uart.h"
#include "modbus_slave.h"
#include "string.h"

uint8_t MonitPara[30];

/*
*********************************************************************************************************
*	? ? ?: list_init
*	????: ?????
*	?    ?: ?
*	? ? ?: ?
*********************************************************************************************************
*/
void ModBus_RegRefresh(void)
{
	memcpy(MonitPara + 0, (uint8_t*)&Chn_Data.value, 4);  /* ????? */
	*(MonitPara + 4) = 0x00;
	*(MonitPara + 5) = Chn_Data.nbr_status;  				/* NBR */
	memcpy(MonitPara + 6, (uint8_t*)&Chn_Data.sensi_value, 4);   /* ????? */
	memcpy(MonitPara + 10, (uint8_t*)&Chn_Data.fact, 4);   /* ???? */
	memcpy(MonitPara + 14, (uint8_t*)&Chn_Data.back_ground, 4);   /* ?? */
	*(MonitPara + 18) = 0x00;
	*(MonitPara + 19) = Chn_Data.unit;
	*(MonitPara + 20) = 0x00;
	*(MonitPara + 21) = SysInfo.DeviceID;
	
	memset(MonitPara + 22, 0x00, 8);
}

/*0
*********************************************************************************************************
*	? ? ?: Para_Refresh
*	????: ????
*	?    ?: ?
*	? ? ?: ?
*********************************************************************************************************
*/
static void Para_Refresh(void)
{
	uint16_t value;
	
	if(memcmp(MonitPara + 6, (uint8_t*)&Chn_Data.sensi_value, 4) != 0x00)
	{
		memcpy((uint8_t*)&Chn_Data.sensi_value, MonitPara + 6, 4);   /* ???? */
		write_eeprom((uint8_t*)&Chn_Data.sensi_value, e2rom_addr(_Sensi_Value), 4);
	}
	
	if(memcmp(MonitPara + 10, (uint8_t*)&Chn_Data.fact, 4) != 0x00)
	{
		memcpy((uint8_t*)&Chn_Data.fact, MonitPara + 10, 4);   /* ???? */
		write_eeprom((uint8_t*)&Chn_Data.fact, e2rom_addr(_Fact), 4);
	}
	
	if(memcmp(MonitPara + 14, (uint8_t*)&Chn_Data.back_ground, 4) != 0x00)
	{
		memcpy((uint8_t*)&Chn_Data.back_ground, MonitPara + 14, 4);   /* ?? */
		write_eeprom((uint8_t*)&Chn_Data.back_ground, e2rom_addr(_BackGroung), 4);
	}
	
	*(MonitPara + 18) = 0x00;
	if(*(MonitPara + 19) > 0x02)
	{
		*(MonitPara + 19) = 0x02;
	}
	if(*(MonitPara + 19) != Chn_Data.unit)
	{
		Chn_Data.unit = *(MonitPara + 19);
		write_eeprom(&Chn_Data.unit, e2rom_addr(_Unit), 1);
	}
	
	*(MonitPara + 20) = 0x00;
	if(*(MonitPara + 21) != SysInfo.DeviceID)
	{
		SysInfo.DeviceID = *(MonitPara + 21);
		write_eeprom(&SysInfo.DeviceID, e2rom_addr(_Device_Id), 1);
	}
	

	/* ?????? */
	value = (*(MonitPara + 22) << 8) + *(MonitPara + 23);
	if(value != 0x00)
	{
		*(MonitPara + 22) = 0x00;
		*(MonitPara + 23) = 0x00;
		set_eeprom(0x00, e2rom_addr(_E2rom_Flag), 1);		
		/* ???? */
		NVIC_SystemReset();
	}

}


void Task_com(void)
{
	
	if(g_tUart3.ucRxFrame != 0x00)
	{
		MODS_Poll(g_tUart2.pRxBuf, g_tUart2.usRxCount);
		memset(g_tUart2.pRxBuf, 0x00, g_tUart2.usRxCount);
		g_tUart2.usRxCount = 0;
		g_tUart2.ucRxFrame = 0x00;
		
		Para_Refresh();   /* ???? */
	}
	
}



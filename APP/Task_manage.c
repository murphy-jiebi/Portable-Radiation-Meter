#include "Task_manage.h"
#include "user.h"
#include "bsp_eeprom.h"

static uint32_t eepromSetFlag = 0;

void Uart3SetBaud(uint32_t baud);

void comUartSetup(uint8_t dtType)
{
    static uint8_t preType = 0;
    if(preType != dtType)
    {
        preType = dtType;

        if (dtType == DETECTOR_1)
        {
             Uart3SetBaud(19200);
        }
        else if (dtType == DETECTOR_2)
        {
            Uart3SetBaud(9600);
        }
        else    
        {
            Uart3SetBaud(9600);
        }
        
    }
}

/***********************************************************************************
 * Function	    : SYS_Param_Init
 * Description	: ϵͳ�����ϵ��ʼ��?
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

    read_eeprom(&result, EEPROM_ADDR_FLAG, 1); /* 读取系统参数标志�? */
    if (result != SOFT_RESET)
    {
        delay_ms(50);
        read_eeprom(&result, EEPROM_ADDR_FLAG, 1);
    }
    if (result != SOFT_RESET)
    {
        result = SOFT_RESET;

        write_eeprom((uint8_t *)&sensi_value, e2rom_addr(_Sensi_Value), 4);
        write_eeprom((uint8_t *)&fact, e2rom_addr(_Fact), 4);
        write_eeprom((uint8_t *)&back_ground, e2rom_addr(_BackGroung), 4);
        write_eeprom(&unit, e2rom_addr(_Unit), 1);

        write_eeprom(&result, e2rom_addr(_E2rom_Flag), 1);
    }

    delay_ms(50);
    read_eeprom((uint8_t *)&Chn_Data.sensi_value, e2rom_addr(_Sensi_Value), 4);
    read_eeprom((uint8_t *)&Chn_Data.fact, e2rom_addr(_Fact), 4);
    read_eeprom((uint8_t *)&Chn_Data.back_ground, e2rom_addr(_BackGroung), 4);
    read_eeprom(&Chn_Data.unit, e2rom_addr(_Unit), 1);

    Chn_Data.cps = 0.0f;
    Chn_Data.value = 0.0f;
}

void eepromSetStatus(uint32_t setType)
{
    eepromSetFlag |= setType;
}


void eepromDataRefresh(void)
{
    if(eepromSetFlag & EEPROM_SET_BASIC_PARAM)
    {
        write_eeprom((uint8_t *)&basicParam, EEPROM_ADDR_BASIC_PARAM, sizeof(basicParam));
    }
    
    if(eepromSetFlag & EEPROM_SET_WARN_RANGE)
    {
        write_eeprom((uint8_t *)&warnRangeRate, EEPROM_ADDR_WARN_RANGE, sizeof(warnRangeRate));
        write_eeprom((uint8_t *)&warnRangeDose, EEPROM_ADDR_WARN_RANGE + 4, sizeof(warnRangeDose));
    }

    if (eepromSetFlag & EEPROM_SET_INT_DOESE_PARAM)
    {
        write_eeprom((uint8_t *)intDoseParam, EEPROM_ADDR_INT_DOSE_PARAM, sizeof(intDoseParam));
    }

    if (eepromSetFlag & EEPROM_SET_DISPLAY_UNIT)
    {
        write_eeprom((uint8_t *)&displayUnit, EEPROM_ADDR_DISPLATY_UNIT, sizeof(displayUnit));
    }
    
    if(eepromSetFlag & EEPROM_SET_DETECTOR_TYPE)
    {
        write_eeprom((uint8_t *)&detectorType, EEPROM_ADDR_DETECTOR_TYPE, sizeof(detectorType));
    }

    if (eepromSetFlag & EEPROM_SET_DETECTOR_PARAM)
    {
        write_eeprom((uint8_t *)&Chn_Data, EEPROM_ADDR_DETECTOR_PARAM, sizeof(Chn_Data));
    }

        eepromSetFlag = 0;
}


void Task_manage(void)
{
    comUartSetup(detectorType);
}



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
 * Description	: ϵͳ�����ϵ��ʼ��??
 * Input Para	:
 * Output Para   :
 * Return Value  :
 ***********************************************************************************/
#define SOFT_RESET  0xA5

void SYS_Param_Init(void)
{
 
    uint8_t result;
    float sensi_value = 500.0f;
    float fact = 0.00067f;
	uint8_t unit = 1;
    IntDoseParam_typedef  temp_intDoseparam=
    {
        0,0,0,0,0,0.7f,0.0f,1.2f
    };
    struct basic_param_typedef temp_basicParam=
    {
        1,      // keyBeep
        50,     // backLight
        10      // blTime
    };

    float temp_warnRangeRate = 2.5;
    float temp_warnRangeDose = 50;
    uint8_t dt = DETECTOR_NBR;
    uint8_t temp_ps[4] = {0,0,0,0};

    read_eeprom(&result, EEPROM_ADDR_FLAG, 1); /* 读取系统参数标志�?? */
    if (result != SOFT_RESET)
    {
        delay_ms(50);
        read_eeprom(&result, EEPROM_ADDR_FLAG, 1);
    }
    if (result != SOFT_RESET)
    {
        result = SOFT_RESET;
        write_eeprom(&result, EEPROM_ADDR_FLAG, 1);
        
        write_eeprom((uint8_t *)&temp_basicParam, EEPROM_ADDR_BASIC_PARAM, sizeof(temp_basicParam));
        write_eeprom((uint8_t *)&temp_warnRangeRate, EEPROM_ADDR_WARN_RANGE, sizeof(temp_warnRangeRate));
        write_eeprom((uint8_t *)&temp_warnRangeDose, EEPROM_ADDR_WARN_RANGE + 4, sizeof(temp_warnRangeDose));
        write_eeprom((uint8_t *)&temp_intDoseparam, EEPROM_ADDR_INT_DOSE_PARAM, sizeof(temp_intDoseparam));
        write_eeprom((uint8_t *)&unit, EEPROM_ADDR_DISPLATY_UNIT, sizeof(unit));
        write_eeprom((uint8_t *)&dt, EEPROM_ADDR_DETECTOR_TYPE, sizeof(dt));
        write_eeprom((uint8_t *)&Chn_Data, EEPROM_ADDR_DETECTOR_PARAM, sizeof(Chn_Data));

        write_eeprom((uint8_t *)&fact, EEPROM_ADDR_DETECTOR_PARAM, sizeof(fact));
        write_eeprom((uint8_t *)&sensi_value, EEPROM_ADDR_DETECTOR_PARAM + 4, sizeof(sensi_value));
        read_eeprom((uint8_t *)&temp_ps, EEPROM_ADDR_PASSWORD, sizeof(temp_ps));
    }

    delay_ms(50);

    read_eeprom((uint8_t *)&basicParam, EEPROM_ADDR_BASIC_PARAM, sizeof(basicParam));
    read_eeprom((uint8_t *)&warnRangeRate, EEPROM_ADDR_WARN_RANGE, sizeof(warnRangeRate));
    read_eeprom((uint8_t *)&warnRangeDose, EEPROM_ADDR_WARN_RANGE + 4, sizeof(warnRangeDose));
    read_eeprom((uint8_t *)&intDoseParam, EEPROM_ADDR_INT_DOSE_PARAM, sizeof(intDoseParam));
    read_eeprom((uint8_t *)&displayUnit, EEPROM_ADDR_DISPLATY_UNIT, sizeof(displayUnit));
    read_eeprom((uint8_t *)&detectorType, EEPROM_ADDR_DETECTOR_TYPE, sizeof(detectorType));
    read_eeprom((uint8_t *)&Chn_Data.fact, EEPROM_ADDR_DETECTOR_PARAM, sizeof(Chn_Data.fact));
    read_eeprom((uint8_t *)&Chn_Data.sensi_value, EEPROM_ADDR_DETECTOR_PARAM + 4, sizeof(Chn_Data.sensi_value));
    read_eeprom((uint8_t *)&passWord, EEPROM_ADDR_PASSWORD, sizeof(passWord));

    Chn_Data.cps = 0.0f;
    Chn_Data.value = 0.0f;
    Chn_Data.back_ground =0.0f;
    Chn_Data.unit = 1;

    intDoseParam.doserate_cps = 0.0f;
    intDoseParam.doserate_uSv_h = 0.0f;
    intDoseParam.doserate_uGy_h = 0.0f;
    if (passWord[0] > 9)
    {
        passWord[0] = 0;
    }
    if (passWord[1] > 9)
    {
        passWord[1] = 0;
    }
    if (passWord[2] > 9)
    {
        passWord[2] = 0;
    }
    if (passWord[3] > 9)
    {
        passWord[3] = 0;
    }
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
        write_eeprom((uint8_t *)&intDoseParam, EEPROM_ADDR_INT_DOSE_PARAM, sizeof(intDoseParam));
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
        write_eeprom((uint8_t *)&Chn_Data.fact, EEPROM_ADDR_DETECTOR_PARAM, sizeof(Chn_Data.fact));
        write_eeprom((uint8_t *)&Chn_Data.sensi_value, EEPROM_ADDR_DETECTOR_PARAM + 4, sizeof(Chn_Data.sensi_value));

    }
    if (eepromSetFlag & EEPROM_SET_PASSWORD)
    {
        
    }
        eepromSetFlag = 0;
}


void Task_manage(void)
{
    comUartSetup(detectorType);
    eepromDataRefresh();
}



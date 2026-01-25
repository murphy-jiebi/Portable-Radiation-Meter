#include "Task_dose.h"
#include "user.h"  

extern uint32_t pulseCntInt ;
extern uint32_t pulseCntExt ;

static float list_buff[100];
struct tag_chn_dat Chn_Data;
static list_t ch_cps;
static uint32_t ch_size;

IntDoseParam_typedef intDoseParam=
{
	0,0,0,0,0,1.0f,0.0f,1.2f
};


void list_init(list_t *cbuf, float *buf, uint32_t size) 
{
    cbuf->buf = buf;

    cbuf->tail = 0;
    
    cbuf->fast_up = 0;
    
    cbuf->fast_down = 0;
    
    cbuf->fast_back = 0;

    cbuf->size = size;
}

void list_add(list_t *cbuf, float dat) 
{
    uint32_t next_tail;

    next_tail = cbuf->tail + 1;

    if (next_tail >= cbuf->size) 
    {
        next_tail = 0;
    }
        
    cbuf->buf[cbuf->tail] = dat;

    cbuf->tail = next_tail;
}

void list_refresh(list_t *cbuf, float dat) 
{
    uint32_t i;
    
    for(i = 0; i< cbuf->size; i++)
    {
        cbuf->buf[i] = dat;
    }
}

float get_list_avage(list_t *cbuf)
{
    uint16_t i;
    float  avage = 0.0f;
    
    for( i = 0x00; i < cbuf->size; i++)
    {
        avage += cbuf->buf[i];
    }
    if(cbuf->size != 0)
    {
        avage /= cbuf->size;
    }
    else
    {
        avage = 0.0f;
    }
    return avage;
}

void chn_clac_init(void)
{
    ch_size = 60;
    list_init(&ch_cps, list_buff, ch_size);
    list_refresh(&ch_cps, 0.0f);
}


void IntDoseCalc(void)
{
	//TODO:cps data smooth function
	
	intDoseParam.doserate_cps 		= pulseCntInt /1.0f;
	
	if(intDoseParam.doserate_cps * intDoseParam.deadtime > 0.97f)
	{
		intDoseParam.doserate_uSv_h = intDoseParam.doserate_cps;
	}else{
		intDoseParam.doserate_uSv_h = intDoseParam.doserate_cps / (1 - intDoseParam.deadtime * intDoseParam.doserate_cps);
		intDoseParam.doserate_uSv_h /= intDoseParam.factor;
	}
	intDoseParam.doserate_uGy_h = intDoseParam.doserate_uSv_h * intDoseParam.transferFactor;
	
	intDoseParam.dose_uSv += (intDoseParam.doserate_uSv_h /3600.0f);
	intDoseParam.dose_uGy += (intDoseParam.doserate_uGy_h /3600.0f);
}


void ExtDoseCalc(void)
{
	float cps, value;
	
	cps = pulseCntExt / 1.0f;
	
/*
*********************************************************************************************************
*                                            平滑后计算
*********************************************************************************************************
*/
    if(cps < Chn_Data.sensi_value)                           /* 10个计数以内不做快速响应 */
    {      
        ch_cps.fast_up = 0x00;
        ch_cps.fast_down = 0x00;
        if(Chn_Data.cps > Chn_Data.sensi_value)             /* 快速回归本底 */
        {
            ch_cps.fast_back++;
            if(ch_cps.fast_back > 0x05)
            {
                ch_cps.fast_back = 0x00;
                list_refresh(&ch_cps, cps);
            }
            else
            {
                 list_add(&ch_cps, Chn_Data.cps); 
            }
        }
        else
        {
            list_add(&ch_cps, cps);      
        }

    }
    else
    {
        ch_cps.fast_back = 0x00;
        if(cps > (Chn_Data.cps * 1.3f))
        {
            ch_cps.fast_down = 0x00;
            ch_cps.fast_up ++;
            if(ch_cps.fast_up > 0x05)
            {
                ch_cps.fast_up = 0x00;
                list_refresh(&ch_cps, cps);
            }
            else
            {
                 list_add(&ch_cps, Chn_Data.cps); 
            }
        }
        else if(cps < (Chn_Data.cps * 0.7f))
        {
            ch_cps.fast_up = 0x00;
            ch_cps.fast_down++;
            if(ch_cps.fast_down > 0x05)
            {
                ch_cps.fast_down = 0x00;
                list_refresh(&ch_cps, cps);
            }
            else
            {
                 list_add(&ch_cps, Chn_Data.cps); 
            }
        }
        else
        {
            list_add(&ch_cps, cps); 
        }
    }
    /* 计算平均计数率 */
    Chn_Data.cps = get_list_avage(&ch_cps);
    
    if(Chn_Data.unit == 0x00)                     /* 单位为cps */
    {
        Chn_Data.value = Chn_Data.cps;
    }
    else 
    {
        value = Chn_Data.cps * Chn_Data.fact;
        if(value > Chn_Data.back_ground)
        {
            value -= Chn_Data.back_ground;
        }
        if(Chn_Data.unit == 0x01)                /* 单位为Sv/h */
        {
            Chn_Data.value = value * 1.16f;
        }
        else 
        {
             Chn_Data.value = value;
        }     
    }
}


void Task_dose(void)
{
	IntDoseCalc();
	ExtDoseCalc();
}



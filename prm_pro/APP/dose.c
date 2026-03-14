/**********************************************************************************************************
**********************************************************************************************************
* FILE		   : Task_Dose.C
* Description  : 剂量率计算模块
*			  
* Copyright (c) 2023 by SHAANXI WEIFENG INSTRUMENT Corp.,Ltd   All Rights Reserved.
* 
* History      :
*
* Version		  Name       	   	Date			           Description
   0.1		     jinlei  	      2023/01/07                 Initial Version
   
**********************************************************************************************************
**********************************************************************************************************/
#include <stdio.h>
#include "bsp.h"
#include "user.h"


#define  MAX_AVERAGE_COUNT		100
#define  MAX_FAST_COUNT			5
#define	 FAST_MIN_COUNT			5	//5个计数以内不做快速响应
#define	 FAST_UP				1.3	//(参数)快速响应倍数
#define	 FAST_DOWN			    0.7	//(参数)快速响应倍数




//全局变量定义
static uint8  Count_No_Sign = 0x00;                              //计数管无信号计数
static uint16 Count_Slide   = 0x00;
static uint8  Count_Fast    = 0x00;
static float  Slide_Buff[MAX_AVERAGE_COUNT];                     //数据平滑队列        
static float  Fast_Slide_Buff[MAX_FAST_COUNT];                   //快速响应数据队列
static float  Cps_Avage;                                         //平均计数率



/***********************************************************************************
* Function	    : get_avage_count
* Description	: 计算平均值
* Input Para	: 
* Output Para   : 
* Return Value  : 
***********************************************************************************/
static float get_avage_count(float *buff,uint16 len)
{
    uint16 i;
    float  count_total = 0x00000000;
    float  count_avage = 0.0L;
    
    for( i = 0x00; i < len; i++)
    {
        count_total += buff[i];
    }
    if(count_total != 0) 
    {
        count_avage = count_total / len;
    }
    else
    {
        count_avage = 0.0L;
    }
    return count_avage;
}

/***********************************************************************************
* Function	    : add_buff_count
* Description	: 增加buff中数据
* Input Para	: 
* Output Para   : 
* Return Value  : 
***********************************************************************************/
static void add_buff_count(float cps)
{
    Slide_Buff[Count_Slide]  = cps;
    Count_Slide ++;
    if(Count_Slide >= MAX_AVERAGE_COUNT)
    {
        Count_Slide = 0x00;
    }
}

/***********************************************************************************
* Function	    : add_fast_buff_count
* Description	: 增加buff中数据
* Input Para	: 
* Output Para   : 
* Return Value  : 
***********************************************************************************/
static void add_fast_buff_count(float cps)
{
    Fast_Slide_Buff[Count_Fast]  = cps;
    Count_Fast ++;
    if(Count_Fast >= MAX_FAST_COUNT)
    {
        Count_Fast = 0x00;
    }
}


/***********************************************************************************
* Function	    : count_data_check
* Description	: 计数数据检查
* Input Para	: 
* Output Para   : 
* Return Value  : 
***********************************************************************************/
static void count_data_check(uint32 count,uint8 sec)
{
    uint16 i;
    float   cps,count_fast_avage;
    static  uint8 fast_up_count = 0x00;
    static  uint8 fast_down_count = 0x00;
    if(count == 0)
    {
        if(Count_No_Sign < 120)
        {
            Count_No_Sign ++;       
        }
    }
    else
    {
        Count_No_Sign = 0x00;        
    }
    
    if(sec != 0x00)
    {
        cps = count; 
        cps /= sec;
    } 
    else
    {
        cps = 0.0L;
    } 
    add_fast_buff_count(cps);
    count_fast_avage = get_avage_count(Fast_Slide_Buff,MAX_FAST_COUNT);                
    if((count_fast_avage > (Cps_Avage * FAST_UP))&&(count_fast_avage > FAST_MIN_COUNT))                          //快速上升
    {
        fast_up_count ++;
        if(fast_down_count > 0x00)
        {
            fast_down_count--;
        }
        if(fast_up_count >= MAX_FAST_COUNT)
        {
            fast_up_count = 0x00;
            Count_Slide = 0x00;
            Count_Fast  = 0x00;
            for( i = 0x00; i < MAX_AVERAGE_COUNT ; i++)
            {
                 Slide_Buff[i]  = count_fast_avage;
            }
        }
        else
        {
            add_buff_count(Cps_Avage);                                                //使用平均值填充
        }
    }
    else if((count_fast_avage < (Cps_Avage * FAST_DOWN))&&(count_fast_avage > 0.2L))    //快速下降
    {
        if(fast_up_count > 0x00)
        {
            fast_up_count--;
        }
        fast_down_count ++;
        if(fast_down_count >= MAX_FAST_COUNT)
        {
            fast_down_count = 0x00;
            Count_Slide = 0x00;
            Count_Fast  = 0x00;
            for( i = 0x00; i < MAX_AVERAGE_COUNT ; i++)
            {
                Slide_Buff[i]  = count_fast_avage;
            }
        }
        else
        {
             add_buff_count(Cps_Avage);                         //使用平均值填充 
        }
     }
     else                                                       //合理范围之内
     {
        if(fast_up_count > 0x00)
        {
            fast_up_count--;
        }
        if(fast_down_count > 0x00)
        {
            fast_down_count--;
        }
        add_buff_count(cps);                               
     }          
}

/***********************************************************************************
* Function	    : count_to_rate
* Description	: 计数率转化为剂量率
* Input Para	: 
* Output Para   : 
* Return Value  : 
***********************************************************************************/
static float count_to_rate(float cps)
{
    uint8 i;
    float value = 0L;
    


    value = cps * intDoseParam.deadtime;
    if(value < 1)
    {
        value = cps/(1- value);
    }
    else
    {
        value = cps;
    }
    value /= intDoseParam.factor;


    return value;  
}


/***********************************************************************************
* Function	    : Dose_Init
* Description	: 
* Input Para	: 
* Output Para   : 
* Return Value  : 
***********************************************************************************/
void Dose_Init(void)
{	
    uint16 i;
    
    for( i = 0x00; i < MAX_FAST_COUNT ; i++)
    {
        Fast_Slide_Buff[i]  = 0.0L;
    }
    for(i = 0x00; i < MAX_AVERAGE_COUNT ; i++)
    {
        Slide_Buff[i] = 0.2L;
    }
    Cps_Avage = 0.2L;

}


/***********************************************************************************
* Function	    : Get_ins_cps
* Description	: 获取瞬时计数率
* Input Para	: 
* Output Para   : 
* Return Value  : 
***********************************************************************************/
float Get_ins_cps(void)
{
    float      cps_ins;
    
    cps_ins = get_avage_count(Fast_Slide_Buff,MAX_FAST_COUNT); 
    
    return cps_ins;
}

/***********************************************************************************
* Function	    : CalcDose
* Description	: 剂量计算
* Input Para	: 
* Output Para   : 
* Return Value  : 
***********************************************************************************/
float CalcDose(uint32_t dose_count)
{
    float      dose_ins;
    
    count_data_check(dose_count,1);
    if(Count_No_Sign >= 120)
    {
        //计数管有问题
        dose_ins = 0.0L;
    }
    else
    {
        //计算剂量数据
        Cps_Avage   = get_avage_count(Slide_Buff,MAX_AVERAGE_COUNT);
        dose_ins    = count_to_rate(Cps_Avage);               
     }
    return dose_ins;
}





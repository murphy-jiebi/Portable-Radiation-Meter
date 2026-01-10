#ifndef _TASK_DOSE_H
#define _TASK_DOSE_H

#include <stdint.h>


typedef struct
{
	float doserate_cps;
	float doserate_uSv_h;
	float doserate_uGy_h;
	float dose_uSv;
	float dose_uGy;
	float factor;
	float deadtime;
	float transferFactor;
}IntDoseParam_typedef;

 struct tag_chn_dat
{
	uint8_t nbr_status;
	
	float cps; 
    
    float value;
    
    float fact;
    
    float back_ground;
	
	float sensi_value; /* 快速响应值 */
    
    uint8_t unit;
};

typedef struct
{
    float *buf;         // 存储元素的指针

    uint32_t tail;      // 队列指针
    
    uint32_t fast_up;   // 快速上升
    
    uint32_t fast_down; // 快速下降
    
    uint32_t fast_back; // 快速本底

    uint32_t size;      // 队列大小

} list_t;

extern IntDoseParam_typedef intDoseParam;


void Task_dose(void);

#endif


#ifndef _TASK_DOSE_H
#define _TASK_DOSE_H

#include <stdint.h>


typedef struct
{
    float *buf;         // 存储元素的指针

    uint32_t tail;      // 队列指针
    
    uint32_t fast_up;   // 快速上升
    
    uint32_t fast_down; // 快速下降
    
    uint32_t fast_back; // 快速本底

    uint32_t size;      // 队列大小

} list_t;


float CalcDose(uint32_t dose_count);

void Task_dose(void);

#endif


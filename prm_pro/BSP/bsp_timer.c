/**********************************************************************************************************
**********************************************************************************************************
* FILE		   : BSP_UWB.C
* Description  : UWBģ������
*			  
* Copyright (c) 2016 by SHAANXI WEIFENG INSTRUMENT Corp.,Ltd   All Rights Reserved.
* 
* History      :
*
* Version		  Name       	   	Date			           Description
   0.1		      jinlei      	    2024/10/30                 Initial Version
   
**********************************************************************************************************
**********************************************************************************************************/
#include "bsp.h"
#include "user.h"


/*static*/ uint32_t timerCnt = 0;
static uint32_t timer0OverflowCnt = 0;
static uint32_t timer1OverflowCnt = 0;

extern uint32_t pulseCntInt ;
extern uint32_t pulseCntExt ;

uint32_t TimerGetCnt(void)
{
	return timerCnt;
}
/******************************************************************************
�������ƣ�TIM0_Init
����������TIM0��ʼ�� 1�붨ʱ��
�����������
�����������
�������أ���
******************************************************************************/
void TIM0_Init(void)
{
    /* TIMER0 configuration: generate PWM signals with different duty cycles:
       TIMER0CLK = SystemCoreClock / 108 = 1MHz */
    timer_parameter_struct timer_initpara;
	rcu_periph_clock_enable(RCU_AF);
	
    rcu_periph_clock_enable(RCU_TIMER0);
	
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
	
    timer_deinit(TIMER0);

    /* TIMER0 configuration */
    timer_initpara.prescaler         = 0;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 0xFFFF;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER0,&timer_initpara);

    timer_input_trigger_source_select(TIMER0, TIMER_SMCFG_TRGSEL_ETIFP);
	
    timer_slave_mode_select(TIMER0, TIMER_SLAVE_MODE_EXTERNAL0);
    
	timer_external_clock_mode1_config(TIMER0, TIMER_EXT_TRI_PSC_OFF, TIMER_ETP_RISING, 0x0);
	
	timer_master_slave_mode_config(TIMER0, TIMER_MASTER_SLAVE_MODE_ENABLE);
	
	timer_counter_value_config(TIMER0, 0);
	
	timer_interrupt_flag_clear(TIMER0, TIMER_INT_FLAG_UP);

    nvic_irq_enable(TIMER0_TRG_CMT_IRQn, 0, 0);
	
	timer_interrupt_enable(TIMER0, TIMER_INT_UP);
	
    timer_enable(TIMER0);
}

void TIM1_Init(void)
{
	timer_parameter_struct timer_initpara;
    
    rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_TIMER1);
    
	
	gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
	
    timer_deinit(TIMER1);
    
        timer_struct_para_init(&timer_initpara);

    /* TIMER0 configuration */
    timer_initpara.prescaler         = 0;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 0xFFFF;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1,&timer_initpara);
	
    
//    timer_input_trigger_source_select(TIMER1, TIMER_SMCFG_TRGSEL_ETIFP);
//	
//    timer_slave_mode_select(TIMER1, TIMER_SLAVE_MODE_EXTERNAL0);
//    
//	timer_external_clock_mode1_config(TIMER1, TIMER_EXT_TRI_PSC_OFF, TIMER_ETP_FALLING, 0x0);
//	
//	timer_master_slave_mode_config(TIMER1, TIMER_MASTER_SLAVE_MODE_ENABLE);
//	
//	timer_counter_value_config(TIMER1, 0);
//	
//	timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);

//    nvic_irq_enable(TIMER1_IRQn, 0, 0);
//	
//	timer_interrupt_enable(TIMER1, TIMER_INT_UP);
//	
//    timer_enable(TIMER1);

    timer_slave_mode_select(TIMER1,TIMER_SLAVE_MODE_EXTERNAL0);
    timer_input_trigger_source_select(TIMER1, TIMER_SMCFG_TRGSEL_ETIFP);
    timer_external_clock_mode1_config(TIMER1, TIMER_EXT_TRI_PSC_OFF, TIMER_ETP_RISING, 0X0);
    timer_master_slave_mode_config(TIMER1, TIMER_MASTER_SLAVE_MODE_ENABLE);
    timer_counter_value_config(TIMER1, 0);
    
    /* enable the TIMER interrupt */
    timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);
    timer_interrupt_enable(TIMER1, TIMER_INT_UP);
	
	timer_enable(TIMER1);
	
	/* enable and set key EXTI interrupt to the specified priority */
    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    nvic_irq_enable(TIMER1_IRQn, 0, 0);
}

/******************************************************************************
�������ƣ�TIM5_Init
����������TIM5��ʼ�� 1ms�붨ʱ��  54MHz
�����������
�����������
�������أ���
******************************************************************************/
void TIM5_Init(void)
{
	timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER5);
	
    timer_deinit(TIMER5);

    /* TIMER0 configuration */
    timer_initpara.prescaler         = 108-1;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 999;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER5,&timer_initpara);


    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER5);
    timer_flag_clear(TIMER5,TIMER_FLAG_UP);
    timer_interrupt_enable(TIMER5,TIMER_INT_UP);
    timer_enable(TIMER5);
	
    nvic_irq_enable(TIMER5_IRQn,0,1);
	
}

void BSP_TIM_Init(void)
{
    TIM0_Init();
    TIM1_Init();
    TIM5_Init();
}

void TIMER0_UP_IRQHandler(void)
{
	if(RESET != timer_interrupt_flag_get(TIMER0,TIMER_INT_FLAG_UP))
	{
		timer_interrupt_flag_clear(TIMER0,TIMER_INT_FLAG_UP);
		
		timer0OverflowCnt++;
	}
}
void TIMER1_IRQHandler(void)
{
	if(RESET != timer_interrupt_flag_get(TIMER1,TIMER_INT_FLAG_UP))
	{
		timer_interrupt_flag_clear(TIMER1,TIMER_INT_FLAG_UP);
		
		timer1OverflowCnt++;
	}
}



void TIMER5_IRQHandler(void)
{
	
	if(RESET != timer_interrupt_flag_get(TIMER5,TIMER_INT_UP))
	{
		timer_interrupt_flag_clear(TIMER5,TIMER_INT_UP);
		
		timerCnt++;
	
	}
}

void PulseCntGet(void)
{
    pulseCntInt = TIMER_CNT(TIMER0) + timer0OverflowCnt * 65535;
    pulseCntExt = TIMER_CNT(TIMER1) + timer1OverflowCnt * 65535;
    
    timer0OverflowCnt = 0;
    timer1OverflowCnt = 0;
    
    timer0OverflowCnt = 0;
    timer1OverflowCnt = 0;
    TIMER_CNT(TIMER0) = 0;
    TIMER_CNT(TIMER1) = 0;
    
}


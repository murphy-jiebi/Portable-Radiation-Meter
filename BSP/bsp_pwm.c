#include "bsp_pwm.h"
#include "gd32f10x.h"


void BSP_PWM_Init(void)
{
	
	timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;
	
	rcu_periph_clock_enable(RCU_TIMER3);
	rcu_periph_clock_enable(RCU_GPIOB);
	
	gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	
	timer_deinit(TIMER3);
	
    /* TIMER1 configuration */
    timer_initpara.prescaler         = 54 - 1;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 1000 - 1;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER3, &timer_initpara);	
	
    /* CH4 configuration in PWM mode1 */
    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
	
    timer_channel_output_config(TIMER3, TIMER_CH_3, &timer_ocintpara);

    /* CH1 configuration in PWM mode1,duty cycle 25% */
    timer_channel_output_pulse_value_config(TIMER3, TIMER_CH_3, 500 - 1);
    timer_channel_output_mode_config(TIMER3, TIMER_CH_3, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER3, TIMER_CH_3, TIMER_OC_SHADOW_DISABLE);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER3);
    /* auto-reload preload enable */
    timer_enable(TIMER3);
	
}

/*
*********************************************************************************************************
*	函 数 名: SetLcdBacklight
*	功能说明: 设置背光亮度
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SetLcdBacklight(uint8_t _ucRate)
{
	uint32_t pulse;
	
	pulse = _ucRate * 10;
	
	TIMER_CH3CV(TIMER3) = (uint32_t)pulse;
}

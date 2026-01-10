/**********************************************************************************************************
*
*	模块名称 : adc电量监测
*	文件名称 : bsp_adc.c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期             作者              说明
*		V1.0    2022-04-08   
*
**********************************************************************************************************/
#include "gd32f10x_it.h"
#include "bsp.h"
#include "user.h"

#define ADC_CHANNEL    ADC_CHANNEL_11   // PC1对应ADC通道11
#define ADC_GPIO_PORT  GPIOC
#define ADC_GPIO_PIN   GPIO_PIN_1

// 电压计算参数
#define VREF            3.3f           // 参考电压3.3V
#define ADC_RESOLUTION  4095.0f        // 12位ADC分辨率

/***********************************************************************************
* Function	    : Gpio_Init
* Description	: 初始化gpio
* Input Para	: 
* Output Para	: 
* Return Value  : 
***********************************************************************************/
static void Gpio_Init(void)
{
	rcu_periph_clock_enable(RCU_GPIOC);
	gpio_init(ADC_GPIO_PORT,GPIO_MODE_AIN,GPIO_OSPEED_10MHZ,ADC_GPIO_PIN);
}
 
/***********************************************************************************
* Function	    : Adc_Init
* Description	: 初始化adc配置 
* Input Para	: 
* Output Para	: 
* Return Value  : 
***********************************************************************************/
static void Adc_Init(void)
{
	rcu_periph_clock_enable(RCU_GPIOC);
	// 使能时钟
	rcu_periph_clock_enable(RCU_ADC0);
	// 设置分频系数，设置分频系数为6分频,120M的时钟频率6分频后得到的时钟主频为20
	rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);
	
	 adc_deinit(ADC0);
	// 设置独立模式
	adc_mode_config(ADC_MODE_FREE);
	// 设置单次模式
	adc_special_function_config(ADC0,ADC_CONTINUOUS_MODE,DISABLE);
	adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE);
	// 设置数据对齐
	adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT);
	// 设置和转换通道个数
	adc_channel_length_config(ADC0,ADC_REGULAR_CHANNEL,1);
	// 设置转换的通道以及所处在的序列位置,PC5对应的通道为15，放在寄存器序列0中，239.5个周期
	adc_regular_channel_config(ADC0,0,ADC_CHANNEL_11,ADC_SAMPLETIME_239POINT5);
	// 设置选择哪一个外部触发源，使用软件的方式触发
	adc_external_trigger_source_config(ADC0,ADC_REGULAR_CHANNEL,ADC0_1_2_EXTTRIG_REGULAR_NONE);
	// 使能外部触发,规则组触发，使能
	adc_external_trigger_config(ADC0,ADC_REGULAR_CHANNEL,ENABLE);
	// 使能ADC
	adc_enable(ADC0);
	delay_ms(1);
	// 内部校准
	adc_calibration_enable(ADC0);
 
	
}
 
/***********************************************************************************
* Function	    : BSP_ADC_Init
* Description	: 初始化adc 
* Input Para	: 
* Output Para	: 
* Return Value  : 
***********************************************************************************/
void BSP_ADC_Init(void)
{
	Gpio_Init();
	Adc_Init();
}
 
 
/***********************************************************************************
* Function	    : GetAdcVal
* Description	: 获取电压
* Input Para	: 
* Output Para	: 
* Return Value  : 
***********************************************************************************/
uint16_t GetAdcVal(void)
{
	// 使能软件触发,每一次转换都需要软件触发一次
	adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
	// 判断ADC标志位是否置位
	while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
	// 读取ADC规则组数据并返回
	return adc_regular_data_read(ADC0);
}
 
/***********************************************************************************
* Function	    : Get_voltage
* Description	: 获取电压
* Input Para	: 
* Output Para	: 
* Return Value  : 
***********************************************************************************/
void Get_voltage(void)
{
	float vol_temp = 0;
	static int8_t Voltage_buff_count = 59;
	static float Voltage_buff_sum = 0.0f,Voltage_buff[60] = {0};
	// 获取返回的数据
	uint16_t AdcVal = GetAdcVal();
	// 对数据进行转换，将数据转换为电压的值
	vol_temp = (float)AdcVal / 4095 * 3.3f;
	
	
	vol_temp = vol_temp - 1.7f;
	if(vol_temp < 0)
	{
		vol_temp = 0;
	}
	else if(vol_temp > 0.35)
	{
		vol_temp = 100;	
	}
	else
	{
		vol_temp = vol_temp*100;
		vol_temp = (uint8_t)(vol_temp/0.35);
	}
 
	if(Voltage_buff_count >= 0)
	{
		Voltage_buff[Voltage_buff_count] = vol_temp;
		Voltage_buff_count-- ;
		Voltage_buff_sum += vol_temp;
		Voltage = Voltage_buff_sum/(59 -Voltage_buff_count);
	}
	else
	{
		Voltage_buff_sum = 0;
		for(uint8_t i=59;i>0;i--)
		{
			Voltage_buff[i] = Voltage_buff[i-1];
			Voltage_buff_sum += Voltage_buff[i-1];        
		}
		Voltage_buff[0] = vol_temp;
		Voltage_buff_sum += Voltage_buff[0];
		Voltage = Voltage_buff_sum/60;
	}
}
 
uint8_t ADC_GetVolPer(void)
{
	float vol_temp = 0;
	uint8_t per = 0;
	uint16_t AdcVal = GetAdcVal();
	// 对数据进行转换，将数据转换为电压的值
	vol_temp = (float)AdcVal / 4095 * 3.3f;
	
	per = (vol_temp - 1.4)/(2.1-1.4);
	if(per > 100)
	{
		per = 100;
	}
	return per;
}
 
 
 


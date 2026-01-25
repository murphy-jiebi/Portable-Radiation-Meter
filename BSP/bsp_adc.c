/**********************************************************************************************************
*
*	ģ������ : adc�������
*	�ļ����� : bsp_adc.c
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����             ����              ˵��
*		V1.0    2022-04-08   
*
**********************************************************************************************************/
#include "gd32f10x_it.h"
#include "bsp.h"
#include "user.h"

#define ADC_CHANNEL    ADC_CHANNEL_11   // PC1��ӦADCͨ��11
#define ADC_GPIO_PORT  GPIOC
#define ADC_GPIO_PIN   GPIO_PIN_1

// ��ѹ�������
#define VREF            3.3f           // �ο���ѹ3.3V
#define ADC_RESOLUTION  4095.0f        // 12λADC�ֱ���

/***********************************************************************************
* Function	    : Gpio_Init
* Description	: ��ʼ��gpio
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
* Description	: ��ʼ��adc���� 
* Input Para	: 
* Output Para	: 
* Return Value  : 
***********************************************************************************/
static void Adc_Init(void)
{
	rcu_periph_clock_enable(RCU_GPIOC);
	// ʹ��ʱ��
	rcu_periph_clock_enable(RCU_ADC0);
	// ���÷�Ƶϵ�������÷�Ƶϵ��Ϊ6��Ƶ,120M��ʱ��Ƶ��6��Ƶ��õ���ʱ����ƵΪ20
	rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);
	
	 adc_deinit(ADC0);
	// ���ö���ģʽ
	adc_mode_config(ADC_MODE_FREE);
	// ���õ���ģʽ
	adc_special_function_config(ADC0,ADC_CONTINUOUS_MODE,DISABLE);
	adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE);
	// �������ݶ���
	adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT);
	// ���ú�ת��ͨ������
	adc_channel_length_config(ADC0,ADC_REGULAR_CHANNEL,1);
	// ����ת����ͨ���Լ������ڵ�����λ��,PC5��Ӧ��ͨ��Ϊ15�����ڼĴ�������0�У�239.5������
	adc_regular_channel_config(ADC0,0,ADC_CHANNEL_11,ADC_SAMPLETIME_239POINT5);
	// ����ѡ����һ���ⲿ����Դ��ʹ�������ķ�ʽ����
	adc_external_trigger_source_config(ADC0,ADC_REGULAR_CHANNEL,ADC0_1_2_EXTTRIG_REGULAR_NONE);
	// ʹ���ⲿ����,�����鴥����ʹ��
	adc_external_trigger_config(ADC0,ADC_REGULAR_CHANNEL,ENABLE);
	// ʹ��ADC
	adc_enable(ADC0);
	delay_ms(1);
	// �ڲ�У׼
	adc_calibration_enable(ADC0);
 
	
}
 
/***********************************************************************************
* Function	    : BSP_ADC_Init
* Description	: ��ʼ��adc 
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
* Description	: ��ȡ��ѹ
* Input Para	: 
* Output Para	: 
* Return Value  : 
***********************************************************************************/
uint16_t GetAdcVal(void)
{
	// ʹ����������,ÿһ��ת������Ҫ��������һ��
	adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
	// �ж�ADC��־λ�Ƿ���λ
	while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
	// ��ȡADC���������ݲ�����
	return adc_regular_data_read(ADC0);
}
 
/***********************************************************************************
* Function	    : Get_voltage
* Description	: ��ȡ��ѹ
* Input Para	: 
* Output Para	: 
* Return Value  : 
***********************************************************************************/
void Get_voltage(void)
{
	float vol_temp = 0;
	static int8_t Voltage_buff_count = 59;
	static float Voltage_buff_sum = 0.0f,Voltage_buff[60] = {0};
	// ��ȡ���ص�����
	uint16_t AdcVal = GetAdcVal();
	// �����ݽ���ת����������ת��Ϊ��ѹ��ֵ
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
 

uint8_t Get_BatValueSOC(float _pBatValue, uint8_t _pBatValueSOC)
{
	__IO uint16_t temp_d = 0x00; // ����һ��uint16_t����temp_d����ʼ��Ϊ0

	uint16_t deltaVoltage = 0;	// �����ѹ��ֵ����
	uint16_t voltageOffset = 0; // �����ѹƫ��������
	float deltaRatio = 0;		// �����ֵ����

	/* �����ѹ��Ͷ�Ӧ��SOC������ */
	uint16_t voltagePoints[] = {3000, 3120, 3240, 3360, 3480, 3600, 3720, 3840, 3960, 4080, 4200}; // ��ѹ��    (��λ: mV)
	uint8_t socPoints[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};							   // ��ӦSOC�� (%)
	uint8_t numPoints = sizeof(voltagePoints) / sizeof(voltagePoints[0]);						   // �����ѹ�������Ԫ�ظ���

	temp_d = (uint16_t)(_pBatValue * 1000L); /* ��ȡ��ص�ѹֵ��ת��ΪmV��λ */

	if (temp_d < voltagePoints[0])
	{
		return socPoints[0];
	}
	else if (temp_d > voltagePoints[numPoints - 1])
	{
		return socPoints[numPoints - 1];
	}
	else
	{
		;
	}

	for (uint8_t i = 0; i < numPoints - 1; i++)
	{
		if (temp_d >= voltagePoints[i] && temp_d <= voltagePoints[i + 1])
		{
			deltaVoltage = voltagePoints[i + 1] - voltagePoints[i]; /* ���㵱ǰ�����ڵĵ�ѹ�� */
			voltageOffset = temp_d - voltagePoints[i];				/* ���㵱ǰ��ѹֵ�����������ʼ��ѹ���ƫ���� */

			deltaRatio = (float)voltageOffset / deltaVoltage; /* �����ֵ���� */

			if (_pBatValueSOC == 0)
				return (deltaRatio > 0.75f) ? socPoints[i + 1] : socPoints[i];

			if (deltaRatio < 0.25f)
				return socPoints[i];
			else if (deltaRatio > 0.75f)
				return socPoints[i + 1];
			else
				return _pBatValueSOC;
		}
	}

	return 0;
}

uint8_t ADC_GetVolPer(void)
{
	float vol_temp = 0;
	uint8_t per = 0;
	uint16_t AdcVal = GetAdcVal();
	// �����ݽ���ת����������ת��Ϊ��ѹ��ֵ
	vol_temp = (float)AdcVal / 4095 * 3.3f;

	// per = (vol_temp - 1.4) / (2.1 - 1.4);
	// if (per > 100)
	// {
	// 	per = 100;
	// }
	per = Get_BatValueSOC(vol_temp, per);
	
	return per;
}


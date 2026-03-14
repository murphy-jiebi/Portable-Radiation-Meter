/**********************************************************************************************************
*
*	ģ������ : bsp �ײ�����ͷ�ļ�
*	�ļ����� : bsp.h
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����             ����              ˵��
*		V1.0    2022-04-08  
*
**********************************************************************************************************/
#ifndef _BSP_H_
#define _BSP_H_

#include "user.h"
#include "string.h"

/* ����ȫ���жϵĺ� */
#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */

// bsp gpio
void BSP_Key_Init(void);

void BSP_TIM_Init(void);

void BSP_RTC_Init(void);

void BSP_PWM_Init(void);
	
void BSP_GPIO_Init(void);

uint32_t TimerGetCnt(void);

//bsp uart



//bsp EEPROM
struct e2rom_data
{
	uint8_t  _E2rom_Flag;               /* eeprom初始化标志位 */
	uint8_t  _Device_Id;                /* 设备编号 */
	float    _Sensi_Value;		/* 灵敏度系数 */
	float    _Fact;    			/* 转换因子 */
	float    _BackGroung;    	/* 本底 */
	uint8_t  _Unit;    			/* 转换因子 */
};
#define E2ROM_START_ADDR    0x0010
#define E2ROM_DATA_SIZE     sizeof(struct e2rom_data)

#define e2rom_addr(mb)      (E2ROM_START_ADDR + (uint32)(&(((struct e2rom_data *)0)->mb)))
#define e2rom_len(mb)       ((uint8)(sizeof(((struct e2rom_data *)0)->mb)))

extern void BSP_EEPROM_Init(void);
extern uint8_t read_eeprom(uint8_t *_ucpBuf, uint16_t _usAddress, uint16_t _usSize);
extern uint8_t write_eeprom(uint8_t *_ucpBuf, uint16_t _usAddress, uint16_t _usSize);
extern uint8_t set_eeprom(uint8_t set_data, uint16_t _usAddress, uint16_t _usSize);


//bsp adc
extern void BSP_ADC_Init(void);
void Get_voltage(void);

uint8_t ADC_GetVolPer(void);

#endif /* _BSP_H_ */



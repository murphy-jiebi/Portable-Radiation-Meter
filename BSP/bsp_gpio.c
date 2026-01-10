/**********************************************************************************************************
 *
 *	ģ������ : GPIO�ܽſ���
 *	�ļ����� : bsp_gpio.c
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
#include "bsp_gpio.h"
/*********************************************************************************************************
 *                                            �궨��
 **********************************************************************************************************/
/************************** output  ***************/
#define PWR_EN_PORT GPIOC
#define PWR_EN_PIN GPIO_PIN_13

#define LED_RED_PORT GPIOC
#define LED_RED_PIN GPIO_PIN_2

#define LED_BLUE_PORT GPIOC
#define LED_BLUE_PIN GPIO_PIN_14

#define LED_GREEN_PORT GPIOC
#define LED_GREEN_PIN GPIO_PIN_3

#define BUZ_EN_PORT GPIOC
#define BUZ_EN_PIN GPIO_PIN_15

#define CHG_EN_PORT GPIOC
#define CHG_EN_PIN GPIO_PIN_9

/************************** input  ***************/
#define CHG_DET_PORT GPIOC
#define CHG_DET_PIN GPIO_PIN_0

#define CHG_STS1_PORT GPIOA
#define CHG_STS1_PIN GPIO_PIN_8

#define CHG_STS2_PORT GPIOA
#define CHG_STS2_PIN GPIO_PIN_11

#define NBR_DET_PORT GPIOA
#define NBR_DET_PIN GPIO_PIN_1

#define PWR_DET_PORT GPIOD
#define PWR_DET_PIN GPIO_PIN_2
/**********************************************/
#define PWR_EN_ON 		GPIO_BOP(PWR_EN_PORT) = (uint32_t)PWR_EN_PIN
#define PWR_EN_OFF 		GPIO_BC(PWR_EN_PORT) = (uint32_t)PWR_EN_PIN

#define LED_RED_OFF		GPIO_BOP(LED_RED_PORT) = (uint32_t)LED_RED_PIN
#define LED_RED_ON	 	GPIO_BC(LED_RED_PORT) = (uint32_t)LED_RED_PIN

#define LED_BLUE_OFF 	GPIO_BOP(LED_BLUE_PORT) = (uint32_t)LED_BLUE_PIN
#define LED_BLUE_ON 	GPIO_BC(LED_BLUE_PORT) = (uint32_t)LED_BLUE_PIN

#define LED_GREEN_OFF 	GPIO_BOP(LED_GREEN_PORT) = (uint32_t)LED_GREEN_PIN
#define LED_GREEN_ON 	GPIO_BC(LED_GREEN_PORT) = (uint32_t)LED_GREEN_PIN

#define BUZ_EN_ON 		GPIO_BOP(BUZ_EN_PORT) = (uint32_t)BUZ_EN_PIN
#define BUZ_EN_OFF 		GPIO_BC(BUZ_EN_PORT) = (uint32_t)BUZ_EN_PIN

#define CHG_EN_ON 		GPIO_BOP(CHG_EN_PORT) = (uint32_t)CHG_EN_PIN
#define CHG_EN_OFF 		GPIO_BC(CHG_EN_PORT) = (uint32_t)CHG_EN_PIN

#define CHG_DET_GET 	((uint32_t)RESET != (GPIO_ISTAT(CHG_DET_PORT) & (CHG_DET_PIN)))

#define CHG_STS1_GET 	((uint32_t)RESET != (GPIO_ISTAT(CHG_STS1_PORT) & (CHG_STS1_PIN)))

#define CHG_STS2_GET 	((uint32_t)RESET != (GPIO_ISTAT(CHG_STS2_PORT) & (CHG_STS2_PIN)))

#define NBR_DET_GET 	((uint32_t)RESET != (GPIO_ISTAT(NBR_DET_PORT) & (NBR_DET_PIN)))

#define PWR_DET_GET 	((uint32_t)RESET != (GPIO_ISTAT(PWR_DET_PORT) & (PWR_DET_PIN)))
/*********************************************************************************************************
 *	�� �� ��: BSP_GPIO_Init
 *	����˵��:
 *	��    ��: ��
 *	�� �� ֵ: ��
 *********************************************************************************************************/
void BSP_GPIO_Init(void)
{
	uint8_t i;
	rcu_periph_clock_enable(RCU_GPIOA); // 
	rcu_periph_clock_enable(RCU_GPIOB); // 
	rcu_periph_clock_enable(RCU_GPIOC); // 
	rcu_periph_clock_enable(RCU_GPIOD); // 

	/************************** output  ******************/
	gpio_init(PWR_EN_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, PWR_EN_PIN);
	gpio_init(LED_RED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_RED_PIN);
	gpio_init(LED_BLUE_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_BLUE_PIN);
	gpio_init(LED_GREEN_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_GREEN_PIN);
	gpio_init(BUZ_EN_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, BUZ_EN_PIN);
	gpio_init(CHG_EN_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, CHG_EN_PIN);

	/************************** input  ***************/
	gpio_init(CHG_DET_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, CHG_DET_PIN); /* ��ʼ��IO */
	gpio_init(CHG_STS1_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, CHG_STS1_PIN);
	gpio_init(CHG_STS2_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, CHG_STS2_PIN);
	gpio_init(NBR_DET_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, NBR_DET_PIN);

	if (PWR_DET_GET == 1)
	{
		delay_ms(100);
	}
	else
	{
		while ((PWR_DET_GET == 0) && (i < 150))
		{
			i++;
			delay_ms(10);
		}
	}
	PWR_EN_ON;
	
	LED_RED_OFF;
	LED_BLUE_OFF;
	LED_GREEN_OFF;
}

void PWR_shutdown(void)
{
	PWR_EN_OFF;
}

void BEEP_SetState(uint8_t state)
{
	if(state > 0)
	{
		BUZ_EN_ON;
	}else{
		BUZ_EN_OFF;
	}
}

void LED_SetState(uint8_t state)
{
	if(state & 0x01)
	{
		LED_GREEN_ON;
	}else{
		LED_GREEN_OFF;
	}
	
	if(state & 0x02)
	{
		LED_BLUE_ON;
	}else{
		LED_BLUE_OFF;
	}
	
	if(state & 0x04)
	{
		LED_RED_ON;
	}else{
		LED_RED_OFF;
	}
}

uint8_t CHG_plugCheck(void)
{
	return CHG_DET_GET;
}

uint8_t CHG_GetChgState(void)
{
	if(!CHG_DET_GET)
	{
		return 0;  //no plug in cable or no power
	}
	
	if((CHG_STS1_GET > 0) && (CHG_STS2_GET == 0))
	{
		return 1; //charging
	}
	
	if((CHG_STS1_GET == 0) && (CHG_STS2_GET > 0))
	{
		return 2; //full
	}
	
	return 0;
}

void CHG_SetChg(uint8_t state)
{
	if(state > 0)
	{
		CHG_EN_ON;
	}else{
		CHG_EN_OFF;
	}
}
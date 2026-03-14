#include "bsp_key.h"
#include "gd32f10x.h"

#define KEY_NUM			4
#define KEY_ENTER_PORT	GPIOD
#define KEY_ENTER_PIN	GPIO_PIN_2

#define KEY_BACK_PORT	GPIOB
#define KEY_BACK_PIN	GPIO_PIN_0

#define KEY_UP_PORT		GPIOB
#define KEY_UP_PIN		GPIO_PIN_8

#define KEY_DOWN_PORT	GPIOB
#define KEY_DOWN_PIN	GPIO_PIN_1

#define KEY_FIFO_SIZE	10		/* FIFO缓冲区大小 */

#define KEY_ENTER_GET 		((uint32_t)RESET != (GPIO_ISTAT(KEY_ENTER_PORT) & (KEY_ENTER_PIN)))
#define KEY_BACK_GET 		((uint32_t)RESET != (GPIO_ISTAT(KEY_BACK_PORT) & (KEY_BACK_PIN)))
#define KEY_UP_GET 			((uint32_t)RESET != (GPIO_ISTAT(KEY_UP_PORT) & (KEY_UP_PIN)))
#define KEY_DOWN_GET 		((uint32_t)RESET != (GPIO_ISTAT(KEY_DOWN_PORT) & (KEY_DOWN_PIN)))

#define KEY_FILTER_TIME   2			/* 滤波时间 */
#define KEY_LONG_TIME     10			/* 单位10ms，持续2S，认为是长按事件 */

static uint8_t keyState = 0;

uint16_t keyPressTimeCnt[KEY_NUM] = {0};

typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* 键值缓冲区 */
	uint8_t Read;					/* 缓冲区读指针1 */
	uint8_t Write;					/* 缓冲区写指针 */
}KEY_FIFO_T;

KEY_FIFO_T keyValList;

typedef struct
{
	uint32_t gpio;
	uint32_t pin;
}GPIO_T;

GPIO_T keyList[KEY_NUM]=
{
	{KEY_ENTER_PORT,KEY_ENTER_PIN	},
	{KEY_BACK_PORT,	KEY_BACK_PIN	},
	{KEY_UP_PORT,	KEY_UP_PIN		},
	{KEY_DOWN_PORT,	KEY_DOWN_PIN	},
};

void BSP_Key_Init(void)
{
	rcu_periph_clock_enable(RCU_GPIOB); 
	rcu_periph_clock_enable(RCU_GPIOD); 
	
	gpio_init(keyList[0].gpio, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, keyList[0].pin);
	gpio_init(keyList[1].gpio, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, keyList[1].pin);
	gpio_init(keyList[2].gpio, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, keyList[2].pin);
	gpio_init(keyList[3].gpio, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, keyList[3].pin);
}


void bsp_PutKey(uint8_t _KeyCode)
{
	keyValList.Buf[keyValList.Write] = _KeyCode;
	
	if (++keyValList.Write >= KEY_FIFO_SIZE)
	{
		keyValList.Write = 0;
	}
}


/*
*********************************************************************************************************
*	函 数 名: bsp_GetKey
*	功能说明:  从按键FIFO缓冲区读取一个键值
*	形    参: _KeyCode：按键代码
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t bsp_GetKey(void)
{
	uint8_t result;
	
	if(keyValList.Read == keyValList.Write)
	{
		return KEY_NONE;
	}
	else
	{
		result = keyValList.Buf[keyValList.Read];
		
		if(++keyValList.Read >= KEY_FIFO_SIZE)
		{
			keyValList.Read = 0;
		}
		return result;
	}
}


/*
*********************************************************************************************************
*	函 数 名: bsp_ClearKey
*	功能说明:  清空FIFO缓冲区
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_ClearKey(void)
{
	keyValList.Read = keyValList.Write;
}

uint8_t KeyGetState(void)
{
	return keyState;
}
void bsp_KeyScan(void)
{
	uint8_t i = 0;
	
	for(i=0;i<KEY_NUM;i++)
	{
		if(gpio_input_bit_get(keyList[i].gpio, keyList[i].pin) == RESET )
		{
			keyPressTimeCnt[i]++;
			if(keyPressTimeCnt[i] >= KEY_LONG_TIME)
			{
				bsp_PutKey(i+5);
				keyPressTimeCnt[i] = 0;
			}
			keyState |= (1 << i);
		}else{
			if(keyPressTimeCnt[i] < KEY_FILTER_TIME)
			{
				
			}
			else if(keyPressTimeCnt[i] < KEY_LONG_TIME)
			{
				bsp_PutKey(i+1);
			}else{
				bsp_PutKey(i+5);
			}
			keyPressTimeCnt[i] = 0;
			keyState &= (~(1 << i));
		}
	}
}



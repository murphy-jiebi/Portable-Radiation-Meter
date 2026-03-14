/*********************************************************************************************************
*
*	Ä£ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ : IWDG ï¿½ï¿½Ê¼ï¿½ï¿½
*	ï¿½Ä¼ï¿½ï¿½ï¿½ï¿½ï¿½ : bsp_iwdg.c
*	ï¿½ï¿½    ï¿½ï¿½ : V1.0
*	Ëµ    ï¿½ï¿½ : IWDG ï¿½ï¿½Ê¼ï¿½ï¿½
*
*	ï¿½Þ¸Ä¼ï¿½Â¼ :
*		ï¿½æ±¾ï¿½ï¿½  ï¿½ï¿½ï¿½ï¿½             ï¿½ï¿½ï¿½ï¿½              Ëµï¿½ï¿½
*		V1.0    2022-04-08      
*
**********************************************************************************************************/
#include <string.h>
#include "gd32f10x_it.h"
#include "bsp_uart.h"
#include "user.h"

/*********************************************************************************************************
*                                            ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
**********************************************************************************************************/
#define UART0_BAUD	115200                              //ï¿½ï¿½UWB
#define UART0_TX_BUF_SIZE	1 * 512
#define UART0_RX_BUF_SIZE	1 * 512

#define UART1_BAUD	9600                              //ï¿½ï¿½LoRa
#define UART1_TX_BUF_SIZE	1 * 512
#define UART1_RX_BUF_SIZE	1 * 512

#define UART2_BAUD	9600                              //ï¿½ï¿½GPS
#define UART2_TX_BUF_SIZE	1 * 512
#define UART2_RX_BUF_SIZE	1 * 512

#define UART3_BAUD	9600                              //Ô¤ï¿½ï¿½
#define UART3_TX_BUF_SIZE	1 * 512
#define UART3_RX_BUF_SIZE	1 * 512

#define RS485_EN_PORT 		GPIOC
#define RS485_EN_PIN		GPIO_PIN_12

#define RS485_TX_ON			GPIO_BOP(RS485_EN_PORT) = (uint32_t)RS485_EN_PIN
#define RS485_RX_ON			GPIO_BC(RS485_EN_PORT) = (uint32_t)RS485_EN_PIN

UART_T g_tUart0;
uint8_t g_TxBuf0[UART0_TX_BUF_SIZE];
uint8_t g_RxBuf0[UART0_RX_BUF_SIZE];

#if 0
UART_T g_tUart1;
uint8_t g_TxBuf1[UART1_TX_BUF_SIZE];
uint8_t g_RxBuf1[UART1_RX_BUF_SIZE];

UART_T g_tUart2;
uint8_t g_TxBuf2[UART2_TX_BUF_SIZE];
uint8_t g_RxBuf2[UART2_RX_BUF_SIZE];
#endif

UART_T g_tUart3;
uint8_t g_TxBuf3[UART3_TX_BUF_SIZE];
uint8_t g_RxBuf3[UART3_RX_BUF_SIZE];




/**********************************************************************************************************
*	ï¿½ï¿½ ï¿½ï¿½ ï¿½ï¿½: uart_var_init
*	ï¿½ï¿½ï¿½ï¿½Ëµï¿½ï¿½: ï¿½ï¿½ï¿½Ú²ï¿½ï¿½ï¿½ï¿½ï¿½Ê¼ï¿½ï¿½
*	ï¿½ï¿½    ï¿½ï¿½: ï¿½ï¿½
*	ï¿½ï¿½ ï¿½ï¿½ Öµ: ï¿½ï¿½
**********************************************************************************************************/
static void uart_var_init(void)
{
	g_tUart0.uart = USART0;
	g_tUart0.pTxBuf = g_TxBuf0;
	g_tUart0.pRxBuf = g_RxBuf0;
	g_tUart0.usTxBufSize = UART0_TX_BUF_SIZE;
	g_tUart0.usRxBufSize = UART0_RX_BUF_SIZE;
	g_tUart0.usTxCount = 0;
	g_tUart0.usRxCount = 0;
	g_tUart0.ucTxFrame = 0;
	g_tUart0.ucRxFrame = 0;
   
	#if 0
	g_tUart1.uart = USART1;
	g_tUart1.pTxBuf = g_TxBuf1;
	g_tUart1.pRxBuf = g_RxBuf1;
	g_tUart1.usTxBufSize = UART1_TX_BUF_SIZE;
	g_tUart1.usRxBufSize = UART1_RX_BUF_SIZE;
	g_tUart1.usTxCount = 0;
	g_tUart1.usRxCount = 0;
	g_tUart1.ucTxFrame = 0;
	g_tUart1.ucRxFrame = 0;
    
    g_tUart2.uart = USART2;
	g_tUart2.pTxBuf = g_TxBuf2;
	g_tUart2.pRxBuf = g_RxBuf2;
	g_tUart2.usTxBufSize = UART2_TX_BUF_SIZE;
	g_tUart2.usRxBufSize = UART2_RX_BUF_SIZE;
	g_tUart2.usTxCount = 0;
	g_tUart2.usRxCount = 0;
	g_tUart2.ucTxFrame = 0;
	g_tUart2.ucRxFrame = 0;
	
	#endif

    g_tUart3.uart = USART2;
	g_tUart3.pTxBuf = g_TxBuf3;
	g_tUart3.pRxBuf = g_RxBuf3;
	g_tUart3.usTxBufSize = UART3_TX_BUF_SIZE;
	g_tUart3.usRxBufSize = UART3_RX_BUF_SIZE;
	g_tUart3.usTxCount = 0;
	g_tUart3.usRxCount = 0;
	g_tUart3.ucTxFrame = 0;
	g_tUart3.ucRxFrame = 0;
	
}

/***********************************************************************************
* Function	    : UartSend
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value  : 
***********************************************************************************/
static uint8_t UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen)
{
	uint16_t i;
    
    usart_interrupt_disable(_pUart->uart, USART_INT_TBE);	
	
	if((_usLen + _pUart->usTxCount) > _pUart->usTxBufSize)  //ï¿½ï¿½ï¿½Ú·ï¿½ï¿½ï¿½buf
	{
	    return _FALSE;                       
	}

	for (i = 0; i < _usLen; i++)      //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ë·¢ï¿½Í»ï¿½ï¿½ï¿½ï¿½ï¿½ 
	{	
		_pUart->pTxBuf[_pUart->usTxCount++] = _ucaBuf[i];
	}
	
	usart_interrupt_enable(_pUart->uart, USART_INT_TBE);	
	return _TRUE;
}

uint8_t add_parity_bit(uint8_t data, bool parity_type)
{
	// Ö»È¡µÍ7Î»£¬È·±£ÊäÈëÕýÈ·
	uint8_t seven_bit_data = data & 0x7F;

	// ¼ÆËãÊý¾ÝÖÐ1µÄ¸öÊý
	uint8_t count = 0;
	uint8_t temp = seven_bit_data;

	// Ê¹ÓÃBrian KernighanËã·¨¼ÆËã1µÄ¸öÊý
	while (temp)
	{
		temp &= (temp - 1);
		count++;
	}

	// ¸ù¾ÝÐ£ÑéÀàÐÍ¾ö¶¨Ð£ÑéÎ»
	uint8_t parity_bit;
	if (parity_type)
	{
		// ÆæÐ£Ñé£º1µÄ¸öÊý£¨°üÀ¨Ð£ÑéÎ»£©ÎªÆæÊý
		parity_bit = (count % 2 == 0) ? 0x80 : 0x00;
	}
	else
	{
		// Å¼Ð£Ñé£º1µÄ¸öÊý£¨°üÀ¨Ð£ÑéÎ»£©ÎªÅ¼Êý
		parity_bit = (count % 2 == 0) ? 0x00 : 0x80;
	}

	// ½«Ð£ÑéÎ»ÉèÖÃÔÚbit7£¬·µ»Ø8Î»Êý¾Ý
	return seven_bit_data | parity_bit;
}

/**********************************************************************************************************
*	ï¿½ï¿½ ï¿½ï¿½ ï¿½ï¿½: uart_send_buf
*	ï¿½ï¿½ï¿½ï¿½Ëµï¿½ï¿½: 
*	ï¿½ï¿½    ï¿½ï¿½: 
*	ï¿½ï¿½ ï¿½ï¿½ Öµ: ï¿½ï¿½
**********************************************************************************************************/
void uart_send_buf(UART_T *_pUart, uint8_t *_ucpBuf, uint16_t _usBufLen)
{
	uint16_t i = 0;

	if (_pUart->uart == USART2)
	{
		RS485_TX_ON;
	}
	for(i = 0; i < _usBufLen; i++)
	{
		if (_pUart->uart == USART2)
		{
			usart_data_transmit(_pUart->uart, add_parity_bit(_ucpBuf[i], 0));
		}else{
			usart_data_transmit(_pUart->uart, _ucpBuf[i]);
		}
//		usart_data_transmit(_pUart->uart, _ucpBuf[i]);
		while(RESET == usart_flag_get(_pUart->uart, USART_FLAG_TBE));
	}
	_pUart->ucTxFrame = 0x01;
	delay_ms(2);
	if (_pUart->uart == USART2)
	{
		RS485_RX_ON;
	}
}

/***********************************************************************************
* Function	    : UartIRQ
* Description	: ï¿½ï¿½ï¿½Ð¶Ï·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ã£ï¿½Í¨ï¿½Ã´ï¿½ï¿½ï¿½ï¿½Ð¶Ï´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
* Input Para	: 
* Output Para	: 
* Return Value  : 
***********************************************************************************/
void UART_IRQn(UART_T *_pUart)
{
	/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ð¶ï¿½  */
	if (usart_interrupt_flag_get(_pUart->uart, USART_INT_FLAG_RBNE) != RESET)
	{
		/* ï¿½Ó´ï¿½ï¿½Ú½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ý¼Ä´ï¿½ï¿½ï¿½ï¿½ï¿½È¡ï¿½ï¿½ï¿½Ý´ï¿½Åµï¿½ï¿½ï¿½ï¿½ï¿½FIFO */
		uint8_t ch;

		ch = usart_data_receive(_pUart->uart);

		if (_pUart->usRxCount < _pUart->usRxBufSize)
		{
			_pUart->pRxBuf[_pUart->usRxCount] = ch;
			_pUart->usRxCount++;
		}
        usart_interrupt_flag_clear(_pUart->uart,USART_INT_FLAG_RBNE);
        usart_interrupt_enable(_pUart->uart,USART_INT_RBNE);
		usart_interrupt_enable(_pUart->uart,USART_INT_IDLE);	/* Ê¹ï¿½Ü½ï¿½ï¿½Õ¿ï¿½ï¿½ï¿½ï¿½Ð¶ï¿½ */
	}

	/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Í»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ð¶ï¿½ */
	if (usart_interrupt_flag_get(_pUart->uart, USART_INT_FLAG_TBE) != RESET)
	{
		if (_pUart->usTxIndex >= _pUart->usTxCount)
		{
			/* ï¿½ï¿½ï¿½Í»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È¡ï¿½ï¿½Ê±ï¿½ï¿½ ï¿½ï¿½Ö¹ï¿½ï¿½ï¿½Í»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ð¶ï¿½ ï¿½ï¿½×¢ï¿½â£ºï¿½ï¿½Ê±ï¿½ï¿½ï¿?1ï¿½ï¿½ï¿½ï¿½ï¿½Ý»ï¿½Î´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ï£ï¿?*/
			usart_interrupt_disable(_pUart->uart,USART_INT_TBE);
			/* Ê¹ï¿½ï¿½ï¿½ï¿½ï¿½Ý·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ð¶ï¿? */
			usart_interrupt_enable(_pUart->uart,USART_INT_TC);;
		}
		else
		{
			/* ï¿½Ó·ï¿½ï¿½ï¿½FIFOÈ¡1ï¿½ï¿½ï¿½Ö½ï¿½Ð´ï¿½ë´®ï¿½Ú·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ý¼Ä´ï¿½ï¿½ï¿½ */
			usart_data_transmit(_pUart->uart, _pUart->pTxBuf[_pUart->usTxIndex]);
			_pUart->usTxIndex++;
		}
	}
	/* ï¿½ï¿½ï¿½ï¿½bitÎ»È«ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ïµï¿½ï¿½Ð¶ï¿? */
	else if (usart_interrupt_flag_get(_pUart->uart, USART_INT_FLAG_TC) != RESET)
	{
		_pUart->usTxIndex = 0x00;
		_pUart->usTxCount = 0x00;
        _pUart->ucTxFrame = 0x00;
		usart_interrupt_flag_clear(_pUart->uart, USART_INT_FLAG_TC);
        usart_interrupt_disable(_pUart->uart, USART_INT_TC);
	}
	/* ï¿½ï¿½ï¿½Ú¿ï¿½ï¿½ï¿½ï¿½Ð¶ï¿½ */
	else if (usart_interrupt_flag_get(_pUart->uart,USART_INT_FLAG_IDLE) != RESET)
	{
		_pUart->ucRxFrame  = 0x01;
		usart_interrupt_flag_clear(_pUart->uart,USART_INT_FLAG_IDLE);
        usart_interrupt_disable(_pUart->uart,USART_INT_IDLE);	
	}
}

/*********************************************************************************************************
*	ï¿½ï¿½ ï¿½ï¿½ ï¿½ï¿½: BSP_Uart_Init
*	ï¿½ï¿½ï¿½ï¿½Ëµï¿½ï¿½: 
*	ï¿½ï¿½    ï¿½ï¿½: 
*	ï¿½ï¿½ ï¿½ï¿½ Öµ: ï¿½ï¿½
**********************************************************************************************************/
void BSP_Uart_Init(void)
{
    /* enable GPIO clock */
	rcu_periph_clock_enable(RCU_USART0);

	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_USART1);   /* Ê¹ï¿½ï¿½Ê±ï¿½ï¿½ */	
    
    rcu_periph_clock_enable(RCU_GPIOC);
	rcu_periph_clock_enable(RCU_UART3);   /* Ê¹ï¿½ï¿½Ê±ï¿½ï¿½ */
	
	uart_var_init();
    
    //uart0
	rcu_periph_clock_enable(RCU_USART0);   /* Ê¹ï¿½ï¿½Ê±ï¿½ï¿½ */	
	/* connect port to USARTx_Tx */
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	/* connect port to USARTx_Rx */
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);	
    
    usart_deinit(USART0);  /* ï¿½ï¿½Î»ï¿½ï¿½ï¿½ï¿½ */
	
	usart_baudrate_set(USART0, UART0_BAUD);   /* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */
	
	usart_parity_config(USART0, USART_PM_NONE);   /* ï¿½ï¿½Å¼ï¿½ï¿½ï¿½ï¿½Î» */
	
	usart_word_length_set(USART0, USART_WL_8BIT); /* ï¿½Ö³ï¿½ */
	
	usart_stop_bit_set(USART0, USART_STB_1BIT);  /* Í£Ö¹Î» */
	
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
	
	usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);  /* ï¿½ï¿½ï¿½ï¿½Ê¹ï¿½ï¿½ */
	
	usart_receive_config(USART0, USART_RECEIVE_ENABLE);    /* Ê¹ï¿½Ü½ï¿½ï¿½ï¿½ */

	usart_enable(USART0); /* ï¿½ï¿½ï¿½ï¿½Ê¹ï¿½ï¿½ */
	
	usart_interrupt_enable(USART0,USART_INT_RBNE); 

	nvic_irq_enable(USART0_IRQn, 0, 0);

	#if 0
   //uart1
	/* connect port to USARTx_Tx */
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
	/* connect port to USARTx_Rx */
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_3);	
	
	usart_deinit(USART1);  /* ï¿½ï¿½Î»ï¿½ï¿½ï¿½ï¿½ */
	
	usart_baudrate_set(USART1, UART1_BAUD);   /* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */
	
	usart_parity_config(USART1, USART_PM_NONE);   /* ï¿½ï¿½Å¼ï¿½ï¿½ï¿½ï¿½Î» */
	
	usart_word_length_set(USART1, USART_WL_8BIT); /* ï¿½Ö³ï¿½ */
	
	usart_stop_bit_set(USART1, USART_STB_1BIT);  /* Í£Ö¹Î» */
	
   usart_hardware_flow_rts_config(USART1, USART_RTS_DISABLE);
   usart_hardware_flow_cts_config(USART1, USART_CTS_DISABLE);
	
	usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);  /* ï¿½ï¿½ï¿½ï¿½Ê¹ï¿½ï¿½ */
	
	usart_receive_config(USART1, USART_RECEIVE_ENABLE);    /* Ê¹ï¿½Ü½ï¿½ï¿½ï¿½ */

	usart_enable(USART1); /* ï¿½ï¿½ï¿½ï¿½Ê¹ï¿½ï¿½ */
	
	usart_interrupt_enable(USART1,USART_INT_RBNE);  /* Ê¹ï¿½Ü¿ï¿½ï¿½ï¿½ï¿½Ð¶ï¿½ */

	nvic_irq_enable(USART1_IRQn, 0, 1);
#endif
	// uart2
	/* connect port to USARTx_Tx */
	gpio_init(RS485_EN_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, RS485_EN_PIN);
	/* connect port to USARTx_Tx */
	gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
	/* connect port to USARTx_Rx */
	gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

	gpio_pin_remap_config(GPIO_USART2_PARTIAL_REMAP, ENABLE);

	rcu_periph_clock_enable(RCU_USART2);

	usart_deinit(USART2); /* ï¿½ï¿½Î»ï¿½ï¿½ï¿½ï¿½ */

	usart_baudrate_set(USART2, UART2_BAUD);   /* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */
	
	usart_parity_config(USART2, USART_PM_NONE);   /* ï¿½ï¿½Å¼ï¿½ï¿½ï¿½ï¿½Î» */
	
	usart_word_length_set(USART2, USART_WL_8BIT); /* ï¿½Ö³ï¿½ */

	usart_stop_bit_set(USART2, USART_STB_2BIT); /* Í£Ö¹Î» */

	usart_hardware_flow_rts_config(USART2, USART_RTS_DISABLE);
	usart_hardware_flow_cts_config(USART2, USART_CTS_DISABLE);
	
	usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);  /* ï¿½ï¿½ï¿½ï¿½Ê¹ï¿½ï¿½ */
	
	usart_receive_config(USART2, USART_RECEIVE_ENABLE);    /* Ê¹ï¿½Ü½ï¿½ï¿½ï¿½ */

	usart_enable(USART2); /* ï¿½ï¿½ï¿½ï¿½Ê¹ï¿½ï¿½ */
	
	usart_interrupt_enable(USART2,USART_INT_RBNE);  /* Ê¹ï¿½Ü¿ï¿½ï¿½ï¿½ï¿½Ð¶ï¿½ */

	nvic_irq_enable(USART2_IRQn, 0, 2);

	RS485_RX_ON;
	//uart3
	
#if 0	
	
	
	
	usart_deinit(UART3);  /* ï¿½ï¿½Î»ï¿½ï¿½ï¿½ï¿½ */
	
	usart_baudrate_set(UART3, UART3_BAUD);   /* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */

	usart_parity_config(UART3, USART_PM_NONE); /* ï¿½ï¿½Å¼ï¿½ï¿½ï¿½ï¿½Î» */

	usart_word_length_set(UART3, USART_WL_8BIT); /* ï¿½Ö³ï¿½ */

	usart_stop_bit_set(UART3, USART_STB_2BIT); /* Í£Ö¹Î» */

	usart_hardware_flow_rts_config(UART3, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(UART3, USART_CTS_DISABLE);
	
	usart_transmit_config(UART3, USART_TRANSMIT_ENABLE);  
	
	usart_receive_config(UART3, USART_RECEIVE_ENABLE);    

	usart_enable(UART3); 
	
	usart_interrupt_enable(UART3,USART_INT_RBNE);  

	nvic_irq_enable(UART3_IRQn, 0, 3);
 
    RS485_RX_ON;
#endif
}

void Uart3SetBaud(uint32_t baud)
{
	usart_deinit(UART3);  /* ï¿½ï¿½Î»ï¿½ï¿½ï¿½ï¿½ */
	
	usart_baudrate_set(UART3, baud);   /* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */

	usart_parity_config(UART3, USART_PM_EVEN); /* ï¿½ï¿½Å¼ï¿½ï¿½ï¿½ï¿½Î» */

	usart_word_length_set(UART3, USART_WL_8BIT); /* ï¿½Ö³ï¿½ */

	usart_stop_bit_set(UART3, USART_STB_2BIT); /* Í£Ö¹Î» */

	usart_hardware_flow_rts_config(UART3, USART_RTS_DISABLE);
	usart_hardware_flow_cts_config(UART3, USART_CTS_DISABLE);
	
	usart_transmit_config(UART3, USART_TRANSMIT_ENABLE);  
	
	usart_receive_config(UART3, USART_RECEIVE_ENABLE);    

	usart_enable(UART3); 
}




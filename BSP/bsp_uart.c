/*********************************************************************************************************
*
*	ģ������ : IWDG ��ʼ��
*	�ļ����� : bsp_iwdg.c
*	��    �� : V1.0
*	˵    �� : IWDG ��ʼ��
*
*	�޸ļ�¼ :
*		�汾��  ����             ����              ˵��
*		V1.0    2022-04-08      
*
**********************************************************************************************************/
#include <string.h>
#include "gd32f10x_it.h"
#include "bsp_uart.h"
#include "user.h"

/*********************************************************************************************************
*                                            ��������
**********************************************************************************************************/
#define UART0_BAUD	115200                              //��UWB
#define UART0_TX_BUF_SIZE	1 * 512
#define UART0_RX_BUF_SIZE	1 * 512

#define UART1_BAUD	9600                              //��LoRa
#define UART1_TX_BUF_SIZE	1 * 512
#define UART1_RX_BUF_SIZE	1 * 512

#define UART2_BAUD	9600                              //��GPS
#define UART2_TX_BUF_SIZE	1 * 512
#define UART2_RX_BUF_SIZE	1 * 512

#define UART3_BAUD	9600                              //Ԥ��
#define UART3_TX_BUF_SIZE	1 * 512
#define UART3_RX_BUF_SIZE	1 * 512

#define RS485_EN_PORT 		GPIOC
#define RS485_EN_PIN		GPIO_PIN_12

#define RS485_TX_ON			GPIO_BOP(RS485_EN_PORT) = (uint32_t)RS485_EN_PIN
#define RS485_RX_ON			GPIO_BC(RS485_EN_PORT) = (uint32_t)RS485_EN_PIN

UART_T g_tUart0;
uint8_t g_TxBuf0[UART0_TX_BUF_SIZE];
uint8_t g_RxBuf0[UART0_RX_BUF_SIZE];

UART_T g_tUart1;
uint8_t g_TxBuf1[UART1_TX_BUF_SIZE];
uint8_t g_RxBuf1[UART1_RX_BUF_SIZE];

UART_T g_tUart2;
uint8_t g_TxBuf2[UART2_TX_BUF_SIZE];
uint8_t g_RxBuf2[UART2_RX_BUF_SIZE];

UART_T g_tUart3;
uint8_t g_TxBuf3[UART3_TX_BUF_SIZE];
uint8_t g_RxBuf3[UART3_RX_BUF_SIZE];




/**********************************************************************************************************
*	�� �� ��: uart_var_init
*	����˵��: ���ڲ�����ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
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
	
    g_tUart3.uart = UART3;
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
	
	if((_usLen + _pUart->usTxCount) > _pUart->usTxBufSize)  //���ڷ���buf
	{
	    return _FALSE;                       
	}

	for (i = 0; i < _usLen; i++)      //�����������뷢�ͻ����� 
	{	
		_pUart->pTxBuf[_pUart->usTxCount++] = _ucaBuf[i];
	}
	
	usart_interrupt_enable(_pUart->uart, USART_INT_TBE);	
	return _TRUE;
}

/**********************************************************************************************************
*	�� �� ��: uart_send_buf
*	����˵��: 
*	��    ��: 
*	�� �� ֵ: ��
**********************************************************************************************************/
void uart_send_buf(UART_T *_pUart, uint8_t *_ucpBuf, uint16_t _usBufLen)
{
	uint16_t i = 0;
	
	if(_pUart->uart == UART3)
	{
		RS485_TX_ON;
	}
	for(i = 0; i < _usBufLen; i++)
	{
		usart_data_transmit(_pUart->uart,_ucpBuf[i]);
	}
	_pUart->ucTxFrame = 0x01;
	
	if(_pUart->uart == UART3)
	{
		RS485_RX_ON;
	}
}

/***********************************************************************************
* Function	    : UartIRQ
* Description	: ���жϷ��������ã�ͨ�ô����жϴ�������
* Input Para	: 
* Output Para	: 
* Return Value  : 
***********************************************************************************/
void UART_IRQn(UART_T *_pUart)
{
	/* ���������ж�  */
	if (usart_interrupt_flag_get(_pUart->uart, USART_INT_FLAG_RBNE) != RESET)
	{
		/* �Ӵ��ڽ������ݼĴ�����ȡ���ݴ�ŵ�����FIFO */
		uint8_t ch;

		ch = usart_data_receive(_pUart->uart);

		if (_pUart->usRxCount < _pUart->usRxBufSize)
		{
			_pUart->pRxBuf[_pUart->usRxCount] = ch;
			_pUart->usRxCount++;
		}
        usart_interrupt_flag_clear(_pUart->uart,USART_INT_FLAG_RBNE);
        usart_interrupt_enable(_pUart->uart,USART_INT_RBNE);
		usart_interrupt_enable(_pUart->uart,USART_INT_IDLE);	/* ʹ�ܽ��տ����ж� */
	}

	/* �������ͻ��������ж� */
	if (usart_interrupt_flag_get(_pUart->uart, USART_INT_FLAG_TBE) != RESET)
	{
		if (_pUart->usTxIndex >= _pUart->usTxCount)
		{
			/* ���ͻ�������������ȡ��ʱ�� ��ֹ���ͻ��������ж� ��ע�⣺��ʱ���1�����ݻ�δ����������ϣ�*/
			usart_interrupt_disable(_pUart->uart,USART_INT_TBE);
			/* ʹ�����ݷ�������ж� */
			usart_interrupt_enable(_pUart->uart,USART_INT_TC);;
		}
		else
		{
			/* �ӷ���FIFOȡ1���ֽ�д�봮�ڷ������ݼĴ��� */
			usart_data_transmit(_pUart->uart, _pUart->pTxBuf[_pUart->usTxIndex]);
			_pUart->usTxIndex++;
		}
	}
	/* ����bitλȫ��������ϵ��ж� */
	else if (usart_interrupt_flag_get(_pUart->uart, USART_INT_FLAG_TC) != RESET)
	{
		_pUart->usTxIndex = 0x00;
		_pUart->usTxCount = 0x00;
        _pUart->ucTxFrame = 0x00;
		usart_interrupt_flag_clear(_pUart->uart, USART_INT_FLAG_TC);
        usart_interrupt_disable(_pUart->uart, USART_INT_TC);
	}
	/* ���ڿ����ж� */
	else if (usart_interrupt_flag_get(_pUart->uart,USART_INT_FLAG_IDLE) != RESET)
	{
		_pUart->ucRxFrame  = 0x01;
		usart_interrupt_flag_clear(_pUart->uart,USART_INT_FLAG_IDLE);
        usart_interrupt_disable(_pUart->uart,USART_INT_IDLE);	
	}
}

/*********************************************************************************************************
*	�� �� ��: BSP_Uart_Init
*	����˵��: 
*	��    ��: 
*	�� �� ֵ: ��
**********************************************************************************************************/
void BSP_Uart_Init(void)
{
    /* enable GPIO clock */

    
    rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_USART1);   /* ʹ��ʱ�� */	
    
    rcu_periph_clock_enable(RCU_GPIOC);
	rcu_periph_clock_enable(RCU_UART3);   /* ʹ��ʱ�� */
		
	uart_var_init();
    
    //uart0
	rcu_periph_clock_enable(RCU_USART0);   /* ʹ��ʱ�� */	
	/* connect port to USARTx_Tx */
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	/* connect port to USARTx_Rx */
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);	
    
    usart_deinit(USART0);  /* ��λ���� */
	
	usart_baudrate_set(USART0, UART0_BAUD);   /* ������ */
	
	usart_parity_config(USART0, USART_PM_NONE);   /* ��ż����λ */
	
	usart_word_length_set(USART0, USART_WL_8BIT); /* �ֳ� */
	
	usart_stop_bit_set(USART0, USART_STB_1BIT);  /* ֹͣλ */
	
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
	
	usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);  /* ����ʹ�� */
	
	usart_receive_config(USART0, USART_RECEIVE_ENABLE);    /* ʹ�ܽ��� */

	usart_enable(USART0); /* ����ʹ�� */
	
	usart_interrupt_enable(USART0,USART_INT_RBNE); 

	nvic_irq_enable(USART0_IRQn, 0, 0);

	#if 0
   //uart1
	/* connect port to USARTx_Tx */
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
	/* connect port to USARTx_Rx */
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_3);	
	
	usart_deinit(USART1);  /* ��λ���� */
	
	usart_baudrate_set(USART1, UART1_BAUD);   /* ������ */
	
	usart_parity_config(USART1, USART_PM_NONE);   /* ��ż����λ */
	
	usart_word_length_set(USART1, USART_WL_8BIT); /* �ֳ� */
	
	usart_stop_bit_set(USART1, USART_STB_1BIT);  /* ֹͣλ */
	
   usart_hardware_flow_rts_config(USART1, USART_RTS_DISABLE);
   usart_hardware_flow_cts_config(USART1, USART_CTS_DISABLE);
	
	usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);  /* ����ʹ�� */
	
	usart_receive_config(USART1, USART_RECEIVE_ENABLE);    /* ʹ�ܽ��� */

	usart_enable(USART1); /* ����ʹ�� */
	
	usart_interrupt_enable(USART1,USART_INT_RBNE);  /* ʹ�ܿ����ж� */

	nvic_irq_enable(USART1_IRQn, 0, 1);

   //uart2
	/* connect port to USARTx_Tx */
	gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
	/* connect port to USARTx_Rx */
	gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);	
	
	usart_deinit(USART2);  /* ��λ���� */
	
	usart_baudrate_set(USART2, UART2_BAUD);   /* ������ */
	
	usart_parity_config(USART2, USART_PM_NONE);   /* ��ż����λ */
	
	usart_word_length_set(USART2, USART_WL_8BIT); /* �ֳ� */
	
	usart_stop_bit_set(USART2, USART_STB_1BIT);  /* ֹͣλ */
	
   usart_hardware_flow_rts_config(USART2, USART_RTS_DISABLE);
   usart_hardware_flow_cts_config(USART2, USART_CTS_DISABLE);
	
	usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);  /* ����ʹ�� */
	
	usart_receive_config(USART2, USART_RECEIVE_ENABLE);    /* ʹ�ܽ��� */

	usart_enable(USART2); /* ����ʹ�� */
	
	usart_interrupt_enable(USART2,USART_INT_RBNE);  /* ʹ�ܿ����ж� */

	nvic_irq_enable(USART2_IRQn, 0, 2);
	#endif
    //uart3
	
	gpio_init(RS485_EN_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, RS485_EN_PIN);
	/* connect port to USARTx_Tx */
	gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
	/* connect port to USARTx_Rx */
	gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);	
	
	
	usart_deinit(UART3);  /* ��λ���� */
	
	usart_baudrate_set(UART3, UART2_BAUD);   /* ������ */
	
	usart_parity_config(UART3, USART_PM_NONE);   /* ��ż����λ */
	
	usart_word_length_set(UART3, USART_WL_8BIT); /* �ֳ� */
	
	usart_stop_bit_set(UART3, USART_STB_1BIT);  /* ֹͣλ */
	
    usart_hardware_flow_rts_config(UART3, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(UART3, USART_CTS_DISABLE);
	
	usart_transmit_config(UART3, USART_TRANSMIT_ENABLE);  
	
	usart_receive_config(UART3, USART_RECEIVE_ENABLE);    

	usart_enable(UART3); 
	
	usart_interrupt_enable(UART3,USART_INT_RBNE);  

	nvic_irq_enable(UART3_IRQn, 0, 3);
 
    RS485_RX_ON;
}




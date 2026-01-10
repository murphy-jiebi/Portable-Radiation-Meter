#ifndef _BSP_UART_H
#define _BSP_UART_H

#include "gd32f10x.h"

typedef struct
{
	uint32_t	uart;			/* STM32 内部串口设备指针 */
	uint8_t 	*pTxBuf;		/* 发送缓冲区 */
	uint8_t 	*pRxBuf;		/* 接收缓冲区 */
	uint16_t 	usTxBufSize;	/* 发送缓冲区大小 */
	uint16_t	usRxBufSize;	/* 接收缓冲区大小 */
    __IO uint16_t usTxIndex;	
	__IO uint16_t usRxIndex;	
	__IO uint16_t usTxCount;	/* 等待发送的数据个数 */
	__IO uint16_t usRxCount;	/* 接收到的数据个数 */
	__IO uint8_t  ucTxFrame;	/* 数据发送完成标志帧 */ 
	__IO uint8_t  ucRxFrame;	/* 接收到数据帧标志 */ 
}UART_T;

extern UART_T g_tUart0;
extern UART_T g_tUart1;
extern UART_T g_tUart2;
extern UART_T g_tUart3;
extern UART_T g_tUart4;

extern void uart_send_buf(UART_T *_pUart, uint8_t *_ucpBuf, uint16_t _usBufLen);
extern void UART_IRQn(UART_T *_pUart);
extern void BSP_Uart_Init(void);


#endif

/**********************************************************************************************************
**********************************************************************************************************
* FILE		   : BSP_UWB.C
* Description  : UWB模组驱动
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
#include "math.h"
#define UWB_UART  g_tUart0
uint64_t temp64_t;


struct tag_uwb_packet_def
{
	uint8_t   start[2];                        	//包头
	uint8_t   cmd;                        		//指令
    uint8_t   len;                           	//数据长度
    uint8_t   data_len;                         //报文长度
    uint8_t   reserve[16];                      //报文
	uint8_t   check[2];                         //校验码
};

/**********************************************************************************************************
*	函 数 名: UWB_Checksum_u16
*	功能说明: 求取校验码，除去包头包尾数据段相加求和取反
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
uint16_t UWB_Checksum_u16(uint8_t* pdata, uint32_t len) 
{ 
	uint16_t sum = 0; 
	uint32_t i; 
	for(i=0; i<len; i++) 
		sum += pdata[i]; 
	sum = ~sum; 
	return sum;
}



/***********************************************************************************
* Function	    : UWB_SendInfo
* Description	: 发送UWB预留信息
* Input Para	: 
* Output Para   : 
* Return Value  : 
***********************************************************************************/
void UWB_Send_Init(void)
{
	uint8_t data_len[10] = {0x55 ,0xAA ,0x03 ,0x06 ,0x02 ,0x0E ,0x01 ,0x01 ,0xE4 ,0xFF }; 
	
	uart_send_buf(&UWB_UART,data_len,10);
	
	UWB_UART.usRxCount = 0x0000;
	UWB_UART.ucRxFrame = 0x00;
}


/***********************************************************************************
* Function	    : UWB_SendInfo
* Description	: 发送UWB预留信息
* Input Para	: 
* Output Para   : 
* Return Value  : 
***********************************************************************************/
void UWB_SendInfo(void)
{
	uint16_t crc16;
	uint8_t data_float[4];

    struct tag_uwb_packet_def tag_uwb_packet = { 
					.start = {0x66,0xBB},
					.cmd = 0x11,
					.len = 0x13,
					.data_len = 0x10,
					.reserve = {0},
					.check = {0}
	};
	
    tag_uwb_packet.reserve[0] = tag_dis.type;
	
	tag_uwb_packet.reserve[1] = tag_dis.nuclide;
	
	temp64_t = tag_dis.strong *  tag_dis.SW_check_flag;
	memcpy(tag_uwb_packet.reserve+2,&temp64_t,8);
	
	 FloatToData(tag_dis.factor,data_float);
	memcpy(tag_uwb_packet.reserve+10,data_float,4);
	
	crc16 = UWB_Checksum_u16(((uint8_t*)&tag_uwb_packet)+2,19);
	tag_uwb_packet.check[0] = crc16;
	tag_uwb_packet.check[1] = crc16>>8;
	
	uart_send_buf(&UWB_UART,(uint8_t*)&tag_uwb_packet,23);
	
	
	UWB_UART.usRxCount = 0x0000;
	UWB_UART.ucRxFrame = 0x00;
}

/**********************************************************************************************************
*	函 数 名: BSP_UWB_Init
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
void BSP_UWB_Init(void)
{
    UWB_POWER_OFF();
}



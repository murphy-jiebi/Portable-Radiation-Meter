/**********************************************************************************************************
*
*	模块名称 : 通信
*	文件名称 : comm.c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期             作者              说明
*		V1.0    2022-04-08   
*
**********************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "gd32f10x.h"
#include "bsp.h"
#include "user.h"

extern uint8_t TAG_CheckFrame(UART_T *pUart);

#define DEVICE_TYPE 0x60

struct tgaBufFrame
{
	uint8_t Start;
	uint8_t Len;
	uint8_t DeviceType;
	uint8_t DeviceNum;
	uint8_t Cmd;
	uint8_t Data[100];
};



/*
*********************************************************************************************************
*	函 数 名: Get_CrcXOR
*	功能说明: 8 位 CRC 校验
*	形    参: _ucpBuffer       被校验的数组
*             _usBufSize       数组的长度
*	返 回 值: 校验结果
*********************************************************************************************************
*/
uint8_t Get_CrcXOR(uint8_t *_ucpBuffer, uint16_t _usLen)
{
	uint16_t i;
	uint8_t  Temp_Crc = 0x00;	
	for(i = 0; i < _usLen; i++)
	{
		Temp_Crc ^= *_ucpBuffer++;
	}
	return Temp_Crc;
}

/*
*********************************************************************************************************
*	函 数 名: RxBuf_Handel
*	功能说明: 8 位 CRC 校验
*	形    参: 
*	返 回 值: 接收帧数据校验
*********************************************************************************************************
*/
uint8_t RxBuf_Handel(uint8_t *_RxBuf, uint16_t _RxLen, uint8_t *_Cmd, uint8_t *_Data)
{
	struct tgaBufFrame *Ptr;
	uint16_t FrameLen;
	uint8_t Crc[2];
	
	
	Ptr = memchr(_RxBuf, 0x68, _RxLen);
	
	if(Ptr == NULL)  /* 帧头 */
	{
		return _FALSE;
	}
	
	if(Ptr->DeviceType != DEVICE_TYPE)  /* 判断设备类型 */
	{
		return _FALSE;
	}
	
	if(Ptr->DeviceNum != sys_param.addr)  /* 设备地址 */
	{
		return _FALSE;
	}
	
	Crc[0] = Get_CrcXOR(&Ptr->Start, Ptr->Len);
	
	Crc[1] = *(&Ptr->Start + Ptr->Len);
	
	if(Crc[0] != Crc[1])  /* 校验位 */
	{
		return _FALSE;
	}
	
	*_Cmd = Ptr->Cmd; /* 获取指令 */
	
	memcpy(_Data, Ptr->Data, Ptr->Len - 5);
	
	return _TRUE;
}

/*
*********************************************************************************************************
*	函 数 名: SendBuf_Handel
*	功能说明: 数据处理
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
uint16_t SendBuf_Handel(uint8_t _Cmd, uint8_t *_Data, struct tgaBufFrame *_TxBuf)
{
	uint8_t Crc;
	double BeiDou;
	_TxBuf->Start = 0x5A;
	_TxBuf->Len = 5;
	_TxBuf->DeviceType = DEVICE_TYPE;
	_TxBuf->DeviceNum = sys_param.addr;
	_TxBuf->Cmd = _Cmd;
	
	switch(_Cmd)
	{
		case 0xF1:
			_TxBuf->Len += 16;
			BeiDou = StrTodouble(g_GPS.WeiDu); 
			memcpy(&_TxBuf->Data[0], (uint8_t*)&BeiDou, 8);
			BeiDou = StrTodouble(g_GPS.JingDu); 
			memcpy(&_TxBuf->Data[8], (uint8_t*)&BeiDou, 8);
		
			break;
		case 0xF2:
			_TxBuf->Len += 1;
			_TxBuf->Data[0] = Voltage;
		
			break;
		case 0xF3:
			_TxBuf->Len += 1;
			_TxBuf->Data[0] = tag_dis.type;
		
			break;		
		case 0xF4:
			_TxBuf->Len += 1;
			_TxBuf->Data[0] = tag_dis.nuclide;
		
			break;	
		case 0xF5:
			_TxBuf->Len += 8;
			memcpy(&_TxBuf->Data[0], (uint8_t*)&tag_dis.strong, 8);

			break;	
		
		case 0xFE:
			_TxBuf->Len += 27;
			_TxBuf->Data[0] = tag_dis.type;
			_TxBuf->Data[1] = tag_dis.nuclide;
			memcpy(&_TxBuf->Data[2], (uint8_t*)&tag_dis.strong, 8);
			BeiDou = StrTodouble(g_GPS.WeiDu); 
			memcpy(&_TxBuf->Data[10], (uint8_t*)&BeiDou, 8);
			BeiDou = StrTodouble(g_GPS.JingDu); 
			memcpy(&_TxBuf->Data[18], (uint8_t*)&BeiDou, 8);
			_TxBuf->Data[26] = Voltage;
			break;
		
		case 0xD0:
			_TxBuf->Len += 1;
			_TxBuf->Data[0] = sys_param.uwb_en;
			
			break;	
		case 0xD1:
			_TxBuf->Len += 1;
			_TxBuf->Data[0] = sys_param.beidou_en;

			break;				
		case 0x90:
			_TxBuf->Len += 6;
			_TxBuf->Data[0] = 0x00;
			_TxBuf->Data[1] = 0x00;
			_TxBuf->Data[2] = 0x00;
			_TxBuf->Data[3] = 0x00;
			_TxBuf->Data[4] = 0x00;
			_TxBuf->Data[5] = 0x00;
			break;
		case 0x80:
			_TxBuf->Len += 16;
			memset(&_TxBuf->Data[0], 0x00, 16);
			break;
		case 0x73:
			tag_dis.type = _Data[0];
			write_eeprom(&tag_dis.type, e2rom_addr(type), 1); 
			break;
		case 0x74:
			tag_dis.nuclide = _Data[0];
			write_eeprom(&tag_dis.nuclide, e2rom_addr(nuclide), 1); 

			break;
		case 0x75:
			memcpy((uint8_t*)&tag_dis.strong, _Data, 8);
			write_eeprom((uint8_t*)&tag_dis.strong, e2rom_addr(strong), 8);
			break;
		case 0x50:
			sys_param.uwb_en = _Data[0];
			write_eeprom(&sys_param.uwb_en, e2rom_addr(uwb_en), 1); 
			break;
		case 0x51:
			sys_param.beidou_en = _Data[0];
			write_eeprom(&sys_param.beidou_en, e2rom_addr(beidou_en), 1); 

			break;
		case 0x10:

//			_TxBuf->Data[0] = _Data[0];
//			_TxBuf->Data[1] = _Data[1];
//			_TxBuf->Data[2] = _Data[2];
//			_TxBuf->Data[3] = _Data[3];
//			_TxBuf->Data[4] = _Data[4];
//			_TxBuf->Data[5] = _Data[5];

			break;
		default:
			return 0;
	}

	
	Crc = Get_CrcXOR(&(_TxBuf->Start), _TxBuf->Len);
	_TxBuf->Data[_TxBuf->Len - 5] = Crc;
	
	return (_TxBuf->Len + 1);
}




 /******************************************************************************
函数名称：AppLora
功能描述：Lora数据帧处理
输入参数：无
输出参数：无
函数返回：无
*****************************************************************************/
void AppLora(void)
{ 
    uint8_t result;
	uint8_t Cmd;
	uint8_t Data[100];
	uint8_t TxBuf[100];
	uint16_t TxLen;
	
	if(g_tUart1.ucRxFrame != 0x00)
	{
		
		result = RxBuf_Handel(g_tUart1.pRxBuf, g_tUart1.usRxCount, &Cmd, Data);
		if(result == _TRUE)
		{
			TxLen = SendBuf_Handel(Cmd, Data, (struct tgaBufFrame*)&TxBuf);
			if(TxLen > 0)
			{
				uart_send_buf(&g_tUart1, TxBuf, TxLen);
			}
		}
		
		memset(g_tUart1.pRxBuf, 0x00, g_tUart1.usRxCount);
		g_tUart1.usRxCount = 0;
		g_tUart1.ucRxFrame = 0;
	}
	
	
	
//	result = TAG_CheckFrame(&g_tUart1);
	
	
//    result = TAG_CheckFrame(&g_tUart1);
//    if(result == _TRUE)
//    {
//        if(memcmp(g_tUart1.pRxBuf,"$GETSTA",7) == 0x00)        //应答帧
//        {
//			if(Get_DATA_GETSTA(g_tUart1.pRxBuf))
//			{
//				Lora_Send_Proess_ACKSTA();
//			}
//        }
//        
//        else if(memcmp(g_tUart1.pRxBuf,"$SETADDR",8) == 0x00)   //参数修改
//        {
//			if(Set_DATA_SETADDR(g_tUart1.pRxBuf))
//			{
//				Lora_Send_Proess_ACKADDR(_TRUE);
//			}
//			else
//			{
//				Lora_Send_Proess_ACKADDR(_FALSE);	
//			}
//        }
//        else if(memcmp(g_tUart1.pRxBuf,"$SETCONFIG",10) == 0x00)   //地址设置
//        {
//			if(Set_DATA_SETCONFIG(g_tUart1.pRxBuf))
//			{
//				Lora_Send_Proess_ACKSET(_TRUE);
//			}
//			else
//			{
//				Lora_Send_Proess_ACKSET(_FALSE);	
//			}
//        }
//        g_tUart1.usRxCount = 0x0000;
//	    g_tUart1.ucRxFrame = 0x00;
//		memset(g_tUart0.pRxBuf,0,512);
//    }
}


/***********************************************************************************
* Function	    : TC_Send_Test_Frame
* Description	: 
* Input Para	: 
* Output Para   : 
* Return Value  : 
***********************************************************************************/
uint8 TC_Send_Test_Frame(UART_T *pUart)
{
	if(g_tUart3.ucRxFrame)
	{
		uart_send_buf(pUart,g_tUart3.pRxBuf,g_tUart3.usRxCount);
        g_tUart3.usRxCount = 0x0000;
	    g_tUart3.ucRxFrame = 0x00;
        pUart->usRxCount = 0x0000;
	    pUart->ucRxFrame = 0x00;
	}
	if(pUart->ucRxFrame)
	{
		uart_send_buf(&g_tUart3,pUart->pRxBuf,pUart->usRxCount);
        g_tUart3.usRxCount = 0x0000;
	    g_tUart3.ucRxFrame = 0x00;
        pUart->usRxCount = 0x0000;
	    pUart->ucRxFrame = 0x00;
	}
	return _TRUE;
}





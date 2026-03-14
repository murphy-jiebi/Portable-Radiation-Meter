/* *********************************************************************************************************
**********************************************************************************************************
* FILE		   : sys_gm.C
* Description  : 探测器通信协议
*			  
* Copyright (c) 2024 by SHAANXI WEIFENG INSTRUMENT Corp.,Ltd   All Rights Reserved.
* 
* History      :
*
* Version		  Name       	   	Date			           Description
   0.1		      wangfeng  	    2024/05/20                 Initial Version
   
**********************************************************************************************************
**********************************************************************************************************/
#include "user.h"
#include "bsp.h"
#include <string.h>
#include <stdio.h>

double WeiDu_double = 0.0l,JingDu_double = 0.0l;
static uint64_t temp64_t;
/*********************************************************************************************************
*                                            变量声明
*********************************************************************************************************/


/**********************************************************************************************************
* Function	    : CheckXor
* Description	: 计算效验码
* Input Para	: 
* Output Para   : 
* Return Value  : 
**********************************************************************************************************/
static uint8_t CheckXor(uint8_t *_ucaBuf, uint16_t _usLen)
{
	uint8_t ucXorSum;
	uint8_t ucaBuf[2];
	uint16_t i;

	if (_usLen < 3)
	{
		return _FALSE;
	}
	if (_ucaBuf[_usLen - 3] != '*')
	{
		return _FALSE;
	}
	for (i = 0; i < _usLen - 3; i++)
	{
		if ((_ucaBuf[i] & 0x80) || (_ucaBuf[i] == 0))
		{
			return _FALSE;
		}
	}
	ucXorSum = _ucaBuf[0];
	for (i = 1; i < _usLen - 3; i++)
	{
		ucXorSum = ucXorSum ^ _ucaBuf[i];
	}

	HexToAscii(&ucXorSum, ucaBuf, 2);

	if (memcmp(&_ucaBuf[_usLen - 2], ucaBuf, 2) == 0)
	{
		return _TRUE;
	}

	return _FALSE;
}


/***********************************************************************************
* Function	    : TAG_CheckFrame
* Description	: 对接收到的数据帧进行解析
* Input Para	: 
* Output Para   : 
* Return Value  : 
***********************************************************************************/
uint8_t TAG_CheckFrame(UART_T *pUart)
{
    uint8_t  *ptr;
    
    if(pUart->ucRxFrame != 0x01 )
    {
        return _FALSE;
    }
    
	if(pUart->usRxCount < 5)                            
	{
		pUart->usRxCount = 0x0000;
	    pUart->ucRxFrame = 0x00;
        return _FALSE;
	}
    
    ptr = pUart->pRxBuf;
	if(*ptr != '$')       //判断帧头
	{
		pUart->usRxCount = 0x0000;
	    pUart->ucRxFrame = 0x00;
	    return _FALSE;
	}
    
	if (CheckXor((uint8_t *)ptr + 1, pUart->usRxCount -3) == _TRUE)
	{
	    return _TRUE;
	} 	
    pUart->usRxCount = 0x0000;
	pUart->ucRxFrame = 0x00;
	return _FALSE;
}

/**********************************************************************************************************
* Function	    : Get_DATA_GETSTA
* Description	: 解析获取参数命令
* Input Para	: 
* Output Para   : 
* Return Value  : 
**********************************************************************************************************/
uint8_t Get_DATA_GETSTA(uint8_t *_ucaBuf)
{
	char *p;

	p = (char *)_ucaBuf;

	/* 字段1 */
	p = strchr(p, ',');
	if (p == 0)
	{
		return _FALSE;
	}
	p++;
	if( StrToInt(p) != 2)
    {
        return _FALSE;
    }

	/* 字段2 */
	p = strchr(p, ',');
	if (p == 0)
	{
		return _FALSE;
	}
	p++;
	if( StrToInt(p) != sys_param.addr)
    {
        return _FALSE;
    }

	return _TRUE;
}


/**********************************************************************************************************
* Function	    : Set_DATA_SETADDR
* Description	: 解析设置地址命令
* Input Para	: 
* Output Para   : 
* Return Value  : 
**********************************************************************************************************/
uint8_t Set_DATA_SETADDR(uint8_t *_ucaBuf)
{
	char *p;
    uint8_t temp;

	p = (char *)_ucaBuf;

	/* 字段1 */
	p = strchr(p, ',');
	if (p == 0)
	{
		return _FALSE;
	}
	p++;
	if( StrToInt(p) != 2)
    {
        return _FALSE;
    }

	/* 字段2 */
	p = strchr(p, ',');
	if (p == 0)
	{
		return _FALSE;
	}
	p++;
	if( StrToInt(p) != sys_param.addr)
    {
        return _FALSE;
    }
	
	/* 字段3 */
	p = strchr(p, ',');
	if (p == 0)
	{
		return _FALSE;
	}
	p++;
    temp = StrToInt(p);
    if(temp != sys_param.addr)
    {
        sys_param.addr = temp;
        write_eeprom(&sys_param.addr, e2rom_addr(addr),1);
    }
	return _TRUE;
}


/**********************************************************************************************************
* Function	    : Set_DATA_SETCONFIG
* Description	: 解析设置参数命令
* Input Para	: 
* Output Para   : 
* Return Value  : 
**********************************************************************************************************/
uint8_t Set_DATA_SETCONFIG(uint8_t *_ucaBuf)
{
	char *p;
    uint8_t temp;

    float temp_float;
	
	p = (char *)_ucaBuf;

	/* 字段1 */
	p = strchr(p, ',');
	if (p == 0)
	{
		return _FALSE;
	}
	p++;
	if( StrToInt(p) != 2)
    {
        return _FALSE;
    }

	/* 字段2 */
	p = strchr(p, ',');
	if (p == 0)
	{
		return _FALSE;
	}
	p++;
	if( StrToInt(p) != sys_param.addr)
    {
        return _FALSE;
    }
	
	/* 字段3 */
	p = strchr(p, ',');
	if (p == 0)
	{
		return _FALSE;
	}
	p++;
    temp = StrToInt(p);
    if(temp != tag_dis.type)
    {
        tag_dis.type = temp;
        write_eeprom(&tag_dis.type, e2rom_addr(type),1);
    }
	
	/* 字段4 */
	p = strchr(p, ',');
	if (p == 0)
	{
		return _FALSE;
	}
	p++;
    temp = StrToInt(p);
    if(temp != tag_dis.nuclide)
    {
        tag_dis.nuclide = temp;
        write_eeprom(&tag_dis.nuclide, e2rom_addr(nuclide),1);
    }

	/* 字段5 */
	p = strchr(p, ',');
	if (p == 0)
	{
		return _FALSE;
	}
	p++;
    temp64_t = StrTo64Int(p);
    if(temp64_t != tag_dis.strong)
    {
        tag_dis.strong = temp64_t;
        write_eeprom((uint8_t *)&tag_dis.strong, e2rom_addr(strong),8);
    }
	
	/* 字段6 */
	p = strchr(p, ',');
	if (p == 0)
	{
		return _FALSE;
	}
	p++;
    temp_float = StrToFloat(p);
    if(temp_float != tag_dis.factor)
    {
        tag_dis.factor = temp_float;
        write_eeprom((uint8_t *)&tag_dis.factor, e2rom_addr(factor),4);
    }
	
	return _TRUE;
}


/***********************************************************************************
* Function	    : TAG_SendFrame
* Description	: 
* Input Para	: 
* Output Para   : 
* Return Value  : 
***********************************************************************************/
uint8_t TAG_SendFrame(UART_T *pUart,char *frame)
{
	uint8_t i,len;
	uint8_t ucXorSum = 0x00;
	char str[256];
	
	memset(str,0x00,128);
	str[0] = '$';
	strcat(str,frame);
	len = strlen(str);
	str[len] = '*';
	for (i = 1; i < len; i++)
	{
		ucXorSum = ucXorSum ^ str[i];
	}	
	HexToAscii(&ucXorSum, (uint8_t*)(str + len + 1u), 2);
	str[len + 3] = 0x0D;
	str[len + 4] = 0x0A;
	
	uart_send_buf(pUart,(uint8_t *)str,len + 5);
	return _TRUE;
}


/*********************************************************************************************************
*	函 数 名: Lora_Send_Proess_ACKSTA
*	功能说明: 通信模块发送数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************/
uint8_t Lora_Send_Proess_ACKSTA(void)
{
    char  str[256];
	WeiDu_double = StrTodouble(g_GPS.WeiDu); 
	JingDu_double = StrTodouble(g_GPS.JingDu); 
	
    sprintf(str,"ACKSTA,%02u,%02u,%02u,%02u,%019lld,%.4f,%02u,%lf,%lf,%03u",02,sys_param.addr,tag_dis.type,tag_dis.nuclide,tag_dis.strong,tag_dis.factor,g_GPS.PositionOk,JingDu_double,WeiDu_double,Voltage);
    
    TAG_SendFrame(&g_tUart1,str);
    
    return _TRUE;
}

/*********************************************************************************************************
*	函 数 名: Lora_Send_Proess_ACKSET
*	功能说明: 通信模块发送数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************/
uint8_t Lora_Send_Proess_ACKSET(uint8_t result)
{
    char  str[50];

    sprintf(str,"ACKSET,%02u",result);
    
    TAG_SendFrame(&g_tUart1,str);
    
    return _TRUE;
}

/*********************************************************************************************************
*	函 数 名: Lora_Send_Proess_ACKADDR
*	功能说明: 通信模块发送数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************/
uint8_t Lora_Send_Proess_ACKADDR(uint8_t result)
{
    char  str[50];

    sprintf(str,"ACKSET,%02u",result);
    
    TAG_SendFrame(&g_tUart1,str);
    
    return _TRUE;
}









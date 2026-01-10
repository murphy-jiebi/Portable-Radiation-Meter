/**********************************************************************************************************
**********************************************************************************************************
* FILE		   : BSP_GNSS.C
* Description  : GNSS模组驱动
*			  
* Copyright (c) 2016 by SHAANXI WEIFENG INSTRUMENT Corp.,Ltd   All Rights Reserved.
* 
* History      :
*
* Version		  Name       	   	Date			           Description
   0.1		      jinlei      	    2024/10/30                 Initial Version
   
**********************************************************************************************************
**********************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "bsp.h"
#include "user.h"

#define GNSS_UART  g_tUart2

/**********************************************************************************************************
*	函 数 名: CheckXor
*	功能说明: 检查0183数据包的校验和是否正确
*	形    参:  _ucaBuf  收到的数据
*			 _usLen    数据长度
*	返 回 值: TRUE 或 FALSE.
**********************************************************************************************************/
uint8_t CheckXor(uint8_t *_ucaBuf, uint16_t _usLen)
{
	uint8_t ucXorSum;
	uint8_t ucaBuf[2];
	uint16_t i;

	if (_usLen < 3)
	{
		return _FALSE;
	}

	/* 如果没有校验字节，也认为出错 */
	if (_ucaBuf[_usLen - 3] != '*')
	{
		return _FALSE;
	}


	/* 不允许出现非ASCII字符 */
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


/**********************************************************************************************************
*	函 数 名: gpsGPRMC
*	功能说明: 分析0183数据包中的 GPGSV 命令，结果存放到全局变量
*	形    参:  _ucaBuf  收到的数据
*			 _usLen    数据长度
*	返 回 值: 无
**********************************************************************************************************/
/*
例：$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50
字段0：$GPRMC，语句ID，表明该语句为Recommended Minimum Specific GPS/TRANSIT Data（RMC）推荐最小定位信息
字段1：UTC时间，hhmmss.sss格式
字段2：状态，A=定位，V=未定位
字段3：纬度ddmm.mmmm，度分格式（前导位数不足则补0）
字段4：纬度N（北纬）或S（南纬）
字段5：经度dddmm.mmmm，度分格式（前导位数不足则补0）
字段6：经度E（东经）或W（西经）
字段7：速度，节，Knots
字段8：方位角，度
字段9：UTC日期，DDMMYY格式
字段10：磁偏角，（000 - 180）度（前导位数不足则补0）
字段11：磁偏角方向，E=东W=西
字段16：校验值
*/
uint8_t gpsGPRMC(uint8_t *_ucaBuf, uint16_t _usLen, GPS_T *g_tGPS)
{
	uint8 i;
	char *p;

	p = (char *)_ucaBuf;
	p[_usLen] = 0;

	/* 字段1 UTC时间，hhmmss.sss格式 */
	p = strchr(p, ',');
	if (p == 0)
	{
		return _FALSE;
	}
	p++;
	g_tGPS->Time[_hh] = StrToIntFix(p, 2);
	p += 2;
	g_tGPS->Time[_mm] = StrToIntFix(p, 2);
	p += 2;
	g_tGPS->Time[_ss] = StrToIntFix(p, 2);
	p += 3;

	/* 字段2 状态，A=定位，V=未定位 */
	p = strchr(p, ',');
	if (p == 0)
	{
		return _FALSE;
	}
	p++;
	if (*p != 'A')
	{
		//未定位成功
		g_tGPS->PositionOk = 0;
	}
	else
	{    
		//定位成功
	    g_tGPS->PositionOk = 1;
	}
	
	p ++;

	/* 字段3 纬度ddmm.mmmm，度分格式（前导位数不足则补0） */
	p = strchr(p, ',');
	if (p == 0)
	{
		return _FALSE;
	}
	p++;
	if(g_tGPS->PositionOk == 1)
	{
		memcpy(g_tGPS->WeiDu,p,9);
	    p += 9;
	}

	/* 字段4 纬度N（北纬）或S（南纬）*/
	p = strchr(p, ',');
	if (p == 0)
	{
		return _FALSE;
	}
	p++;
	if(g_tGPS->PositionOk == 1)
	{
		if (*p == 'S')
	    {
		    g_tGPS->NS = 'S';
	    }
	    else if (*p == 'N')
	    {
		    g_tGPS->NS = 'N';
	    }
	    else
	    {
		    g_tGPS->NS = '?';
	    }
	}

	/* 字段5 经度dddmm.mmmm，度分格式（前导位数不足则补0） */
	p = strchr(p, ',');
	if (p == 0)
	{
		return _FALSE;
	}
	p++;
	if(g_tGPS->PositionOk == 1)
	{
	    memcpy(g_tGPS->JingDu,p,10);
	    p += 10;
	}

	/* 字段6：经度E（东经）或W（西经） */
	p = strchr(p, ',');
	if (p == 0)
	{
		return _FALSE;
	}
	p++;
	if(g_tGPS->PositionOk == 1)
	{
	    if (*p == 'E')
	    {
		    g_tGPS->EW = 'E';
	    }
	    else if (*p == 'W')
	    {
		    g_tGPS->EW = 'W';
	    }
		else
		{
		    g_tGPS->EW = '?';
		}
	}


	/* 字段7：速度，节，Knots  10.05,*/
	p = strchr(p, ',');
	if (p == 0)
	{
		return _FALSE;
	}
	p++;
	if(g_tGPS->PositionOk == 1)
	{
	    for(i = 0x00; i < 10; i++)
		{
		     if(*p == ',')
			 {
			     break;
			 }
			 else
			 {
			     g_tGPS->SuDu[i] = *p;
			 }
			 p++;
		}
	}

	/* 字段8：方位角，度 ,324.27 */
	p = strchr(p, ',');
	if (p == 0)
	{
		return _FALSE;
	}
	p++;
	if(g_tGPS->PositionOk == 1)
	{
	    for(i = 0x00; i < 10; i++)
		{
		     if(*p == ',')
			 {
			     break;
			 }
			 else
			 {
			     g_tGPS->Angel[i] = *p;
			 }
			 p++;
		}
	}
	/* 字段9：UTC日期，DDMMYY格式  150706 */
	p = strchr(p, ',');
	if (p == 0)
	{
		return _FALSE;
	}
	p++;
	g_tGPS->Time[_dd] = StrToIntFix(p, 2);
	p += 2;
	g_tGPS->Time[_mon] = StrToIntFix(p, 2);
	p += 2;
	g_tGPS->Time[_yy] = StrToIntFix(p, 2);
	p += 2;
	return _TRUE;
}

/***********************************************************************************
* Function	    : GPS_Recv_Frame
* Description	: GPS接收数据帧
* Input Para	: 
* Output Para	: 
* Return Value  : 
***********************************************************************************/
uint8_t GPS_Recv_Frame(GPS_T *g_tGPS)
{
	char *ptr1, *ptr2;
	uint8_t result = _FALSE;
    uint16 count,len;
	
	if(GNSS_UART.ucRxFrame != 0x00)
	{
        count = 0x0000;
        while(count < GNSS_UART.usRxCount)
        {
            ptr1 = strchr((char *)GNSS_UART.pRxBuf + count,'$');
            if (ptr1 != _NULL)
            {
                ptr1 ++;
                ptr2 = strchr(ptr1,'\n');
                if (ptr2 != _NULL)
                {
                    len = ptr2 - ptr1;
                    count += len;
                    if(len > 10)
                    {
                        if (CheckXor((uint8_t *)ptr1, len - 1) == _TRUE)
                        {
                            if ((memcmp(ptr1, "GPRMC,", 6) == 0)||(memcmp(ptr1, "GNRMC,", 6) == 0)||(memcmp(ptr1, "BDRMC,", 6) == 0))
                            {
                                 result = gpsGPRMC((uint8_t *)ptr1, count,g_tGPS);
                            } 
                        } 
                    }
                }
                else
                {
                    break;
                }
            }
            else  
            {
                break;
            }                
        }

		memset(GNSS_UART.pRxBuf,0x00,GNSS_UART.usRxCount);
		GNSS_UART.usRxCount = 0x0000;
		GNSS_UART.ucRxFrame = 0x00;	
	}
	return result;
}


/**********************************************************************************************************
*	函 数 名: BSP_GNSS_Init
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
void BSP_GNSS_Init(void)
{
	GNSS_POWER_OFF();
}



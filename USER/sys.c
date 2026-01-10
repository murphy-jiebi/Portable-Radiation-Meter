/**********************************************************************************************************
*
*	模块名称 : 系统共用任务
*	文件名称 : sys.h
*	版    本 : V1.0
*	说    明 : 系统共用任务
*
*	修改记录 :
*		版本号  日期             作者              说明
*		V1.0    2022-04-08   
*
**********************************************************************************************************/
#include "user.h"
#include "math.h"
#include "string.h"
/*********************************************************************************************************
*                                            变量声明
*********************************************************************************************************/
 
/***********************************************************************************
* Function	: delay_us
* Description	:
* Input Para	: 
* Output Para	: 
* Return Value: 
***********************************************************************************/
void delay_us(uint32_t us)
{
    uint32_t i;
	while(us--)
    {
    	for(i = 0x00; i<20; i++);
    }
}

/***********************************************************************************
* Function	: delay_ms
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value: 
***********************************************************************************/
void delay_ms(uint32_t ms)
{
    while(ms--)
    {
        delay_us(500);
	}
}

/**********************************************************************************************************
* Function	: HexToAscii
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value: 
**********************************************************************************************************/
void HexToAscii(uint8_t *_ucpHex, uint8_t *_ucpAscII, uint8_t _ucLenasc)
{
	uint8_t i;
	uint8_t ucTemp;

	for (i = 0; i < _ucLenasc; i++)
	{
		ucTemp = *_ucpHex;
		if ((i&0x01) == 0x00)
			ucTemp = ucTemp >> 4;
		else
		{
			ucTemp = ucTemp & 0x0f;
			_ucpHex++;
		}
		if (ucTemp < 0x0a)
			ucTemp += 0x30;
		else
			ucTemp += 0x37;
		_ucpAscII[i] = ucTemp;
	}
	//--------debug--------//
	_ucpAscII[i] = '\0';
	//--------end----------//
}

/**********************************************************************************************************
*	函 数 名: StrToIntFix
*	功能说明: 将ASCII码字符串转换成十进制, 给定长度
*	形    参: _pStr :待转换的ASCII码串. 可以以逗号或0结束
*			 _ucLen : 固定长度
*	返 回 值: 二进制整数值
**********************************************************************************************************/
int32_t StrToIntFix(char *_pStr, uint8_t _ucLen)
{
	uint8_t flag;
	char *p;
	uint32_t ulInt;
	uint8_t i;
	uint8_t ucTemp;

	p = _pStr;
	if (*p == '-')
	{
		flag = 1;	/* 负数 */
		p++;
		_ucLen--;
	}
	else
	{
		flag = 0;
	}

	ulInt = 0;
	for (i = 0; i < _ucLen; i++)
	{
		ucTemp = *p;
		if (ucTemp == '.')	/* 遇到小数点，自动跳过1个字节 */
		{
			p++;
			ucTemp = *p;
		}
		if ((ucTemp >= '0') && (ucTemp <= '9'))
		{
			ulInt = ulInt * 10 + (ucTemp - '0');
			p++;
		}
		else
		{
			break;
		}
	}

	if (flag == 1)
	{
		return -ulInt;
	}
	return ulInt;
}

/**********************************************************************************************************
* Function	    : StrToInt
* Description	: 字符串转整数
* Input Para	: 
* Output Para	: 
* Return Value: 
**********************************************************************************************************/
int32_t StrToInt(char *_pStr)
{
	uint8_t flag;
	char *p;
	uint32_t ulInt;
	uint8_t i;
	uint8_t ucTemp;

	p = _pStr;
	if (*p == '-')
	{
		flag = 1;	
		p++;
	}
	else
	{
		flag = 0;
	}

	ulInt = 0;
	for (i = 0; i < 15; i++)
	{
		ucTemp = *p;
		if (ucTemp == '.')	
		{
			p++;
			ucTemp = *p;
		}
		if ((ucTemp >= '0') && (ucTemp <= '9'))
		{
			ulInt = ulInt * 10 + (ucTemp - '0');
			p++;
		}
		else
		{
			break;
		}
	}

	if (flag == 1)
	{
		return -ulInt;
	}
	return ulInt;
}

/**********************************************************************************************************
* Function	    : StrTo64Int
* Description	: 字符串转64位整数
* Input Para	: 
* Output Para	: 
* Return Value: 
**********************************************************************************************************/
__int64 StrTo64Int(char *arrTmp)
{
	int len =0;
	int i=0;
	int j =0;
	__int64 nTmpRes =0;
	__int64 ntmp10=1;
	char *p;
	if (arrTmp == NULL)
	{
		return 0;
	}
	p = arrTmp;
	p = strchr(p, ',');
	len=p - arrTmp;
 
	for (i =len-1 ; i >=0;i--)
	{
		ntmp10 =1;
		for (j=1;j<(len -i); j++)
		{
			ntmp10 = ntmp10*10;
		}
		nTmpRes =nTmpRes + (arrTmp[i]-48)* ntmp10;
	}
 
	return nTmpRes;
}

/**********************************************************************************************************
* Function	    : StrToFloat
* Description	: 字符串转浮点数
* Input Para	: 
* Output Para	: 
* Return Value: 
**********************************************************************************************************/
float StrToFloat(char *_pStr)
{
	uint8_t flag;
	char *p;
	uint32_t ulInt;
	float    ulTemp;
	float    ulDot;
	uint8_t i;
	uint8_t ucTemp;

	p = _pStr;
	if (*p == '-')
	{
		flag = 1;	
		p++;
	}
	else
	{
		flag = 0;
	}

	ulInt = 0;
	for (i = 0; i < 15; i++)   
	{
		ucTemp = *p;
		if (ucTemp == '.')	
		{
			p++;
			break;
		}
		if ((ucTemp >= '0') && (ucTemp <= '9'))
		{
			ulInt = ulInt * 10 + (ucTemp - '0');
			p++;
		}
		else
		{
			break;
		}
	}
	ulTemp = 0L;
	ulDot  = 1L;
	for (; i < 15; i++)                //è?D?êy
	{
		ucTemp = *p;
		if ((ucTemp >= '0') && (ucTemp <= '9'))
		{
			ulDot *= 0.1L;
			ulTemp +=  (ucTemp - '0') * ulDot;
			p++;
		}
		else
		{
			break;
		}
	}
	
	ulTemp += ulInt;
	
	if (flag == 1)
	{
		return -ulTemp;
	}
	return ulTemp;
}


/**********************************************************************************************************
* Function	    : StrTodouble
* Description	: 字符串转双精度浮点数
* Input Para	: 
* Output Para	: 
* Return Value: 
**********************************************************************************************************/
double StrTodouble(char *_pStr)
{
	uint8_t flag;
	char *p;
	uint32_t ulInt;
	double    ulTemp;
	double    ulDot;
	uint8_t i;
	uint8_t ucTemp;

	p = _pStr;
	if (*p == '-')
	{
		flag = 1;	
		p++;
	}
	else
	{
		flag = 0;
	}

	ulInt = 0;
	for (i = 0; i < 15; i++)   
	{
		ucTemp = *p;
		if (ucTemp == '.')	
		{
			p++;
			break;
		}
		if ((ucTemp >= '0') && (ucTemp <= '9'))
		{
			ulInt = ulInt * 10 + (ucTemp - '0');
			p++;
		}
		else
		{
			break;
		}
	}
	ulTemp = 0L;
	ulDot  = 1L;
	for (; i < 15; i++)                //è?D?êy
	{
		ucTemp = *p;
		if ((ucTemp >= '0') && (ucTemp <= '9'))
		{
			ulDot *= 0.1L;
			ulTemp +=  (ucTemp - '0') * ulDot;
			p++;
		}
		else
		{
			break;
		}
	}
	
	ulTemp += ulInt;
	
	if (flag == 1)
	{
		return -ulTemp;
	}
	return ulTemp;
}

/******************************************************************************
函数名称：DataTofloat
功能描述：四字节数据转换为float数据
输入参数：dat：四字节数据地址
输出参数：无
函数返回：float数据
******************************************************************************/
float DataTofloat(uint8 *dat)
{
	uint8 i;
	union
	{
	    char byte[4];
		float n;
	} num;
	
	for(i = 0; i < 0x04; i++ )
	{
	    num.byte[0x03 - i] = dat[i];
	}
	
	return num.n;
}

/******************************************************************************
函数名称：FloatToData
功能描述：float数据转换为四字节数据
输入参数：f：flaot数据
输出参数：dat：四字节数据地址
函数返回：无
******************************************************************************/
void FloatToData(float f,uint8 *dat)
{
	uint8 i;
	union
	{
	    char byte[4];
		float n;
	} num;
	
	num.n = f;
	
	for(i = 0; i < 0x04; i++ )
	{
	    dat[i] = num.byte[0x03 - i];
	}
}

/***********************************************************************************
** Function Name:	uint32_to_4byte
** Input Parameters:    uint32_t位 HEX码
** Output Parameters:  
** Description: 
***********************************************************************************/
void uint32_to_4byte(uint32_t s,uint8_t *num)
{
  uint8_t  i;
  uint32_t temp;
  
  temp  = s;
  for(i=0;i<4;i++)
  {
      num[3 - i] = (uint8_t)temp;
      temp >>= 8;
  }
}

/***********************************************************************************
** Function Name:	byte4_to_uint32
** Input Parameters:    uint32_t位 HEX码
** Output Parameters:  
** Description: 
***********************************************************************************/
uint32_t byte4_to_uint32(uint8_t *num)
{
  uint8_t  i;
  uint32_t temp = 0x00000000;
  
  for(i=3;i>0;i--)
  {
     temp += num[i];
     temp <<= 8;
  }
  temp += num[0];
  return temp;
}

/***********************************************************************************
** Function Name:	byte2_to_uint16
** Input Parameters:    uint32_t位 HEX码
** Output Parameters:  
** Description: 
***********************************************************************************/
uint16_t byte2_to_uint16(uint8_t *num)
{
  uint8_t  i;
  uint16_t temp = 0x00000000;
  
  for(i=2;i>0;i--)
  {
     temp += num[i];
     temp <<= 8;
  }
  temp += num[0];
  return temp;
}

/*
*********************************************************************************************************
*	函 数 名: Byte_BCDToBin
*	功能说明: 单字节BCD转Bin
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
uint8_t Byte_BCDToBin(uint8_t num)
{
    uint8_t result;
    result = num & 0x0F;	
	result += (num >> 4) * 10;
	return result;
}

/*
*********************************************************************************************************
*	函 数 名: Byte_BinToBCD
*	功能说明: 单字节Bin转BCD
*	形    参: 
*	返 回 值:
*********************************************************************************************************
*/
uint8_t Byte_BinToBCD(uint8_t num)
{
    uint8_t result;
    result = num / 10;
    result <<= 4;	
	result += (num % 10);
	return result;
}




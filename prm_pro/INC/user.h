#ifndef __SYS_H
#define __SYS_H

#include "gd32f10x_it.h"

#define     _NULL         ((void*)0)
#define		_FALSE	      0x00
#define		_TRUE	      0x01


//�������Ͷ���

typedef	long long	      uint64;
typedef	unsigned long	  uint32;
typedef	signed long	      int32;
typedef	unsigned short    uint16;
typedef	signed short      int16;
typedef	unsigned char	  uint8;
typedef	signed char	      int8;


//RTC ʱ��ƫ����
#define		_ss	    0		
#define		_mm	    1
#define		_hh		2
#define		_dd		3
#define		_mon	4
#define		_yy		5
#define		_ww     6

#define		COMM_ERROR  0xff

enum E_DETECTOR_TYPE
{
    DETECTOR_NBR,
    DETECTOR_1,
    DETECTOR_2,
};


struct tag_sys_param
{
	uint8   addr;               //
	uint8   uwb_en;			    //
    uint8   beidou_en;
    uint8   lora_en;
};

struct tag_uwb_param
{
	uint8_t SW_check_flag;  //Դǿ����
    uint8   type;           //00 gama  01 ����
    uint8   nuclide;        //��������
    uint64  strong;         //Դǿ
    float   factor;         //˥������
//	float   activity;    //Դ���?
};

struct basic_param_typedef
{
    uint8_t keyBeep;
    uint8_t backLight;
    uint16_t blTime;
};

typedef struct
{
    float doserate_cps;
    float doserate_uSv_h;
    float doserate_uGy_h;
    float dose_uSv;
    float dose_uGy;
    float factor;
    float deadtime;
    float transferFactor;
} IntDoseParam_typedef;

struct tag_chn_dat
{
    uint8_t nbr_status;

    float cps;

    float value;

    float fact;

    float back_ground;

    float sensi_value; /* ???????? */

    uint8_t unit;
};


uint8_t Byte_BCDToBin(uint8_t num);
uint8_t Byte_BinToBCD(uint8_t num);

extern IntDoseParam_typedef intDoseParam;
extern struct tag_chn_dat Chn_Data;

extern struct basic_param_typedef basicParam;
extern float warnRangeRate ;
extern float warnRangeDose ;
extern uint8_t displayUnit;
extern uint8_t brightness;
extern struct tag_sys_param sys_param;
extern struct tag_uwb_param tag_dis;       //����Դ
extern uint8 Voltage;
extern uint8_t detectorType;
extern uint8_t passWord[4];
extern float extDtDoserate;
    // ��������
    // sys_config
void PulseCntGet(void);
void setSystemTick(uint32_t ms);
//sys
extern void delay_us(uint32_t us);
extern void delay_ms(uint32_t ms);
extern void HexToAscii(uint8_t *_ucpHex, uint8_t *_ucpAscII, uint8_t _ucLenasc);
extern int32_t StrToInt(char *_pStr);
extern __int64 StrTo64Int(char *arrTmp);
extern int32_t StrToIntFix(char *_pStr, uint8_t _ucLen);
extern float StrToFloat(char *_pStr);
extern double StrTodouble(char *_pStr);
extern void FloatToData(float f,uint8 *dat);
extern float DataTofloat(uint8 *dat);
extern void uint32_to_4byte(uint32_t s,uint8_t *num);
extern uint32_t byte4_to_uint32(uint8_t *num);
extern uint16_t byte2_to_uint16(uint8_t *num);
//sys_gm

extern uint8_t Get_DATA_GETSTA(uint8_t *_ucaBuf);
extern uint8_t Set_DATA_SETADDR(uint8_t *_ucaBuf);
extern uint8_t Set_DATA_SETCONFIG(uint8_t *_ucaBuf);
extern uint8_t Lora_Send_Proess_ACKSTA(void);
extern uint8_t Lora_Send_Proess_ACKSET(uint8_t result);
extern uint8_t Lora_Send_Proess_ACKADDR(uint8_t result);
//comm
extern void AppLora(void);
//��ʾ




#define false 0
#define true 1
    
#endif // __SYS_H


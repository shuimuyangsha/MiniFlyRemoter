#ifndef __JOYSTICK_H
#define __JOYSTICK_H
#include "sys.h"
#include <stdbool.h>

/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly_Remotor
 * ҡ����������
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/

#define MAX(a,b) a>=b? a:b
#define MIN(a,b) a<=b? a:b

enum dir_e
{
	CENTER,
	FORWARD,
	BACK,
	LEFT,
	RIGHT,
	BACK_LEFT,
	BACK_RIGHT,
};

//ҡ�˵��������̲���
struct range_s
{
	s16 range_pos;	//ҡ��������
	u16 mid;
	s16 range_neg;	//ҡ�˸�����
};

//ҡ�����̲���
typedef struct
{
	struct range_s thrust;
	struct range_s roll;
	struct range_s pitch;
	struct range_s yaw;
}joystickParam_t;

//�ɿ����ݽṹ
typedef struct 
{
	float roll;
	float pitch;
	float yaw;
	float thrust;
}joystickFlyf_t;

//�ɿ����ݽṹ
typedef struct 
{
	u16 roll;
	u16 pitch;
	u16 yaw;
	u16 thrust;
}joystickFlyui16_t;


void joystickInit(void);

void getFlyDataADCValue(joystickFlyui16_t *adcValue);

void ADCtoFlyDataPercent(joystickFlyf_t *flyDataPercent);

void joystcikParamSet(joystickParam_t set);

enum dir_e getJoystick1Dir(u8 mode);

enum dir_e getJoystick2Dir(u8 mode);


#endif /*__JOYSTICK_H*/

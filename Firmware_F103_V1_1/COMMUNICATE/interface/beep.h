#ifndef __BEEP_H
#define __BEEP_H	 
#include "sys.h"
#include <stdbool.h>
/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly_Remotor
 * ��������������	
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/

#define  BEEP  PCout(14)


typedef struct 
{
	bool isActive;
	bool isLoop;
	u16 waitMS;
}beepSeq_t;


enum beepAction
{
	TRIM_BEEP = 0,
	FLIP_BEEP,
	LOWPOWER_BEEP,
	CANFLY_BEEP,
};


void beepInit(void);
void runBeepAcktion(enum beepAction runaction);
void stopBeepAcktion(enum beepAction runaction);
#endif

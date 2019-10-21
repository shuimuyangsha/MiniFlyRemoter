#ifndef __MAIN_UI_H
#define __MAIN_UI_H
#include <stdint.h>
#include <stdbool.h>
#include "atkp.h"
/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly_Remotor
 * ���������	
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/ 

void main_ui(void);
void miniFlyMsgACKProcess(atkp_t* p);
	
void setRCLock(bool set);
bool getRCLock(void);

void setTrimFlag(bool set);
bool getTrimFlag(void);

void setMFLowPower(bool set);
bool getMFLowPower(void);

void setIsMFCanFly(bool set);
bool getIsMFCanFly(void);
#endif /*__MAIN_UI_H*/

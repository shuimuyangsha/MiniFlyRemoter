#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly_Remotor
 * LED��������	
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/

#define LED_BLUE 		PBout(3)
#define LED_RED 		PBout(7)

void ledInit(void);/* LED��ʼ�� */
			    
#endif

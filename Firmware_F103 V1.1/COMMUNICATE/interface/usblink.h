#ifndef __USB_LINK_H
#define __USB_LINK_H
#include <stdint.h>
#include <stdbool.h>
#include "atkp.h"

/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly_Remotor
 * USBͨ����������	
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/


void usblinkInit(void);
bool usblinkSendPacket(const atkp_t *p);
bool usblinkSendPacketBlocking(const atkp_t *p);
bool usblinkReceivePacket(atkp_t *p);
bool usblinkReceivePacketBlocking(atkp_t *p);
void usblinkTxTask(void* param);
void usblinkRxTask(void *param);


#endif /*usblink.h*/


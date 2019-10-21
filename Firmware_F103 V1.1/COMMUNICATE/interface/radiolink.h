#ifndef __RADIO_LINK_H
#define __RADIO_LINK_H
#include <stdbool.h>
#include "sys.h"
#include "atkp.h"
/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"

/*����ָ��*/
#define  U_RADIO_RSSI		0x01
#define  U_RADIO_CONFIG		0x02	

/*����ָ��*/
#define  D_RADIO_HEARTBEAT	0xFF
#define  D_RADIO_GET_CONFIG	0x01
#define  D_RADIO_SET_CONFIG	0x02

/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly_Remotor
 * ����ͨ����������	
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/

extern xTaskHandle radiolinkTaskHandle;

void radiolinkInit(void);
bool radiolinkSendPacket(const atkp_t *p);
bool radiolinkSendPacketBlocking(const atkp_t *p);
bool radiolinkReceivePacket(atkp_t *p);
bool radiolinkReceivePacketBlocking(atkp_t *p);
void radiolinkTask(void* param);
u16 radioinkFailRxcount(void);
bool radioinkConnectStatus(void);
void radiolinkEnable(FunctionalState state);

#endif /*RADIO_LINK_H*/

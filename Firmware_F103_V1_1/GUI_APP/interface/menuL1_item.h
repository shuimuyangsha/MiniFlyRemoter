#ifndef __MENUL1_ITEM_H
#define __MENUL1_ITEM_H
#include "sys.h"
#include "gui_menu.h"
#include "remoter_ctrl.h"

/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly_Remotor
 * һ���˵���������	
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/

extern MenuItem_Typedef  mainMenu[];
extern const u8 finger_img[32];

void mainMenuInit(void);
void expModuleMenuSet(enum expModuleID id);

#endif /*__MENUL1_ITEM_H*/

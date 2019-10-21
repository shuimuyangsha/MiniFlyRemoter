#ifndef __GUI_MENU_H
#define __GUI_MENU_H
#include <stdint.h>
#include "gui_basic.h"
#include "gui_menu.h"
#include "windows.h"
#include "scrollbar.h"

/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly_Remotor
 * �˵���ʾ�������	
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/

extern WINDOWS MenuWindow;
extern Scrollbar_Typedef MenuScrollbar;

void menu_test(void);
void Menu_Run(void);//���в˵�
void MenuHookFun(uint8_t key);//���Ӻ���
void exitMenu(void);
void gotoNextMenu(void);
void gotoLastMenu(void);


#endif /*__GUI_MENU_H*/

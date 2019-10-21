/****************************************************************************
* �ļ�����LCMDRV.H
* ���ܣ�12864ͼ��Һ��ģ����������
*       ����ZLG/GUI�û�ͼ�ν��档
*  
*  ���ߣ����ܱ�
*  ���ڣ�2005/3/7
****************************************************************************/
#ifndef  __LCMDRV_H
#define  __LCMDRV_H  
#include "gui_config.h"



void  GUI_Initialize(void);
void  GUI_FillSCR(TCOLOR dat);
void  GUI_ClearSCR(void);
void  GUI_Point(uint16_t x, uint8_t y, TCOLOR color);
uint8_t  GUI_ReadPoint(uint16_t x, uint8_t y, TCOLOR *ret);
void  GUI_HLine(uint16_t x0, uint8_t y0, uint16_t x1, TCOLOR color);
void  GUI_RLine(uint16_t x0, uint8_t y0, uint8_t y1, TCOLOR color);
void  GUI_Refresh(void);




#endif

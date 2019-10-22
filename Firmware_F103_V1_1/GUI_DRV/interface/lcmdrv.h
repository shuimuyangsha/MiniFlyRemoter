/****************************************************************************
* 文件名：LCMDRV.H
* 功能：12864图形液晶模块驱动程序。
*       用于ZLG/GUI用户图形界面。
*  
*  作者：黄绍斌
*  日期：2005/3/7
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

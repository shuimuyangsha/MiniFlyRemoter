#include "gui_menu.h"
#include "gui_basic.h"
#include "text.h"
#include "oled.h"

/****************************************************************************
* ���ƣ�GUI_MenuItemDraw()
* ���ܣ���ʾ��Ŀ�˵�
* ��ڲ�����ico		ͼ��˵����
* ���ڲ���������0��ʾ����ʧ�ܣ�����1��ʾ�����ɹ�
****************************************************************************/
uint8_t  GUI_MenuItemDraw(uint32_t x, uint32_t y, MenuItem_Typedef *item)
{  
	if( x>GUI_LCM_XMAX || y>GUI_LCM_YMAX) return(0);
	if(y<2) return(0);
	if(x<18) return(0);
	if( (item->title)==NULL )  return(0);

	if(item->isSelect == false)
	{  
		if(item->icoUnselected != NULL)
			oled_showPicture(x-18, y-2, (uint8_t *)item->icoUnselected ,16, 16);
		show_str(x, y,(uint8_t *) item->title,12,12,1);
	}
	else
	{  
		if(item->icoSelected != NULL)
			oled_showPicture(x-18, y-2, (uint8_t *)item->icoSelected ,16, 16);
		show_str(x,y,(uint8_t *) item->title, 12,12,0);
	}
	return(1);
}



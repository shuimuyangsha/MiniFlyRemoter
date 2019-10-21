#include "menuL3_item.h"
#include "menuL2_item.h"
#include "menu_ui.h"
#include "windows.h"
#include "scrollbar.h"
#include "joystick.h"
#include "keyTask.h"
#include "display.h"
#include "OLED.h"
#include "config_param.h"
#include "24l01.h"
#include "menuL1_item.h"
#include "atkp.h"
#include "remoter_ctrl.h"

/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly_Remotor
 * �����˵���������	
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/

/*�˵�����*/
#define  LED_RING_MENU_NUM   10
#define  CAMERA_MENU_NUM     2

MenuItem_Typedef  ledringMenu[LED_RING_MENU_NUM];
MenuItem_Typedef  cameraMenu[CAMERA_MENU_NUM];

const u8* ledringLanguage[3][LED_RING_MENU_NUM]={
{
"�ر�",
"��ɫ����",
"��̬��Ӧ",
"���ݸ�Ӧ",
"��˸����",
"�ֵ�Ͳ",
"������",
"��ˮ��1",
"��ˮ��2",
"��ˮ��3",
},
{
"Turn off",
"Color Test",
"Attitude induce",
"Gyro induce",
"Blink LED",
"Flashlight",
"Breathing LED",
"Spin Effect1",
"Spin Effect2",
"Spin Effect3",
},
{
"�P�]",
"�ɫ�yԇ",
"�ˑB�Б�",
"���ݸБ�",
"�W�q����",
"���Ͳ",
"������",
"��ˮ��1",
"��ˮ��2",
"��ˮ��3",
},
};
const u8* cameraLanguage[3][CAMERA_MENU_NUM]={
{
"����",
"�ر�",
},
{
"Turn on",
"Turn off",
},
{
"�_��",
"�P�]",
}	
};

static void setLedringEffect0(void)
{
	exitMenu();
	sendRmotorCmd(CMD_LEDRING_EFFECT, 0);
}

static void setLedringEffect1(void)
{
	exitMenu();
	sendRmotorCmd(CMD_LEDRING_EFFECT, 1);
}

static void setLedringEffect2(void)
{
	exitMenu();
	sendRmotorCmd(CMD_LEDRING_EFFECT, 2);
}

static void setLedringEffect3(void)
{
	exitMenu();
	sendRmotorCmd(CMD_LEDRING_EFFECT, 3);
}

static void setLedringEffect4(void)
{
	exitMenu();
	sendRmotorCmd(CMD_LEDRING_EFFECT, 4);
}

static void setLedringEffect5(void)
{
	exitMenu();
	sendRmotorCmd(CMD_LEDRING_EFFECT, 5);
}

static void setLedringEffect6(void)
{
	exitMenu();
	sendRmotorCmd(CMD_LEDRING_EFFECT, 6);
}


static void setLedringEffect7(void)
{
	exitMenu();
	sendRmotorCmd(CMD_LEDRING_EFFECT, 7);
}

static void setLedringEffect8(void)
{
	exitMenu();
	sendRmotorCmd(CMD_LEDRING_EFFECT, 8);
}

static void setLedringEffect9(void)
{
	exitMenu();
	sendRmotorCmd(CMD_LEDRING_EFFECT, 9);
}

/*��ʼ��ledring�˵�*/
void ledringMenuInit(void)
{	
	for(int i=0; i<LED_RING_MENU_NUM; i++)
	{
		ledringMenu[i].menuItemCount = LED_RING_MENU_NUM;
		ledringMenu[i].isSelect = false;
		ledringMenu[i].icoSelected = radio_selected_img;
		ledringMenu[i].icoUnselected = radio_unselected_img;
		ledringMenu[i].title = ledringLanguage[configParam.language][i];
		ledringMenu[i].parentMenu = &expModuleMenu[1];
		ledringMenu[i].childrenMenu = NULL;
	}
	
	ledringMenu[0].isSelect = true;
	ledringMenu[0].Function = setLedringEffect0;
	ledringMenu[1].Function = setLedringEffect1;
	ledringMenu[2].Function = setLedringEffect2;
	ledringMenu[3].Function = setLedringEffect3;
	ledringMenu[4].Function = setLedringEffect4;
	ledringMenu[5].Function = setLedringEffect5;
	ledringMenu[6].Function = setLedringEffect6;
	ledringMenu[7].Function = setLedringEffect7;
	ledringMenu[8].Function = setLedringEffect8;
	ledringMenu[9].Function = setLedringEffect9;
}

static void turnOnExpModule(void)
{
	exitMenu();
	sendRmotorCmd(CMD_POWER_MODULE, 1);
}

static void turnOffExpModule(void)
{
	exitMenu();
	sendRmotorCmd(CMD_POWER_MODULE, 0);
}
/*��ʼ����ͷ���ò˵�*/
void cameraMenuInit(void)
{	
	for(int i=0; i<CAMERA_MENU_NUM; i++)
	{
		cameraMenu[i].menuItemCount = CAMERA_MENU_NUM;
		cameraMenu[i].isSelect = false;
		cameraMenu[i].icoSelected = radio_selected_img;
		cameraMenu[i].icoUnselected = radio_unselected_img;
		cameraMenu[i].title = cameraLanguage[configParam.language][i];
		cameraMenu[i].Function = NULL;
		cameraMenu[i].parentMenu = &expModuleMenu[2];
		cameraMenu[i].childrenMenu = NULL;
	}
	
	cameraMenu[0].isSelect = true;
	cameraMenu[0].Function = turnOnExpModule;
	cameraMenu[1].Function = turnOffExpModule;
}


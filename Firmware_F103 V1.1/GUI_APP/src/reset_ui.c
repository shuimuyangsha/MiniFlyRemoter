#include "reset_ui.h"
#include "windows.h"
#include "scrollbar.h"
#include "joystick.h"
#include "keyTask.h"
#include "display.h"
#include "messagebox.h"
#include "config_param.h"

/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly_Remotor
 * ���öԻ���������	
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/

static bool isInit;

//���öԻ���
MessageBox_Typedef ResetSystemMessageBox={
0,
0,	
128,
64,
"Reset",	
"System will be reset?",
"OK",
"Cancel"
};

const u8 *resetSystemLanguage[4][3]={
{
"����",
"Reset",
"����",
},
{
"ȷ���ָ�ΪĬ�����ã�",
"Restore the default Settings?",
"�_���֏͞�Ĭ�J�O�ã�",
},
{
"ȷ��",
"OK",
"�_��",
},
{
"ȡ��",
"Cancel",
"ȡ��",
}
};

/*��ʾ���öԻ���*/
static void msgboxDispalyInit(void)
{
	if(isInit) return;
	
	ResetSystemMessageBox.title = (u8*)resetSystemLanguage[0][configParam.language];
	ResetSystemMessageBox.msg = (u8*)resetSystemLanguage[1][configParam.language];
	ResetSystemMessageBox.ok = (u8*)resetSystemLanguage[2][configParam.language];
	ResetSystemMessageBox.cancel = (u8*)resetSystemLanguage[3][configParam.language];
	GUI_MessageBoxDraw(&ResetSystemMessageBox);
	
	isInit = true;
}

/*���ý���*/
void reset_ui(void)
{
	enum dir_e joystick2;
	static int timeout;
	
	msgboxDispalyInit();
	joystick2 = getJoystick2Dir(0);
	if(joystick2 != CENTER)
	{
		timeout = 0;
	}
	switch(joystick2)
	{
		case RIGHT:  //ROLL����
			GUI_MessageBoxButtonStatus(&ResetSystemMessageBox, 0);
			break;
			
		case LEFT:  //ROLL����
			GUI_MessageBoxButtonStatus(&ResetSystemMessageBox, 1);
			break;
		default :break;
	}
	
	u8 keyState = getKeyState();
	if(keyState == KEY_J2_SHORT_PRESS)//����ҡ�˼�ִ�в˵���Ӧ�Ķ���
	{	
		if(GUI_MessageBoxResult(&ResetSystemMessageBox) == 1)
		{
			configParamReset();
			INTX_DISABLE();/*��ֹȫ���ж�*/
			NVIC_SystemReset();	/*����*/
		}
		isInit = false;
		setShow_ui(MAIN_UI);
	}
	if(timeout++ > 100)//��ʱ�˳��˵�
	{
		timeout = 0;
		isInit = false;
		setShow_ui(MAIN_UI);
	}
}

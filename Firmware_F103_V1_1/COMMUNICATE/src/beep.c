#include "beep.h"
/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly_Remotor
 * ��������������	
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/


/*��������Ӧ��������ʱʱ�䣨ms��*/
#define  TRIM			100
#define  FLIP			50 
#define  LOWPOWER		500
#define  CANFLY			100

beepSeq_t beepAction[] =
{
	{false, true, TRIM},
	{false, true, FLIP},
	{false, true, LOWPOWER},
	{false, false, CANFLY},
};

#define  ACTION_NUM	(sizeof(beepAction)/sizeof(beepSeq_t))//��������


static bool isInit;
static bool beepEnable;
static TimerHandle_t beepTimer;
enum beepAction currentRun;
	
/* ������IO�ڳ�ʼ�� */
static void beepIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
 	/* ��ʼ��������(PC14) */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_14);
}

static void runBeepseq(xTimerHandle xTimer)
{
	u8 i;
	if(beepAction[currentRun].isLoop == false)
	{
		beepAction[currentRun].isActive = false;
	}	
	for(i=0; i<ACTION_NUM; i++)
	{
		if(beepAction[i].isActive == true) 
		{
			xTimerChangePeriod(beepTimer, beepAction[i].waitMS, 0);
			break;
		}
	}
	if(i == ACTION_NUM)/*��ǰ�޶�������*/
	{
		beepEnable = false;
	}
	if(beepEnable)
		BEEP = !BEEP;
	else
		BEEP = 0;
}

void beepInit(void)
{
	if(isInit) return;
	beepIO_Init();
	beepTimer = xTimerCreate("beepTimer", 1000, pdTRUE,
						0, runBeepseq);
	isInit = true;
}

void runBeepAcktion(enum beepAction action)
{
	currentRun = action;
	beepAction[action].isActive = true;
	xTimerChangePeriod(beepTimer, beepAction[action].waitMS, portMAX_DELAY);
	BEEP = 1;
	beepEnable = true;
}

void stopBeepAcktion(enum beepAction action)
{
	u8 i;
	beepAction[action].isActive = false;
	for(i=0; i<ACTION_NUM; i++)
	{
		if(beepAction[i].isActive == true)
		{
			currentRun = (enum beepAction)i;
			xTimerChangePeriod(beepTimer, beepAction[currentRun].waitMS, portMAX_DELAY);
			return;
		}	
	}
	beepEnable = false;
}


#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "oled.h"
#include "24l01.h"
#include "adc.h"
#include "hw_config.h"
#include "config_param.h"
#include "beep.h"
#include "radiolink.h"
#include "usblink.h"
#include "remoter_ctrl.h"
#include "atkp.h"
#include "display.h"
#include "keyTask.h"
/* FreeRtos includes */
#include "FreeRTOS.h"
#include "task.h"

/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly_Remotor
 * main.c	
 * ����ϵͳ��ʼ���ʹ�������
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/

static TaskHandle_t startTaskHandle;
static void startTask(void *param);

int main(void)
{	
	NVIC_SetVectorTable(FIRMWARE_START_ADDR,0);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);/*�ж����ó�ʼ��*/
	delay_init(); 		/*delay��ʼ��*/
	configParamInit();	/*���ò�����ʼ��*/
	ledInit();			/*led��ʼ��*/
	oledInit(); 		/*oled��ʼ��*/
	beepInit();			/*��������ʼ��*/
	keyInit();			/*������ʼ��*/
	joystickInit();		/*ҡ�˳�ʼ��*/
	usb_vcp_init();		/*usb���⴮�ڳ�ʼ��*/
	
	radiolinkInit();	/*����ͨ�ų�ʼ��*/
	usblinkInit();		/*usbͨ�ų�ʼ��*/
	displayInit();		/*��ʾ��ʼ��*/
	
	xTaskCreate(startTask, "START_TASK", 100, NULL, 2, &startTaskHandle);/*������ʼ����*/
	vTaskStartScheduler();/*�����������*/
	
	while(1){};/* ������Ⱥ󲻻�ִ�е��� */
}

/*��������*/
void startTask(void *param)
{
	taskENTER_CRITICAL();	/*�����ٽ���*/
	
	xTaskCreate(radiolinkTask, "RADIOLINK", 100, NULL, 6, &radiolinkTaskHandle);/*����������������*/
	
	xTaskCreate(usblinkTxTask, "USBLINK_TX", 100, NULL, 5, NULL);	/*����usb��������*/
	xTaskCreate(usblinkRxTask, "USBLINK_RX", 100, NULL, 5, NULL);	/*����usb��������*/
	
	xTaskCreate(commanderTask, "COMMANDER", 100, NULL, 4, NULL);	/*�����ɿ�ָ�������*/
	
	xTaskCreate(keyTask, "BUTTON_SCAN", 100, NULL, 3, NULL);		/*��������ɨ������*/
	
	xTaskCreate(displayTask, "DISPLAY", 200, NULL, 1, NULL);		/*������ʾ����*/
	
	xTaskCreate(configParamTask, "CONFIG_TASK", 100, NULL, 1, NULL);/*����������������*/
	
	xTaskCreate(radiolinkDataProcessTask, "DATA_PROCESS", 100, NULL, 6, NULL);	/*��������ͨ�����ݴ�������*/
	xTaskCreate(usblinkDataProcessTask, "DATA_PROCESS", 100, NULL, 6, NULL);	/*����USBͨ�����ݴ�������*/
	
	vTaskDelete(startTaskHandle);									/*ɾ����ʼ����*/
	
	taskEXIT_CRITICAL();	/*�˳��ٽ���*/
}


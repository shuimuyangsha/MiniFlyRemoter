#ifndef __CONFIG_PARAM_H
#define __CONFIG_PARAM_H
#include <stdbool.h>
#include "sys.h"
#include "24l01.h"
#include "remoter_ctrl.h"
#include "joystick.h"

/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly
 * ���ò�����������	
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/

#define BOOTLOADER_SIZE		(9*1024)		/*9K bootloader*/
#define CONFIG_PARAM_SIZE	(127*1024)		/*128K���ڱ������*/

#define CONFIG_PARAM_ADDR 	(FLASH_BASE + CONFIG_PARAM_SIZE)/*���ò��������ַ*/	
#define FIRMWARE_START_ADDR (FLASH_BASE + BOOTLOADER_SIZE)

/* Ĭ�����ò��� */
#define  VERSION	11		/*��ʾ�汾ΪV1.1*/
#define  DISPLAY_LANGUAGE	SIMPLE_CHINESE

#define  RADIO_CHANNEL 		2
#define  RADIO_DATARATE 	DATA_RATE_250K
#define  RADIO_ADDRESS 		0x123456789AULL

#define  FLIGHT_CTRL_MODE	ALTHOLD_MODE
#define  FLIGHT_MODE		HEAD_LESS
#define  FLIGHT_SPEED		LOW_SPEED
#define  FLIP_SET			FLIP_DISABLE


enum ctrlMode
{
	ALTHOLD_MODE,
	MANUAL_MODE,
};

enum flightMode
{
	HEAD_LESS,
	X_MODE,
};

enum flightSpeed
{
	LOW_SPEED,
	MID_SPEED,
	HIGH_SPEED,
};

enum flipEnable
{
	FLIP_ENABLE,
	FLIP_DISABLE,
};

enum language
{
	SIMPLE_CHINESE,
	ENGLISH,
	COMPLEX_CHINESE,
};

/*�������ýṹ*/
typedef struct{
	enum ctrlMode ctrl;
	enum flightMode mode;
	enum flightSpeed speed;
	enum flipEnable flip;
}flightConfig_t;

/*�������ýṹ*/
typedef struct{
	u8 channel;		
	enum nrfRate dataRate;
	u32 addressHigh;/*ͨ�ŵ�ַ��4�ֽ�*/
	u32 addressLow;	/*ͨ�ŵ�ַ��4�ֽ�*/
}radioConfig_t;

/*����΢���ṹ*/
typedef __packed struct{
	float pitch;
	float roll;
}trim_t;

/*��������ṹ*/
typedef struct{
	u8 version;				/*����汾��*/
	enum language language;	/*��ʾ����*/
	radioConfig_t radio;	/*�������ò���*/
	flightConfig_t flight;	/*�������ò���*/
	joystickParam_t jsParam;/*ҡ��У׼����*/
	trim_t trim;			/*��̬΢������*/
	u8 cksum;				/*У��*/
} configParam_t;


extern configParam_t configParam;


void configParamInit(void);
void configParamTask(void* param);
void writeConfigParamToFlash(void);
void configParamReset(void);

#endif


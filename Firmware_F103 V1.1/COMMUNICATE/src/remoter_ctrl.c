#include <string.h>
#include "remoter_ctrl.h"
#include "joystick.h"
#include "atkp.h"
#include "radiolink.h"
#include "main_ui.h"
#include "trim_ui.h"
#include "config_param.h"
/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"

/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly_Remotor
 * �ɿ�ָ����������	
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/

#define  LOW_SPEED_THRUST   (75.0)
#define  LOW_SPEED_PITCH    (10.0)
#define  LOW_SPEED_ROLL     (10.0)

#define  MID_SPEED_THRUST   (85.0)
#define  MID_SPEED_PITCH    (18.0)
#define  MID_SPEED_ROLL     (18.0)

#define  HIGH_SPEED_THRUST  (95.0)
#define  HIGH_SPEED_PITCH   (30.0)
#define  HIGH_SPEED_ROLL    (30.0)

#define  MIN_THRUST			(25.0)
#define  ALT_THRUST		    (50.0)
#define  MAX_YAW			(200.0)

static joystickFlyf_t flydata;


/*����ң������*/
void sendRmotorCmd(u8 cmd, u8 data)
{
	if(radioinkConnectStatus() == false)
		return;
	atkp_t p;
	p.msgID = DOWN_REMOTOR;
	p.dataLen = 3;
	p.data[0] = REMOTOR_CMD;
	p.data[1] = cmd;
	p.data[2] = data;
	radiolinkSendPacketBlocking(&p);
}
/*����ң�ؿ�������*/
void sendRmotorData(u8 *data, u8 len)
{
	if(radioinkConnectStatus() == false)
		return;
	atkp_t p;
	p.msgID = DOWN_REMOTOR;
	p.dataLen = len + 1; 
	p.data[0] = REMOTOR_DATA;
	memcpy(p.data+1, data, len);
	radiolinkSendPacket(&p);
}

float limit(float value,float min, float max)
{
	if(value > max)
	{
		value = max;
	}
	else if(value < min)
	{
		value = min;
	}
	return value;
}

/*���ͷɿ���������*/
void commanderTask(void* param)
{
	float max_thrust = LOW_SPEED_THRUST;
	float max_pitch = LOW_SPEED_PITCH;
	float max_roll = LOW_SPEED_ROLL;
	joystickFlyf_t  percent;
	
	while(1)
	{
		vTaskDelay(10);
		switch(configParam.flight.speed)
		{
			case LOW_SPEED:
				max_thrust = LOW_SPEED_THRUST;
				max_pitch = LOW_SPEED_PITCH;
				max_roll = LOW_SPEED_ROLL;
				break;
			case MID_SPEED:
				max_thrust = MID_SPEED_THRUST;
				max_pitch = MID_SPEED_PITCH;
				max_roll = MID_SPEED_ROLL;
				break;
			case HIGH_SPEED:
				max_thrust = HIGH_SPEED_THRUST;
				max_pitch = HIGH_SPEED_PITCH;
				max_roll = HIGH_SPEED_ROLL;
				break;
		}
		
		ADCtoFlyDataPercent(&percent);
		
		//THRUST
		if(configParam.flight.ctrl == ALTHOLD_MODE)/*����ģʽ*/
		{
			flydata.thrust = percent.thrust * ALT_THRUST;
			flydata.thrust += ALT_THRUST;
			flydata.thrust = limit(flydata.thrust, 0, 100);
		}
		else
		{
			flydata.thrust = percent.thrust * (max_thrust - MIN_THRUST);
			flydata.thrust += MIN_THRUST;
			flydata.thrust = limit(flydata.thrust, MIN_THRUST, max_thrust);
		}
		//ROLL
		flydata.roll = percent.roll * max_roll;
		flydata.roll = limit(flydata.roll, -max_roll, max_roll);
		//PITCH
		flydata.pitch = percent.pitch * max_pitch;
		flydata.pitch = limit(flydata.pitch, -max_pitch, max_pitch);
		//YAW
		flydata.yaw = percent.yaw * MAX_YAW;
		flydata.yaw = limit(flydata.yaw, -MAX_YAW, MAX_YAW);
		
		/*���ͷɿ�����*/
		if(getRCLock()==false && radioinkConnectStatus()==true)
		{	
			remoterData_t send;
			switch(configParam.flight.mode)
			{
				case HEAD_LESS:
					send.flightMode = 1;
					break;
				case X_MODE:
					send.flightMode = 0;
					break;
			}
			
			switch(configParam.flight.ctrl)
			{
				case ALTHOLD_MODE:
					send.ctrlMode = 1;
					break;
				case MANUAL_MODE:
					send.ctrlMode = 0;
					break;
			}
			
			if(flydata.thrust<=MIN_THRUST && send.ctrlMode==0)
			{
				send.thrust = 0;
			}
			else
			{
				send.thrust = flydata.thrust;
			}
			
			if(getTrimFlag() == true)
			{
				send.pitch = 0;
				send.roll = 0;
			}
			else
			{
				send.pitch = flydata.pitch ;
				send.roll = flydata.roll;
			}
			send.yaw = flydata.yaw;
			send.trimPitch = configParam.trim.pitch;
			send.trimRoll = configParam.trim.roll;
			
			/*���ͷɿ�����*/
			sendRmotorData((u8*)&send, sizeof(send));
		}
		
		/*����ң��������������λ��*/
		if(radioinkConnectStatus()==true)
		{
			atkp_t p;
			joystickFlyui16_t rcdata;
			
			rcdata.thrust = flydata.thrust*10 + 1000;
			rcdata.pitch = percent.pitch*500 + 1500;
			rcdata.roll = percent.roll*500 + 1500;
			rcdata.yaw = percent.yaw*500 + 1500;
			
			p.msgID = DOWN_RCDATA;
			p.dataLen = sizeof(rcdata);
			memcpy(p.data, &rcdata, p.dataLen);
			radiolinkSendPacket(&p);
		}
	}
}

/*��ȡ�ɿ�����*/
joystickFlyf_t getFlyControlData(void)
{
	return flydata;
}


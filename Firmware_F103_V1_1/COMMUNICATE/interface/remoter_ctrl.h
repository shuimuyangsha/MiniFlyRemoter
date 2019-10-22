#ifndef __COMMANDER_H
#define __COMMANDER_H
#include <stdbool.h>
#include "atkp.h"
#include "joystick.h"
/********************************************************************************	 
 * 本程序只供学习使用，未经作者许可，不得用于其它任何用途
 * ALIENTEK MiniFly_Remotor
 * 飞控指令驱动代码	
 * 正点原子@ALIENTEK
 * 技术论坛:www.openedv.com
 * 创建日期:2017/5/2
 * 版本：V1.0
 * 版权所有，盗版必究。
 * Copyright(C) 广州市星翼电子科技有限公司 2014-2024
 * All rights reserved
********************************************************************************/

/*遥控数据类别*/
typedef enum 
{
	REMOTOR_CMD,
	REMOTOR_DATA,
}remoterType_e;

/*下行命令*/
#define  CMD_GET_MSG		0x01	/*获取四轴信息（自检）*/
#define  CMD_GET_CANFLY		0x02	/*获取四轴是否能飞*/
#define  CMD_FLIGHT_LAND	0x03	/*起飞、降落*/
#define  CMD_EMER_STOP		0x04	/*紧急停机*/
#define  CMD_FLIP			0x05	/*4D翻滚*/
#define  CMD_POWER_MODULE	0x06	/*打开关闭扩展模块电源*/
#define  CMD_LEDRING_EFFECT	0x07	/*设置RGB灯环效果*/

/*上行报告*/
#define  ACK_MSG			0x01	/*四轴返回的信息*/

/*扩展模块类型枚举*/
enum expModuleID
{
	NO_MODULE,
	LED_RING,
	CAMERA,
	MODULE1,
	MODULE2,
};

/*遥控数据结构*/
typedef __packed struct
{
	float roll;      
	float pitch;  
	float yaw;      
	float thrust;
	float trimPitch;
	float trimRoll;
	bool ctrlMode;
	bool flightMode;
	bool RCLock;
} remoterData_t;

typedef __packed struct
{
	u8 version;
	bool mpu_selfTest;
	bool baro_slfTest;
	bool isCanFly;
	bool isLowpower;
	enum expModuleID moduleID;
} MiniFlyMsg_t;

void commanderTask(void* param);
joystickFlyf_t getFlyControlData(void);
float limit(float value,float min, float max);
void sendRmotorCmd(u8 cmd, u8 data);
void sendRmotorData(u8 *data, u8 len);

#endif /*__COMMANDER_H*/

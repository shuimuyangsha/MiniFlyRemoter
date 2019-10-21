#ifndef  __PROTOCOL_H
#define  __PROTOCOL_H
#include <stdint.h>

/********************************************************************************	 
 * 本程序只供学习使用，未经作者许可，不得用于其它任何用途
 * ALIENTEK MiniFly
 * 传输协议	
 * 正点原子@ALIENTEK
 * 技术论坛:www.openedv.com
 * 创建日期:2017/5/2
 * 版本：V1.0
 * 版权所有，盗版必究。
 * Copyright(C) 广州市星翼电子科技有限公司 2014-2024
 * All rights reserved
********************************************************************************/

//传输帧对象
typedef struct
{
	//最大帧长度
	#define MAX_FRAME_LENGTH (256+6)
	//最小帧长度
	#define MIN_FRAME_LENGTH  5	
	//设备地址
	uint8_t Device_Address;
	//帧功能
	uint8_t Function_Type;
	//帧序列
	uint8_t Sequence;
	//有效数据长度
	uint8_t Data_Length;
	//数据
	uint8_t *Data;
	//校验值
	uint16_t Checksum;

}TransportProtocol_Typedef;

//传输结果
typedef enum
{
	//帧格式错误
	FRAME_FORMAT_ERR = 1,		
	//校验值格式错误
	CHECK_FORMAR_ERR = 2,
	//校验值错位
	CHECK_ERR = 3,
	//解包成功
	UPACKED_SUCCESS = 4

}TransportProtocol_Result;

//协议管理器
typedef struct
{	
	//传输帧
	TransportProtocol_Typedef * TransportProtocol;
	//接收的字节数
	uint32_t  RecieveByteCount;

	//传输帧缓存
	uint8_t* Buf;
	//帧总长度
	uint16_t FrameTotalLength;
	//解包函数
	TransportProtocol_Result (*Unpacked)(void);
	//打包函数
	void (*Packed)(void);
	//校验函数
	uint16_t (*Check)(uint8_t *,uint16_t len);

}TransportProtocol_Manager_Typedef;

//外部声明协议管理器
extern TransportProtocol_Manager_Typedef TransportProtocol_Manager;
//初始化传输协议
void  TransportProtocol_Init(TransportProtocol_Typedef *TransportProtocol,uint8_t *buf,uint16_t (*check)(uint8_t *,uint16_t len));

#endif



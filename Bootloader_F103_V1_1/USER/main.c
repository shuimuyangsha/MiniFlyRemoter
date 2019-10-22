#include <string.h>
#include "sys.h"
#include "delay.h"
#include "config.h"
#include "led.h"
#include "iap.h"
#include "protocol.h"
#include "check.h"
#include "stmflash.h"
#include "hw_config.h"
#include "key.h"
/********************************************************************************	 
 * 本程序只供学习使用，未经作者许可，不得用于其它任何用途
 * ALIENTEK MiniFly
 * main.c	
 * 正点原子@ALIENTEK
 * 技术论坛:www.openedv.com
 * 创建日期:2017/5/2
 * 版本：V1.0
 * 版权所有，盗版必究。
 * Copyright(C) 广州市星翼电子科技有限公司 2014-2024
 * All rights reserved
********************************************************************************/

extern _usb_usart_fifo uu_rxfifo;

const u8 HexTable[] ={
'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'
};


//设备信息缓存
u8 DeviceInfoBuffer[13];

#define DEVICE_INFO_BUFFER_SIZE   (sizeof(DeviceInfoBuffer)/sizeof(u8)) 

//转换字符串的buffer
u8 RecieveBuf[10];

#define IAP_BUFFER_SIZE 2048
//iap 缓存
u8 ipap_buf[IAP_BUFFER_SIZE];

//定义一帧协议
TransportProtocol_Typedef TransportProtocol;


//判断进行固件升级还是跳转APP
void isUpgradeFirmware(void)
{
	if(READ_KEY_L() == 0)
	{
		delay_ms(1500);
		if(READ_KEY_L() != 0)
		{
			iap_load_app(FIRMWARE_START_ADDR);	
		}
	}
	else
	{
		iap_load_app(FIRMWARE_START_ADDR);
	}
	LED_BLUE = 0;
}

//得到设备信息
void GetDeviceInfo(void)
{
	u8 i=0;
	u16 buf = 1;
	
	STMFLASH_Read(CONFIG_PARAM_ADDR, &buf, 1);
	DeviceInfoBuffer[0] = (u8)buf;  //软件版本 11表示V1.1
	for(i=1;i<=12;i++)
		DeviceInfoBuffer[i]=*(vu32*)(0x1FFFF7E8+i); //软件序列号
}


//一个字节转换为16进制字符形式的字符串
void  OneByteToStr(u8 byte,u8* str)
{
	*str = HexTable[byte/16];
	str++;
	*str = HexTable[byte%16];	
	str++;
	*str = 0;
}

//多个字节转化为16进制字符形式的字符串并以空格隔开
void MultiByteToStr(u8 *byteBuf,u16 len,u8 *str)
{
	while(len--)
	{
		*str = HexTable[(*byteBuf)/16];
		str++;
		*str = HexTable[(*byteBuf)%16];
		str++;
		*str = ' ';
		str++;
		byteBuf++;
	}
	*str = 0;
}

//extern void usbIapResponse(uint8_t* buf, uint32_t len);

//用来响应上位机
void IAP_Response()
{
	TransportProtocol.Device_Address = 0x01;	//设备地址
	TransportProtocol.Sequence = TransportProtocol.Sequence;	//帧序列 和收到的一致，这里不改变
	TransportProtocol_Manager.Packed();			//打包	

	usbsendData(TransportProtocol_Manager.Buf, TransportProtocol_Manager.FrameTotalLength);	
}

int main()
{
	u16 t = 0;
	u32 timeOut = 0;
	u32  revcnt = 0;
	u16 oldcount = 0;
	u8 IsTransportOK = 0;   //标记代码是否传输完毕
	TransportProtocol_Result res;	//传输结果
	u32 Flash_App_Pos = FIRMWARE_START_ADDR;//用来记录代码更新的地址
	
	ledInit();
	keyInit();
	delay_init();
	isUpgradeFirmware(); 	/*判断是否要固件升级*/
	usb_vcp_init();
	
	//初始化传输协议  指向串口接收缓冲区  选择sum的校验方式
	TransportProtocol_Init(&TransportProtocol, uu_rxfifo.buffer, Checksum_Sum);
	
	while(1) 
	{	
		if(uu_rxfifo.writeptr)
		{
			if(oldcount == uu_rxfifo.writeptr)//新周期内,没有收到任何数据,认为本次数据接收完成.
			{
				//获取接收的总字节数
				TransportProtocol_Manager.RecieveByteCount = uu_rxfifo.writeptr;
				//解包
				res = TransportProtocol_Manager.Unpacked();
				if(res!=UPACKED_SUCCESS)  //解包失败的时候，不响应上位机，上位机会自动重发
				{	
					OneByteToStr(res,RecieveBuf);
				}
				else  //解包成功  每接收到2K字节则写入一次FLASH 所以上位机最大有效数据长度最好和2K成倍数关系
				{	
					if(TransportProtocol.Function_Type==0x01)  //该帧为发送的文件数据
					{	
						if(TransportProtocol.Data_Length==0)
						{	
							IsTransportOK = 1;   //代码传输完毕

							iap_write_appbin(Flash_App_Pos,ipap_buf,revcnt);//更新FLASH代码 
							revcnt = 0;

							Flash_App_Pos=FIRMWARE_START_ADDR;  //恢复到原来的起始地址
						}else
						{
							if(IsTransportOK==0)  //可以将代码写入falsh
							{
								memcpy(ipap_buf+revcnt,TransportProtocol.Data,TransportProtocol.Data_Length);
								revcnt += TransportProtocol.Data_Length;
								if(revcnt>=IAP_BUFFER_SIZE)
								{	
									revcnt =0;
									iap_write_appbin(Flash_App_Pos,ipap_buf,IAP_BUFFER_SIZE);//更新FLASH代码   
									Flash_App_Pos += IAP_BUFFER_SIZE;
								}
							}
						}	
						TransportProtocol.Data_Length = 0;            //有效数据大小
						TransportProtocol.Data = 0;			     	      //要发送的数据       
						TransportProtocol.Function_Type = 0x01;				//帧功能			

					}else if(TransportProtocol.Function_Type==0x05) //该帧为查询设备信息的功能
					{	
						GetDeviceInfo();
						TransportProtocol.Data_Length = DEVICE_INFO_BUFFER_SIZE;       //有效数据大小
						TransportProtocol.Data = (u8*)DeviceInfoBuffer;	            //要发送的设备信息   
						TransportProtocol.Function_Type = 0x05;				             //帧功能			           
					}	
					IAP_Response();  //响应上位机						
				}	

				uu_rxfifo.writeptr=0;								
			}else 
			{
				oldcount = uu_rxfifo.writeptr;	
			}
			timeOut = 0;	//超时清零 
			
		}else
		{
			delay_ms(60);
			if(IsTransportOK == 1 || timeOut++ > 1000)	/*60S 超时退出bootloader*/
			{
				timeOut = 0;
				iap_load_app(FIRMWARE_START_ADDR);
			}			
			if(++t >= 10)
			{
				t = 0;
				LED_BLUE = !LED_BLUE;
			}							
		}		
	}
}

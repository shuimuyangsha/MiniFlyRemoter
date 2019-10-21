#include "hw_config.h"
#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_istr.h"
#include "usb_pwr.h" 
#include "usart.h"  
#include "string.h"	
#include "stdarg.h"		 
#include "delay.h"
//#include "stdio.h"

/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������V3
//USB-hw_config ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/28
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
 
_usb_usart_fifo uu_txfifo;					//USB���ڷ���FIFO�ṹ��
_usb_usart_fifo uu_rxfifo;					//USB���ڷ���FIFO�ṹ��

u8  USART_PRINTF_Buffer[USB_USART_REC_LEN];	//usb_printf���ͻ�����

extern LINE_CODING linecoding;				//USB���⴮��������Ϣ
static xQueueHandle usbDataDelivery;		/*USB���⴮�����ݽ��ն���*/

/////////////////////////////////////////////////////////////////////////////////
//��USB����ͨ�ò��ִ���,ST����USB����,�˲��ִ��붼���Թ���.
//�˲��ִ���һ�㲻��Ҫ�޸�!

//USB�����жϷ�����
void USBWakeUp_IRQHandler(void) 
{
	EXTI_ClearITPendingBit(EXTI_Line18);//���USB�����жϹ���λ
} 

//USB�жϴ�����
void USB_LP_CAN1_RX0_IRQHandler(void) 
{
	USB_Istr();
} 

//USBʱ�����ú���,USBclk=48Mhz@HCLK=72Mhz
void Set_USBClock(void)
{
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);//USBclk=PLLclk/1.5=48Mhz	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);	 //USBʱ��ʹ��		 
} 

//USB����͹���ģʽ
//��USB����suspendģʽʱ,MCU����͹���ģʽ
//��������ӵ͹��Ĵ���(�����ʱ�ӵ�)
void Enter_LowPowerMode(void)
{
 	//printf("usb enter low power mode\r\n");
	bDeviceState=SUSPENDED;
} 

//USB�˳��͹���ģʽ
//�û��������������ش���(������������ʱ�ӵ�)
void Leave_LowPowerMode(void)
{
	DEVICE_INFO *pInfo=&Device_Info;
	//printf("leave low power mode\r\n"); 
	if (pInfo->Current_Configuration!=0)bDeviceState=CONFIGURED; 
	else bDeviceState = ATTACHED; 
} 

//USB�ж�����
void USB_Interrupts_Config(void)
{ 
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

 
	/* Configure the EXTI line 18 connected internally to the USB IP */
	EXTI_ClearITPendingBit(EXTI_Line18);
											  //  ������18�ϵ��ж�
	EXTI_InitStructure.EXTI_Line = EXTI_Line18; // USB resume from suspend mode
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//line 18���¼��������ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 	 

	/* Enable the USB interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	//��2�����ȼ���֮ 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the USB Wake-up interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USBWakeUp_IRQn;   //��2�����ȼ����	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_Init(&NVIC_InitStructure);   
}	

//USB�ӿ�����(����1.5K��������,ս��V3����Ҫ����,������)
//NewState:DISABLE,������
//         ENABLE,����
void USB_Cable_Config (FunctionalState NewState)
{ 
//	if (NewState!=DISABLE
//		printf("usb pull up enable\r\n"); 
//	else 
//		printf("usb pull up disable\r\n"); 
}

//USBʹ������/����
//enable:0,�Ͽ�
//       1,��������	   
void USB_Port_Set(u8 enable)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);    //ʹ��PORTAʱ��		 
	if(enable)_SetCNTR(_GetCNTR()&(~(1<<1)));//�˳��ϵ�ģʽ
	else
	{	  
		_SetCNTR(_GetCNTR()|(1<<1));  // �ϵ�ģʽ
		GPIOA->CRH&=0XFFF00FFF;
		GPIOA->CRH|=0X00033000;
		PAout(12)=0;	    		  
	}
}  

//��ȡSTM32��ΨһID
//����USB������Ϣ
void Get_SerialNum(void)
{
	u32 Device_Serial0, Device_Serial1, Device_Serial2;
	Device_Serial0 = *(u32*)(0x1FFFF7E8);
	Device_Serial1 = *(u32*)(0x1FFFF7EC);
	Device_Serial2 = *(u32*)(0x1FFFF7F0);
	Device_Serial0 += Device_Serial2;
	if (Device_Serial0 != 0)
	{
		IntToUnicode (Device_Serial0,&Virtual_Com_Port_StringSerial[2] , 8);
		IntToUnicode (Device_Serial1,&Virtual_Com_Port_StringSerial[18], 4);
	}
} 

//��32λ��ֵת����unicode.
//value,Ҫת����ֵ(32bit)
//pbuf:�洢��ַ
//len:Ҫת���ĳ���
void IntToUnicode (u32 value , u8 *pbuf , u8 len)
{
	u8 idx = 0;
	for( idx = 0 ; idx < len ; idx ++)
	{
		if( ((value >> 28)) < 0xA )
		{
			pbuf[ 2* idx] = (value >> 28) + '0';
		}
		else
		{
			pbuf[2* idx] = (value >> 28) + 'A' - 10; 
		} 
		value = value << 4; 
		pbuf[ 2* idx + 1] = 0;
	}
}
/////////////////////////////////////////////////////////////////////////////////
 
//USB COM�ڵ�������Ϣ,ͨ���˺�����ӡ����. 
bool USART_Config(void)
{
	uu_txfifo.readptr=0;	//��ն�ָ��
	uu_txfifo.writeptr=0;	//���дָ��
	uu_rxfifo.readptr=0;	
	uu_rxfifo.writeptr=0;
	//printf("linecoding.format:%d\r\n",linecoding.format);
  	//printf("linecoding.paritytype:%d\r\n",linecoding.paritytype);
	//printf("linecoding.datatype:%d\r\n",linecoding.datatype);
	//printf("linecoding.bitrate:%d\r\n",linecoding.bitrate);
	return (TRUE);
}
 
//�����USB���⴮�ڽ��յ�������
//databuffer:���ݻ�����
//Nb_bytes:���յ����ֽ���.
void USB_To_USART_Send_Data(u8* data_buffer, u8 Nb_bytes)
{ 
//	u16 i;
//	if(uu_rxfifo.writeptr+1 != uu_rxfifo.readptr)
//	{
//		for(i=0; i<Nb_bytes; i++)
//		{
//			uu_rxfifo.buffer[uu_txfifo.writeptr]=data_buffer[i];
//			uu_rxfifo.writeptr++;
//			if(uu_rxfifo.writeptr==USB_USART_TXFIFO_SIZE)//����buf��С��,����.
//			{
//				uu_rxfifo.writeptr=0;
//			} 
//		}
//	}
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	for(int i=0; i<Nb_bytes; i++)
	{
		uint8_t data = data_buffer[i];
		xQueueSendFromISR(usbDataDelivery, &data, &xHigherPriorityTaskWoken);
	}
} 

//����һ���ֽ����ݵ�USB���⴮��
void USB_SendData(u8 data)
{
	uu_txfifo.buffer[uu_txfifo.writeptr]=data;
	uu_txfifo.writeptr++;
	if(uu_txfifo.writeptr==USB_USART_TXFIFO_SIZE)//����buf��С��,����.
	{
		uu_txfifo.writeptr=0;
	} 
}

void usbsendData(u8* data, u16 length)
{
	u16 i,j;
	i = length;
	for(j=0;j<i;j++)//ѭ����������
	{
		USB_SendData(data[j]); 
	}
}

//usb���⴮��,printf ����
//ȷ��һ�η������ݲ���USB_USART_REC_LEN�ֽ�
void usb_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART_PRINTF_Buffer,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART_PRINTF_Buffer);//�˴η������ݵĳ���
	for(j=0;j<i;j++)//ѭ����������
	{
		USB_SendData(USART_PRINTF_Buffer[j]); 
	}
}

//usb���⴮�ڳ�ʼ��
void usb_vcp_init(void)
{
	Set_USBClock();   
 	USB_Interrupts_Config();    
 	USB_Init();
	USB_Port_Set(0); 	//USB�ȶϿ�
	delay_ms(100);
	USB_Port_Set(1);	//USB�ٴ�����
	
	usbDataDelivery = xQueueCreate(128, sizeof(uint8_t));
}

int usbGetDataWithTimout(uint8_t *c)
{
	if (xQueueReceive(usbDataDelivery, c, 1000) == pdTRUE)
	{
		return 1;
	}
	*c = 0;
	return 0;
}

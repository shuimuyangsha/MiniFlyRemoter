#ifndef  __PROTOCOL_H
#define  __PROTOCOL_H
#include <stdint.h>

/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly
 * ����Э��	
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/

//����֡����
typedef struct
{
	//���֡����
	#define MAX_FRAME_LENGTH (256+6)
	//��С֡����
	#define MIN_FRAME_LENGTH  5	
	//�豸��ַ
	uint8_t Device_Address;
	//֡����
	uint8_t Function_Type;
	//֡����
	uint8_t Sequence;
	//��Ч���ݳ���
	uint8_t Data_Length;
	//����
	uint8_t *Data;
	//У��ֵ
	uint16_t Checksum;

}TransportProtocol_Typedef;

//������
typedef enum
{
	//֡��ʽ����
	FRAME_FORMAT_ERR = 1,		
	//У��ֵ��ʽ����
	CHECK_FORMAR_ERR = 2,
	//У��ֵ��λ
	CHECK_ERR = 3,
	//����ɹ�
	UPACKED_SUCCESS = 4

}TransportProtocol_Result;

//Э�������
typedef struct
{	
	//����֡
	TransportProtocol_Typedef * TransportProtocol;
	//���յ��ֽ���
	uint32_t  RecieveByteCount;

	//����֡����
	uint8_t* Buf;
	//֡�ܳ���
	uint16_t FrameTotalLength;
	//�������
	TransportProtocol_Result (*Unpacked)(void);
	//�������
	void (*Packed)(void);
	//У�麯��
	uint16_t (*Check)(uint8_t *,uint16_t len);

}TransportProtocol_Manager_Typedef;

//�ⲿ����Э�������
extern TransportProtocol_Manager_Typedef TransportProtocol_Manager;
//��ʼ������Э��
void  TransportProtocol_Init(TransportProtocol_Typedef *TransportProtocol,uint8_t *buf,uint16_t (*check)(uint8_t *,uint16_t len));

#endif



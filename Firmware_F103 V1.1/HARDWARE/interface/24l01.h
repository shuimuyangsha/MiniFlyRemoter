#ifndef __24L01_H
#define __24L01_H
#include <stdint.h>
#include "sys.h"   
/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly_Remotor
 * nrf24l01��������	
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/

/***********************NRF SPI�Ĵ���ָ��*********************************/
#define CMD_R_REG       	0x00  // ���Ĵ���ָ��
#define CMD_W_REG       	0x20  // д�Ĵ���ָ��
#define CMD_RX_PL_WID   	0x60  // ��ȡ�������ݳ���
#define CMD_R_RX_PAYLOAD 	0x61  // ��ȡ��������ָ��
#define CMD_W_TX_PAYLOAD  	0xA0  // д��������ָ��
#define CMD_FLUSH_TX      	0xE1  // ��ϴ���� FIFOָ��
#define CMD_FLUSH_RX      	0xE2  // ��ϴ���� FIFOָ��
#define CMD_REUSE_TX_PL   	0xE3  // �����ظ�װ������ָ��
#define CMD_NOP           	0xFF  // ��������������ȡstatus�Ĵ���

#define CMD_W_ACK_PAYLOAD(P)  (0xA8|(P&0x0F))//PRXģʽװ��PAYLOAD��ʹ��Pͨ��
#define CMD_W_TX_PAYLOAD_NO_ACK 0xB0				 //PTXģʽдNO ACK����

//***********************NRF �Ĵ�����ַ***********************************//
#define REG_CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define REG_EN_AA           0x01  // �Զ�Ӧ��������
#define REG_EN_RXADDR       0x02  // �����ŵ�����
#define REG_SETUP_AW        0x03  // �շ���ַ�������
#define REG_SETUP_RETR      0x04  // �Զ��ط���������
#define REG_RF_CH           0x05  // ����Ƶ������
#define REG_RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define REG_STATUS    		0x07  // ״̬�Ĵ���
#define REG_OBSERVE_TX      0x08  // ���ͼ�⹦��
#define REG_RPD             0x09  // ���չ��ʼ��           
#define REG_RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define REG_RX_ADDR_P1      0x0B  // Ƶ��1�������ݵ�ַ
#define REG_RX_ADDR_P2      0x0C  // Ƶ��2�������ݵ�ַ
#define REG_RX_ADDR_P3      0x0D  // Ƶ��3�������ݵ�ַ
#define REG_RX_ADDR_P4      0x0E  // Ƶ��4�������ݵ�ַ
#define REG_RX_ADDR_P5      0x0F  // Ƶ��5�������ݵ�ַ
#define REG_TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define REG_RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���
#define REG_RX_PW_P1        0x12  // ����Ƶ��1�������ݳ���
#define REG_RX_PW_P2        0x13  // ����Ƶ��2�������ݳ���
#define REG_RX_PW_P3        0x14  // ����Ƶ��3�������ݳ���
#define REG_RX_PW_P4        0x15  // ����Ƶ��4�������ݳ���
#define REG_RX_PW_P5        0x16  // ����Ƶ��5�������ݳ���
#define REG_FIFO_STATUS     0x17  // FIFOջ��ջ��״̬�Ĵ�������
#define REG_DYNPD 			0x1C  // ��̬���ݰ�����
#define REG_FEATURE 		0x1D  // �ص�Ĵ���

/* STATUS�Ĵ�������λ */
#define BIT_RX_DR (1<<6)
#define BIT_TX_DS (1<<5)
#define BIT_MAX_RT (1<<4)

#define  NRF_SPI  			SPI1
#define  NRF_CE_H()			PAout(3) = 1 
#define  NRF_CE_L()			PAout(3) = 0 

#define  SPI2_CSN_H()		PAout(4) = 1 
#define  SPI2_CSN_L()		PAout(4) = 0 

enum nrfMode
{
	PTX_MODE,
	PRX_MODE,
};	

enum nrfPower
{
	POWER_M18dBm,
	POWER_M12dBm,
	POWER_M6dBm,
	POWER_0dBm,
};

enum nrfRate
{
	DATA_RATE_250K,
	DATA_RATE_1M,
	DATA_RATE_2M,
};

typedef enum 
{
	IDLE    = 0,
	MAX_RT 	= 1,
	TX_DS 	= 2,
	RX_DR 	= 3,
}nrfEvent_e;
	
/* ��ʼ��NRF24L01���� */
void nrfInit(enum nrfMode model);	

/* ���nrf�Ƿ�ͨѶ���� */
ErrorStatus nrf_check(void);

/* �������ݰ�(PTXģʽ) */
void nrf_txPacket(u8 *tx_buf,u8 len);

/* ����NO_ACK���ݰ�(PTXģʽ) */
void nrf_txPacketNoACK(u8 *tx_buf,u8 len);

/* ����ACK���ݰ�������0ͨ��(PRXģʽ) */
void nrf_txPacket_AP(u8 *tx_buf,u8 len);

/* ����NO_ACK���ݰ�(PTXģʽ) */
void nrf_sendPacketNoACK(u8 *sendBuf,u8 len);
	
/* �������ݰ������ȴ�����ACK (PTXģʽ) */
u8 nrf_sendPacketWaitACK(u8 *sendBuf ,u8 len ,
							u8 *ackBuf  ,u8 *acklen);
/* ��ѯ�¼����������ݰ� */
nrfEvent_e nrf_checkEventandRxPacket(u8 *ackBuf, u8 *acklen);

/*����nrf�жϻص�����*/
void nrf_setIterruptCallback(void(*cb)(void));

/**************************NRF24L01+���ú���***********************************/
void nrf_setAddress(uint64_t address);
void nrf_setChannel(u8 channel);
void nrf_setDataRate(enum nrfRate dataRate);
void nrf_setPower(enum nrfPower power);
void nrf_setArd(void);
void nrf_setArc(u8 arc);
u8 nrf_getRpd(void);
u8 nrf_getTxRetry(void);
void nrf_txPacket(u8 *tx_buf,u8 len);
u8 nrf_rxPacket(u8 *rx_buf);
#endif



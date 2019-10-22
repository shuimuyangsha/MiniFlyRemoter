#ifndef __CHECK_H
#define __CHECK_H
#include <stdint.h>

/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly
 * У���ļ�	
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/

uint16_t Checksum_Sum(uint8_t* buf,uint16_t len);	//��У�� �����ֽ�֮��Ϊ0
uint16_t Checksum_XOR(uint8_t* buf, uint16_t len);	//���У�飬�����ֽ����
uint16_t Checksum_CRC8(uint8_t *buf,uint16_t len);	//CRC8 У��
uint16_t Checksum_CRC16(uint8_t *buf,uint16_t len);//CRC16 У��

#endif


#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//ADC ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

#define  ADC_BAT		0
#define  ADC_ROLL		1
#define  ADC_PITCH	 	2
#define  ADC_YAW		3
#define  ADC_THRUST		4

//��ʼ��ADC��ʹ��DMA����
void Adc_Init(void);
void ADC_Filter(uint16_t* adc_val);	//ADC��ֵ�˲�
uint16_t getAdcValue(uint8_t axis);

#endif 

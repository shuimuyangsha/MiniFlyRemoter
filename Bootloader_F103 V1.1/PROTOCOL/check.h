#ifndef __CHECK_H
#define __CHECK_H
#include <stdint.h>

/********************************************************************************	 
 * 本程序只供学习使用，未经作者许可，不得用于其它任何用途
 * ALIENTEK MiniFly
 * 校验文件	
 * 正点原子@ALIENTEK
 * 技术论坛:www.openedv.com
 * 创建日期:2017/5/2
 * 版本：V1.0
 * 版权所有，盗版必究。
 * Copyright(C) 广州市星翼电子科技有限公司 2014-2024
 * All rights reserved
********************************************************************************/

uint16_t Checksum_Sum(uint8_t* buf,uint16_t len);	//和校验 所有字节之和为0
uint16_t Checksum_XOR(uint8_t* buf, uint16_t len);	//异或校验，所有字节异或
uint16_t Checksum_CRC8(uint8_t *buf,uint16_t len);	//CRC8 校验
uint16_t Checksum_CRC16(uint8_t *buf,uint16_t len);//CRC16 校验

#endif


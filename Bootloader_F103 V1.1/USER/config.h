#ifndef __CONFIG_H
#define __CONFIG_H

/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly	
 * �����ļ�
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/

#define BOOTLOADER_SIZE		(9*1024)		/*9K bootloader����ռ�*/
#define CONFIG_PARAM_SIZE	(127*1024)		/*��128K���ڱ������*/

#define CONFIG_PARAM_ADDR 	(FLASH_BASE + CONFIG_PARAM_SIZE)/*���ò��������ַ*/	
#define FIRMWARE_START_ADDR (FLASH_BASE + BOOTLOADER_SIZE)

#endif











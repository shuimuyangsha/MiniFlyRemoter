
�����������ص�ַ:
	http://www.openedv.com/thread-105197-1-1.html
	
Ӳ����Դ��
	1,MCU:STM32F103C8T6 (FLAH:64K, RAM:20K, ϵͳ����ʱ��Ƶ��:72MHz) 
	2,����ͨ��NRF24L01+(CE:PA3, CSN:PA4, SCK:PA5, MISO:PA5, MOSI:PA6, IRQ:PC13) 
	3,����ҡ�ˣ�ADC_YAW:PB0, ADC_THRUST: PB1��
	4,����ҡ�ˣ�ADC_ROLL:PA1, ADC_PITCH: PA2��
	5,OLED(MOSI:PB15, RST:PB14, CLK:PB13, DC:PB12)
	6,����(KEY_J1��PB10, KEY_J2��PA8, KEY_L:PB11, KEY_R:PC15)
	7,ͨ��ָʾ��(RED_LED0:PB7, BLUE_LED1:PB3)
	8,��������PC14��

ʵ������
	ң������������ʾ�����棬���������Ρ�һ��������ɷ��С�
	
	���
	    RED_LED0����������ͨ��ʧ��
	    BLUE_LED1����������ͨ�ųɹ�

ע������:
	�������غ͵���ǰ���뽫���������ز���STM32����
	bootloader��ʼ��ַ��BOOTLOADER_START_ADDR�� : 0x8000000
	�̼���ʼ��ַ��FIRMWARE_START_ADDR�� : 0x8002400

�̼����¼�¼:
	V1.0 Release(Ӳ���汾:V1.6, DATE:2017-06-30)
	
	
	V1.1 Release(Ӳ���汾:V1.6��V1.7, DATE:2017-10-18)
		1.�޸�config_param.h�����ò���������ʼ��ַ�ɵ�63������127K;
		2.GUI_APP����menuL3_item.c��������չģ��Ĳ˵�����ڹ�����չģ��;
	




					����ԭ��@ALIENTEK
					2017-6-30
					������������ӿƼ����޹�˾
					�绰��020-38271790
					���棺020-36773971
					����http://shop62103354.taobao.com
					http://shop62057469.taobao.com
					��˾��վ��www.alientek.com
					������̳��www.openedv.com






































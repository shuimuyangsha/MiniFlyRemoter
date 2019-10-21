
最新资料下载地址:
	http://www.openedv.com/thread-105197-1-1.html
	
硬件资源：
	1,MCU:STM32F103C8T6 (FLAH:64K, RAM:20K, 系统运行时钟频率:72MHz) 
	2,无线通信NRF24L01+(CE:PA3, CSN:PA4, SCK:PA5, MISO:PA5, MOSI:PA6, IRQ:PC13) 
	3,油门摇杆（ADC_YAW:PB0, ADC_THRUST: PB1）
	4,方向摇杆（ADC_ROLL:PA1, ADC_PITCH: PA2）
	5,OLED(MOSI:PB15, RST:PB14, CLK:PB13, DC:PB12)
	6,按键(KEY_J1：PB10, KEY_J2：PA8, KEY_L:PB11, KEY_R:PC15)
	7,通信指示灯(RED_LED0:PB7, BLUE_LED1:PB3)
	8,蜂鸣器（PC14）

实验现象：
	遥控器开机后显示主界面，蜂鸣器“滴”一声后四轴可飞行。
	
	灯语：
	    RED_LED0亮：与四轴通信失败
	    BLUE_LED1亮：与四轴通信成功

注意事项:
	代码下载和调试前，请将下载器开关拨到STM32档。
	bootloader起始地址（BOOTLOADER_START_ADDR） : 0x8000000
	固件起始地址（FIRMWARE_START_ADDR） : 0x8002400

固件更新记录:
	V1.0 Release(硬件版本:V1.6, DATE:2017-06-30)
	
	
	V1.1 Release(硬件版本:V1.6和V1.7, DATE:2017-10-18)
		1.修改config_param.h将配置参数保存起始地址由第63更改至127K;
		2.GUI_APP增加menuL3_item.c，增加扩展模块的菜单项，用于管理扩展模块;
	




					正点原子@ALIENTEK
					2017-6-30
					广州市星翼电子科技有限公司
					电话：020-38271790
					传真：020-36773971
					购买：http://shop62103354.taobao.com
					http://shop62057469.taobao.com
					公司网站：www.alientek.com
					技术论坛：www.openedv.com






































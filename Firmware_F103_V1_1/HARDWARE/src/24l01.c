#include "24l01.h"
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

/* �û����ò�����Ĭ�ϣ� */
#define DEFAULT_ADDR			0x123456789AULL
#define DEFAULT_CHANNEL 	 	2
#define DEFAULT_DATARATE  		DATA_RATE_250K
#define DEFAULT_POWR 			POWER_0dBm

uint64_t nrfAddress = DEFAULT_ADDR;
static void (*interruptCb)(void) = 0;

/***************************NRF24L01+��������***********************************/

/* NRF��ʼ����ʹ��STM32��SPI1 */
static void NRF_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); 

	/* ����SPI1��SCK(PA5),MISO(PA6),MOSI(PA7)���� */ 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* ����NRF��CE(PA3),CSN(PA4)���� */ 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* ����NRF��IRQ����(PC13) */ 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* ������IRQ�ⲿ�ж� */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
  
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  /* ����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫�� */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		/* ����SPI����ģʽ:����Ϊ��SPI */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	/* ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ */
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;			/* ѡ���˴���ʱ�ӵ���̬:ʱ�����յ� */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;		/* ���ݲ����ڵ�һ��ʱ���� */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			/* NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ���� */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;	/*���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ4 */
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	/* ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ */
	SPI_InitStructure.SPI_CRCPolynomial = 7;			/* CRCֵ����Ķ���ʽ */
	SPI_Init(SPI1, &SPI_InitStructure);  				/* ����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ��� */
 
	SPI_Cmd(SPI1, ENABLE);	/*ʹ��SPI����*/
	
	SPI2_CSN_H();
	NRF_CE_L();
}

static u8 SPI_RWByte(SPI_TypeDef* SPIx , u8 TxData)
{			
	/* ͨ������SPIx����һ������ */
	SPI_I2S_SendData(SPIx, TxData);
	/* ���ָ����SPI��־λ�������:���ͻ���ձ�־λ*/
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
	
	/* ���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ */
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
	
	/* ����ͨ��SPIx������յ����� */
	return SPI_I2S_ReceiveData(SPIx); 	
}

/* д�Ĵ��� */
static u8 writeReg(u8 reg,u8 value)
{
	u8 status;	
	SPI2_CSN_L();                	 	
	status=SPI_RWByte(NRF_SPI,reg|CMD_W_REG);
	SPI_RWByte(NRF_SPI , value); 	
	SPI2_CSN_H();                 	  
	return status;       					
}

/* ���Ĵ��� */ 
static u8 readReg(u8 reg)
{
	u8 reg_val;	    
 	SPI2_CSN_L();         		 			
	SPI_RWByte(NRF_SPI,reg|CMD_R_REG);
	reg_val=SPI_RWByte(NRF_SPI,0xA5);
	SPI2_CSN_H();     								    
	return reg_val;    						
}	

/* �������� */
static u8 readBuf(u8 cmd,u8 *pBuf,u8 len)
{
	u8 status,i;	       
	SPI2_CSN_L();            
	status=SPI_RWByte(NRF_SPI,cmd);   	   
	for(i=0;i<len;i++)
		pBuf[i]=SPI_RWByte(NRF_SPI,0XFF);
	SPI2_CSN_H();
	return status;
}

/* д������ */
static u8 writeBuf(u8 cmd,u8 *pBuf,u8 len)
{
	u8 status,i;	    
	SPI2_CSN_L();          
	status=SPI_RWByte(NRF_SPI,cmd);
	for(i=0;i<len;i++)
		SPI_RWByte(NRF_SPI,*pBuf++);  
	SPI2_CSN_H();  			 	
	return status;  
}

/* �������ݰ�(PTXģʽ) */
void nrf_txPacket(u8 *tx_buf,u8 len)
{	
	NRF_CE_L();	
	writeBuf(CMD_W_TX_PAYLOAD,tx_buf,len);
	NRF_CE_H();		 
}

/* ����NO_ACK���ݰ�(PTXģʽ) */
void nrf_txPacketNoACK(u8 *tx_buf,u8 len)
{	
	NRF_CE_L();		 
	writeBuf(CMD_W_TX_PAYLOAD_NO_ACK,tx_buf,len);
	NRF_CE_H();		
}

/* ����ACK���ݰ�������0ͨ��(PRXģʽ) */
void nrf_txPacket_AP(u8 *tx_buf,u8 len)
{	
	NRF_CE_L();		 	
	writeBuf(CMD_W_ACK_PAYLOAD(0),tx_buf,len);
	NRF_CE_H();		 
}
/* ����NO_ACK���ݰ�(PTXģʽ) */
void nrf_sendPacketNoACK(u8 *sendBuf,u8 len)
{	
	while((readReg(REG_STATUS)&0x01)!=0);	/* �ȴ�TX_FIFO��Ϊfull */
	nrf_txPacketNoACK(sendBuf,len);			/* ����NO_ACK���ݰ� */
}

/**************************NRF24L01+���ú���***********************************/
/* ���÷�����յ�ַ�������շ���ַһ�� */
void nrf_setAddress(uint64_t address)
{
	writeBuf(CMD_W_REG |REG_RX_ADDR_P0,(u8*)&address,5);//����ʹ��P0�ڵ�
	writeBuf(CMD_W_REG |REG_TX_ADDR,(u8*)&address,5); 		
}

/* ����Ƶ��ͨ��,channel:0~125 */
void nrf_setChannel(u8 channel)
{
	if(channel<=125) 
		writeReg(REG_RF_CH, channel);
}

/* ���ô������ʣ�dr:0->250KHz��1->1MHz��2->2MHz�� */
void nrf_setDataRate(enum nrfRate dataRate)
{
	u8 reg_rf = readReg(REG_RF_SETUP);
	reg_rf &= ~((1<<5)|(1<<3));/* ���ԭ������ */
	switch(dataRate)
	{
		case DATA_RATE_250K:
			reg_rf |= 0x20;
			break;
		case DATA_RATE_1M:
			reg_rf |= 0x00;
			break;
		case DATA_RATE_2M:
			reg_rf |= 0x08;
			break;	
	}
	writeReg(REG_RF_SETUP,reg_rf); 	
}

/* ���÷��书��,power: 0->-18dB  1->-12dB  2->-6dB  3->0dB */
void nrf_setPower(enum nrfPower power)
{
	u8 reg_rf = readReg(REG_RF_SETUP);
	reg_rf &= 0xF8;/* ���ԭ�蹦�� */
	switch(power)
	{
		case POWER_M18dBm:
			reg_rf |= 0x00;
			break;
		case POWER_M12dBm:
			reg_rf |= 0x02;
			break;
		case POWER_M6dBm:
			reg_rf |= 0x04;
			break;
		case POWER_0dBm:
			reg_rf |= 0x07;
			break;	
	}
	writeReg(REG_RF_SETUP,reg_rf);
}

/* �����ط�ʱ�������������ʼ��շ��ֽڴ�С���� */
/* ��ϸ˵���ο�nrf24l01.datasheet��P34. */
void nrf_setArd(void)
{
	u8 reg_rf,reg_retr;
	reg_rf=readReg(REG_RF_SETUP);
	reg_retr=readReg(REG_SETUP_RETR);
	if(!(reg_rf&0x20))	/* ���ʲ���250K(�Ĵ���0x20) */
		reg_retr|= 1<<4;/* (1+1)*250=500us,�ڽ���32�ֽ�ʱ */
	else
		reg_retr|= 5<<4;/* (5+1)*250=1500us,�ڽ���32�ֽ�ʱ */
	writeReg(REG_SETUP_RETR,reg_retr);
}

/* �����ط�������arc:0~15 */
void nrf_setArc(u8 arc)
{
	u8 reg_retr;
	if(arc>15)
		return;
	reg_retr=readReg(REG_SETUP_RETR);
	reg_retr|= arc;
	writeReg(REG_SETUP_RETR,reg_retr);
}

/* ��ȡ���չ��ʼ�� */
u8 nrf_getRpd(void)
{
   return readReg(REG_RPD);
}

/* ��ȡ�ط�ʧ�ܴ��� */
u8 nrf_getTxRetry(void)
{
   return readReg(REG_OBSERVE_TX)&0x0F;
}


/* ��ʼ��NRF24L01���� */
/* model: PTX_MODE��PRX_MODE */
void nrfInit(enum nrfMode model)
{
	NRF_Init();
	nrf_setAddress(nrfAddress);
	nrf_setChannel(DEFAULT_CHANNEL);
	nrf_setDataRate(DEFAULT_DATARATE);
	nrf_setPower(DEFAULT_POWR);
	nrf_setArd();
	nrf_setArc(3);
	if(model==PRX_MODE)
	{
		writeReg(REG_CONFIG, 0x0f);   		/* IRQ�շ�����жϿ���,16λCRC,PRX */
		writeReg(REG_DYNPD,0x01);			/* ʹ��RX_P0��̬����PLAYLOAD */
		writeReg(REG_FEATURE,0x06);			/* ʹ�ܶ�̬����PLAYLOAD������ACK PLAYLOAD */
		
		writeReg(REG_EN_AA,0x01); 			/* ʹ��ͨ��0���Զ�Ӧ�� */	
		
		writeReg(CMD_FLUSH_TX,0xff);		/* ��ϴTX_FIFO */
		writeReg(CMD_FLUSH_RX,0xff);
	}
	else							 	
	{
		writeReg(REG_CONFIG, 0x0e);   		/* IRQ�շ�����жϿ���,16λCRC,PTX */
		writeReg(REG_DYNPD,0x01);			/* ʹ��RX_P0��̬����PLAYLOAD */
		writeReg(REG_FEATURE,0x07);			/* ʹ�ܶ�̬���ȡ�ACK PLAYLOAD���͡�W_TX_PAYLOAD_NOACK */
		
		writeReg(CMD_FLUSH_TX,0xff);		/* ��ϴTX_FIFO */
		writeReg(CMD_FLUSH_RX,0xff);
	}
}

/* ���MCU��24l01�Ƿ�ͨѶ���� */
/* ������д�������ַ�Ƿ�һ�� */
ErrorStatus nrf_check(void)
{ 
	uint64_t addr = 0;
	
	NRF_Init();
	writeBuf(CMD_W_REG |REG_TX_ADDR,(u8*)&nrfAddress,5); 
	readBuf(CMD_R_REG|REG_TX_ADDR,(u8*)&addr,5); 
	if(nrfAddress==addr)
		return SUCCESS;
	else
		return ERROR;
}

/* �������ݰ������ذ�����len */
u8 nrf_rxPacket(u8 *rx_buf)
{	
	u8 rx_len = readReg(CMD_RX_PL_WID);
	if(rx_len>0 && rx_len<33)
	{
		NRF_CE_L();	
		readBuf(CMD_R_RX_PAYLOAD,rx_buf,rx_len);
		NRF_CE_H();
	}
	else 
		rx_len = 0;
	writeReg(CMD_FLUSH_RX,0xff);/* ��ϴRX_FIFO */
	return rx_len;		
}

/* ��ѯ�¼����������ݰ� */
nrfEvent_e nrf_checkEventandRxPacket(u8 *ackBuf, u8 *acklen)
{
	nrfEvent_e nrfEvent = IDLE;
	*acklen = 0;
	u8 status = readReg(REG_STATUS);/*���¼���־�Ĵ���*/
	if(status&BIT_MAX_RT)/*�ط�ʧ��*/
	{
		writeReg(CMD_FLUSH_TX,0xff);
		nrfEvent =  MAX_RT;
	}
	else if(status&BIT_RX_DR)/*�������ݵ�RX_FIFO*/
	{
		*acklen =  nrf_rxPacket(ackBuf);
		nrfEvent = RX_DR;
	}
	else if(status&BIT_TX_DS)/*����������TX_FIFO�ɹ�*/
	{
		nrfEvent = TX_DS;
	}
	writeReg(REG_STATUS,0x70);/*�����־*/
	u8 status1 = readReg(REG_STATUS);/*���¼���־�Ĵ���*/
	status1 = status1;
	return nrfEvent;
}

/* �������ݰ������ȴ�����ACK(PTXģʽ) */
/* ����ֵ��1�ɹ���0ʧ��*/
u8 nrf_sendPacketWaitACK(u8 *sendBuf, u8 len, u8 *ackBuf, u8 *acklen)
{ 
	if(len==0) return 0;
	nrf_txPacket(sendBuf,len);
	while((readReg(REG_STATUS)&0x70) == 0);/* �ȴ��¼� */
	nrfEvent_e nrfEvent = nrf_checkEventandRxPacket(ackBuf, acklen);
	if(nrfEvent == MAX_RT)
		return 0;
	return 1;
}

/*����nrf�жϻص�����*/
void nrf_setIterruptCallback(void(*cb)(void))
{
	interruptCb = cb;
}

/*�ⲿ�жϷ�����*/
void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line13)==SET)
	{
		if(interruptCb)
		{
			interruptCb();
		}
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
}

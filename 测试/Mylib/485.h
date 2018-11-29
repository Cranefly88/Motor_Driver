#ifndef _485_H
#define	_485_H
#include "board.h"

#define RS485_RX_EN	 GPIOC->BSRRH = GPIO_Pin_2   	//485ģʽ����.0,����;1,����.
#define RS485_TX_EN	 GPIOC->BSRRL = GPIO_Pin_2   	//485ģʽ����.0,����;1,����.

#define SEND_BUF_SIZE 10	//�������ݳ���,��õ���sizeof(TEXT_TO_SEND)+2��������.
#define RX_BUF_SIZE 4	//�������ݳ���,��õ���sizeof(TEXT_TO_SEND)+2��������.

extern u8 SendBuff[SEND_BUF_SIZE];	//�������ݻ�����
extern int16_t DM50_POSITION_Yaw, DM50_POSITION_Pitch ;
extern int Position_485Yaw,Position_485Whiff,Position_485Pitch,Position_485Whiff_Last,Speed_485Whiff;
extern int START_Position_Yaw,START_Position_Whiff,START_Position_Pitch;

void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr);
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr);

void RS485_Init(u32 bound);
void RS485_Send_Data(u8 *buf,u8 len);
void USART3_IRQHandler(void);





#endif

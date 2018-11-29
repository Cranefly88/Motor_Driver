#ifndef __485motor_H__
#define __485motor_H__

#include "board.h"

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define SEND_BUF_SIZE 10	//�������ݳ���,��õ���sizeof(TEXT_TO_SEND)+2��������.
#define RX_BUF_SIZE 4	//�������ݳ���,��õ���sizeof(TEXT_TO_SEND)+2��������.	  	
#define DM50     0

extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
extern u8 RS485_RX_BUF[64]; 		//���ջ���,���64���ֽ�
extern u8 RS485_RX_CNT;   			//���յ������ݳ���

//ģʽ����
#define RS485_TX_EN		PCout(2)	//485ģʽ����.0,����;1,����.
//����봮���жϽ��գ�����EN_USART2_RXΪ1����������Ϊ0
#define EN_USART3_RX 	1			//0,������;1,����.

extern u8 Send485buf[10]; 
extern u8 rx_rs485buf[10]; 
extern u8 SendBuff[SEND_BUF_SIZE];	//�������ݻ�����
extern u8 RxBuff[1000];	//�������ݻ�����
extern long int DM50_POSTION_SUM;	

void RS485_Init(u32 bound);
void RS485_Send_Data(u8 *buf,u8 len);
void RS485_Receive_Data(u8 *buf,u8 *len);		 
void Init_485motor_send(u16 arr,u16 psc);
void TIM3_IRQHandler(void);
void USART3_IRQHandler(void);
#endif 
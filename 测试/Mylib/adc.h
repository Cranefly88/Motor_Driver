#ifndef __ADC_H
#define __ADC_H	
#include "board.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//ADC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
 							   
void Adc_Init(void); 				//ADCͨ����ʼ��
void Get_Adc(void); 				//���ĳ��ͨ��ֵ 
// u16 Get_Adc(u8 ch)   ;
// u16 Get_Adc_Average(u8 ch,u8 times);//�õ�ĳ��ͨ����������������ƽ��ֵ  

struct GP2Y
		{
			float forward;
			float backward;
			float left;
			float right;
		};

#endif 
















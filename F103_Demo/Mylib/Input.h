#ifndef __INPUT_H__
#define __INPUT_H__
#include "stm32f10x.h"

#define READ_B3    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)//��ȡB3��ƽ 
#define READ_B4    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)//��ȡB4��ƽ
#define READ_B5    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)//��ȡB5��ƽ   
#define READ_B13    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//��ȡB13��ƽ 
#define READ_B14    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//��ȡB14��ƽ
#define READ_B15    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)//��ȡB15��ƽ

#define OUT_ON_A4    GPIO_SetBits(GPIOA,GPIO_Pin_4);//A4����ߵ�ƽ
#define OUT_OFF_A4   GPIO_ResetBits(GPIOA,GPIO_Pin_4);//A4����͵�ƽ
#define OUT_ON_A5    GPIO_SetBits(GPIOA,GPIO_Pin_5);//A5����ߵ�ƽ
#define OUT_OFF_A5   GPIO_ResetBits(GPIOA,GPIO_Pin_5);//A5����͵�ƽ 
#define OUT_ON_A8    GPIO_SetBits(GPIOA,GPIO_Pin_8);//A8����ߵ�ƽ
#define OUT_OFF_A8   GPIO_ResetBits(GPIOA,GPIO_Pin_8);//A8����͵�ƽ
#define OUT_ON_A11    GPIO_SetBits(GPIOA,GPIO_Pin_11);//A11����ߵ�ƽ
#define OUT_OFF_A11   GPIO_ResetBits(GPIOA,GPIO_Pin_11);//A11����͵�ƽ
#define OUT_ON_A12    GPIO_SetBits(GPIOA,GPIO_Pin_12);//A12����ߵ�ƽ
#define OUT_OFF_A12   GPIO_ResetBits(GPIOA,GPIO_Pin_12);//A12����͵�ƽ 
#define OUT_ON_A15    GPIO_SetBits(GPIOA,GPIO_Pin_15);//A15����ߵ�ƽ
#define OUT_OFF_A15   GPIO_ResetBits(GPIOA,GPIO_Pin_15);//A15����͵�ƽ
void Input_init(void);
void Output_init(void);

#endif

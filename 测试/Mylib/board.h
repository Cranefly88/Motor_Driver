#ifndef __BOARD_H__
#define __BOARD_H__

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

/***************Ӳ���жϷ���******************/
#define NVIC_GROUP   NVIC_PriorityGroup_3

#include "stdint.h"
#include "TDT_User.h"
#include "dbus.h"
#include "schedule.h"
#include "time.h"
#include "control.h"
#include "math.h"
#include "mymath.h"
#include "can1.h"
#include "can2.h"
#include "parameter.h"
#include "filter.h"
#include "timer.h"
#include "led.h"
#include "task_control.h"
#include "Robomodule.h"
#include "mlt8530.h"
#include "softiic2.h"
#include "softiic1.h"
#include "OLED_I2C.h"
#include "codetab.h"
#include "485.h"
#include "timer485.h"
#include "mpu6050.h"
#include "imu.h"
#include "filter.h"
#include "distance.h"
#include "Engine_gpio.h"
#include "Camera.h"
#include "Distance_Voice.h"
//#include "myiic.h"
#include "usart1.h"
#include "model_ident.h"
#include "judgement.h"
#include "crc.h"
#include "data_send.h"
#include "spi.h"
#include "DRV8305.h"
#include "TIM1.h"


#define  RCC_LED           RCC_AHB1Periph_GPIOA
#define  LED_PORT          GPIOA
#define  RED_LED_Pin       GPIO_Pin_11 
#define  GREEN_LED_Pin     GPIO_Pin_10
#define  BLUE_LED_Pin			 GPIO_Pin_8

//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).M4ͬM3����,ֻ�ǼĴ�����ַ����.
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     

#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //��� 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //����

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //��� 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //����
void SysTick_Configuration(void);
uint32_t GetSysTime_us(void);
void DelayUs(uint32_t us);
void DelayMs(uint32_t ms);
void Board_ALL_Init(void);

extern int unm11;

extern volatile uint32_t sysTickUptime;
#endif /* __BOARD_H__ */


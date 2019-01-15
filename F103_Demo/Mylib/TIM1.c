#include "TIM1.h"

void TIM1_Init(u16  arr,  u16 psc)
{
 	GPIO_InitTypeDef GPIO_InitStructure;   //����һ���ṹ�������������ʼ��GPIO

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//����һ���ṹ�������������ʼ����ʱ��

	TIM_OCInitTypeDef TIM_OCInitStructure;//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
  
  TIM_BDTRInitTypeDef TIM_BDTRStructure;//����ɲ��

	/* ����ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_TIM1,ENABLE);

	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
	GPIO_Init(GPIOB,&GPIO_InitStructure);

  //��ӳ��
  GPIO_AFIODeInit();
  GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1 , ENABLE); 

	//TIM1��ʱ����ʼ��
  TIM_DeInit(TIM1);
	TIM_TimeBaseInitStructure.TIM_Period =arr;	   //����Ƶ,PWM Ƶ��=72000/900=8Khz//�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler =psc ;//����������ΪTIMxʱ��Ƶ��Ԥ��Ƶֵ��100Khz����Ƶ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;	//TIM���ϼ���ģʽ
  TIM_TimeBaseInitStructure.TIM_RepetitionCounter =1;//���ڼ�����ֵ 
	TIM_TimeBaseInit(TIM1, & TIM_TimeBaseInitStructure);

	//PWM��ʼ��	  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
  TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;//��MOE=0��������ȽϿ���״̬
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;//��MOE=0��������Ƚ�N����״
  TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High;// //��������Ƚϼ��Ը�
  TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//����Ƚϼ��Ը�
  TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//ʹ������Ƚ�״̬
  TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;//ʹ�ܻ�������Ƚ�״̬
  TIM_OCInitStructure.TIM_Pulse=0;//��װ�벶��ȽϼĴ���������ֵ
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
  TIM_OC2Init(TIM1,&TIM_OCInitStructure);
  TIM_OC3Init(TIM1,&TIM_OCInitStructure);
  //PWMʹ��
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);//ʹ�ܻ���ʧ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);//ʹ�ܻ���ʧ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);//ʹ�ܻ���ʧ��TIMx��CCR3�ϵ�Ԥװ�ؼĴ���
  
  //ɲ����������
  TIM_BDTRStructure.TIM_AutomaticOutput=TIM_AutomaticOutput_Enable;// �Զ��������ʹ��
  TIM_BDTRStructure.TIM_Break=TIM_Break_Disable;//ʧ��ɲ������
  TIM_BDTRStructure.TIM_BreakPolarity=TIM_BreakPolarity_High; //ɲ������ܽż��Ը�
  TIM_BDTRStructure.TIM_DeadTime=1; //����򿪺͹ر�״̬֮�����ʱ     =1/72Mhz=0.0138us
  TIM_BDTRStructure.TIM_LOCKLevel=TIM_LOCKLevel_OFF;// ����ƽ����: �����κ�λ
  TIM_BDTRStructure.TIM_OSSIState=TIM_OSSIState_Disable; //����������ģʽ�·ǹ���״̬ѡ��
  TIM_BDTRStructure.TIM_OSSRState=TIM_OSSRState_Disable; //����������ģʽ�·ǹ���״̬ѡ��
  TIM_BDTRConfig(TIM1,&TIM_BDTRStructure);
	
  TIM_ARRPreloadConfig(TIM1,ENABLE); //ʹ��TIM8��ARR�ϵ�Ԥװ�ؼĴ���
  TIM_CtrlPWMOutputs(TIM1,ENABLE);//PWMʹ�����
	TIM_Cmd(TIM1,ENABLE);//ʹ�ܻ���ʧ��TIMx���� 
 
}

#include "TIM1.h"


void TIM1_Init(u16  arr,  u16 psc)
{
 	GPIO_InitTypeDef GPIO_InitStructure;   //����һ���ṹ�������������ʼ��GPIO
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//����һ���ṹ�������������ʼ����ʱ��
	TIM_OCInitTypeDef TIM_OCInitStructure;//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx 
  TIM_BDTRInitTypeDef TIM_BDTRStructure;//����ɲ��
  NVIC_InitTypeDef NVIC_InitStructure; //TIM1�ж�����
     
	/* ����ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_TIM1,ENABLE);
 
	/*  ����TIM1��GPIO��ģʽ��IO�� */
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
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;	//TIM���ϼ���ģʽ
  TIM_TimeBaseInitStructure.TIM_RepetitionCounter =0;//���ڼ�����ֵ 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);

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
  
  
  //OC4Ϊ����ADC1ע��ͨ������
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;  
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;                  
  TIM_OCInitStructure.TIM_Pulse =700; 
  
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;         
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;            
  
  TIM_OC4Init(TIM1, &TIM_OCInitStructure); 
 
   
  //ͨ��4�����ж�����
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

//  NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

//  //enable interrupt
//  TIM_ITConfig(TIM1, TIM_IT_CC4, ENABLE);//CCR4���жϣ����ͨ������CCR4��pulse�����Ʋ����ж��൱��PWM-ON��λ��
//  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
//  TIM_ITConfig(TIM1, TIM_IT_COM,ENABLE);

//  TIM_GenerateEvent(TIM1, TIM_EventSource_COM); 

  //PWMʹ��
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);//ʹ�ܻ���ʧ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);//ʹ�ܻ���ʧ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);//ʹ�ܻ���ʧ��TIMx��CCR3�ϵ�Ԥװ�ؼĴ���
  TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);//ʹ�ܻ���ʧ��TIMx��CCR3�ϵ�Ԥװ�ؼĴ���
  
  //ɲ����������
  TIM_BDTRStructure.TIM_AutomaticOutput=TIM_AutomaticOutput_Enable;// �Զ��������ʹ��
  TIM_BDTRStructure.TIM_Break=TIM_Break_Disable;//ʧ��ɲ������
  TIM_BDTRStructure.TIM_BreakPolarity=TIM_BreakPolarity_High; //ɲ������ܽż��Ը�
  TIM_BDTRStructure.TIM_DeadTime=1; //����򿪺͹ر�״̬֮�����ʱ     =1/72Mhz=0.0138us
  TIM_BDTRStructure.TIM_LOCKLevel=TIM_LOCKLevel_OFF;// ����ƽ����: �����κ�λ
  TIM_BDTRStructure.TIM_OSSIState=TIM_OSSIState_Disable; //����������ģʽ�·ǹ���״̬ѡ��
  TIM_BDTRStructure.TIM_OSSRState=TIM_OSSRState_Disable; //����������ģʽ�·ǹ���״̬ѡ��
  TIM_BDTRConfig(TIM1,&TIM_BDTRStructure);
	
  TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);
    /* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);
    //enable interrupt
  TIM_ITConfig(TIM1, TIM_IT_CC4, ENABLE);//CCR4���жϣ����ͨ������CCR4��pulse�����Ʋ����ж��൱��PWM-ON��λ��
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
  TIM_ITConfig(TIM1, TIM_IT_COM,ENABLE);

  /* BLDC_TIMER_NUM counter enable */
  TIM_Cmd(TIM1, ENABLE);

  /* Main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
 
 
}


void TIM1_UP_IRQHandler(void)							//
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
    if(TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);
    }
    if(TIM_GetITStatus(TIM1, TIM_IT_COM) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_COM);
    }
    
     
}

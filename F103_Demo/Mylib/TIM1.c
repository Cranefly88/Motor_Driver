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
 
	/*  ����TIM1��GPIO��ģʽ��IO�� */
	GPIO_InitStructure .GPIO_Pin=GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
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
	TIM_TimeBaseInitStructure.TIM_Period =arr;	   //����Ƶ,PWM Ƶ��=72000/4500=16Khz
	TIM_TimeBaseInitStructure.TIM_Prescaler =psc ;//����������ΪTIMxʱ��Ƶ��Ԥ��Ƶֵ��100Khz����Ƶ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;	//TIM���ϼ���ģʽ
  TIM_TimeBaseInitStructure.TIM_RepetitionCounter =1;//���ڼ�����ֵ 
  TIM_ARRPreloadConfig(TIM1,ENABLE); ////�����Զ���װ�ؼĴ���
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
 
  TIM_SelectSlaveMode(TIM1,TIM_SlaveMode_Trigger);
  TIM_SelectInputTrigger(TIM1,TIM_TS_ITR1);
  
  TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_OC4Ref);  
  TIM_SelectMasterSlaveMode(TIM1,TIM_MasterSlaveMode_Disable);

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
  TIM_OCInitStructure.TIM_Pulse =((PWM_PERIOD_CYCLES) / 2) - (HTMIN);                    
  TIM_OC4Init(TIM1, &TIM_OCInitStructure); 
  
  //PWMʹ��
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);//ʹ�ܻ���ʧ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);//ʹ�ܻ���ʧ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);//ʹ�ܻ���ʧ��TIMx��CCR3�ϵ�Ԥװ�ؼĴ���
  TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);//ʹ�ܻ���ʧ��TIMx��CCR4�ϵ�Ԥװ�ؼĴ���
  
  //ɲ����������
  TIM_BDTRStructure.TIM_AutomaticOutput=TIM_AutomaticOutput_Disable;// �Զ��������ʹ��
  TIM_BDTRStructure.TIM_Break=TIM_Break_Disable;//ʧ��ɲ������
  TIM_BDTRStructure.TIM_BreakPolarity=TIM_BreakPolarity_Low; //ɲ������ܽż��Ը�
  TIM_BDTRStructure.TIM_DeadTime=28; //����򿪺͹ر�״̬֮�����ʱ     =28/72Mhz=0.3864us
  TIM_BDTRStructure.TIM_LOCKLevel=TIM_LOCKLevel_1;// ����ƽ����: �����κ�λ
  TIM_BDTRStructure.TIM_OSSIState=TIM_OSSIState_Enable; //����������ģʽ�·ǹ���״̬ѡ��
  TIM_BDTRStructure.TIM_OSSRState=TIM_OSSRState_Enable; //����������ģʽ�·ǹ���״̬ѡ��
  TIM_BDTRConfig(TIM1,&TIM_BDTRStructure);
	

  //enable interrupt
  TIM_ITConfig(TIM1, TIM_IT_CC4, ENABLE);//CCR4���жϣ����ͨ������CCR4��pulse�����Ʋ����ж��൱��PWM-ON��λ��
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
  TIM_ITConfig(TIM1, TIM_IT_Break,ENABLE);

  TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_OC4Ref);
  TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_Update);
  /* Main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
  TIM_GenerateEvent(TIM1,TIM_EventSource_CC4);   
  TIM_GenerateEvent(TIM1,TIM_EventSource_Update); 
  
    /* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);
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

#include "TIM1.h"

void TIM1_ADC_Init(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_BDTRInitTypeDef TIM_BDTRStructure;
    ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;    
  
    ADC_DeInit();
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_ADC1,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB,ENABLE);
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10; //TIM1_CH1,2,3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//TIM1_CHN1,2,3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;     
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA,&GPIO_InitStructure); 
           
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_TIM1);       
  
    TIM_DeInit(TIM1);
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; //ʱ�ӷָ�
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_CenterAligned1;// ���ϼ���ģʽ
    TIM_TimeBaseInitStructure.TIM_Period=arr; //�Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //Ԥ��Ƶֵ
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter=1; //���ڼ�����ֵ
    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
        
    //PWM����
    TIM_OCInitStruct.TIM_OCIdleState=TIM_OCIdleState_Reset;//��MOE=0��������ȽϿ���״̬
    TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1; //�����ȵ���ģʽ1
    TIM_OCInitStruct.TIM_OCNIdleState=TIM_OCNIdleState_Reset;//��MOE=0��������Ƚ�N����״
    TIM_OCInitStruct.TIM_OCNPolarity=TIM_OCNPolarity_High;// //��������Ƚϼ��Ը�
    TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;//����Ƚϼ��Ը�
    TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;//ʹ������Ƚ�״̬
    TIM_OCInitStruct.TIM_OutputNState=TIM_OutputNState_Enable;//ʹ�ܻ�������Ƚ�״̬
    TIM_OCInitStruct.TIM_Pulse=0;//��װ�벶��ȽϼĴ���������ֵ
    TIM_OC1Init(TIM1,&TIM_OCInitStruct);
    TIM_OC2Init(TIM1,&TIM_OCInitStruct);
    TIM_OC3Init(TIM1,&TIM_OCInitStruct);//����TIM1_OCInitStruct��ָ���Ĳ�����ʼ�� TIM1ͨ��x
  
    //Set ADC sample time 
    TIM_OCInitStruct.TIM_Pulse=0x505;//��װ�벶��ȽϼĴ���������ֵ    
    TIM_OC4Init(TIM1,&TIM_OCInitStruct);
    TIM_ITConfig(TIM1, TIM_IT_CC4, ENABLE); //CCR4���жϣ����ͨ������CCR4��pulse�����Ʋ����ж��൱��PWM-ON��λ��
    //PWMʹ��
    TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);
//   TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);
    //ɲ����������
    TIM_BDTRStructure.TIM_AutomaticOutput=TIM_AutomaticOutput_Enable;// �Զ��������ʹ��
    TIM_BDTRStructure.TIM_Break=TIM_Break_Disable;//ʧ��ɲ������
    TIM_BDTRStructure.TIM_BreakPolarity=TIM_BreakPolarity_High; //ɲ������ܽż��Ը�
    TIM_BDTRStructure.TIM_DeadTime=1; //����򿪺͹ر�״̬֮�����ʱ     =1/84Mhz=0.0119us
    TIM_BDTRStructure.TIM_LOCKLevel=TIM_LOCKLevel_OFF;// ����ƽ����: �����κ�λ
    TIM_BDTRStructure.TIM_OSSIState=TIM_OSSIState_Disable; //����������ģʽ�·ǹ���״̬ѡ��
    TIM_BDTRStructure.TIM_OSSRState=TIM_OSSRState_Disable; //����������ģʽ�·ǹ���״̬ѡ��
    TIM_BDTRConfig(TIM1,&TIM_BDTRStructure);

    TIM_ARRPreloadConfig(TIM1,ENABLE); //ʹ��TIM1��ARR�ϵ�Ԥװ�ؼĴ���

    TIM_Cmd(TIM1,ENABLE);//��TIM1    
    
    TIM_CtrlPWMOutputs(TIM1,ENABLE);//PWMʹ�����

 
    //ADC��ʼ��
    ADC_CommonInitStructure.ADC_Mode=ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler=ADC_Prescaler_Div4;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//DMAʧ��
    ADC_CommonInitStructure.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;
    
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;    
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //��������
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_RisingFalling;    
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigInjecConv_T1_CC4;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

    ADC_Init(ADC1,&ADC_InitStructure);      
      
}



#include "TIM1.h"

void TIM1_Init(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_BDTRInitTypeDef TIM_BDTRStructure;
      
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB,ENABLE);
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;     
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    
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
    TIM_OC3Init(TIM1,&TIM_OCInitStruct);//����TIM8_OCInitStruct��ָ���Ĳ�����ʼ�� TIM8ͨ��x
    //PWMʹ��
    TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);
    
    //ɲ����������
    TIM_BDTRStructure.TIM_AutomaticOutput=TIM_AutomaticOutput_Enable;// �Զ��������ʹ��
    TIM_BDTRStructure.TIM_Break=TIM_Break_Disable;//ʧ��ɲ������
    TIM_BDTRStructure.TIM_BreakPolarity=TIM_BreakPolarity_High; //ɲ������ܽż��Ը�
    TIM_BDTRStructure.TIM_DeadTime=1; //����򿪺͹ر�״̬֮�����ʱ     =1/84Mhz=0.0119us
    TIM_BDTRStructure.TIM_LOCKLevel=TIM_LOCKLevel_OFF;// ����ƽ����: �����κ�λ
    TIM_BDTRStructure.TIM_OSSIState=TIM_OSSIState_Disable; //����������ģʽ�·ǹ���״̬ѡ��
    TIM_BDTRStructure.TIM_OSSRState=TIM_OSSRState_Disable; //����������ģʽ�·ǹ���״̬ѡ��
    TIM_BDTRConfig(TIM1,&TIM_BDTRStructure);

    TIM_ARRPreloadConfig(TIM1,ENABLE); //ʹ��TIM8��ARR�ϵ�Ԥװ�ؼĴ���

    TIM_CtrlPWMOutputs(TIM1,ENABLE);//PWMʹ�����

    TIM_Cmd(TIM1,ENABLE);//��TIM1
    
}



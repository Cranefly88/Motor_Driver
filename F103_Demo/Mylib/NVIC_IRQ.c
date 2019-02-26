#include "NVIC_IRQ.h"

void FOC_NIVC_IRQ(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; //TIM1�ж�����

    //ͨ��4�����ж�����
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 
}

void R3LM1_Init(void)
{
  
    ADC_ITConfig(ADC1,ADC_IT_JEOC,ENABLE);//ENABLE INJECTED INTERRUPT
    ADC_ExternalTrigInjectedConvCmd(ADC1,ENABLE); 
    ADC_ExternalTrigInjectedConvCmd(ADC2,ENABLE);  
    
    ADC_Cmd(ADC1, ENABLE);
    ADC_Cmd(ADC2, ENABLE);
  
    //ʹ��ADC1��λУ׼�Ĵ���
    ADC_ResetCalibration(ADC1);
    ADC_ResetCalibration(ADC2);    
    //���У׼�Ĵ����Ƿ�λ���
    while(ADC_GetResetCalibrationStatus(ADC1));
    while(ADC_GetResetCalibrationStatus(ADC2));   
  
    ADC_SoftwareStartConvCmd(ADC1,ENABLE);
    ADC_SoftwareStartConvCmd(ADC2,ENABLE); 
  
//    ADC_InjectedSequencerLengthConfig(ADC1,0);
//    ADC_InjectedSequencerLengthConfig(ADC2,0);
    
    ADC_StartCalibration(ADC1);
    ADC_StartCalibration(ADC2); 
  
    //���У׼�Ĵ����Ƿ�λ���
    while(ADC_GetResetCalibrationStatus(ADC1)||ADC_GetResetCalibrationStatus(ADC2))
    {
    }  
  /*Enable external trigger fo injected conv of ADC2 (mandatory for Dual mode)*/ 
//    SET_BIT(ADC2->CR2, (ADC_CR2_JSWSTART | ADC_CR2_JEXTTRIG));
     
}

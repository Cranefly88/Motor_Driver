#include "ADC.h"

union adcbuff ADCBUFF;

void Protect_AdcInit(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    //����ADCʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
    //����������
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);    //���� ADC ��Ƶ���� 6   //72M/6=12,ADC ���ʱ�䲻�ܳ��� 14M
  
    GPIO_InitStructure.GPIO_Pin =/*GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|*/GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;  //A0 VSENA A1 VSENB A2 VSENC A3 VSENPVDD A4 ISENA A5 ISENB A6 ISENC  ()
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    ADC_DeInit(ADC1);    //��λ ADC1,������  ADC1  ��ȫ���Ĵ�������Ϊȱʡֵ 
    ADC_InitStructure.ADC_Mode = ADC_Mode_InjecSimult;   //ͬ��ע��ģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_DiscModeCmd(ADC1,DISABLE);    
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_DeInit(ADC2);    //��λ ADC2,������  ADC2  ��ȫ���Ĵ�������Ϊȱʡֵ 
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_DiscModeCmd(ADC2,DISABLE);    
    ADC_Init(ADC2, &ADC_InitStructure);

    //������ע��ͨ��������ע��ͨ�����ó�TIM1��CCR4�¼�����
    ADC_AutoInjectedConvCmd(ADC1,DISABLE);   //�����Զ�ע����ת��
    ADC_InjectedDiscModeCmd(ADC1,DISABLE);   //ʧ�ܲ�����ģʽ
    ADC_SetInjectedOffset(ADC1,ADC_InjectedChannel_1,0);    //У׼ֵ�趨
    ADC_SetInjectedOffset(ADC1,ADC_InjectedChannel_2,0);    //У׼ֵ�趨
    ADC_SetInjectedOffset(ADC1,ADC_InjectedChannel_3,0);    //У׼ֵ�趨    
    ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_T1_CC4);//�������ʹ��time1��CCR4�¼�����������
    ADC_InjectedSequencerLengthConfig(ADC1, 3);
    ADC_InjectedChannelConfig(ADC1,ADC_Channel_6,1,ADC_SampleTime_28Cycles5);
    ADC_InjectedChannelConfig(ADC1,ADC_Channel_5,2,ADC_SampleTime_28Cycles5);
    ADC_InjectedChannelConfig(ADC1,ADC_Channel_4,3,ADC_SampleTime_28Cycles5);

    //ADC2 ע��ͨ����ͬ��ע��
    ADC_AutoInjectedConvCmd(ADC2,DISABLE);   //�����Զ�ע����ת��
    ADC_InjectedDiscModeCmd(ADC2,DISABLE);   //ʧ�ܲ�����ģʽ
    ADC_SetInjectedOffset(ADC2,ADC_InjectedChannel_1,0);    //У׼ֵ�趨
    ADC_SetInjectedOffset(ADC2,ADC_InjectedChannel_2,0);    //У׼ֵ�趨
    ADC_SetInjectedOffset(ADC2,ADC_InjectedChannel_3,0);    //У׼ֵ�趨        
    ADC_InjectedSequencerLengthConfig(ADC2, 3);
    ADC_InjectedChannelConfig(ADC2,ADC_Channel_6,1,ADC_SampleTime_28Cycles5);
    ADC_InjectedChannelConfig(ADC2,ADC_Channel_5,2,ADC_SampleTime_28Cycles5);
    ADC_InjectedChannelConfig(ADC2,ADC_Channel_4,3,ADC_SampleTime_28Cycles5);   
  
  
//    ADC_Cmd(ADC1,ENABLE);
//    ADC_Cmd(ADC2,ENABLE);
        
}   

s32 inj_v1[3];
//int Feedback_Theta=0,SetAdvance_Theta=200;
void ADC1_2_IRQHandler(void)
{
  int16_t wAux1 , wAux2;

    if(ADC_GetITStatus(ADC1,ADC_IT_JEOC) != RESET) 
    {
                                         
        switch(Section)   
        {
          case 1: case 6:   //1=B 2=C
                  wAux1 = (int16_t)( ADC1->JDR1 );
                  wAux2 = (int16_t)( ADC2->JDR1 );          
//                  wAux1 *= 1;  wAux2 *= 1;
//                  wAux1 = (int16_t)( DRV8305.Offset.PhaseBOffset ) - wAux1;
//                  wAux2 = (int16_t)( DRV8305.Offset.PhaseCOffset ) - wAux2;         
                  wAux1 = (int16_t)(limit( wAux1 , -32767 , 32767));
                  wAux2 = (int16_t)(limit( wAux2 , -32767 , 32767));          
                  DRV8305.Clack.Ia = wAux1-16384;
                  DRV8305.Clack.Ib = wAux2-16384;
                  DRV8305.Clack.Ic = 0-wAux1-wAux2+32768;
                  break;
          
          case 2: case 3:  //1=A 2=C
                  wAux1 = (int16_t)( ADC1->JDR1 );
                  wAux2 = (int16_t)( ADC2->JDR1 );          
//                  wAux1 *= 1;  wAux2 *= 1;
//                  wAux1 = (int16_t)( DRV8305.Offset.PhaseAOffset ) - wAux1;
//                  wAux2 = (int16_t)( DRV8305.Offset.PhaseBOffset ) - wAux2;         
                  wAux1 = (int16_t)(limit( wAux1 , -32767 , 32767));
                  wAux2 = (int16_t)(limit( wAux2 , -32767 , 32767));          
                  DRV8305.Clack.Ia = wAux1-16384;
                  DRV8305.Clack.Ib = wAux2-16384;
                  DRV8305.Clack.Ic = 0-wAux1-wAux2+32768;
                  break;

          case 4: case 5:   //1=A 2=B
                  wAux1 = (int16_t)( ADC1->JDR1 );
                  wAux2 = (int16_t)( ADC2->JDR1 );          
//                  wAux1 *= 1;  wAux2 *= 1;
//                  wAux1 = (int16_t)( DRV8305.Offset.PhaseAOffset ) - wAux1;
//                  wAux2 = (int16_t)( DRV8305.Offset.PhaseBOffset ) - wAux2;         
                  wAux1 = (int16_t)(limit( wAux1 , -32767 , 32767));
                  wAux2 = (int16_t)(limit( wAux2 , -32767 , 32767));          
                  DRV8305.Clack.Ia = wAux1-16384;
                  DRV8305.Clack.Ib = wAux2-16384;
                  DRV8305.Clack.Ic = 0-wAux1-wAux2+32768;
                  break;
          
          default :  
             break;                    
        }  
 
        ADC_ClearITPendingBit(ADC1,ADC_IT_JEOC);                                                                                   
    }

}



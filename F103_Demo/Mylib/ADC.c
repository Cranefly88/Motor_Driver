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
    ADC_InitStructure.ADC_NbrOfChannel = 0;
    ADC_DiscModeCmd(ADC1,DISABLE);    
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_DeInit(ADC2);    //��λ ADC2,������  ADC2  ��ȫ���Ĵ�������Ϊȱʡֵ 
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;
    ADC_InitStructure.ADC_NbrOfChannel = 0;
    ADC_DiscModeCmd(ADC2,DISABLE);    
    ADC_Init(ADC2, &ADC_InitStructure);

    //������ע��ͨ��������ע��ͨ�����ó�TIM1��CCR4�¼�����
    ADC_AutoInjectedConvCmd(ADC1,DISABLE);   //�����Զ�ע����ת��
    ADC_InjectedDiscModeCmd(ADC1,DISABLE);   //ʧ�ܲ�����ģʽ
    ADC_SetInjectedOffset(ADC1,ADC_InjectedChannel_1,0);    //У׼ֵ�趨
    ADC_SetInjectedOffset(ADC1,ADC_InjectedChannel_2,0);    //У׼ֵ�趨
    ADC_SetInjectedOffset(ADC1,ADC_InjectedChannel_3,0);    //У׼ֵ�趨    
    ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_T1_CC4);//�������ʹ��time1��CCR4�¼�����������
    ADC_InjectedSequencerLengthConfig(ADC1, 2);
    ADC_InjectedChannelConfig(ADC1,ADC_Channel_4,1,ADC_SampleTime_41Cycles5);
    ADC_InjectedChannelConfig(ADC1,ADC_Channel_6,2,ADC_SampleTime_41Cycles5);
    ADC_ExternalTrigInjectedConvCmd(ADC1,ENABLE); 
    //ADC2 ע��ͨ����ͬ��ע��
    ADC_AutoInjectedConvCmd(ADC2,DISABLE);   //�����Զ�ע����ת��
    ADC_InjectedDiscModeCmd(ADC2,DISABLE);   //ʧ�ܲ�����ģʽ
    ADC_SetInjectedOffset(ADC2,ADC_InjectedChannel_1,0);    //У׼ֵ�趨
    ADC_SetInjectedOffset(ADC2,ADC_InjectedChannel_2,0);    //У׼ֵ�趨
    ADC_SetInjectedOffset(ADC2,ADC_InjectedChannel_3,0);    //У׼ֵ�趨        
    ADC_InjectedSequencerLengthConfig(ADC2, 1);
    ADC_InjectedChannelConfig(ADC2,ADC_Channel_5,1,ADC_SampleTime_41Cycles5);
    
    
    
    ADC_ITConfig(ADC1,ADC_IT_JEOC,ENABLE);//ENABLE INJECTED INTERRUPT
    /* Enable automatic injected conversion start after regular one */
    //  ADC_AutoInjectedConvCmd(ADC1, ENABLE);//�Ƿ��ڹ���ͨ������֮���Զ���ʼע��ͨ������

//    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
    ADC_Cmd(ADC2, ENABLE);

        //ʹ��ADC1��λУ׼�Ĵ���
    ADC_ResetCalibration(ADC1);
    ADC_ResetCalibration(ADC2);    
    //���У׼�Ĵ����Ƿ�λ���
    while(ADC_GetResetCalibrationStatus(ADC1)||ADC_GetResetCalibrationStatus(ADC2))
    {
    }
    
   ADC_ExternalTrigInjectedConvCmd(ADC2,ENABLE);
                
    
}   

s32 inj_v1[3];
//int Feedback_Theta=0,SetAdvance_Theta=200;
void ADC1_2_IRQHandler(void)
{

    if(ADC_GetITStatus(ADC1,ADC_IT_JEOC) != RESET) 
    {
                                         
        switch(Section)   
        {
          case 1: case 6:
                  DRV8305.Clack.Ib = ADC_GetInjectedConversionValue(ADC2,ADC_InjectedChannel_1)-15800;
                  DRV8305.Clack.Ic = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_2)-15897;
                  DRV8305.Clack.Ia = 0-DRV8305.Clack.Ib-DRV8305.Clack.Ic;
                  break;
          
          case 2: case 3:
                  DRV8305.Clack.Ia = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_1)-15796;
                  DRV8305.Clack.Ic = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_2)-15897;
                  DRV8305.Clack.Ib = 0-DRV8305.Clack.Ia-DRV8305.Clack.Ic;
                  break;

          case 4: case 5:
                  DRV8305.Clack.Ia = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_1)-15796;
                  DRV8305.Clack.Ib = ADC_GetInjectedConversionValue(ADC2,ADC_InjectedChannel_1)-15897;
                  DRV8305.Clack.Ic = 0-DRV8305.Clack.Ib-DRV8305.Clack.Ia;
                  break;
          
          default : DRV8305.Clack.Ia = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_1)-15796;
                    DRV8305.Clack.Ib = ADC_GetInjectedConversionValue(ADC2,ADC_InjectedChannel_1)-15897;
                    DRV8305.Clack.Ic = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_2)-15897;
                    break;                    
        }  


      
        ADC_ClearITPendingBit(ADC1,ADC_IT_JEOC);
    }

}

//��ʼ��AD
void Adcdma_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    //---------------------���AD��ʼ��--------------------
    //����DMAʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    //����ADC1ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    //����������
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //DMA1ͨ��1����
    DMA_DeInit(DMA1_Channel1);
    //�����ַ
    DMA_InitStructure.DMA_PeripheralBaseAddr =  (uint32_t)&ADC1->DR;
    //�ڴ��ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCBUFF.ADCConvertedValue;
    //dma���䷽����
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    //����DMA�ڴ���ʱ�������ĳ���
    DMA_InitStructure.DMA_BufferSize = 7;
    //����DMA���������ģʽ��һ������
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //����DMA���ڴ����ģʽ
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //���������ֳ�
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    //�ڴ������ֳ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    //����DMA�Ĵ���ģʽ���������ϵ�ѭ��ģʽ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    //����DMA�����ȼ���
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    //����DMA��2��memory�еı����������
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    //ʹ��ͨ��1
    DMA_Cmd(DMA1_Channel1, ENABLE);

    //ADC1����
    //��������ģʽ
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    //ɨ�跽ʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    //����ת��
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    //�ⲿ������ֹ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    //�����Ҷ���
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    //����ת����ͨ����
    ADC_InitStructure.ADC_NbrOfChannel = 7;
    ADC_Init(ADC1, &ADC_InitStructure);

    //����ģʽͨ������
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_239Cycles5);

    //ʹ��ADC1��DMA
    ADC_DMACmd(ADC1, ENABLE);

    //ʹ��ADC1
    ADC_Cmd(ADC1, ENABLE);

    //ʹ��ADC1��λУ׼�Ĵ���
    ADC_ResetCalibration(ADC1);
    //���У׼�Ĵ����Ƿ�λ���
    while(ADC_GetResetCalibrationStatus(ADC1));

    //��ʼУ׼
    ADC_StartCalibration(ADC1);
    //����Ƿ�У׼���
    while(ADC_GetCalibrationStatus(ADC1));

    //����ADC1�����ת��
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


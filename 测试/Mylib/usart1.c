#include "usart1.h"

u8 USART1_RX_BUF[1000];	//�������ݻ�����
int Voice_distance=0,Voice_distance_last=0,VoiceStart_Flag=0;


void Usart1_Distance_Init(u32 bound)
{  	 
	
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
//  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);//ʹ��FSMCʱ��  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART3ʱ��
	
  //����1���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOB10����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9 ,GPIO_AF_USART1); //GPIOB9����ΪUSART1
	
	//USART1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA1��GPIOA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA2��PA3
	
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); //��ʼ������2
  USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);

  USART_Cmd(USART1, ENABLE);  //ʹ�ܴ��� 1
	USART_ClearFlag(USART1, USART_FLAG_TC);	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//���������ж�
//	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);      //ʹ�ܴ��ڿ����ж�
	
	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

}

u8 j=0;
/*���ڿ����ж�ʹ��DMA  DMA1 Stream1 CH4  USART3_RX */  
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
    USART1_RX_BUF[j] =USART_ReceiveData(USART1); 
		j++;
		if(j==2)
		{ Voice_distance=USART1_RX_BUF[0]<<8|USART1_RX_BUF[1];
			Voice_distance=(int)(LPF2pApply_3(Voice_distance));
//			if(ABS(Voice_distance-Voice_distance_last)>700)
//			Voice_distance=Voice_distance_last;
//      else			
//			Voice_distance_last=Voice_distance;
		  j=0;
    }	
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	
	}

} 

void Usart1_Send_Data()
{
		USART_SendData(USART1, 0xe8);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);//ѭ������,ֱ���������
		DelayUs(40);
		USART_SendData(USART1, 0x02);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);//ѭ������,ֱ���������
		DelayUs(40);
		USART_SendData(USART1, 0xb4);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);//ѭ������,ֱ���������	
	
}




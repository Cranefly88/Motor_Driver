#include "485motor.h"

	u8 Send485buf[10]; 
	u8 rx_rs485buf[10]; 
	u8 SendBuff[SEND_BUF_SIZE];	//�������ݻ�����
  u8 RxBuff[1000];	//�������ݻ�����
   	int16_t DM50_POSTION=0;
	int16_t DM50_SPEED=0;
	int16_t speed=0;
	long int DM50_POSTION_SUM=0;
	
	void RS485_Init(u32 bound)
{  	 
	
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOAʱ��
 // RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);//ʹ��FSMCʱ��  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART2ʱ��
	
  //����2���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10����ΪUSART3TX
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11����ΪUSART3RX
	
	//USART2    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA2��GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PA2��PA3
	
	//PG8���������485ģʽ����  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //GPIOC2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PG8
//	RS485_TX_EN=1;
	USART_OverSampling8Cmd(USART3,ENABLE);
   //USART2 ��ʼ������
	USART_DeInit(USART3); 
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure); //��ʼ������2
  USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);

	
  USART_Cmd(USART3, ENABLE);  //ʹ�ܴ��� 2
	USART_ClearFlag(USART3, USART_FLAG_TC);
	
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//���������ж�
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);      //ʹ�ܴ��ڿ����ж�
	

	//Usart2 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

	
	RS485_TX_EN=0;				//Ĭ��Ϊ����ģʽ	
}

//���ͺ���
void Init_485motor_send(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��

  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}


//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
	u8 sum=0;
	u16 i;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		RS485_TX_EN=1;			//����Ϊ����ģʽ
		DelayUs(10);
		SendBuff[0]= 0xA0;
		SendBuff[1] = (u8)((int16_t)(speed)>>8);
		SendBuff[2] = (u8)((int16_t)(speed));
		SendBuff[3] = (u8)((int16_t)(speed)>>8);
		SendBuff[4] = (u8)((int16_t)(speed));
		SendBuff[5] = (u8)((int16_t)(speed)>>8);
		SendBuff[6] = (u8)((int16_t)(speed));
		SendBuff[7] = (u8)((int16_t)(speed)>>8);
		SendBuff[8] = (u8)((int16_t)(speed));
		for(i=0;i<9;i++)
		{
			sum+=SendBuff[i];
		}
		SendBuff[9]=sum;

		USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA����     
    MYDMA_Enable(DMA1_Stream3,SEND_BUF_SIZE);     //��ʼһ��DMA���䣡	
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}
//���ջ����� 	
u8 RS485_RX_BUF[64];  	//���ջ���,���64���ֽ�.
//���յ������ݳ���
u8 RS485_RX_CNT=0;   
void USART3_IRQHandler(void)
{
	static long int DM50_POSTION_SUM_LAST=0;
	static int16_t DM50_POSTION_LAST=0;
	static long int count=0;
	u16 ii;

	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		DMA_Cmd(DMA1_Stream1,DISABLE);
		ii = USART3->SR;  
		ii = USART3->DR; 
		DM50_POSTION = (int16_t)((RxBuff[1]<<8)|(RxBuff[2]));
		if(DM50_POSTION - DM50_POSTION_LAST<-16384)
			count++;
		if(DM50_POSTION - DM50_POSTION_LAST>16384)
			count--;
		
		DM50_POSTION_SUM = count*32768 + DM50_POSTION;
	
		DM50_SPEED = DM50_POSTION_SUM - DM50_POSTION_SUM_LAST;
		

		DM50_POSTION_LAST = DM50_POSTION;
		DM50_POSTION_SUM_LAST = DM50_POSTION_SUM;
		
		USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
		DMA1_Stream1->NDTR = RX_BUF_SIZE;
		USART_ClearITPendingBit(USART3, USART_IT_IDLE);
		DMA_Cmd(DMA1_Stream1,ENABLE);
		
	}
	
} 
//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
//	RS485_TX_EN=1;			//����Ϊ����ģʽ
  	for(t=0;t<len;t++)		//ѭ����������
	{
	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //�ȴ����ͽ���		
    USART_SendData(USART3,buf[t]); //��������
	}	 
	//while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //�ȴ����ͽ���		
	RS485_RX_CNT=0;	  
//	RS485_TX_EN=0;				//����Ϊ����ģʽ	
}
//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//Ĭ��Ϊ0
	if(rxlen==RS485_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//��¼�������ݳ���
		RS485_RX_CNT=0;		//����
	}
}
#include "485.h"
int Position_485Yaw=0,Position_485Whiff_Last,Position_485Whiff=0,Position_485Pitch;
int Speed_485Whiff=0; 
int START_Position_Yaw=0;
int START_Position_Whiff=0;
int START_Position_Pitch=0;
int16_t DM50_POSITION_Pitch=0;
int16_t DM50_POSITION_Whiff=0;
int16_t DM50_POSITION_Yaw=0;
//���ջ����� 	
u8 RS485_RX_BUF[64];  	//���ջ���,���64���ֽ�.
//���յ������ݳ���
u8 RS485_RX_CNT=0;   

u8 SendBuff[SEND_BUF_SIZE];	//�������ݻ�����
u8 RxBuff[1000];	//�������ݻ�����
u8 SendBuff[SEND_BUF_SIZE];	//�������ݻ�����


void RS485_Init(u32 bound)
{  	 
	
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOAʱ��
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);//ʹ��FSMCʱ��  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��
	
  //����3���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART2); //GPIOB10����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART2); //GPIOB11����ΪUSART3
	
	//USART3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; //GPIOA2��GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PA2��PA3
	
	//PC2���������485ģʽ����  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //GPIOC2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PG8
//	RS485_TX_EN=1;
	USART_OverSampling8Cmd(USART3,ENABLE);
   //USART2 ��ʼ������
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

	
	RS485_RX_EN;				//Ĭ��Ϊ����ģʽ	
}

int dif_yaw = 0;
/*���ڿ����ж�ʹ��DMA  DMA1 Stream1 CH4  USART3_RX */  
void USART3_IRQHandler(void)
{
	u16 ii;
  u8 sum_Pitch=0, sum_Yaw=0,sum_Whiff=0;
	static int16_t DM50_POSITION_Yaw_Last,DM50_POSITION_Pitch_Last,DM50_POSITION_Whiff_Last;
	static _Bool zero_Yaw_flag=0,start_Yaw_flag=0;
	static _Bool zero_Whiff_flag=0,start_Whiff_flag=0;
	static _Bool zero_Pitch_flag=0,start_Pitch_flag=0;
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		DMA_Cmd(DMA1_Stream1,DISABLE);

		ii = USART3->SR;  
		ii = USART3->DR;
    if(RxBuff[0]==0xB4)		
		{ 
			sum_Yaw=RxBuff[0]+RxBuff[1]+RxBuff[2];
			if(RxBuff[3]==sum_Yaw)
      {	
			DM50_POSITION_Yaw_Last=DM50_POSITION_Yaw; 				
			DM50_POSITION_Yaw= (int16_t)((RxBuff[1]<<8)|(RxBuff[2]));
			}
			else
			DM50_POSITION_Yaw=DM50_POSITION_Yaw_Last;
					
			if((DM50_POSITION_Yaw-DM50_POSITION_Yaw_Last)>15000)	
			Position_485Yaw+=DM50_POSITION_Yaw-DM50_POSITION_Yaw_Last-32767;	
			else if((DM50_POSITION_Yaw-DM50_POSITION_Yaw_Last)<-15000)						
			Position_485Yaw+=DM50_POSITION_Yaw-DM50_POSITION_Yaw_Last+32767;	
			else
			Position_485Yaw+=DM50_POSITION_Yaw-DM50_POSITION_Yaw_Last;

      if(Position_485Yaw>=(MID_YAW_ANGLE+16384)) 
      Position_485Yaw-=32767;
      else if(Position_485Yaw<=(MID_YAW_ANGLE-16384)) 
      Position_485Yaw+=32767;        

      
			if(DM50_POSITION_Yaw_Last==0&&zero_Yaw_flag==0)
			Position_485Yaw=DM50_POSITION_Yaw;
		
			DM50_POSITION_Yaw_Last=DM50_POSITION_Yaw;	  //mid   31318
			zero_Yaw_flag=1;		
		}
		
		if(RxBuff [0]==0xB2)		
		{ 
			sum_Pitch=RxBuff[0]+RxBuff[1]+RxBuff[2];
			if(RxBuff[3]==sum_Pitch)
			{
			DM50_POSITION_Pitch_Last=DM50_POSITION_Pitch;				
			DM50_POSITION_Pitch = (int16_t)((RxBuff[1]<<8)|(RxBuff[2]));
			}
		}	

		if(RxBuff [0]==0xB1)		
		{ 
			sum_Whiff=RxBuff[0]+RxBuff[1]+RxBuff[2];
			if(RxBuff[3]==sum_Whiff)
			{
			DM50_POSITION_Whiff_Last=DM50_POSITION_Whiff;				
			DM50_POSITION_Whiff = (int16_t)((RxBuff[1]<<8)|(RxBuff[2]));
			}
			else
			DM50_POSITION_Whiff=DM50_POSITION_Whiff_Last;

			if((DM50_POSITION_Whiff-DM50_POSITION_Whiff_Last)>15000)	
			Position_485Whiff+=DM50_POSITION_Whiff-DM50_POSITION_Whiff_Last-32767;	
			else if((DM50_POSITION_Whiff-DM50_POSITION_Whiff_Last)<-15000)						
			Position_485Whiff+=DM50_POSITION_Whiff-DM50_POSITION_Whiff_Last+32767;	
			else
			Position_485Whiff+=DM50_POSITION_Whiff-DM50_POSITION_Whiff_Last;	

			DM50_POSITION_Whiff_Last=DM50_POSITION_Whiff;	  //mid   31744			
		}			
	
		USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
		DMA1_Stream1->NDTR = RX_BUF_SIZE;
		USART_ClearITPendingBit(USART3, USART_IT_IDLE);
		DMA_Cmd(DMA1_Stream1,ENABLE);	
	}
	
} 

void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
//	RS485_TX_EN=1;			//����Ϊ����ģʽ
  	for(t=0;t<len;t++)		//ѭ����������
	{
	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //�ȴ����ͽ���		
    USART_SendData(USART3,buf[t]); //��������
	}	 
	RS485_RX_CNT=0;	  
//	RS485_TX_EN=0;				//����Ϊ����ģʽ	
}



//DMAx�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMAͨ��ѡ��,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:�����ַ
//mar:�洢����ַ
//ndtr:���ݴ�����  
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
{ 
 
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	if((u32)DMA_Streamx>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 
		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1ʱ��ʹ�� 
	}
  DMA_DeInit(DMA_Streamx);
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//�ȴ�DMA������ 

	/*����dma NVIC����*/
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;                   //����DMA   Stream3 CH4 USART3_TX
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
		
	/*����DMA����*/
	DMA_DeInit(DMA1_Stream1);
	DMA_InitStructure.DMA_Channel= DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)RxBuff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = RX_BUF_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream1,&DMA_InitStructure);
	DMA_Cmd(DMA1_Stream1,ENABLE);

		
  /* ���� DMA Stream */
  DMA_InitStructure.DMA_Channel = chx;  //ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//�洢��������ģʽ
  DMA_InitStructure.DMA_BufferSize = ndtr;//���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ʹ����ͨģʽ 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//�е����ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
  DMA_Init(DMA_Streamx, &DMA_InitStructure);//��ʼ��DMA Stream
	DMA_ITConfig(DMA_Streamx,DMA_IT_TC,ENABLE);

} 

//����һ��DMA����
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:���ݴ�����  
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
 
	DMA_Cmd(DMA_Streamx, DISABLE);                      //�ر�DMA���� 
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//ȷ��DMA���Ա�����  
		
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //���ݴ����� 
		
	DMA_Cmd(DMA_Streamx, ENABLE);                      //����DMA���� 
	

}	  
int pro=0;

 void DMA1_Stream3_IRQHandler(void)
 {
		if(DMA_GetFlagStatus(DMA1_Stream3,DMA_FLAG_TCIF3)!=RESET)//�ȴ�DMA1_Steam3�������
		{ 
			DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3);//���DMA1_Steam3������ɱ�־
//			LED1=!LED1;//DS1��ת
			pro=DMA_GetCurrDataCounter(DMA1_Stream3);		
			DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);
			DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_TCIF3);
			DelayUs(20);
			RS485_RX_EN;				//����Ϊ����ģʽ
		}
	 
 }



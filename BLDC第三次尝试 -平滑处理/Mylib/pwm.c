#include "main.h"

//PA8----EN
//PA9----PWM---TIM1_CH2
//PA10---EN
//PA11---PWM---TIM1_CH4

/*************************************************************************
                              PWM��ʼ��       
*************************************************************************/
void PWM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  tim;
	TIM_OCInitTypeDef        oc;
  GPIO_InitTypeDef         gpio;
    
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);	
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE);	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1, ENABLE);	
	
  gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_9;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);
    
	tim.TIM_Period=5000;
	tim.TIM_Prescaler=0;
	tim.TIM_ClockDivision=TIM_CKD_DIV1;
	tim.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &tim);
	TIM_TimeBaseInit(TIM1, &tim);
	
	oc.TIM_OCMode = TIM_OCMode_PWM2;
  oc.TIM_OutputState = TIM_OutputState_Enable;
  oc.TIM_OutputNState = TIM_OutputNState_Disable;
	oc.TIM_Pulse = 0;
	oc.TIM_OCPolarity = TIM_OCPolarity_Low;
  oc.TIM_OCNPolarity = TIM_OCPolarity_High;
  oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
  oc.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM2, &oc);
  TIM_OC2Init(TIM2, &oc);
	TIM_OC2Init(TIM1, &oc);
	
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	
  TIM_CtrlPWMOutputs(TIM2,ENABLE);
  TIM_CtrlPWMOutputs(TIM1,ENABLE);

	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
	TIM1->CCR2=1000;
}
void INH_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(INH_RCC, ENABLE);	 //ʹ�ܶ˿�ʱ��
 GPIO_InitStructure.GPIO_Pin = INH_U | INH_V;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(INH_PORT, &GPIO_InitStructure);					 //�����趨������ʼ��

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ�ܶ˿�ʱ��
 GPIO_InitStructure.GPIO_Pin = INH_W;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��
 INH_U_OFF;
 INH_V_OFF;
 INH_W_OFF;
	
 //INH_U_ON;
 //INH_V_ON;
// INH_W_ON;
}

void BLDC_U(int16_t value)
{
	value = LIMIT(value,0,MAX_PWM_BLDC);
	TIM2->CCR1 = value;
}

void BLDC_V(int16_t value)
{
	value = LIMIT(value,0,MAX_PWM_BLDC);
	TIM2->CCR2 = value;
}

void BLDC_W(int16_t value)
{
	value = LIMIT(value,0,MAX_PWM_BLDC);
	TIM1->CCR2 = value;
}
/*************************************************************************
                              ��ѹ���       
*************************************************************************/
void PWM_OutSet(int value)
{
	int output = limit(value, -3000, 3000);						//����ֵ�޷�
	
	if(output > 0)																		//���ݸ���ֵ�����������������ͨ��
	{
		TIM2->CCR1 = output;
		TIM2->CCR2 = 0;
	}
	else
	{
		TIM2->CCR1 = 0;
		TIM2->CCR2 = -output;
	}
}

int Commutation(int pos)
{
	int i=0,pos2step=0;
	int step=-1;
	if(BLDC_DIR_FLG == 0)						 //�ж���ת����
	{
		if(pos<BLDC_HALL_POS[65]*6||pos>=BLDC_HALL_POS[0]*6)                            //��е�Ƕ���10-3533֮��
		{
					if(pos>3552.5*6)   //3576.5    90��
						return 4;
					else
						return -1;                //�ɹ���λ����ûԽ������㣻
		}
	}
	if(BLDC_DIR_FLG == 1)						 //�ж���ת����
	{
		if(pos<=BLDC_HALL_POS[65]*6||pos>BLDC_HALL_POS[0]*6)                            //��е�Ƕ���10-3533֮��
		{
			if(pos<3600.5*6&&pos>3532*6)
				return 1;
			else
				return -1;                //�ɹ���λ����ûԽ������㣻
		}
	}
	while(i<65)                                                                //��е�Ƕ���3533-10֮�䣬����Ϊ66������
	{
		if(BLDC_DIR_FLG == 0)						 //�ж���ת����
		{
			if(BLDC_HALL_POS[i+1]*6<=pos&&pos<BLDC_HALL_POS[i]*6)
			{
					if(pos>=(BLDC_HALL_POS[i]*6-(BLDC_HALL_POS[i]*6-BLDC_HALL_POS[i+1]*6)/CHANGE_POINT))
					{
						pos2step = i-1;
						if(pos2step<-1||pos2step==-1)
							pos2step=pos2step+66;
						step = BLDC_POS2STEP[pos2step];
						return step;
					}
					else
						return -1;                //�ɹ���λ����ûԽ������㣻
			}
		}
		if(BLDC_DIR_FLG == 1)						 //�ж���ת����
		{
			if(BLDC_HALL_POS[i+1]*6<pos&&pos<=BLDC_HALL_POS[i]*6)
		{
				if(pos<=(BLDC_HALL_POS[i]*6-(BLDC_HALL_POS[i]*6-BLDC_HALL_POS[i+1]*6)/(4-CHANGE_POINT)))       //�ж��Ƿ�ת��30�����Ƕ�
				{
					pos2step = i+2;
					if(pos2step>66||pos2step==66)
						pos2step=pos2step-66;
					step = BLDC_POS2STEP[pos2step];
					return step;
				}
				else
					return -1;                //�ɹ���λ����ûԽ������㣻
			}
		}
		i++;
	}
	return 0;  //error!!!
	
}

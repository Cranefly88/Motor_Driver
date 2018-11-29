#include "main.h"
int BLDC_POSTION=0; 
int temp_max =0;
int temp_min =6000;
int BLDC_step=1;
int BLDC_pwm=500;
int BLDC_flag=0;
_Bool BLDC_DIR_FLG=0;  
int BLDC_POS_DIFF=0;;
//     A    B    C
//     U    V    W

// 1   UV
// 2   UW
// 3   VW
// 4   VU
// 5   WU
// 6   WV

//UW
//WV
//UV
//VU
//VW
//WU
 

// 172	110	46	10	3532	3472	3429	3364	3310	3263	3199	
//
int main(void)
{
		static u32 BLDC_POSTION_last=0;
		static int BLDC_flag_last=0;
		PWM_Configuration();		  //pwm����
	
		INH_Init();
	
		Load_Parameters();				//���ز���

		TIM3_Cap_Init(0XFFFF,3-1);	//��1Mhz��Ƶ�ʼ��� 
	
		KEY_Configuration();

		SysTick_Init();						//�δ�ʱ����ʼ��
	
		LPF2pSetCutoffFreq_1(1000,25);
	
	
    while(1)
    {
				runFlag.shootFlag = 1;
				if(TIM3CH3_CAPTURE_STA&0X80)//�ɹ�������һ��������
				{
					BLDC_POSTION=TIM3CH3_CAPTURE_STA&0X3F;
					BLDC_POSTION*=65536;//���ʱ���ܺ�
					BLDC_POSTION+=TIM3CH3_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
					TIM3CH3_CAPTURE_STA=0;//������һ�β���
					if(BLDC_POSTION>24000)
						BLDC_POSTION=BLDC_POSTION_last;
					BLDC_POS_DIFF = BLDC_POSTION-BLDC_POSTION_last;
					if(BLDC_POSTION>temp_max)
						temp_max=BLDC_POSTION;
					if(BLDC_POSTION<temp_min)
						temp_min=BLDC_POSTION;
					BLDC_POSTION_last=BLDC_POSTION;
			}
				
			BLDC_flag=Commutation(BLDC_POSTION);
			if(BLDC_flag ==-1)
				BLDC_flag=BLDC_flag_last;
			BLDC_flag_last=BLDC_flag;
			if(BLDC_flag!=0)
				BLDC_step=BLDC_flag;
				switch(BLDC_step)
				{
					case 0:                //ֹͣ
						INH_U_OFF;
						INH_V_OFF;
						INH_W_OFF;
						BLDC_U(0);
						BLDC_V(0);
						BLDC_W(0);
						break;
					
					case 1:									//UV
						BLDC_U(BLDC_pwm);
						BLDC_V(0);
						BLDC_W(0);
						INH_U_ON;
						INH_V_ON;
						INH_W_OFF;
						break;
					
					case 2:									//UW
						BLDC_U(BLDC_pwm);
						BLDC_V(0);
						BLDC_W(0);
						INH_U_ON;
						INH_V_OFF;
						INH_W_ON;
						break;
					
					case 3:									//VW
						BLDC_U(0);
						BLDC_V(BLDC_pwm);
						BLDC_W(0);
						INH_U_OFF;
						INH_V_ON;
						INH_W_ON;
						break;
					
					case 4:									//VU
						BLDC_U(0);
						BLDC_V(BLDC_pwm);
						BLDC_W(0);
						INH_U_ON;
						INH_V_ON;
						INH_W_OFF;
						break;
					
					case 5:									//WU
						BLDC_U(0);
						BLDC_V(0);
						BLDC_W(BLDC_pwm);
						INH_U_ON;
						INH_V_OFF;
						INH_W_ON;
						break;
	
					case 6:									//WV
						BLDC_U(0);
						BLDC_V(0);
						BLDC_W(BLDC_pwm);
						INH_U_OFF;
						INH_V_ON;
						INH_W_ON;
						break;
				}
				
				

    }
}




//				switch(BLDC_step)
//				{
//					case 0:                //ֹͣ
//						INH_U_OFF;
//						INH_V_OFF;
//						INH_W_OFF;
//						BLDC_U(0);
//						BLDC_V(0);
//						BLDC_W(0);
//						break;
//					
//					case 1:									//UW
//						BLDC_U(BLDC_pwm);
//						BLDC_V(0);
//						BLDC_W(0);
//						INH_U_ON;
//						INH_V_OFF;
//						INH_W_ON;
//						break;
//					
//					case 2:									//WV
//						BLDC_U(0);
//						BLDC_V(0);
//						BLDC_W(BLDC_pwm);
//						INH_U_OFF;
//						INH_V_ON;
//						INH_W_ON;
//						break;
//					
//					case 3:									//UV
//						BLDC_U(BLDC_pwm);
//						BLDC_V(0);
//						BLDC_W(0);
//						INH_U_ON;
//						INH_V_ON;
//						INH_W_OFF;
//						break;
//					
//					case 4:									//VU
//						BLDC_U(0);
//						BLDC_V(BLDC_pwm);
//						BLDC_W(0);
//						INH_U_ON;
//						INH_V_ON;
//						INH_W_OFF;
//						break;
//					
//					case 5:									//VW
//						BLDC_U(0);
//						BLDC_V(BLDC_pwm);
//						BLDC_W(0);
//						INH_U_OFF;
//						INH_V_ON;
//						INH_W_ON;
//						break;
//	
//					case 6:									//WU
//						BLDC_U(0);
//						BLDC_V(0);
//						BLDC_W(BLDC_pwm);
//						INH_U_ON;
//						INH_V_OFF;
//						INH_W_ON;
//						break;
//				}

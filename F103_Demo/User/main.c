#include "main.h"

//��ôű�������Ӧ�Ļ�е��0�ȣ���Ӧ��Ƕ�=>252�ȣ�����1�ȣ�����е�Ƕ��ǵ�Ƕȵ�14����
//���е�Ƕ����ǶȵĻ����ϵΪ����Ƕ�=[����е�Ƕ�/��16384/14����*360+252]%360
u16 DRV=0,Check=0xBFFF;
int FOC_T=500;
long Change=0;
int Feedback_Theta=0,SetAdvance_Theta=0;
int main(void)
{

    Board_ALL_Init();

    while(1)
    { 
      
//  DRV8305_SCS_L;        
//  DRV=SPI1_ReadWriteByte(Check); 
//  DelayMs(100);      
//  DRV8305_SCS_H;
//  DelayUs(1);
     
//  DelayUs(100);
    Feedback_Theta=((int)(as5048_A.reg/(16384.0/14.0)*360.0+252))%360;  
      
    DRV8305.Park.Theta=Feedback_Theta*10+SetAdvance_Theta;
    if(DRV8305.Park.Theta>3599)
    DRV8305.Park.Theta=0;    

    Anti_Park_Calc();     
    Svpwm_Module();      
    }
}     


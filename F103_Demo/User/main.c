#include "main.h"

//��ôű�������Ӧ�Ļ�е��0�ȣ���Ӧ��Ƕ�=>165�ȣ�����1�ȣ�����е�Ƕ��ǵ�Ƕȵ�14����
//���е�Ƕ����ǶȵĻ����ϵΪ����Ƕ�=[����е�Ƕ�/��16384/14����*360+165]%360
//��while��1����һ��ѭ��96us������schedule��100us�ж���
int FOC_T=500;
long Change=0;
int main(void)
{

  Board_ALL_Init(); 
  
    while(1)
    {    
 
//  as5048_singelread_angle();  
//  
//  Feedback_Theta=((int)(as5048_A.reg/(16384.0/14.0)*360.0+42))%360;  //��е�ǶȻ���Ƕ�
//        
//  DRV8305.Park.Theta=Feedback_Theta*10+SetAdvance_Theta;
//      
//  if(DRV8305.Park.Theta>3599)
//  DRV8305.Park.Theta=0; 
//  else if(DRV8305.Park.Theta<0)
//  DRV8305.Park.Theta=3599;  

//  Clarke_Trans();
// // Park_Trans();     
//  Anti_Park_Calc();     
//  Svpwm_Module(); 

    
    }
}     


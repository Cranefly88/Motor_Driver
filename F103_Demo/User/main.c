#include "main.h"

//��ôű�������Ӧ�Ļ�е��0�ȣ���Ӧ��Ƕ�=>42�ȣ�����1�ȣ�����е�Ƕ��ǵ�Ƕȵ�14����
//���е�Ƕ����ǶȵĻ����ϵΪ����Ƕ�=[����е�Ƕ�/��16384/14����*360+42]%360
//��while��1����һ��ѭ��96us������schedule��100us�ж���
u16 DRV=0,Set_80V=0x503F,Set_40V=0x502A,Set_20V=0x5015;
int FOC_T=500;
long Change=0;
int main(void)
{

   Board_ALL_Init();

  DRV8305_SCS_L;        
  DRV=SPI1_ReadWriteByte(Set_80V);   //���ó�80V����
  DelayMs(100);      
  DRV8305_SCS_H;
  DelayUs(1);
     
  DelayUs(100);  
  
    while(1)
    {        
    

    
    }
}     


#include "Distance_Voice.h"


//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 KS103_ReadOneByte(u8 address, u8 reg)
{				  
	u8 temp=0;		  	    																 
    I2C2_Soft_Start();  
    I2C2_Soft_SendByte(address);   //���͵͵�ַ
	I2C2_Soft_WaitAck();	 
	I2C2_Soft_SendByte(reg);   //���͵͵�ַ
	I2C2_Soft_WaitAck();	   
	I2C2_Soft_Start();  	 	   
	I2C2_Soft_SendByte (address + 1);           //�������ģʽ			   
	I2C2_Soft_WaitAck();	 

	I2C2_Soft_Delay();
//	delay_us(50);	   //���Ӵ˴���ͨ�ųɹ�������
    temp=I2C2_Soft_ReadByte();	  //���Ĵ���3	   
    I2C2_Soft_Stop();//����һ��ֹͣ����	    
	return temp;
}

void KS103_WriteOneByte(u8 address,u8 reg,u8 command)
{				   	  	    																 
  I2C2_Soft_Start();   
	I2C2_Soft_SendByte(address);	    //����д����
	I2C2_Soft_WaitAck();
	I2C2_Soft_SendByte(reg);//���͸ߵ�ַ	  
	I2C2_Soft_WaitAck();	   
  I2C2_Soft_SendByte(command);   //���͵͵�ַ
	I2C2_Soft_WaitAck(); 	 										  		   
  I2C2_Soft_Stop();//����һ��ֹͣ���� 
//	delay_ms(80);	 
}

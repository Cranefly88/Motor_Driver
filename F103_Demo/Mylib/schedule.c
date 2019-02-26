#include "schedule.h"
long Time_1ms=0;
int Feedback_Theta=0,SetAdvance_Theta=200;
void TDT_Loop_10000Hz(void)//100usִ��һ��
{
 	float loop_time_10000hz;
	loop_time_10000hz = Get_Cycle_T(2);     /*��ȡ100us׼ȷʱ��*/
  
  as5048_singelread_angle();  
  
  Feedback_Theta=((int)(as5048_A.reg/(16384.0/14.0)*360.0+42))%360;  //��е�ǶȻ���Ƕ�
        
  DRV8305.Park.Theta=Feedback_Theta*10+SetAdvance_Theta;
    
  if(DRV8305.Park.Theta>3599)
  DRV8305.Park.Theta=0; 
  else if(DRV8305.Park.Theta<0)
  DRV8305.Park.Theta=3599;  

  Anti_Park_Calc();     
  Svpwm_Module();          
  

}

void TDT_Loop_1000Hz(void)//1msִ��һ��
{
 	float loop_time_1000hz;
	loop_time_1000hz = Get_Cycle_T(1);     /*��ȡ1ms׼ȷʱ��*/


}

void TDT_Loop_500Hz(void)	//2msִ��һ��
{	
	float loop_time_500hz;
	loop_time_500hz = Get_Cycle_T(0);     //��ȡ2ms׼ȷʱ��

}
 

void TDT_Loop_200Hz(void)	//5msִ��һ��
{

  

}   


void TDT_Loop_100Hz(void)	//10msִ��һ��
{
	float loop_time_100hz;
	loop_time_100hz = Get_Cycle_T(2);     //��ȡ10ms׼ȷʱ��  
}


void TDT_Loop_50Hz(void)	//20msִ��һ��
{

}

void TDT_Loop_20Hz(void)	//50msִ��һ��
{
   static u8 timer_50ms = 0;//��¼50ms����
	 if(++timer_50ms > 10)
	 {
		  timer_50ms = 0;
  		LED_TOGGLE;  //�����ƣ�1s��˸1��
//		  LED_GREEN_TOGGLE; 
         
	 }
}


void TDT_Loop(schedule* robotSchdule)
{
  	if(robotSchdule->cnt_100us >= 1)
	{
		TDT_Loop_10000Hz();	
		robotSchdule->cnt_100us = 0;
	}
	if(robotSchdule->cnt_1ms >= 10)
	{
		TDT_Loop_1000Hz();	
		robotSchdule->cnt_1ms = 0;
	}
	if(robotSchdule->cnt_2ms >= 20)
	{
		TDT_Loop_500Hz();
		robotSchdule->cnt_2ms = 0;
	}		
	if(robotSchdule->cnt_5ms >= 50)
	{	
		TDT_Loop_200Hz();
		robotSchdule->cnt_5ms = 0;
	}
	if(robotSchdule->cnt_10ms >= 100)
	{		
		TDT_Loop_100Hz();
		robotSchdule->cnt_10ms = 0;
	}
	if(robotSchdule->cnt_20ms >= 200)
	{		
		TDT_Loop_50Hz();
		robotSchdule->cnt_20ms = 0;
	}
	if(robotSchdule->cnt_50ms >= 500)
	{		
		TDT_Loop_20Hz();
		robotSchdule->cnt_50ms = 0;
	}
}

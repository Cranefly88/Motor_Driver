#include "schedule.h"
void TDT_Loop_1000Hz(void)//1msִ��һ��
{
 	float loop_time_1000hz;
	loop_time_1000hz = Get_Cycle_T(1);     /*��ȡ5ms׼ȷʱ��*/
  as5048_singelread_angle();

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
	if(robotSchdule->cnt_1ms >= 1)
	{
		TDT_Loop_1000Hz();	
		robotSchdule->cnt_1ms = 0;
	}
	if(robotSchdule->cnt_2ms >= 2)
	{
		TDT_Loop_500Hz();
		robotSchdule->cnt_2ms = 0;
	}		
	if(robotSchdule->cnt_5ms >= 5)
	{	
		TDT_Loop_200Hz();
		robotSchdule->cnt_5ms = 0;
	}
	if(robotSchdule->cnt_10ms >= 10)
	{		
		TDT_Loop_100Hz();
		robotSchdule->cnt_10ms = 0;
	}
	if(robotSchdule->cnt_20ms >= 20)
	{		
		TDT_Loop_50Hz();
		robotSchdule->cnt_20ms = 0;
	}
	if(robotSchdule->cnt_50ms >= 50)
	{		
		TDT_Loop_20Hz();
		robotSchdule->cnt_50ms = 0;
	}
}

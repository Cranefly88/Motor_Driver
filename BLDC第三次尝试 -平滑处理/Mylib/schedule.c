#include "main.h"

struct _schedule schedule;

int BLDC_SPEED=0;
int BLDC_SPEED_DIFF=0;
float BLDC_SPEED_FILT=0;
int BLDC_SPEED_FILT_scop=0;
void Loop_1000Hz(void)	//1msִ��һ��
{
	static int now_postion=0;
	static int last_postion=0;
	static int BLDC_SPEED_last=0;
	static int32_t FILT_BUF[SPEED_FILT_NUM]={0};
	int32_t FILT_TMP=0;
	int i;
	now_postion = BLDC_POSTION;
	BLDC_SPEED = now_postion - last_postion;
	if(ABS(BLDC_SPEED - BLDC_SPEED_last)>400)
		BLDC_SPEED = BLDC_SPEED_last;
	BLDC_SPEED_DIFF = BLDC_SPEED - BLDC_SPEED_last;
  for(i=SPEED_FILT_NUM-1;i>=1;i--)
  {
    FILT_BUF[i] = FILT_BUF[i-1];
  }
	FILT_BUF[0] = BLDC_SPEED;
	for(i=0;i<SPEED_FILT_NUM;i++)
	{
		FILT_TMP += FILT_BUF[i];
	}
	BLDC_SPEED_FILT= (float)( FILT_TMP )/(float)SPEED_FILT_NUM;
	BLDC_SPEED_FILT = LPF2pApply_1(BLDC_SPEED_FILT);
	
	BLDC_SPEED_FILT_scop = BLDC_SPEED_FILT*10;
	last_postion = now_postion;
	BLDC_SPEED_last = BLDC_SPEED;
	
	BLDC_Control(1);

	
//		//ÿ��֮���ʱ��������ʱ��������
//		if(!runFlag.shootAllow)
//		{
//			if(schedule.sysTime_ms - schedule.beatStart_ms > runFlag.beatTime)
//				runFlag.shootAllow = 1;
//		}
//		
//		//������ʱ�����յ��������صķ�����������Ŀ�ʼ���ѷ�����ӵ������㣬����¼�����Ŀ�ʼ��ʱ�䣩
//		if(runFlag.shootAllow && runFlag.shootFlag)
//		{
//			runFlag.bulletFired = 0;
//			runFlag.shootAllow = 0;
//			schedule.beatStart_ms = schedule.sysTime_ms;
//		}
}

void Loop_500Hz(void)	 //2msִ��һ��
{	
}
int scop_sample=0;
int BLDC_postion[1024];
void Loop_200Hz(void)	 //5msִ��һ��
{  
//	static int num=0;
//	static int timer_5ms = 0;//��¼50ms����
//	static int timer_5ms_1 = 0;//��¼50ms����
//	static _Bool flag_delay=0;
//	 if(timer_5ms > 100)
//	 {
//		  timer_5ms = 0;
//		 	BLDC_step++;
//		 if(BLDC_step>6)
//			 BLDC_step=1;
//		 timer_5ms_1=0;
//		 flag_delay=0;
//	 }
//	 if(timer_5ms_1>75&&flag_delay==0)
//	 {
//		 BLDC_postion[num]=BLDC_POSTION;
//		 scop_sample=BLDC_POSTION;
//		 flag_delay=1;
//		 num++;
//	 }
//	 timer_5ms++;
//	timer_5ms_1++;
}

void Loop_100Hz(void)  //10msִ��һ��5
{
//		//��������λ�ã�0-749��
//		encoder.position = encoder.totalPulse%1280;
//		//λ�ÿ���
//		 Position_Control(runFlag.positionIndex);
//		//���pwm����
//		PWM_OutSet(-950);
}

void Loop_50Hz(void)	//20msִ��һ��
{

}

void Loop_20Hz(void)	//50msִ��һ��
{

}

void Loop_10Hz(void)	//100msִ��һ��
{
//	static int timer_100ms = 0;//��¼50ms����
//	int deta=40;
//	static int flag=1;
//	if(++timer_100ms > 10)
//	{
//	  timer_100ms = 0;
//		BLDC_set_speed=0 + flag*deta;
//		flag=-flag;
//	}
//		static int timer_100ms = 0;//��¼50ms����
//	int deta=2000;
//	static int flag=1;
//	if(++timer_100ms > 10)
//	{
//	  timer_100ms = 0;
//		set_postion=5000 + flag*deta;
//		flag=-flag;
//	}
	

	
//	  //��������ķ��͵��ӵ�����û����ɣ�������λ�ü�1��������ӵ�����1
//		if(runFlag.bulletFired < runFlag.shootMode)
//		{
//			runFlag.positionIndex++;
//			runFlag.bulletFired++;
//		}
//		
//		//һȦ7���ӵ�
//		if(runFlag.positionIndex >= 5)	runFlag.positionIndex = 0;
}

void Loop(void)
{
	if(schedule.cnt_1ms >= 1)
	{
		Loop_1000Hz();	
		schedule.cnt_1ms = 0;
	}
	if(schedule.cnt_2ms >= 2)
	{
		Loop_500Hz();
		schedule.cnt_2ms = 0;
	}
	if(schedule.cnt_5ms >= 5)
	{
		Loop_200Hz();
		schedule.cnt_5ms = 0;
	}
	if(schedule.cnt_10ms >= 10)
	{
		Loop_100Hz();
		schedule.cnt_10ms = 0;
	}
	if(schedule.cnt_20ms >= 20)
	{
		Loop_50Hz();
		schedule.cnt_20ms = 0;
	}
	if(schedule.cnt_50ms >= 50)
	{
		Loop_20Hz();
		schedule.cnt_50ms = 0;
	}
	if(schedule.cnt_100ms >= 100)
	{
		Loop_10Hz();
		schedule.cnt_100ms = 0;
	}
}

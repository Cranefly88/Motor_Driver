#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "stm32f10x.h"

//����ģʽ
#define burstsMode1 		0x01			//����			���200ms������һ��
#define burstsMode2  		0x02			//������    ���475ms������һ��
#define burstsMode3 		0x03			//������    ���900ms������һ��
#define burstsMode4 		0x04			//������    ���9050ms������һ��
#define burstsMode5 		0x05			//������    ���1000ms������һ��
#define burstsMode6     0x06      //����ģʽ  ʱ����������

//������ģʽ��Ӧ�Ľ���֮���ʱ��������֤��1��ʱ���ڲ���5��������ģʽ���⣩
#define beatTimeMode1   200
#define beatTimeMode2   475
#define beatTimeMode3   900
#define beatTimeMode4   950
#define beatTimeMode5   1000
#define beatTimeMode6   0        

typedef struct _position_pid {
																float kp;
																float ki;
																float kd;
																int exp;
																int feedback;
																int err;
																float p_out;
																float err_inc;
																float i_max;
																float i_out;
																float out;
																float sampleTime;
														 } _position_pid;

typedef struct _run_flag {
														u8 shootFlag;
														u8 shootMode;
														u8 shootAllow;
														u8 bulletFired;
														u8 positionIndex;
														u16 beatTime;
												 } _run_flag;

												 
typedef struct _pid
{
	float setValue;
	float feedbackValue;
	float error;
	float error_0;       //��ǰ���
	float error_1;				//�ϴ����
	float error_2;				//���ϴ����
	float lastError;
	float deltaError;
	float integralError;
	float integralErrorMax;
	float kp;
	float ki;
	float kd;
	float pOut;
	float iOut;
	float dOut;
	float out;
}pid;



extern _position_pid position_pid;
extern _run_flag runFlag;
extern float BLDC_set_speed;
void Position_Control(u8 index);
void TDT_bldc_InnerControl(float* setValue, float* fbValue, float* result, float T);
void BLDC_Control(float T);
extern int set_postion;


#endif

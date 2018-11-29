#ifndef _CONTROL_H
#define	_CONTROL_H

#include "board.h"

#define  Peak_Position 0
#define  Bottom_Position 1

#define MAX_PITCH_ASPPED  1200	///��̨pitch�������ת���ٶ�
#define MID_YAW_ANGLE     11816
#define BULLET_POSITION   300
#define DISTANCE_SENSOR   1710  //���������� 51CM
#define GET_POSITION      95000
#define WHIFF_ON           TIM3->CCR2=9000
#define WHIFF_OFF          TIM3->CCR2=4000
#define YAW_MID           30793 
#define WHIFF_MID         31744 
#define PITCH_MID         31800
#define SINGLESHOOT  			32768  //8192*36/9
typedef struct 
{
	int16_t speed_set;       //�ٶȸ���
	int16_t speed_output;    //�ٶȸ���,ֱ������can���ͣ���speed_set�ó�
}Motor;

/*****************ģʽѡ��*************************/
typedef enum
{
  ANGLE_CTRL_MODE,        //��̨��е�ǿ���ģʽ
  RADPS_CTRL_MODE,        //��̨�����ǽǶȿ���ģʽ
	FOLLOWING_MODE,         //���̸���ģʽ
  SWING_MODE,             //����ҡ��ģʽ
	MANUAL_MODE,            //��ͨ�ֶ�ģʽ
	UNCONTROL_MODE,         //����ģʽ
	GETTING_MODE,           //ȡ��ģʽ
	REVOLVE_UP_MODE,        //�ϵ�������תģʽ 
	REVOLVE_DOWN_MODE,      //�µ�������תģʽ
	PARALLEL_MODE,          //У׼������̬ƽ����Դ��ģʽ
}_mode_e;


struct _Chassis_data
{
	u8 enable_flag;
	int forward_back_data;
	int left_right_data;
	int rotation_data;
	_mode_e mode_data;
};



extern Motor motor[6];
extern struct _Chassis_data Chassis_data;
extern float Elevator_right_outer,Elevator_left_outer,Position_set_zhua,Position_set_dan,Elastic_3510_outer ,GM3510_SPEED_Pitch;
extern int Yaw_aimposition,Whiff_aimposition,Number_box;
//extern float forward_back,forward_back_out,left_right, rotation;
/*�����ٶ�����*/
void Chassis_Speed_Set(void);
 
/*���̵��PID����*/
void TDT_Chassis_PidControl(vec4f* setValue, vec4f* fbValue, vec4f* result, float T, u8 dimConVar);
/*���̵��PID����*/
void TDT_Chassis_Control(float T);
/*���̵�����ݷ���*/
void TDT_Chassis_OutUpdate(vec4f* controllerOut);

void TDT_Elevator_OutUpdate(float* controllerOut_right, float* controllerOut_left, float* elastic_out);
void TDT_Position_InnerControl(float* setValue, float* fbValue, float* result, float T);
void TDT_Position_OuterControl(float* setValue, double* fbValue, float* result, float T);
void TDT_Position_Slow_OuterControl(float* setValue, double* fbValue, float* result, float T);
void TDT_3510_Position_Control(float T);
void TDT_3510_Elastic_Control(float T);
void TDT_3510_Talon_Control(float T);
void TDT_Pitch_MPU_Control(float T);
void TDT_Pitch_Mech_Control(float T);
void TDT_Yaw_Control(float T);
void TDT_Whiff_Control(float T);
void TDT_Pitch_MPU_OuterControl(float* setValue, float* fbValue, float* result, float T);
void TDT_Pitch_MPU_InnerControl(float* setValue, float* fbValue, float* result, float T);
void TDT_Pitch_Mech_OuterControl(float* setValue, float* fbValue, float* result, float T);
void TDT_Pitch_Mech_InnerControl(float* setValue, float* fbValue, float* result, float T);
void TDT_Yaw_OuterControl(float* setValue, float* fbValue, float* result, float T);
void TDT_Yaw_InnerControl(float* setValue, float* fbValue, float* result, float T);
void TDT_Whiff_OuterControl(float* setValue, float* fbValue, float* result, float T);
void TDT_Whiff_InnerControl(float* setValue, float* fbValue, float* result, float T);
void Elevator_right_Control(float T);
void Elevator_left_Control (float T);
void TDT_Elevator_Control(int mode, float T );
void Action_Control( float T);
void TDT_feedSprocket_InnerControl(float* setValue, float* fbValue, float* result, float T);
void TDT_FeedSprocket_OuterControl(float* setValue, float* fbValue, float* result, float T);
void TDT_FeedSprocket_Position_Control(float T);
#endif

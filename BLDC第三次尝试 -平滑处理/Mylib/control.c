#include "main.h"

_position_pid position_pid;
_run_flag runFlag;
float BLDC_set_speed=100;

int aimPosition[5] = {0,256,512,768,1024};

void Position_Control(u8 index)
{
	/*����λ���뷴��λ����ƫ��*/
	position_pid.exp = aimPosition[index];
	position_pid.feedback = encoder.position;
	position_pid.err = position_pid.exp - position_pid.feedback;

	/*P���*/	
	position_pid.err = contain( position_pid.err, -640, 640);
	position_pid.p_out = position_pid.kp * position_pid.err;
	
	/*I���*/	
	position_pid.err_inc += (float)position_pid.err * position_pid.sampleTime;
	position_pid.err_inc = limit(position_pid.err_inc, -position_pid.i_max, position_pid.i_max);
	position_pid.i_out = position_pid.ki * position_pid.err_inc;
	
	/*�����*/
	position_pid.out = position_pid.p_out + position_pid.i_out;
}

//void TDT_bldc_InnerControl(float* setValue, float fbValue, float* result, float T)
//{
//	static pid bldcInner;
//	
//	static u8 paraLoadFlag = 0;
//	
//	/* �������û�м��أ����ز���*/	
//	if(!paraLoadFlag)
//	{
//		TDT_Get_PIDparameters(&bldcInner, pidSPEED);
//		paraLoadFlag = 1;
//	}
//		/* �趨ֵ */	
//		bldcInner.setValue = *setValue;		
//		/* ����ֵ */			
//		bldcInner.feedbackValue = fbValue;		
//		/* ƫ�� = �趨ֵ - ����ֵ */
//		bldcInner.error = bldcInner.setValue - bldcInner.feedbackValue;	
//		
//		/* ƫ����л��� */
//		bldcInner.integralError += bldcInner.error*T;		
//		/* ƫ��Ļ��ֽ������� */
//		bldcInner.integralError = LIMIT(bldcInner.integralError, -bldcInner.integralErrorMax, bldcInner.integralErrorMax);	
//		/* ���������� */
//		bldcInner.pOut = bldcInner.kp * bldcInner.error;	
//		/* ���������� */
//		bldcInner.iOut = bldcInner.ki * bldcInner.integralError;	
//		/* �ܵ���� = ���������� + ��������� */
//		bldcInner.out = bldcInner.pOut + bldcInner.iOut;	
//	  /* ���� */
//	  bldcInner.lastError = bldcInner.error ;
//		/* �ܵ�������ܳ����������ֵ�ķ�Χ */	
//		*result = LIMIT(bldcInner.out, -MAX_PWM_BLDC, MAX_PWM_BLDC);	
//}

void TDT_pos_OuterControl(float* setValue, float* fbValue, float* result, float T)
{
	static pid posOuter;
	
	static u8 paraLoadFlag = 0;
	
	/* �������û�м��أ����ز���*/	
	if(!paraLoadFlag)
	{
		TDT_Get_PIDparameters(&posOuter, PID_POSTION);
		paraLoadFlag = 1;
	}

		/* �趨ֵ */	
		posOuter.setValue = *setValue;		
		/* ����ֵ */			
		posOuter.feedbackValue = *fbValue;		
		/* ƫ�� = �趨ֵ - ����ֵ */
		posOuter.error = posOuter.setValue - posOuter.feedbackValue;	
		/* ƫ����л��� */
		posOuter.integralError += posOuter.error * T;		
		/* ƫ��Ļ��ֽ������� */
		posOuter.integralError = LIMIT(posOuter.integralError, -posOuter.integralErrorMax, posOuter.integralErrorMax);	
		/* ���������� */
		posOuter.pOut = posOuter.kp * posOuter.error;	
		/* ���������� */
		posOuter.iOut = posOuter.ki * posOuter.integralError;
		/* �ܵ���� = ���������� + ��������� */
		posOuter.out = posOuter.pOut + posOuter.iOut;
		/* �ڻ��ĸ���ֵ��[-300,300]��ÿ�� */	
		*result = LIMIT(posOuter.out, -200, 200);	
}

int set_postion=4000;
int set_speed_scop=0;
void BLDC_Control(float T)
{
	float my_result,fbvalue;
	static u8 outerControl = 0;
	float outerFbValue = 0;
	static float outerSetValue = 0, outerResult = 0;
	
	fbvalue = BLDC_SPEED_FILT;
	outerSetValue = set_postion;
	set_speed_scop = BLDC_set_speed;
	TDT_bldc_InnerControl(&BLDC_set_speed,&fbvalue,&my_result,T);
	if(my_result>0)
		BLDC_DIR_FLG=0;
	else
	{
		BLDC_DIR_FLG=1;
		my_result=-my_result;
	}
	BLDC_pwm=my_result;
	
	if(++outerControl >= 5)
	{
		outerControl = 0;
		outerFbValue = BLDC_POSTION;
		TDT_pos_OuterControl(&outerSetValue,&outerFbValue,&outerResult,T);
		BLDC_set_speed = outerResult;
	}

}


void TDT_bldc_InnerControl(float* setValue, float* fbValue, float* result, float T)
{
	static pid bldcInner;
	float ep,ei,ed;
	
	static u8 paraLoadFlag = 0;
	
	/* �������û�м��أ����ز���*/	
	if(!paraLoadFlag)
	{
		TDT_Get_PIDparameters(&bldcInner, pidSPEED);
		paraLoadFlag = 1;
	}
		/* �趨ֵ */	
		bldcInner.setValue = *setValue;		
		/* ����ֵ */			
		bldcInner.feedbackValue = *fbValue;		
		/* ƫ�� = �趨ֵ - ����ֵ */
		bldcInner.error_0 = bldcInner.setValue - bldcInner.feedbackValue;	
		
		ep = bldcInner.error_0 - bldcInner.error_1;
	
		ei = bldcInner.error_0;
	
	  ed = bldcInner.error_0 - 2.0*bldcInner.error_1 + bldcInner.error_2;
	
		/* ���������� */
		bldcInner.pOut = bldcInner.kp * ep;	
		/* ���������� */
		bldcInner.iOut = bldcInner.ki * ei;
		/* ΢�������� */
		bldcInner.dOut = bldcInner.kd * ed;
		/* �ܵ���� = ���������� + ��������� */
		bldcInner.out += bldcInner.pOut + bldcInner.iOut + bldcInner.dOut; 	
		
  	bldcInner.out = LIMIT(bldcInner.out,-bldcInner.integralErrorMax,bldcInner.integralErrorMax); 	
	  /* ���� */
	  bldcInner.error_2 = bldcInner.error_1;
		bldcInner.error_1 = bldcInner.error_0;
		/* �ܵ�������ܳ����������ֵ�ķ�Χ */	
		*result = LIMIT(bldcInner.out, -MAX_PWM_BLDC, MAX_PWM_BLDC);	
}


#include "control.h"
float keyboard_f;//���̿��Ƶ����ٶ�ϵ��
int Yaw_aimposition=0,Whiff_aimposition=0,Pitch_aimposition=-5000;
struct _Chassis_data Chassis_data;
float sin_rad,cos_rad;
double yaw_rad;
float forward_back=0,forward_back_out=0,left_right=0, rotation=0;
float Elastic_3510_outer=0,Position_Ball=0,Talon_3510_outer=0,xishu=0.1;
float adjust_ele=0;
float swing_signal;
float Dan_zhua=0,GM3510_SPEED_Pitch=0;
int Number_box=0;
  _Bool wind_flag=-1;


/**
  * @brief  �����ƶ����pid����
  * @param  setValue: �ٶ��趨ֵ
	*					fbValue: �ٶȷ���ֵ
	*					result: ���������
	*					T: ��������
	*					dimConVar: ���ر����ĸ���
  * @retval None
  */
void TDT_Chassis_PidControl(vec4f* setValue, vec4f* fbValue, vec4f* result, float T, u8 dimConVar)
{	
	static pid chassisMotor[4];
	static u8 paraLoadFlag = 0;
	
	u8 indexConVar;
	/* �������û�м��أ����ز����������ĸ����PID����Ĭ����ͬ */	
	if(!paraLoadFlag)
	{
		for(indexConVar = 0; indexConVar < dimConVar; indexConVar++)
		 	TDT_Get_PIDparameters(&chassisMotor[indexConVar], pidChassisMove);
		paraLoadFlag = 1;
	}

	/* ѭ��dimConVar�Σ��ֱ����PID���� */		
	for(indexConVar = 0; indexConVar < dimConVar; indexConVar++)
	{
		/* �趨ֵ */	
		chassisMotor[indexConVar].setValue = setValue->data[indexConVar];		
		/* ����ֵ */			
		chassisMotor[indexConVar].feedbackValue = fbValue->data[indexConVar];		
		/* ƫ�� = �趨ֵ - ����ֵ */			
		chassisMotor[indexConVar].error = chassisMotor[indexConVar].setValue - chassisMotor[indexConVar].feedbackValue;		
		/* ƫ����л��� */				
		chassisMotor[indexConVar].integralError += chassisMotor[indexConVar].error * T;		
		/* ƫ��Ļ��ֽ������� */		
		chassisMotor[indexConVar].integralError = LIMIT(chassisMotor[indexConVar].integralError, -chassisMotor[indexConVar].integralErrorMax, chassisMotor[indexConVar].integralErrorMax);		
		/* ���������� */			
		chassisMotor[indexConVar].pOut = chassisMotor[indexConVar].kp * chassisMotor[indexConVar].error;		
		/* ���������� */	
		chassisMotor[indexConVar].iOut = chassisMotor[indexConVar].ki * chassisMotor[indexConVar].integralError;	
		/* �ܵ���� = ���������� + ���������� */			
		chassisMotor[indexConVar].out = chassisMotor[indexConVar].pOut + chassisMotor[indexConVar].iOut;		
		/* �ܵ�������ܳ����������ֵ�ķ�Χ */	
		result->data[indexConVar] = LIMIT(chassisMotor[indexConVar].out, -15000, 15000);
	}
}


/**
  * @brief  ���̿���
  * @param  T: ��������
  * @retval None
  */
void TDT_Chassis_Control(float T)
{
	/* �ĸ����̵��ת�ٵ��趨ֵ������ֵ��PID����������Ĵ洢λ�� */
	vec4f setValue, fbValue, result;
	static pid chassisRotate;
	static u8 paraLoadFlag = 0,Camera_flag=0;
	/* �������� */
	float controlCycle = T;
	/* �������û�м��أ����ز��� */	
	if(!paraLoadFlag)
	{
		TDT_Get_PIDparameters(&chassisRotate, pidChassisRotate);
		paraLoadFlag = 1;
	}
  	chassisRotate.setValue = MID_YAW_ANGLE;	
	  chassisRotate.feedbackValue = Position_485Yaw%32767;
   	chassisRotate.error = chassisRotate.setValue - chassisRotate.feedbackValue;
  	chassisRotate.pOut = chassisRotate.kp * chassisRotate.error;
    chassisRotate.out = - chassisRotate.pOut-(RC.CH[0]+RC.CH[6]*15) * ROT_MAXASPEED * 15.5f /660;//18.5
	
		/* ���̿��������ٶ�*/
	if(RC.key.SHIFT == 1)//����shiftȫ��
	  keyboard_f = 1;
	else if(RC.key.CTRL == 1)//����ctrl����
		keyboard_f = 0.2;
	else keyboard_f = 0.5;
  
	/*���̲����浫ʼ����ǹ�ܷ���ǰ��*/
	yaw_rad = (MID_YAW_ANGLE - Position_485Yaw % 32767)*0.000191748f;  //yaw��λ��ƫ��ǹ���м䷽���ֵת��Ϊ�Ƕȣ�*2pi/32768
	if(yaw_rad < 0)	//
		yaw_rad = yaw_rad + 6.283185f;
	sin_rad = my_sin(yaw_rad);	//yaw��ͳ����мнǵ�����
	cos_rad = my_cos(yaw_rad);  //yaw��ͳ����мнǵ�����
	
	/* ң������ǰ�������복����ת���ƵĽ�������ÿ�������ת���趨ֵ */
	if(RC.CH[5]==1)														//ң�����ҿ������Ϸ�
	Chassis_data.mode_data = UNCONTROL_MODE;	//���̴��ڻ���ģʽ
	else
	Chassis_data.mode_data = MANUAL_MODE;			//���̴����ֶ�ģʽ
	switch(Chassis_data.mode_data)
	{
		case MANUAL_MODE:												//�ֶ�ģʽ
	  Chassis_data.forward_back_data = RC.CH[3] * FB_MAXSPEED +(RC.key.W-RC.key.S)*FB_MAXSPEED*660*keyboard_f; //��ҡ�������W��S����������ǰ���ƶ�
	  Chassis_data.left_right_data = RC.CH[2] * LR_MAXSPEED +(RC.key.D-RC.key.A)*LR_MAXSPEED*660*keyboard_f; //��ҡ�˺����A��D���������������ƶ�
    Chassis_data.rotation_data = chassisRotate.out;   //���̸���Ľ���������̵���ת
    if(RC.CH[4]==1)													//ң�����󿪹����Ϸ�
    Chassis_data.rotation_data = 3500;			//������תֵ��3500      
		/*�õ���ʼ����ǹ�ܷ���ǰ����һ��ǰ��������һ��ִ�е��̸���*/
		setValue.data[0] = (+(Chassis_data.forward_back_data*cos_rad+Chassis_data.left_right_data*sin_rad) + (Chassis_data.left_right_data*cos_rad-Chassis_data.forward_back_data*sin_rad))*keyboard_f + Chassis_data.rotation_data;
		setValue.data[1] = (+(Chassis_data.forward_back_data*cos_rad+Chassis_data.left_right_data*sin_rad) - (Chassis_data.left_right_data*cos_rad-Chassis_data.forward_back_data*sin_rad))*keyboard_f + Chassis_data.rotation_data;
		setValue.data[2] = (-(Chassis_data.forward_back_data*cos_rad+Chassis_data.left_right_data*sin_rad) - (Chassis_data.left_right_data*cos_rad-Chassis_data.forward_back_data*sin_rad))*keyboard_f + Chassis_data.rotation_data;
		setValue.data[3] = (-(Chassis_data.forward_back_data*cos_rad+Chassis_data.left_right_data*sin_rad) + (Chassis_data.left_right_data*cos_rad-Chassis_data.forward_back_data*sin_rad))*keyboard_f + Chassis_data.rotation_data;		
		break;
		
	 case SWING_MODE:													//ҡ��ģʽ
		swing_signal = model_ident_cos(1,1,0.1,200,1000);
    setValue.data[0] = swing_signal;
	  setValue.data[1] = swing_signal;
	  setValue.data[2] = swing_signal;
	  setValue.data[3] = swing_signal;	
    break;			
		
	 case UNCONTROL_MODE:											//����ģʽ�����̲���
		setValue.data[0] =0 ;
		setValue.data[1] =0 ;
		setValue.data[2] =0 ;
		setValue.data[3] =0 ;	
    break;
		default:break;
	}	

  /* ���̵�����ٶ���can1���� */
	fbValue.data[0] = can1_feedback.chassisMotor1speed;   
	fbValue.data[1] = can1_feedback.chassisMotor2speed;;
	fbValue.data[2] = can1_feedback.chassisMotor3speed;;
	fbValue.data[3] = can1_feedback.chassisMotor4speed;;
	
	/* ���趨ֵ������ֵ��������λ�á��������ڡ����̵������Ŀ��Ϊ���������ٶ�PID���ƣ�λ��ʽPI�� */
	TDT_Chassis_PidControl(&setValue, &fbValue, &result, controlCycle, WHEELNUM);
	
	/* �����̵���ٶȿ����������ͨ��can1���͸��ĸ����̵�� */
	TDT_Chassis_OutUpdate(&result);
}




/**
  * @brief  ��̨pitch�ڻ�����
  * @param  setValue: �������ٶȣ���ÿ�룩
	*         fbValue����ǰ���ٶȷ���ֵ
	*         result�����ƽ���Ĵ��λ��
	*					T����������
  * @retval None
  */
void TDT_Pitch_MPU_InnerControl(float* setValue, float* fbValue, float* result, float T)
{
	static pid pitchInner;
	static u8 paraLoadFlag = 0;
	
	/* �������û�м��أ����ز���*/	
	if(!paraLoadFlag)
	{
		TDT_Get_PIDparameters(&pitchInner, pidPitchInner);
		paraLoadFlag = 1;
	}
    /* �趨ֵ */	
		pitchInner.setValue = *setValue;	
		/* ����ֵ */			
		pitchInner.feedbackValue = *fbValue;	
  	/* ƫ�� = �趨ֵ - ����ֵ */
		pitchInner.error = pitchInner.setValue - pitchInner.feedbackValue;	
		/* ���������� */			
		pitchInner.pOut = pitchInner.kp * pitchInner.error;
		/* ΢�������� */			
		pitchInner.dOut = pitchInner.kd * (pitchInner.error - pitchInner.lastError);
	  pitchInner.lastError = pitchInner.error;
		/* �ܵ���� = ����������*/			
		pitchInner.out = pitchInner.pOut+pitchInner.dOut;	
		/* �ܵ�������ܳ����������ֵ�ķ�Χ */	
		*result = LIMIT(pitchInner.out, -MAXSETDM50, MAXSETDM50);	
}

/**
  * @brief  ��̨pitch�⻷����
  * @param  setValue: �����Ƕȣ��ȣ�
	*         fbValue����ǰ�Ƕȷ���ֵ
	*         result�����ƽ���Ĵ��λ��
	*					T����������
  * @retval None
  */
void TDT_Pitch_MPU_OuterControl(float* setValue, float* fbValue, float* result, float T)
{
	static pid pitchOuter;
	static u8 paraLoadFlag = 0;
	
	/* �������û�м��أ����ز���*/	
	if(!paraLoadFlag)
	{
		TDT_Get_PIDparameters(&pitchOuter, pidPitchOuter);
		paraLoadFlag = 1;
	}

		/* �趨ֵ */	
		pitchOuter.setValue = *setValue;		
		/* ����ֵ */			
		pitchOuter.feedbackValue = *fbValue;		
		/* ƫ�� = �趨ֵ - ����ֵ */
		pitchOuter.error = pitchOuter.setValue - pitchOuter.feedbackValue;	
		/* ƫ����л��� */
		pitchOuter.integralError += pitchOuter.error * T;		
		/* ƫ��Ļ��ֽ������� */
		pitchOuter.integralError = LIMIT(pitchOuter.integralError, -pitchOuter.integralErrorMax, pitchOuter.integralErrorMax);	
		/* ���������� */
		pitchOuter.pOut = pitchOuter.kp * pitchOuter.error;	
		/* ���������� */
		pitchOuter.iOut = pitchOuter.ki * pitchOuter.integralError;
		/* �ܵ���� = ���������� + ��������� */
		pitchOuter.out = pitchOuter.pOut + pitchOuter.iOut;
		/* �ڻ��ĸ���ֵ��[-300,300]��ÿ�� */	
		*result = LIMIT(pitchOuter.out, -MAX_PITCH_ASPPED, MAX_PITCH_ASPPED);	
}
   /**
  * @brief  ��̨pitch����  (MPU6050)
  * @param  T����������
  * @retval None
  */

  
void TDT_Pitch_MPU_Control(float T)
{
	float innerSetValue = 0, innerFbValue = 0, innerResult = 0;
	float outerFbValue = 0;
	static float outerSetValue , outerResult = 0;//6000
  static float  pitchSetAngle=0,mouseAngle=0;
	static u8 outerControl = 0;
	
	float controlCycle = T;

	/* �ڻ��趨ֵ�����⻷���ֵ */	
	innerSetValue = outerResult;
	/* ��̨��y����ת����������ֵ��Ϊ�ڻ����ٶȷ���ֵ,��ʱ��Ϊ�����ڻ��趨Ϊ�� */
	innerFbValue = mpu6050Top.gyro.dps.data[y];
	TDT_Pitch_MPU_InnerControl(&innerSetValue, &innerFbValue, &innerResult, controlCycle);

	/* �⻷�Ŀ����������ڻ���2�� */
	if(++outerControl >= 2)
	{
			outerControl = 0;
		  pitchSetAngle = -RC.CH[1]*55.0f /660.0f; //��ҡ��������Ϊpitch����趨ֵ
      /*����Pitch������Χ*/
			pitchSetAngle = LIMIT(pitchSetAngle, -30,25);
		  /*�д�����Ƶ����Ӹ����Ƕ�*/
			outerSetValue = pitchSetAngle;
      /*6623�Ļ�е�Ƕ���Ϊ�⻷�Ƕȷ���*/		
			outerFbValue = gimbalTopAngle.pitch;
			TDT_Pitch_MPU_OuterControl(&outerSetValue, &outerFbValue, &outerResult, 2*controlCycle);
	}				
	DM50_SPEED_Pitch1 = innerResult;			//�ڻ������pitch1��ͨ��485���͸����
  DM50_SPEED_Pitch2 = -DM50_SPEED_Pitch1;//��һ��pitch2ȡpitch1���෴��
}


///**
//  * @brief  ��̨pitch�ڻ�����
//  * @param  setValue: �������ٶȣ���ÿ�룩
//	*         fbValue����ǰ���ٶȷ���ֵ
//	*         result�����ƽ���Ĵ��λ��
//	*					T����������
//  * @retval None
//  */
//void TDT_Pitch_Mech_InnerControl(float* setValue, float* fbValue, float* result, float T)
//{
//	static pid pitchInnerMech;
//	
//	static u8 paraLoadFlag = 0;
//	
//	/* �������û�м��أ����ز���*/	
//	if(!paraLoadFlag)
//	{
//		TDT_Get_PIDparameters(&pitchInnerMech, pidPitchInnerMech);
//		paraLoadFlag = 1;
//	}
//    /* �趨ֵ */	
//		pitchInnerMech.setValue = *setValue;	
//		/* ����ֵ */			
//		pitchInnerMech.feedbackValue = *fbValue;	
//  	/* ƫ�� = �趨ֵ - ����ֵ */
//		pitchInnerMech.error = pitchInnerMech.setValue - pitchInnerMech.feedbackValue;	
//		/* ���������� */			
//		pitchInnerMech.pOut = pitchInnerMech.kp * pitchInnerMech.error;
//  	/* ΢�������� */			
//		pitchInnerMech.dOut = pitchInnerMech.kd * (pitchInnerMech.error - pitchInnerMech.lastError);
//	  pitchInnerMech.lastError = pitchInnerMech.error;
//		/* �ܵ���� = ����������*/			
//		pitchInnerMech.out = pitchInnerMech.pOut+pitchInnerMech.dOut;	

//		/* �ܵ�������ܳ����������ֵ�ķ�Χ */	
//		*result = LIMIT(pitchInnerMech.out, -MAXSETDM50, MAXSETDM50);	
//}

///**
//  * @brief  ��̨pitch�⻷����
//  * @param  setValue: �����Ƕȣ��ȣ�
//	*         fbValue����ǰ�Ƕȷ���ֵ
//	*         result�����ƽ���Ĵ��λ��
//	*					T����������
//  * @retval None
//  */
//void TDT_Pitch_Mech_OuterControl(float* setValue, float* fbValue, float* result, float T)
//{
//	static pid pitchOuterMech;
//	static u8 paraLoadFlag = 0;
//	
//	/* �������û�м��أ����ز���*/	
//	if(!paraLoadFlag)
//	{
//		TDT_Get_PIDparameters(&pitchOuterMech, pidPitchOuterMech);
//		paraLoadFlag = 1;
//	}

//		/* �趨ֵ */	
//		pitchOuterMech.setValue = *setValue;		
//		/* ����ֵ */			
//		pitchOuterMech.feedbackValue = *fbValue;		
//		/* ƫ�� = �趨ֵ - ����ֵ */
//		pitchOuterMech.error = pitchOuterMech.setValue - pitchOuterMech.feedbackValue;	
//		/* ƫ����л��� */
//		pitchOuterMech.integralError += pitchOuterMech.error * T;		
//		/* ƫ��Ļ��ֽ������� */
//		pitchOuterMech.integralError = LIMIT(pitchOuterMech.integralError, -pitchOuterMech.integralErrorMax, pitchOuterMech.integralErrorMax);	
//		/* ���������� */
//		pitchOuterMech.pOut = pitchOuterMech.kp * pitchOuterMech.error;	
//		/* ���������� */
//		pitchOuterMech.iOut = pitchOuterMech.ki * pitchOuterMech.integralError;
//		/* �ܵ���� = ���������� + ��������� */
//		pitchOuterMech.out = pitchOuterMech.pOut + pitchOuterMech.iOut;
//		/* �ڻ��ĸ���ֵ��[-300,300]��ÿ�� */	
//		*result = -LIMIT(pitchOuterMech.out, -MAX_PITCH_ASPPED, MAX_PITCH_ASPPED);	
//}

///**
//  * @brief  ��̨pitch����  (��е�ǿ���)
//  * @param  T����������
//  * @retval None
//  */
//void TDT_Pitch_Mech_Control(float T)
//{
//	float innerSetValue = 0, innerFbValue = 0, innerResult = 0;
//	float outerFbValue = 0;

//	static float outerSetValue , outerResult = 0;//6000
//  static float  pitchSetAngle=0;
//	static u8 outerControl = 0;
//	
//	float controlCycle = T;

//	/* �ڻ��趨ֵ�����⻷���ֵ */	
//	innerSetValue = outerResult;
//	/* ��̨��y����ת����������ֵ��Ϊ�ڻ����ٶȷ���ֵ,��ʱ��Ϊ�����ڻ��趨Ϊ�� */
//	innerFbValue =mpu6050Top.gyro.dps.data[y];
//	TDT_Pitch_Mech_InnerControl(&innerSetValue, &innerFbValue, &innerResult, controlCycle);

//	/* �⻷�Ŀ����������ڻ���2�� */
//	if(++outerControl >= 2)
//	{
//		outerControl = 0;
//		pitchSetAngle = Pitch_aimposition + (PITCH_MID - START_Position_Pitch) ;				 
//		/*�д�����Ƶ����Ӹ����Ƕ�*/
//		outerSetValue = pitchSetAngle;
//    /*6623�Ļ�е�Ƕ���Ϊ�⻷�Ƕȷ���*/		
//		outerFbValue = Position_485Pitch;
//		TDT_Pitch_Mech_OuterControl(&outerSetValue, &outerFbValue, &outerResult, 2*controlCycle);
//	}				
//	DM50_SPEED_Pitch1 = innerResult;		//
//}

/**
  * @brief  ��̨Yaw�ڻ�����
  * @param  setValue: �������ٶȣ���ÿ�룩
	*         fbValue����ǰ���ٶȷ���ֵ
	*         result�����ƽ���Ĵ��λ��
	*					T����������
  * @retval None
  */
void TDT_Yaw_InnerControl(float* setValue, float* fbValue, float* result, float T)
{
	static pid yawInner;
	
	static u8 paraLoadFlag = 0;
	
	/* �������û�м��أ����ز���*/	
	if(!paraLoadFlag)
	{
		TDT_Get_PIDparameters(&yawInner, pidYawInner);
		paraLoadFlag = 1;
	}
    /* �趨ֵ */	
		yawInner.setValue = *setValue;	
		/* ����ֵ */			
		yawInner.feedbackValue = *fbValue;	
  	/* ƫ�� = �趨ֵ - ����ֵ */
		yawInner.error = yawInner.setValue - yawInner.feedbackValue;	
		/* ���������� */			
		yawInner.pOut = yawInner.kp * yawInner.error;
		/* ΢�������� */			
		yawInner.dOut = yawInner.kd * (yawInner.error - yawInner.lastError);
	  yawInner.lastError = yawInner.error;
		/* �ܵ���� = ����������*/			
		yawInner.out = yawInner.pOut+yawInner.dOut;	

		/* �ܵ�������ܳ����������ֵ�ķ�Χ */	
		*result = LIMIT(yawInner.out, -MAXSETDM50, MAXSETDM50);	
}

/**
  * @brief  ��̨yaw�⻷����
  * @param  setValue: �����Ƕȣ��ȣ�
	*         fbValue����ǰ�Ƕȷ���ֵ
	*         result�����ƽ���Ĵ��λ��
	*					T����������
  * @retval None
  */
void TDT_Yaw_OuterControl(float* setValue, float* fbValue, float* result, float T)
{
	static pid yawOuter;
	static u8 paraLoadFlag = 0;
	
	/* �������û�м��أ����ز���*/	
	if(!paraLoadFlag)
	{
		TDT_Get_PIDparameters(&yawOuter, pidYawOuter);
		paraLoadFlag = 1;
	}

		/* �趨ֵ */	
		yawOuter.setValue = *setValue;		
		/* ����ֵ */			
		yawOuter.feedbackValue = *fbValue;		
		/* ƫ�� = �趨ֵ - ����ֵ */
		yawOuter.error = yawOuter.setValue - yawOuter.feedbackValue;	
		/* ƫ����л��� */
		yawOuter.integralError += yawOuter.error * T;		
		/* ƫ��Ļ��ֽ������� */
		yawOuter.integralError = LIMIT(yawOuter.integralError, -yawOuter.integralErrorMax, yawOuter.integralErrorMax);	
		/* ���������� */
		yawOuter.pOut = yawOuter.kp * yawOuter.error;	
		/* ���������� */
		yawOuter.iOut = yawOuter.ki * yawOuter.integralError;
		/* �ܵ���� = ���������� + ��������� */
		yawOuter.out = yawOuter.pOut + yawOuter.iOut;
		/* �ڻ��ĸ���ֵ��[-300,300]��ÿ�� */	
		*result = LIMIT(yawOuter.out, -MAX_PITCH_ASPPED, MAX_PITCH_ASPPED);	
}

/**
  * @brief  ��̨yaw����
  * @param  T����������
  * @retval None
  */
 void TDT_Yaw_Control(float T)
{
	float innerSetValue = 0, innerFbValue = 0, innerResult = 0;
	float outerFbValue = 0;
	static float outerSetValue , outerResult = 0;//6000
	
  static float  yawSetAngle=0;
	static u8 outerControl = 0;
	
	float controlCycle = T;

	/* �ڻ��趨ֵ�����⻷���ֵ */	
	innerSetValue = outerResult;
	/* ��̨��y����ת����������ֵ��Ϊ�ڻ����ٶȷ���ֵ,��ʱ��Ϊ�����ڻ��趨Ϊ�� */
	innerFbValue = -mpu6050Top.gyro.dps.data[z];
	TDT_Yaw_InnerControl(&innerSetValue, &innerFbValue, &innerResult, controlCycle);	//yaw���ڻ�pid����

	/* �⻷�Ŀ����������ڻ���2�� */
	if(++outerControl >= 2)
	{
		outerControl = 0;
		yawSetAngle += RC.CH[0]*5.0/660;		//��ҡ�˺�����Ϊyaw���趨ֵ
     /*����yaw������Χ*/
		//yawSetAngle = LIMIT(yawSetAngle, -45, 45);
		 /*�д�����Ƶ����Ӹ����Ƕ�*/
		outerSetValue = yawSetAngle;
     /*6623�Ļ�е�Ƕ���Ϊ�⻷�Ƕȷ���*/	        
		outerFbValue =-gimbalTopAngle.yawsum;
		TDT_Yaw_OuterControl(&outerSetValue, &outerFbValue, &outerResult, 2*controlCycle);
	}				
	DM50_SPEED_Yaw = innerResult;			//�ڻ������yaw����485���� 
}

/*�������ڻ�pid���ƺ���*/
void TDT_FeedSprocket_InnerControl(float* setValue, float* fbValue, float* result, float T)
{
	static pid feedSprocket_Inner;
	static u8 paraLoadFlag = 0;
	
	/* �������û�м��أ����ز���*/	
	if(!paraLoadFlag)
	{
		TDT_Get_PIDparameters(&feedSprocket_Inner, pidfeedSprocket_Inner);
		paraLoadFlag = 1;
	}
 		/* �趨ֵ */	
		feedSprocket_Inner.setValue = *setValue;	
		/* ����ֵ */			
		feedSprocket_Inner.feedbackValue = *fbValue;	
		/* ƫ�� = �趨ֵ - ����ֵ */			
		feedSprocket_Inner.error = feedSprocket_Inner.setValue - feedSprocket_Inner.feedbackValue;
		/* ƫ������ */			
		feedSprocket_Inner.deltaError = feedSprocket_Inner.error - feedSprocket_Inner.lastError;
		/* ���������� */			
		feedSprocket_Inner.pOut = feedSprocket_Inner.kp * feedSprocket_Inner.error;	
		/* ΢�������� */			
		feedSprocket_Inner.dOut = feedSprocket_Inner.kd * feedSprocket_Inner.deltaError;
		/* �ܵ���� = ���������� */			
		feedSprocket_Inner.out = feedSprocket_Inner.pOut + feedSprocket_Inner.dOut;	
		/* �ܵ�������ܳ����������ֵ�ķ�Χ */	
		*result = LIMIT(-feedSprocket_Inner.out, -15000, 15000);	
}

/*�������⻷pid���ƺ���*/
void TDT_FeedSprocket_OuterControl(float* setValue, float* fbValue, float* result, float T)
{
	static pid feedSprocket_Outer;
	
	static u8 paraLoadFlag = 0;
	
	/* �������û�м��أ����ز���*/	
	if(!paraLoadFlag)
	{
		TDT_Get_PIDparameters(&feedSprocket_Outer, pidfeedSprocket_Outer);
		paraLoadFlag = 1;
	}

		/* �趨ֵ */	
		feedSprocket_Outer.setValue = *setValue;		
		/* ����ֵ */			
		feedSprocket_Outer.feedbackValue = *fbValue;		
		/* ƫ�� = �趨ֵ - ����ֵ */
		feedSprocket_Outer.error = feedSprocket_Outer.setValue - feedSprocket_Outer.feedbackValue;	
		/* ƫ����л��� */
		feedSprocket_Outer.integralError += feedSprocket_Outer.error * T;		
		/* ƫ��Ļ��ֽ������� */
		feedSprocket_Outer.integralError = LIMIT(feedSprocket_Outer.integralError, -feedSprocket_Outer.integralErrorMax, feedSprocket_Outer.integralErrorMax);	
		/* ���������� */
		feedSprocket_Outer.pOut = feedSprocket_Outer.kp * feedSprocket_Outer.error;	
		/* ���������� */
		feedSprocket_Outer.iOut = feedSprocket_Outer.ki * feedSprocket_Outer.integralError;		
		/* �ܵ���� = ���������� + ��������� */
		feedSprocket_Outer.out = feedSprocket_Outer.pOut + feedSprocket_Outer.iOut;	
		/* �ڻ��ĸ���ֵ��[-300,300]��ÿ�� */	
		*result = LIMIT(feedSprocket_Outer.out, -15000, 15000);	
}

/*�����ֿ��ƺ���*/
void TDT_FeedSprocket_Position_Control(float T)
{
	float innerSetValue = 0, innerFbValue = 0, innerResult = 0;
	float outerFbValue = 0;
	float controlCycle = T;
	static float outerSetValue = 0, outerResult = 0, rest_time = 0, free_time = 0.15f;
 
	static u8 shoot_error = 0;
	static u8 outerControl = 0;
	
	innerSetValue = outerResult;	//�⻷������ڻ��趨ֵ
	innerFbValue = can2_feedback.feedSprocket_speed;//�ڻ�����ֵ��can2����
	TDT_FeedSprocket_InnerControl(&innerSetValue, &innerFbValue, &innerResult, controlCycle);	//ִ���ڻ�pid����
	
	if((RC.CH[4] == 2 || RC.CH[8] ==1) && (shoot_error == 0))		//δ�µ���ң�����󿪹������·�������������ʱ���в���
	{
		outerSetValue += SINGLESHOOT;
	}
	
		/*��ת����*/
	if(ABS(outerSetValue - can2_feedback.feedSprocket_position) > SINGLESHOOT*4)
	{
		outerSetValue -= SINGLESHOOT*6;		//�����ת
		shoot_error = 1;					//��ת���
	}
	if(shoot_error == 1)				//�ȴ���ת����
	{
		rest_time += T;
		if(rest_time > 0.8)
		{
			shoot_error = 0;
			rest_time = 0;
			outerSetValue = can2_feedback.feedSprocket_position;		
		}
	}
		/* �⻷�Ŀ����������ڻ���2�� */
	if(++outerControl >= 2)										//�ڻ�pidִ�����Σ��⻷pidִ��1��
	{
		outerControl = 0;
		outerFbValue = can2_feedback.feedSprocket_position;
		TDT_FeedSprocket_OuterControl(&outerSetValue, &outerFbValue, &outerResult, 2*controlCycle);
	}				
	TDT_feedSprocket_OutUpdate(&innerResult);//�����ַ��Ϳ��ƺ���
}



#include "main.h"

/**
  * @brief  Ħ����pid����
  * @param  setValue: �ٶ��趨ֵ
	*					fbValue: �ٶȷ���ֵ
	*					result: ���������
	*					T: ��������
	*					numConVar: ���ر����ĸ���
  * @retval None
  */

void TDT_Friction_PidControl(vec2int16* setValue, vec2int16* fbValue, vec2int16* result, float T, u8 dimConVar)
{
    static pid frictionMotor[2];

    static u8 paraLoadFlag = 0;

    u8 indexConVar;

    /* �������û�м��أ����ز����������ĸ����PID����Ĭ����ͬ */
    if(!paraLoadFlag)
    {
        for(indexConVar = 0; indexConVar < dimConVar; indexConVar++)
            TDT_Get_PIDparameters(&frictionMotor[indexConVar], pidFriction);
        paraLoadFlag = 1;
    }

    /* ѭ��dimConVar�Σ��ֱ����PID���� */
    for(indexConVar = 0; indexConVar < dimConVar; indexConVar++)
    {
        /* �趨ֵ */
        frictionMotor[indexConVar].setValue = setValue->data[indexConVar];
        /* ����ֵ */
        frictionMotor[indexConVar].feedbackValue = fbValue->data[indexConVar];
        /* ƫ�� = �趨ֵ - ����ֵ */
        frictionMotor[indexConVar].error = frictionMotor[indexConVar].setValue - frictionMotor[indexConVar].feedbackValue;
        /* ƫ����л��� */
        frictionMotor[indexConVar].integralError += frictionMotor[indexConVar].error * T;
        /* ƫ��Ļ��ֽ������� */
        frictionMotor[indexConVar].integralError = LIMIT(frictionMotor[indexConVar].integralError, -frictionMotor[indexConVar].integralErrorMax, frictionMotor[indexConVar].integralErrorMax);
        /* ���������� */
        frictionMotor[indexConVar].pOut = frictionMotor[indexConVar].kp * frictionMotor[indexConVar].error;
        /* ���������� */
        frictionMotor[indexConVar].iOut = frictionMotor[indexConVar].ki * frictionMotor[indexConVar].integralError;
        /* �ܵ���� = ���������� + ���������� */
        frictionMotor[indexConVar].out = frictionMotor[indexConVar].pOut + frictionMotor[indexConVar].iOut + 4000;
        /* �ܵ�������ܳ����������ֵ�ķ�Χ */
        result->data[indexConVar] = LIMIT(frictionMotor[indexConVar].out, 4000, 7500);
    }
}



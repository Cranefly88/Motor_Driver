#ifndef __FOC_H
#define __FOC_H
#include "main.h"

typedef struct 
{
    int16_t Ia;     //���룬A�ඨ�ӵ���
    int16_t Ib;     //���룬B�ඨ�ӵ���
    int32_t Ic;     //���룬C�ඨ�ӵ���
    int16_t Ia_last;     //���룬�ϴ�A�ඨ�ӵ���
    int16_t Ib_last;     //���룬�ϴ�B�ඨ�ӵ���
    int32_t Ic_last;     //���룬�ϴ�C�ඨ�ӵ���  
    int32_t IAlpha;  //�������ֹ����ϵAlpha�ᶨ�ӵ���
    int32_t IBeta;   //�������ֹ����ϵBeta�ᶨ�ӵ���
    void (*calcClark)();    
    void (*calcAntiClark)();
}_CLARK;

typedef struct 
{
    int32_t Id;     //�������ת����ϵ�µ�D����ֵ����
    int32_t Iq;         //�������ת����ϵ�µ�Q����ֵ����
    int32_t IAlpha;  //���룬��ֹ����ϵAlpha�ᶨ�ӵ���
    int32_t IBeta;   //���룬��ֹ����ϵBeta�ᶨ�ӵ���
    float Ud;     //�������ת����ϵ�µ�D����ֵ��ѹ
    float Uq;          //�������ת����ϵ�µ�Q����ֵ��ѹ
    long UAlpha;  //���룬��ֹ����ϵAlpha�ᶨ�ӵ�ѹ
    long UBeta;   //���룬��ֹ����ϵBeta�ᶨ�ӵ�ѹ
    long Theta;    //��ת����Ƕ�
    float ActId;  //ʵ��D�����
    float ActIq;  //ʵ��Q�����
    void (*calcPark)();      
    void (*calcAntiPark)();  
}_PARK;

typedef struct 
{
    float UAlpha; //���룬��ֹ����ϵAlpha�ᶨ�ӵ�ѹ
    float UBeta;  //���룬��ֹ����ϵBeta�ᶨ�ӵ�ѹ
    long Ua;      //
    long Ub;      //
    long Uc;      //
    long Tx;        //
    long Ty;        //
    long Tz;        //
    long taOn;    //A��ʱ��
    long tbOn;      //B��ʱ��
    long tcOn;      //C��ʱ��
} _SVPWM;


typedef struct {
                 u16 MOTA;
                 u16 MOTB;
                 u16 MOTC;  
}_duty;

typedef struct {
                 u16 PhaseAOffset;
                 u16 PhaseBOffset;
                 u16 PhaseCOffset;  
}_Offset;

typedef struct {
                 int Id_Result;
                 int Iq_Result; 
                 int SetValue;
                 int Feedback;  
}_Speed;

typedef struct 
{
  float Kp;
  float Ki;
  float Kc;
}Parameter;

typedef struct  
{
  Parameter Id;
  Parameter Iq;
  Parameter Speed;
}_PID;

typedef struct{
               _duty  Duty;
               _SVPWM Svpwm;
               _CLARK Clack;
               _PARK  Park;
               _Offset Offset; 
               _Speed  Speed;
               _PID    PID;
}_DRV8305;

extern _DRV8305 DRV8305;
extern u8 Step_Flag;
extern float Outmax_Id,Outmax_Iq,Outmax_Speed;
void Clarke_Trans(void);
void Park_Trans(void);
void Id_PI_Controller(void);
void Iq_PI_Controller(void);
void Speed_Controller(void);
#endif


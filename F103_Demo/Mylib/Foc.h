#ifndef __FOC_H
#define __FOC_H
#include "main.h"

typedef struct 
{
    u16 Ia;     //���룬A�ඨ�ӵ���
    u16 Ib;     //���룬B�ඨ�ӵ���
    u16 Ic;     //���룬C�ඨ�ӵ���
    u16 IAlpha;  //�������ֹ����ϵAlpha�ᶨ�ӵ���
    u16 IBeta;   //�������ֹ����ϵBeta�ᶨ�ӵ���
    void (*calcClark)();    
    void (*calcAntiClark)();
}_CLARK;

typedef struct 
{
    long Id;     //�������ת����ϵ�µ�D����ֵ����
    long Iq;         //�������ת����ϵ�µ�Q����ֵ����
    long IAlpha;  //���룬��ֹ����ϵAlpha�ᶨ�ӵ���
    long IBeta;   //���룬��ֹ����ϵBeta�ᶨ�ӵ���
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

typedef struct{
               _duty  Duty;
               _SVPWM Svpwm;
               _CLARK Clack;
               _PARK  Park;
               _Offset Offset; 
}_DRV8305;

extern _DRV8305 DRV8305;
extern u8 Step_Flag;


#endif


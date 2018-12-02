#ifndef __DRV8305_H
#define __DRV8305_H
#include "board.h"

#define RCC_DRV8305	     RCC_AHB1Periph_GPIOC 

#define DRV8305_PORT      GPIOC

#define DRV8305_Pin_SCS      GPIO_Pin_4
#define DRV8305_Pin_PWRGD    GPIO_Pin_1
#define DRV8305_Pin_ENGATE   GPIO_Pin_0
#define DRV8305_Pin_WAKE      GPIO_Pin_5

#define DRV8305_SCS_H     GPIO_SetBits(DRV8305_PORT   ,   DRV8305_Pin_SCS)
#define DRV8305_SCS_L     GPIO_ResetBits(DRV8305_PORT ,    DRV8305_Pin_SCS)

#define DRV8305_ENGATE_H     GPIO_SetBits(DRV8305_PORT   ,   DRV8305_Pin_ENGATE)
#define DRV8305_ENGATE_L     GPIO_ResetBits(DRV8305_PORT ,    DRV8305_Pin_ENGATE)

//#define DRV8305_SCS_H     GPIO_SetBits(DRV8305_PORT   ,   DRV8305_Pin_SCS)
//#define DRV8305_SCS_L     GPIO_ResetBits(DRV8305_PORT ,    DRV8305_Pin_SCS)

//#define DRV8305_SCS_H     GPIO_SetBits(DRV8305_PORT   ,   DRV8305_Pin_SCS)
//#define DRV8305_SCS_L     GPIO_ResetBits(DRV8305_PORT ,    DRV8305_Pin_SCS)
 
//#define READ   

typedef struct 
{
    long Ia;     //���룬A�ඨ�ӵ���
    long Ib;     //���룬B�ඨ�ӵ���
    long Ic;     //���룬C�ඨ�ӵ���
    long IAlpha;  //�������ֹ����ϵAlpha�ᶨ�ӵ���
    long IBeta;   //�������ֹ����ϵBeta�ᶨ�ӵ���
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
    float UAlpha;  //���룬��ֹ����ϵAlpha�ᶨ�ӵ�ѹ
    float UBeta;   //���룬��ֹ����ϵBeta�ᶨ�ӵ�ѹ
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

typedef struct  _phase{
                   int   H_duty;
                   int   L_duty;   
}phase;

typedef struct {
                 int MOTA;
                 int MOTB;
                 int MOTC;  
}_duty;

typedef struct{
               _duty  Duty;
               _SVPWM Svpwm;
               _CLARK Clack;
               _PARK  Park;
               
}_DRV8305;

extern _DRV8305 DRV8305;
extern u8 Step_Flag;


void DRV8305_Init(void);
void DRV8305_Output(int Duty[6]);
void DRC8305_Driver_Control(float T);
void Svpwm_Module(void);

#endif
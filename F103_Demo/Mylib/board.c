#include "main.h"

u8 Init_OK;
int i;
//�δ�ʱ���������� ,49������
volatile uint32_t sysTickUptime=0;

#define TICK_PER_SECOND 1000 
#define TICK_US	(1000000/TICK_PER_SECOND)
RCC_ClocksTypeDef  rcc_clocks;
void SysTick_Configuration(void)
{
	
	uint32_t         cnts;

	RCC_GetClocksFreq(&rcc_clocks);

	cnts = (uint32_t)rcc_clocks.HCLK_Frequency / TICK_PER_SECOND;
	cnts = cnts / 8;

	SysTick_Config(cnts);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

uint32_t GetSysTime_us(void) 
{
	register uint32_t ms;
	u32 value;
	ms = sysTickUptime;
 	value = ms * TICK_US + (SysTick->LOAD - SysTick->VAL) * TICK_US / SysTick->LOAD;
	return value;
}


void DelayUs(uint32_t us)
{
    uint32_t now = GetSysTime_us();
    while (GetSysTime_us() - now < us);
}

void DelayMs(uint32_t ms)
{
    while (ms--)
      DelayUs(1000);
}

void Board_ALL_Init(void)
{	
		/*�жϷ���*/
		NVIC_PriorityGroupConfig(NVIC_GROUP);
		/*�δ�ʱ������*/
		SysTick_Configuration();	

    LED_Configuration(); //B12
    
//    TIM2_pwm_init();    //A0 A1 A2 A3
//    TIM3_pwm_init();    //A6 A7 B0 B1
//    TIM4_pwm_init();    //B6 B7 B8 B9
//    Output_init();      //A4 A5 A8 A11 A12 A15 
//    Input_init();       //B3 B4 B5 B13 B14 B15
    //Ԥ��B10 B11/A9 A10 �Ժ�������������ڻ����IIC 
		/*ʱ���ʼ��*/
		Cycle_Time_Init();
    
//    DRV8305_Init();

    Init_OK=1; 

}

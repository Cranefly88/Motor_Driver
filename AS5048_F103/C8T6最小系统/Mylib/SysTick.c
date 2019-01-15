#include "main.h"

/* SystemFrequency / 1000    1ms�ж�һ��
 * SystemFrequency / 100000	 10us�ж�һ��
 * SystemFrequency / 1000000 1us�ж�һ��
 */
RCC_ClocksTypeDef  rcc_clocks;

void SysTick_Init(void)
{
		RCC_GetClocksFreq(&rcc_clocks);

    if (SysTick_Config(SystemCoreClock / 1000))
    {
        /* Capture error */
        while (1);
    }
}

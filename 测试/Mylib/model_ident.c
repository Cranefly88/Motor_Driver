#include "model_ident.h"
#include "math.h"

/* funtion_name: model_ident_sin
 * parameter: low_freq    ���Ҳ������Ƶ��
 * parameter: high_freq   ���Ҳ������Ƶ��
 * parameter: step        Ƶ�ʱ仯�Ĳ���
 * parameter: run_freq    �˺�������Ƶ��
 * parameter: repeat      ÿһƵ���ظ�����
 */
 int my_freq=0;

float model_ident_sin(float low_freq, float high_freq, float step, float run_freq, float repeat)
{
	  float ident_out = 0;
	  unsigned int step_num = 0;
	
    static char ident_start = 0;
	  static float freq = 0;
	  static unsigned int cnt = 0;
	
    if (ident_start == 0)
		{
		    ident_start = 1;
			  freq = low_freq;
			  my_freq=freq;
			
		}
		
		if (freq >= 22)
			step=2;
		else if (freq >= 40)
			step=5;
		
//		repeat = 5 / (1 / freq); 
		step_num = (1 / freq * run_freq * repeat) - 1;
		
		if (cnt <= step_num)
		{
		    ident_out = 1000* sin(2.0f * 3.1415926f * freq / run_freq * (cnt + 0.5f));
		    cnt++;
		}
    else
		{
		    freq += step;
			  cnt = 0;	
		}

    if (freq > high_freq)
		{
		    ident_start = 0;
			  freq = 0;
		}
		
		return ident_out;
}


float model_ident_sin1(float low_freq, float high_freq, float step, float run_freq, float repeat)
{
	  float ident_out = 0;
	  unsigned int step_num = 0;
	
    static char ident_start = 0;
	  static float freq = 0;
	  static unsigned int cnt = 0;
	
    if (ident_start == 0)
		{
		    ident_start = 1;
			  freq = low_freq;
			  my_freq=freq;
			
		}
		repeat = 5 / (1 / freq); 
		step_num = (1 / freq * run_freq * repeat) - 1;
		
		if (cnt <= step_num)
		{
		    ident_out = 500 * sin(2.0f * 3.1415926f * freq / run_freq * (cnt + 0.5f));
		    cnt++;
		}
    else
		{
		    freq += step;
			  cnt = 0;	
		}

    if (freq > high_freq)
		{
		    ident_start = 0;
			  freq = 0;
		}
		
		return ident_out;
}



float model_ident_cos(float low_freq, float high_freq, float step, float run_freq, float repeat)
{
	  float ident_out = 0;
	  unsigned int step_num = 0;
	
    static char ident_start = 0;
	  static float freq = 0;
	  static unsigned int cnt = 0;
	
    if (ident_start == 0)
		{
		    ident_start = 1;
			  freq = low_freq;
		}
		
		step_num = (1 / freq * run_freq * repeat) - 1;
		
		if (cnt <= step_num)
		{
		    ident_out = 4000 * cos(2.0f * 3.1415926f * freq / run_freq * (cnt + 0.5f));
		    cnt++;
		}
    else
		{
		    freq += step;
			  cnt = 0;	
		}

    if (freq > high_freq)
		{
		    ident_start = 0;
			  freq = 0;
		}
		
		return ident_out;
}



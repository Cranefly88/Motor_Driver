#ifndef _SCHEDULE_H
#define _SCHEDULE_H

#include "main.h"

                #
typedef struct _schedule{
													uint16_t   cnt_100us;     
													uint16_t   cnt_1ms;
													uint16_t   cnt_2ms;
													uint16_t   cnt_5ms;
													uint16_t   cnt_10ms;
													uint16_t   cnt_20ms;
													uint16_t   cnt_50ms;
												}schedule;

extern struct GP2Y gp2y;
extern _Bool exxflag2,flag_Up,flag_Down,DanStart_flag;
extern int Feedback_Theta,SetAdvance_Theta;

void TDT_Loop_10000Hz(void); //100usִ��һ��                        
void TDT_Loop_1000Hz(void); //1msִ��һ��
void TDT_Loop_500Hz(void);	//2msִ��һ��
void TDT_Loop_200Hz(void);	//5msִ��һ��
void TDT_Loop_100Hz(void);	//10msִ��һ��
void TDT_Loop_50Hz(void);	  //20msִ��һ��
void TDT_Loop_20Hz(void);	  //50msִ��һ��

void TDT_Loop(schedule* robotSchdule);

#endif

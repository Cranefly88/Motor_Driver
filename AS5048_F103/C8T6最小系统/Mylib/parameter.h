#ifndef __PARAMETER_H__
#define __PARAMETER_H__
#include "main.h"

#define pidFriction 		0

extern int Friction1_Speed_Set;//��
extern int Friction2_Speed_Set;//��
void TDT_Get_PIDparameters(pid* pidStruct, u8 pidIndex);

#endif

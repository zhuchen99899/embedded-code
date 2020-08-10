#ifndef __PID_H_
#define  __PID_H_



#include <stm32f10x.h>

void pid_init(void);

float Abs(float val);

float pid_realize(float tem);





#endif




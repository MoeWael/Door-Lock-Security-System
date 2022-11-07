/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: timer.h
 *
 * Description: Header file for the Timer AVR driver
 *
 * Author: Mohammad Wael
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"

typedef enum{
	NORMAL, COMPARE
}Timer_Mode;

typedef enum{
	NO_CLOCK, NO_PRESCALER, CLK_8, CLK_64, CLK_256, CLK_1024, EXTERNAL_T0_FALLING, EXTERNAL_T0_RISING
}Timer_Prescaler;


typedef struct{
	Timer_Mode mode;
	uint16 Initial_Value;
	uint16 Compare_Value;
	Timer_Prescaler prescaler;
}Timer_ConfigType;

void Timer0_Init(Timer_ConfigType *Config_Ptr);
void Timer0_SetCallback(void (*a_Ptr)(void));
void Timer0_DeInit(void);

#endif /* TIMER_H_ */

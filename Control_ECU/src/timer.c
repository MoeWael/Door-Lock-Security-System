/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: timer.c
 *
 * Description: Source file for the Timer AVR driver
 *
 * Author: Mohammad Wael
 *
 *******************************************************************************/

#include "timer.h"

#include <avr/io.h> /* To use ICU/Timer1 Registers */
#include <avr/interrupt.h>

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

void Timer0_Init(Timer_ConfigType *Config_Ptr){

	/* Set Initial Timer value */
	TCNT0 = Config_Ptr->Initial_Value;

	/* Set Compare Value for CTC mode */
	OCR0 = Config_Ptr->Compare_Value;

	/* FOC0 is only active when using PWM mode */
	TCCR0 |= (1<<FOC0);

	/* Clearing WGM01 before writing */
	TCCR0 &= ~(1<<WGM01);

	/* writing in WGM01 depending on the mode
	 * Normal = 0
	 * CTC = 1                                */
	TCCR0 |= ((Config_Ptr->mode)<<WGM01);

	/* Set the Clock Prescaler */
	TCCR0 |= (Config_Ptr->prescaler);

	/* Enable Interrupts */
	if(Config_Ptr->mode == NORMAL){
		TIMSK |= (1<<TOIE0);
	}
	else if(Config_Ptr->mode == COMPARE){
		TIMSK |= (1<<OCIE0);
	}
}

void Timer0_SetCallback(void (*a_Ptr)(void)){
	g_callBackPtr = a_Ptr;
}

void Timer0_DeInit(void){
	/* Clear all Timer0 Registers */
	TCCR0 = 0x00;
	TCNT0 = 0;
	OCR0 = 0;

	/* Disable Interrupts */
	TIMSK &= ~(1<<OCIE0) & ~(1<<TOIE0);
}

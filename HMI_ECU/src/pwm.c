/******************************************************************************
 *
 * Module: Timer0 PWM
 *
 * File Name: pwm.c
 *
 * Description: source file for the Timer0 PWM driver
 *
 * Author: Mohammad Wael
 *
 *******************************************************************************/

#include "pwm.h"

#include <avr/io.h>
#include "common_macros.h"

void PWM_Timer0_Start(uint8 duty_cycle){

	/* Setup OC0 as output pin */
	SET_BIT(DDRB,PB3);

	/* Set initial timer value to 0 */
	TCNT0 = 0;

	/* Fast PWM
	* Non-Inverting mode
	* Prescaler = FCPU/8 (CS01 = 1)           */
	TCCR0 |= (1<<COM01) | (1<<WGM01) | (1<<WGM00) | (1<<CS01);

	/* Set OCR0 to required compare value */
	OCR0 = duty_cycle * 255/100;
}

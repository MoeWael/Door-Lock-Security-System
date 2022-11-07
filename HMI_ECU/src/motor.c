/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: motor.c
 *
 * Description: source file for the DC Motor driver
 *
 * Author: Mohammad Wael
 *
 *******************************************************************************/

#include "motor.h"

#include "common_macros.h"

#include <avr/io.h>

#include "pwm.h"


void DcMotor_Init(){

	/* Set OC0 as output (PD7)
	 * Set direction pins as output */
	GPIO_setupPinDirection(MOTOR_EN_PORT_ID,MOTOR_EN_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PIN1_PORT_ID,MOTOR_PIN1_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PIN2_PORT_ID,MOTOR_PIN2_PIN_ID,PIN_OUTPUT);

	/* DC Motor initially OFF */
	GPIO_writePin(MOTOR_EN_PORT_ID,MOTOR_EN_PIN_ID,LOGIC_HIGH);
	GPIO_writePin(MOTOR_PIN1_PORT_ID,MOTOR_PIN1_PIN_ID,LOGIC_LOW);
	GPIO_writePin(MOTOR_PIN2_PORT_ID,MOTOR_PIN2_PIN_ID,LOGIC_LOW);
}

void DcMotor_Rotate(DcMotor_State state, uint8 speed){

	/* Write state (OFF, CW, Anti-CW) into motor pins
	 * by writing (0, 1, 2) into two seperate pins
	 * First pin gets first bit value
	 * Second pin gets second bit value shifted to the right
	 */
	GPIO_writePin(MOTOR_PIN1_PORT_ID,MOTOR_PIN1_PIN_ID,((state & 2)>>1));
	GPIO_writePin(MOTOR_PIN2_PORT_ID,MOTOR_PIN2_PIN_ID,(state & 1));

	/* Start Timer0 in PWM Mode */
	PWM_Timer0_Start(speed);
}


 /******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.c
 *
 * Description: Source file for the Buzzer AVR driver
 *
 * Author: Mohammad Wael
 *
 *******************************************************************************/


#include "buzzer.h"

#include <avr/io.h>
#include "gpio.h"

void Buzzer_Init(void){
	GPIO_setupPinDirection(BUZZER_PORT, BUZZER_PIN, PIN_OUTPUT);
}

void Buzzer_on(void){
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_HIGH);
}

void Buzzer_off(void){
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
}

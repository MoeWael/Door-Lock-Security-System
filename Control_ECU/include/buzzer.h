 /******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.h
 *
 * Description: Header file for the Buzzer AVR driver
 *
 * Author: Mohammad Wael
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_


#include "std_types.h"
#include "gpio.h"

#define BUZZER_PORT PORTA_ID
#define BUZZER_PIN  PIN0_ID

void Buzzer_Init(void);

void Buzzer_on(void);

void Buzzer_off(void);

#endif /* BUZZER_H_ */

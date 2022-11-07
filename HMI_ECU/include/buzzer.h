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

#define BUZZER_PORT PORTA
#define BUZZER_PIN  PIN0

void Buzzer_Init(void);

void Buzzer_on(void);

void Buzzer_off(void);

#endif /* BUZZER_H_ */

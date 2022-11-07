/*
 * MCU2.c
 *
 *  Created on: ٢٨‏/١٠‏/٢٠٢١
 *      Author: Moe
 */

#define F_CPU 8000000UL

#include "buzzer.h"
#include "external_eeprom.h"
#include "motor.h"
#include "pwm.h"
#include "timer.h"
#include "twi.h"
#include "uart.h"
#include "lcd.h"

#include <avr/io.h>
#include <util/delay.h>

#define READY '$'
#define FAIL  '@'
#define DANGER '&'

/* Create Config structs for the init functions */
Timer_ConfigType TimerConfig = {NORMAL, 0, 0, CLK_1024};
UART_ConfigType UARTConfig = {9600, 8, 1};
TWI_ConfigType TWIConfig = {400000, 0x01};

/* Timer ticks */
uint16 g_ticks = 0;

void Timer0_CallBack_15(){
	LCD_clearScreen();
	LCD_intgerToString(g_ticks);
	g_ticks++;
	if(g_ticks == 458){
		DcMotor_Rotate(MOTOR_OFF, 0);
		_delay_ms(3000);
		DcMotor_Rotate(MOTOR_ANTICLOCKWISE, 100);
	}
	if(g_ticks == 916){
		DcMotor_Rotate(MOTOR_OFF, 0);
		Timer0_DeInit();
		g_ticks = 0;
	}
}

void Timer0_CallBack_Buzzer(){
	LCD_clearScreen();
	LCD_intgerToString(g_ticks);
	g_ticks++;
	if(g_ticks == 458){
		Buzzer_off();
		Timer0_DeInit();
	}
}


int main(void){

	UART_init(&UARTConfig);
	LCD_init();
	TWI_init(&TWIConfig);
	DcMotor_Init();
	//Buzzer_Init();
	GPIO_setupPinDirection(PORTA_ID, PIN0_ID, PIN_OUTPUT);

	SREG |= (1<<7);

	uint8 receiveString[5] = {0};
	uint8 symbol;

	while(1){
		/* Wait for the Ready symbol */
		symbol = UART_recieveByte();
		LCD_displayCharacter(symbol);
		switch(symbol){
		case DANGER : {
				Timer0_Init(&TimerConfig);
				Timer0_SetCallback(Timer0_CallBack_Buzzer);
				GPIO_writePin(PORTA_ID, PIN0_ID, 1);
				LCD_clearScreen();
				LCD_displayString("BUZZER");
				_delay_ms(5000);

			break;
		}
		case READY :{
		/* Receive and save the password */
		UART_sendByte(READY);
		for(uint8 i = 0; i<5; i++){
			receiveString[i] = UART_recieveByte();
			LCD_intgerToString(receiveString[i]);
			EEPROM_writeByte(0x0000 + i, receiveString[i]);
			}
		break;
		}
		case '-' :{
			uint8 flag = 1;
			uint8 data;
			/* Receive and save the password */
			for(uint8 i = 0; i<5; i++){
				receiveString[i] = UART_recieveByte();
				LCD_intgerToString(receiveString[i]);
				EEPROM_readByte(0x0000 + i, &data);
				if(receiveString[i] != data){
					flag = 0;
				}
			}
			if(flag){
				UART_sendByte('-');
			}
			else{
				UART_sendByte(FAIL);
			}
			break;
		}
		case '+' :{

			uint8 flag = 1;
			uint8 data;
			/* Receive and save the password */
			for(uint8 i = 0; i<5; i++){
				receiveString[i] = UART_recieveByte();
				LCD_intgerToString(receiveString[i]);
				EEPROM_readByte(0x0000 + i, &data);
				/* Check if matched */
				if(receiveString[i] != data){
				flag = 0;
				}
			}
			if(flag){
				/* Send success symbol */
				UART_sendByte('+');
				/* Wait for response */
				LCD_clearScreen();
				LCD_displayCharacter(UART_recieveByte());
				/* Start unlocking and locking the door */
				Timer0_Init(&TimerConfig);
				Timer0_SetCallback(Timer0_CallBack_15);
				DcMotor_Rotate(MOTOR_CLOCKWISE, 100);

			}
			else{
				UART_sendByte(FAIL);
			}
			break;
		}


		}
	}
}

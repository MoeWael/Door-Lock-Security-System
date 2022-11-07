/*
 * MCU1.c
 *
 *  Created on: ٢٨‏/١٠‏/٢٠٢١
 *      Author: Moe
 */

#include "keypad.h"
#include "lcd.h"
#include "timer.h"
#include "uart.h"

#include "common_macros.h"

#include <util/delay.h>

#define READY  '$'
#define FAIL   '@'
#define DANGER '&'

/* Create Config structs for the init functions */
Timer_ConfigType TimerConfig = {NORMAL, 0, 0, CLK_1024};
UART_ConfigType UARTConfig = {9600, 8, 1};

uint8 receiveString1[5] = {0};
uint8 receiveString2[5] = {0};
uint8 rightPass = 1;

/* Timer ticks */
uint16 g_ticks = 0;

/* Counter for password mismatch number */
uint8 mismatchCounter = 0;

void delayfun(uint16 delay){
	_delay_ms(delay);
}

void Timer0_CallBack_15(){
	g_ticks++;
	if(g_ticks == 458){
		_delay_ms(3000);
		LCD_clearScreen();
		LCD_displayString("Locking");
	}
	if(g_ticks == 916){
		LCD_clearScreen();
		Timer0_DeInit();
	}
}

void CheckPassword(){
	/* Initialize the flag to 1 */
	rightPass = 1;
	/* Ask the user for the password */
	LCD_clearScreen();
	LCD_displayString("Enter Password :");
	/* Receive keypad presses
	 * Store in an array
	 * Display password on LCD */
	for(uint8 i = 0; i<5; i++){
		receiveString1[i] = KEYPAD_getPressedKey();
		LCD_moveCursor(1,i);
		LCD_intgerToString(receiveString1[i]);
		delayfun(300);
		LCD_moveCursor(1,i);
		LCD_displayCharacter('*');
	}
	/* Wait for Enter to be pressed */
	while(KEYPAD_getPressedKey() != 13);
	delayfun(300);
	/* Clear screen */
	LCD_clearScreen();
	/* Ask for password again */
	LCD_displayString("Enter Password :");
	/* Receive keypad presses
	 * Store in an array
	 * Display password on LCD
	 * Check if passwords match and update flag */
	for(uint8 i = 0; i<5; i++){
		receiveString2[i] = KEYPAD_getPressedKey();
		LCD_moveCursor(1,i);
		LCD_intgerToString(receiveString1[i]);
		delayfun(300);
		LCD_moveCursor(1,i);
		LCD_displayCharacter('*');
		if(receiveString1[i] != receiveString2[i]){
			rightPass = 0;
		}
	}
	/* Wait for Enter button */
	while(KEYPAD_getPressedKey() != 13);
}

void GetPassword(){
	/* Take the password from the user */
	for(uint8 i = 0; i<5; i++){
		receiveString1[i] = KEYPAD_getPressedKey();
		LCD_moveCursor(1,i);
		LCD_intgerToString(receiveString1[i]);
		delayfun(300);
		LCD_moveCursor(1,i);
		LCD_displayCharacter('*');
	}
	/* Wait for Enter to be pressed */
	while(KEYPAD_getPressedKey() != 13);
}

int main(void){

	uint8 key;
	UART_init(&UARTConfig);
	LCD_init();

	while(1){
		/* Ask the user for the password and check
		 * if it matches 2 times                   */
		CheckPassword();
		/* If passwords don't match display "Wrong Password"
		 * Return to the first stage                         */
		if(!rightPass){
			LCD_clearScreen();
			LCD_displayString("Wrong Password");
			_delay_ms(3000);
			continue;
		}
		/* If passwords match, display Matched */
		LCD_clearScreen();
		LCD_displayString("Matched");
		_delay_ms(2000);
		LCD_clearScreen();
		/* Send a READY symbol to indicate that
		 * the MCU is ready to send the password
		 * and wait for the respond with the same symbol  */
		UART_sendByte(READY);
		UART_recieveByte();
		/* Send the correct password */
		for(uint8 i = 0; i<5; i++){
			UART_sendByte(receiveString1[i]);
			delayfun(300);
			}
		while(1){
		/* Display Options menu */
		LCD_clearScreen();
		LCD_displayString("+ : Open Door");
		LCD_displayStringRowColumn(1,0,"- : Change Pass");
		/* Get the key corresponding to
		 * the desired option           */
		key = KEYPAD_getPressedKey();
		/* Wait until the key is pressed */
		while(key != '+' && key != '-'){
			key = KEYPAD_getPressedKey();
		}
		delayfun(300);
		/* Send signals to Control ECU indicating
		 * the chosen option                      */
		switch(key){
		case '+' : {
			uint8 success;
			/* Ask for the password */
			LCD_clearScreen();
			LCD_displayString("Enter password :");
			/* Send signal to control MCU indicating
			 * the chosen option                     */
			UART_sendByte('+');
			delayfun(100);
			GetPassword();
			/* Send the password to control MCU
			 * for check                        */
			for(uint8 i = 0; i<5; i++){
				UART_sendByte(receiveString1[i]);
				}
			/* Get the check result */
			success = UART_recieveByte();
			/* If passwords match, Display : "Unlocking"
			 * for 15 seconds, then locking for 15 seconds */
			if(success == '+'){
				UART_sendByte('+');
				Timer0_SetCallback(Timer0_CallBack_15);
				Timer0_Init(&TimerConfig);
				LCD_clearScreen();
				LCD_displayString("Unlocking");

			}
			else if (success == FAIL){
				mismatchCounter++;
					if(mismatchCounter == 3){
						mismatchCounter = 0;
						UART_sendByte(DANGER);
						LCD_clearScreen();
						LCD_displayString("Error : Mismatch");
						delayfun(5000);
					}
					else{
						LCD_clearScreen();
						LCD_displayString("Wrong Password");
						delayfun(2000);
				}
			}
			break;
		}

		case '-' : {
			uint8 success;
			/* Ask for the password */
			LCD_clearScreen();
			LCD_displayString("Enter password :");
			/* Send signal to control MCU indicating
			 * the chosen option                     */
			UART_sendByte('-');
			GetPassword();
			/* Send the password to control MCU
			 * for check                        */
			for(uint8 i = 0; i<5; i++){
				UART_sendByte(receiveString1[i]);
				}
			/* Get the check result */
			success = UART_recieveByte();
			/* If correct password, ask for the new password */
			if(success == '-'){
				LCD_clearScreen();
				LCD_displayString("Correct Password");
				_delay_ms(1000);
				CheckPassword();
				if(!rightPass){
					LCD_clearScreen();
					LCD_displayString("Wrong Password");
					_delay_ms(2000);
				}
				/* If passwords match, display Matched */
				if(rightPass){
					LCD_clearScreen();
					LCD_displayString("Matched");
					_delay_ms(2000);
					LCD_clearScreen();
					/* Send a READY symbol to indicate that
					 * the MCU is ready to send the password
					 * and wait for the respond with the same symbol  */
					UART_sendByte(READY);
					LCD_displayCharacter(UART_recieveByte());
					_delay_ms(1000);
					/* Send the correct password */
					for(uint8 i = 0; i<5; i++){
						delayfun(30);
						UART_sendByte(receiveString1[i]);
						}
				}
			}
			else if (success == FAIL){
				mismatchCounter++;
				if(mismatchCounter == 3){
					mismatchCounter = 0;
					UART_sendByte(DANGER);
					LCD_clearScreen();
					LCD_displayString("Error : Mismatch");
					delayfun(5000);
				}
				else{
					LCD_clearScreen();
					LCD_displayString("Wrong Password");
					delayfun(2000);
				}
			}
		}
		}
	}
	}
}


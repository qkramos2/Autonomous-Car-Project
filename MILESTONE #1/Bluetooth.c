/*
 * Bluetooth Module to access car settings in real time
 * 
 * Author : Quentin Ramos II
 * Created: 10/22/23  
*/

// Include standard C headers as well:

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include header files from past labs:

#include "DCMotors.h"
#include "UART.h"
#include "msp.h"
#include "Common.h"

//////////////////////////////////////////////////////////////
// --- Variables ---
//////////////////////////////////////////////////////////////
BOOLEAN printCamera = FALSE;

//////////////////////////////////////////////////////////////
//
// display_menu
//
// displays the menu via the bluetooth module
//
//////////////////////////////////////////////////////////////

void display_menu() {
	uart2_put("Command List:\r\n");
	uart2_put("-> H: help menu displays this menu\r\n");
	uart2_put("-> D: Change speed of DC motor\r\n");
	uart2_put("-> C: Display camera values\r\n");
}

//////////////////////////////////////////////////////////////
//
// get_speed
//
// changes the speed of the DC motor
//
//////////////////////////////////////////////////////////////

int get_speed() {
	char string[3] = "";
	int char_count = 0;
	
	while(1) {
		if (uart2_dataAvailable() == TRUE) {
			char ch = uart2_getchar();
			if (char_count < 3) {
				string[char_count] = ch;
				char_count++;
			}
		}
		else if (string[0] != '\0' && string[1] != '\0'){
			uart2_put("Setting Duty Cycle to: "); uart2_put(string); uart2_put("%\n\r");
			double dutyCycle = (double) atoi(string);
			toggleDCMotorForward(dutyCycle);
			strcpy(string, "");
			char_count = 0;
			return 0;
		}
	}
}

//////////////////////////////////////////////////////////////
//
// execute_cmd
//
// execute the command given via bluetooth
//
//////////////////////////////////////////////////////////////

void execute_cmd(char cmd) {
	switch(cmd){
		case 'H':
			display_menu();
			break;
		case 'D':
			uart2_put("Give a new speed (0 to 100):\r\n");
			get_speed();
			break;
		case 'C':
			if (printCamera == TRUE) {printCamera = FALSE;}
			else {printCamera = TRUE;}
			break;
	}
	uart2_put("Type a command:\r\n");
}

/* 
Name: Quentin Ramos II
Date: 09/25/23
*/

#include "msp.h" 
#include "Common.h"

//------------ Switch1 Init -----------
// Initalize switch 1
// Input: none
// Return: none
//-------------------------------------

void initSwitch1(void) {

	// configure PortPin for Switch 1 as port I/O 
	P1 -> SEL0 &= ~BIT1;
	P1 -> SEL1 &= ~BIT1;
	P1 -> REN  |=  BIT1;
	P1 -> OUT  |=  BIT1;
	
	// configure as input
	P1 -> DIR &= ~BIT1;         
}

//--------- Switch1_Pressed ----------
// Read and return the status of Switch1
// Input: none
// Return: TRUE if pressed, FALSE if not pressed
//------------------------------------

BOOLEAN Switch1_Pressed(void) {
	// check if pressed
	if (P1 -> IN & BIT1) 
		return FALSE;
	else
		return TRUE;
}

//------------ Switch2 Init -----------
// Initalize switch 2
// Input: none
// Return: none
//-------------------------------------

void initSwitch2(void) {
	
	// configure PortPin for Switch 2 as port I/O 
	P1 -> SEL0 &= ~BIT4;
	P1 -> SEL1 &= ~BIT4;
	P1 -> REN  |=  BIT4;
	P1 -> OUT  |=  BIT4;
	
	// configure as input
	P1 -> DIR &= ~BIT4;
}

//--------- Switch2_Pressed ----------
// Read and return the status of Switch2
// Input: none
// Return: TRUE if pressed, FALSE if not pressed
//------------------------------------

BOOLEAN Switch2_Pressed(void) {
	// check if pressed
	if (P1 -> IN & BIT4) 
		return FALSE;
	else
		return TRUE;
}

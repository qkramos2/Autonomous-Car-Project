/* 
Name: Quentin Ramos II
Date: 09/25/23
*/

#include "msp.h" 
#include "Common.h"

// ---------------------------------------------------
// ---------------  LED INITIALIZATION  --------------
// ---------------------------------------------------

void LED1_Init (void) {
    // configure PortPin for LED1 as port I/O 
    P1 -> SEL0 &= ~BIT0;
    P1 -> SEL1 &= ~BIT0;

    // make built-in LED1 LED high drive strength
    P1 -> DS |= BIT0;

    // make built-in LED1 out	 
    P1 -> DIR |= BIT0;

    // turn off LED
    P1 -> OUT &= ~BIT0;
}

void LED2_Init (void) {
    // configure PortPin for LED2 as port I/O 
    P2 -> SEL0 &= ~BIT0;
    P2 -> SEL1 &= ~BIT0;

    // make built-in LED2 LEDs high drive strength
    P2 -> DS |= BIT0;

    // make built-in LED2 out	 
    P2 -> DIR |= BIT0 | BIT1 | BIT2;

    // turn off LED
    P2 -> OUT &= ~BIT0 & ~BIT1 & ~BIT2;
}

// ---------------------------------------------------
// -------------  LED1 TOGGLE FUNCTIONS  -------------
// ---------------------------------------------------

// Off = False, On = True
BOOLEAN LED1_State (void) {
    if (P1 -> OUT & BIT0) 
			return TRUE;
    else 
			return FALSE;
}

void LED1_On (void) { P1 -> OUT |= BIT0; }

void LED1_Off (void) { P1 -> OUT &= ~BIT0; }

// ---------------------------------------------------
// -------------  LED2 TOGGLE FUNCTIONS  -------------
// ---------------------------------------------------

// Off = False, On = True
BOOLEAN LED2_State (void) {
    if ((P2 -> OUT & BIT0) || (P2 -> OUT & BIT1) || (P2 -> OUT & BIT2))
			return TRUE;
    else 
			return FALSE;
}

void LED2_On (uint16_t color) { P2 -> OUT |= color; }

void LED2_Off (uint16_t color) { P2 -> OUT &= ~color; }

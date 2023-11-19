/*  LJBeato 2021
    TimerA functionality to drive DC motor and Servo Motor
 */

#include "msp.h"
#include "Common.h"
#include <stdint.h>
#include <stdio.h>
#include "TimerA.h"
#include "UART.h"

// Make these arrays 5 deep, since we are using indexes 1-4 for the pins
static uint32_t DEFAULT_PERIOD_A0[5] = {0,0,0,0,0};
static uint32_t DEFAULT_PERIOD_A2[5] = {0,0,0,0,0};


// Register Documentation to reference:

// ****************************************************************************************************************************

	// Timer Capture/Compare Control (TIMER_A0->CCTL) Register Description:

	// ********************************************************************************************************
	// BIT 15-14: (XX)		Capture Mode (CM): 	
	//							00 = np capture
	//							01 = rising edge capture
	//              10 = falling edge capture
	//							11 = capture on both edges
	// --------------------------------------------------------------------------------------------------------
	// BIT 13-12: (XX)		Capture/Compare Input Select (CCIS): These bits select the TAxCCR0 input signal
	// 							00 = CCIxA
	// 							01 = CCIxB
	// 							10 = GND
	// 							11 = VCC
	// --------------------------------------------------------------------------------------------------------
	// BIT 11: (X)			Synchronize Capture Source (SCS): synchronize capture input signal with timer clock
	//							0 = asynchronous capture
	//							1 = synchronous capture
	// --------------------------------------------------------------------------------------------------------
	// BIT 8: (X)			Capture Mode (CAP): 	
	//							0 = Compare mode
	//							1 = Capture mode
	// --------------------------------------------------------------------------------------------------------
	// BIT 7-5: (XXX)		Output Mode (OUTMMOD):
	//							000 = OUT bit value
	//							001 = Set
	//							010 = Toggle/reset
	//							011 = Set/reset
	//							100 = Toggle
	//							101 = Reset
	//							110 = Toggle/set
	//							111 = Reset/set
	// --------------------------------------------------------------------------------------------------------
	// BIT 4: (X)			Capture/Compare Interrupt Enable (CCIE): enables interrupt request of corresponding CCIFG flag
	//							0 = Interrupt disabled
	//							1 = Interrupt enabled
	// --------------------------------------------------------------------------------------------------------
	// BIT 2: (X)			Output (OUT): For output mode 0, this bit directly controls the state of the output
	//							0 = Output low
	//							1 = Output high
	// --------------------------------------------------------------------------------------------------------
	// BIT 1: (X)			Capture Overflow (COV): Indicates capture overflow occurred (reset with software)
	//							0 = No capture overflow occurred
	//							1 = Capture overflow occurred
	// --------------------------------------------------------------------------------------------------------
	// BIT 0: (X)			Capture/Compare Interrupt Flag
	//							0 = No interrupt pending
	//							1 = Interrupt pending
	// ********************************************************************************************************

// ****************************************************************************************************************************

	// Timer Control (TIMER_A0->CTL) Register Description:

	// ********************************************************************************************************
	// BIT 9-8 (XX)			Timer_A Clock Source Select (TASSEL):
	//							00 = TAxCLK
	//							01 = ACLK
	//							10 = SMCLK
	//							11 = INCLK
	// --------------------------------------------------------------------------------------------------------
	// BIT 7-6 (XX)			Input Divider (ID): select the divider for the input clock
	//							00 = /1
	//							01 = /2
	//							10 = /4
	//							11 = /8
	// --------------------------------------------------------------------------------------------------------
	// BIT 5-4 (XX)			Mode Control (MC): Setting MCx = 00h when Timer_A not in use conserves power
	//							00 = Stop mode: Timer is halted
	//							01 = Up mode: Timer counts up to TAxCCR0
	//							10 = Continuous mode: Timer counts up to 0FFFFh
	//							11 = Up/down mode: Timer counts up to TAxCCR0 then down to 0000h
	// --------------------------------------------------------------------------------------------------------
	// BIT 2 (X)			Timer_A Clear (TACLR): Set this bit to clear TAxR and clock direction
	// --------------------------------------------------------------------------------------------------------
	// BIT 1 (X)			Timer_A Interrupt Enable (TAIE): enables the TAIFG interrupt request
	//							0 = Interrupt disabled
	//							1 = Interrupt enabled
	// --------------------------------------------------------------------------------------------------------
	// BIT 0 (X)			Timer_A Interrupt Flag (TAIFG):
	//							0 = No Interrupt pending
	//							1 = Interrupt pending
	// ********************************************************************************************************

// ****************************************************************************************************************************


//***************************PWM_Init*******************************
// PWM output on P2.4, P2.5, P2.6, P2.7
// Inputs:  period of P2.4...P2.7 is number of counts before output changes state
//          percentDutyCycle (0 -> 1.0)
//          pin number (1,2,3,4)
// Outputs: none
//*******************************************************************

int TIMER_A0_PWM_Init(uint16_t period, double percentDutyCycle, uint16_t pin) {

	// Timer A0.1
	if (pin == 1) {
		// configure P2.4:
		P2->SEL0 |=  BIT4;
		P2->SEL1 &= ~BIT4;
		P2->DIR  |=  BIT4;
	}

    // Timer A0.2
	else if (pin == 2) {
		// configure P2.5:
		P2->SEL0 |=  BIT5;
		P2->SEL1 &= ~BIT5;
		P2->DIR  |=  BIT5;
	}	

    // Timer A0.3
	else if (pin == 3) {
		// configure P2.6:
		P2->SEL0 |=  BIT6;
		P2->SEL1 &= ~BIT6;
		P2->DIR  |=  BIT6;
	}	

    // Timer A0.4
	else if (pin == 4) {
		// configure P2.7:
		P2->SEL0 |=  BIT7;
		P2->SEL1 &= ~BIT7;
		P2->DIR  |=  BIT7;
	}

	else return -2;
	
	// save the period for this timer instance
	// DEFAULT_PERIOD_A0[pin] where pin is the pin number
	DEFAULT_PERIOD_A0[pin] = period;

	// TIMER_A0->CCR[0]
	TIMER_A0->CCR[0] = DEFAULT_PERIOD_A0[pin];

	// TIMER_A0->CCTL[pin]
	// BIT   8:   0 = Compare Mode
	// BIT 7-5: 111 = Reset/Set
	// 0b 0000 0000 1110 0000
	TIMER_A0->CCTL[pin] = 0x00E0;
	
	// set the duty cycle
	uint16_t dutyCycle = (uint16_t) (percentDutyCycle * (double)DEFAULT_PERIOD_A0[pin]);

	// CCR[n] contains the dutyCycle just calculated, where n is the pin number
	// TIMER_A0->CCR[pin]
	TIMER_A0->CCR[pin] = dutyCycle;

	// TIMER_A0->CTL
	// BIT 9-8: 10 = SMCLK
	// BIT 7-6: 00 = /1 
	// BIT 5-4: 01 = Up Mode, 
	// BIT 2  :  1 = Clear
	// 0b 0000 0010 0001 0100
	TIMER_A0->CTL = 0x0214;

	return 0;
}

//***************************PWM_Duty1*******************************
// change duty cycle of PWM output on pin
// Inputs:  dutycycle, pin
// Outputs: none
// percentDutyCycle is a number between 0 and 1  (ie. 0.5 = 50%)
//*******************************************************************

void TIMER_A0_PWM_DutyCycle(double percentDutyCycle, uint16_t pin) {
	TIMER_A0->CCR[pin] = (uint16_t)(percentDutyCycle * (double)DEFAULT_PERIOD_A0[pin]);
}

//***************************PWM_Init*******************************
// PWM output on P5.6
// Inputs:  period of P5.6 is number of counts before output changes state
//          percentDutyCycle (0 -> 1.0)//          duty cycle
//          pin number (1,2,3,4), but always 1
//
// Outputs: none
//*******************************************************************

int TIMER_A2_PWM_Init(uint16_t period, double percentDutyCycle, uint16_t pin) {

	// NOTE: Timer A2 only exposes 1 PWM pin
	// TimerA2.1
	if (pin == 1) {
		// configure P5.6:
		P5->SEL0 |=  BIT6;
		P5->SEL1 &= ~BIT6;
		P5->DIR  |=  BIT6;
	}
	else return -2; 

	// NOTE: Setup similar to TimerA0
	// You will have to use the prescaler (clock divider) to get down to 20ms

	// save the period for this timer instance
	// DEFAULT_PERIOD_A2[pin] where pin is the pin number
	DEFAULT_PERIOD_A2[pin] = period;

	// TIMER_A2->CCR[0]
	TIMER_A2->CCR[0] = DEFAULT_PERIOD_A2[pin];

	// TIMER_A2->CCTL[pin]
	// BIT   8:   0 = Compare Mode
	// BIT 7-5: 111 = Reset/Set
	// 0b 0000 0000 1110 0000
	TIMER_A2->CCTL[pin] = 0x00E0;
	
	// set the duty cycle
	uint16_t dutyCycle = (uint16_t) (percentDutyCycle * (double)DEFAULT_PERIOD_A2[pin]);

	// CCR[n] contains the dutyCycle just calculated, where n is the pin number
	// TIMER_A2->CCR[pin]
	TIMER_A2->CCR[pin] = dutyCycle;

	// TIMER_A2->CTL
	// BIT 9-8: 10 = SMCLK
	// BIT 7-6: 11 = /8 
	// BIT 5-4: 01 = Up Mode, 
	// BIT 2  :  1 = Clear
	// 0b 0000 0010 1101 0100
	TIMER_A2->CTL = 0x02D4;
	
	// Divides Input by 2:
	TIMER_A2->EX0 = 0x01;
	
	return 0;
}

//***************************PWM_Duty1*******************************
// change duty cycle of PWM output on P5.6
// Inputs:  percentDutyCycle, pin  (should always be 1 for TimerA2.1)
//         
// Outputs: none
//
//*******************************************************************

void TIMER_A2_PWM_DutyCycle(double percentDutyCycle, uint16_t pin) {
	TIMER_A2->CCR[pin] = (uint16_t)(percentDutyCycle * (double)DEFAULT_PERIOD_A2[pin]);
}

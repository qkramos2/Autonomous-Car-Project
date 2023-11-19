/*
 * Main module for DCMotors
 * 
 * Author : Quentin Ramos II & Robbie Riviere
 * Created: 10/22/23  
*/

#include "DCMotors.h"

//////////////////////////////////////////////////////////////
//
// enableMotors: Enables DC Motors
//
//////////////////////////////////////////////////////////////

void enableMotors(void){ P3->OUT |= BIT6; P3->OUT |= BIT7; }

//////////////////////////////////////////////////////////////
//
// disableMotors: Disables DC Motors
//
//////////////////////////////////////////////////////////////

void disableMotors(void){ P3->OUT &= ~BIT6; P3->OUT &= ~BIT7; }

//////////////////////////////////////////////////////////////
//
// initDC: Initalizes DC Motors
//
//////////////////////////////////////////////////////////////

void initDC(){
    
    // DC Motor Pins M1EN & M2EN:
    // Pins 3.6 and 3.7 -> GPIO, OUT
    P3->SEL0 &= ~BIT6; P3->SEL1 &= ~BIT6; P3->DIR |= BIT6; 
    P3->SEL0 &= ~BIT7; P3->SEL1 &= ~BIT7; P3->DIR |= BIT7;

    // Initalize Timers with 10kHz clock & 0% Duty Cycle:
    unsigned long period = DCClock / DCFrequency;
    TIMER_A0_PWM_Init(period, STATIONARY, 1);
    TIMER_A0_PWM_Init(period, STATIONARY, 2);
    TIMER_A0_PWM_Init(period, STATIONARY, 3);
    TIMER_A0_PWM_Init(period, STATIONARY, 4);

	// Enable Motors:
	enableMotors();
}

//////////////////////////////////////////////////////////////
//
// stop: Stops DC Motors
//
//////////////////////////////////////////////////////////////

void stop(void){
	TIMER_A0_PWM_DutyCycle(STATIONARY, 1);
	TIMER_A0_PWM_DutyCycle(STATIONARY, 2);
	TIMER_A0_PWM_DutyCycle(STATIONARY, 3);
	TIMER_A0_PWM_DutyCycle(STATIONARY, 4);
}

//////////////////////////////////////////////////////////////
//
// drive: Move DC Motors forward given a speed
//
//////////////////////////////////////////////////////////////

void drive(double speed){
	// Toggles M1B and M2B:
	TIMER_A0_PWM_DutyCycle(speed/100.0, 2);
	TIMER_A0_PWM_DutyCycle(speed/100.0, 4);
	
	// Sets M1A and M2A to 0:
	TIMER_A0_PWM_DutyCycle(STATIONARY, 1);
	TIMER_A0_PWM_DutyCycle(STATIONARY, 3);
}

//////////////////////////////////////////////////////////////
//
// differentialTurn: create differential steering by making
//	outer wheel spin faster and inner slower by a specifc
//	differential factor that's calculated based on error.
//
// NOTES:
//	- TimerA0_2 / pin 2 = right motor (forward)
//	- TimerA0_4 / pin 4 = left motor  (forward)
//  - The slowest a wheel can go is MIN_SPEED = 25% duty cycle
//
// RIGHT: slow down right motor / speed up left motor
// LEFT : slow down left motor / speed up right motor
//
//////////////////////////////////////////////////////////////

void differentialTurn(double error, double max_error, double speed) {
	
	// Determine the inside and outside:
	int inner_wheel = (error > 0) ? 2 : 4;
	int outer_wheel = (error > 0) ? 4 : 2;
	
	// Vary the speed given the error:
	double differntial_factor = DIFF_COEF * (fabs(error) / max_error);
	double inner_wheel_speed = speed * (1 - differntial_factor);
	double outer_wheel_speed = speed * (1 + differntial_factor);
	
	// Ensure speed is in bounds:
	inner_wheel_speed = inner_wheel_speed < MIN_SPEED ? MIN_SPEED : inner_wheel_speed;
	outer_wheel_speed = outer_wheel_speed > MAX_SPEED ? MAX_SPEED : outer_wheel_speed;
	
	// Differentiate the Motor Speed:
	TIMER_A0_PWM_DutyCycle(inner_wheel_speed/100.0, inner_wheel);
	TIMER_A0_PWM_DutyCycle(outer_wheel_speed/100.0, outer_wheel);
	
}	

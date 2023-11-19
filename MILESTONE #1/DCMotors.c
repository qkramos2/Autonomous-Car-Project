/*
 * Main module for DCMotors
 * 
 * Author : Quentin Ramos II & Robbie Riviere
 * Created: 10/22/23  
*/

#include <math.h>
#include "msp.h"
#include "TimerA.h"
#include "DCMotors.h"

double DC_duty;

void DC_motors_init(){
    
    // DC Motor Pins M1EN & M2EN:
    // Pins 3.6 and 3.7 -> GPIO, OUT
    P3->SEL0 &= ~BIT6; P3->SEL1 &= ~BIT6; P3->DIR |= BIT6; 
    P3->SEL0 &= ~BIT7; P3->SEL1 &= ~BIT7; P3->DIR |= BIT7;

    // Initalize Timers with 10kHz clock & 0% Duty Cycle:
    unsigned long period = DCClock / DCFrequency;
    float duty_cycle = 0.0;
    TIMER_A0_PWM_Init(period, duty_cycle, 1);
    TIMER_A0_PWM_Init(period, duty_cycle, 2);
    TIMER_A0_PWM_Init(period, duty_cycle, 3);
    TIMER_A0_PWM_Init(period, duty_cycle, 4);

}

void enable_DCMotor(void){ P3->OUT |= BIT6; P3->OUT |= BIT7; }

void disable_DCMotor(void){ P3->OUT &= ~BIT6; P3->OUT &= ~BIT7; }

void stopDCMotor(void){
	TIMER_A0_PWM_DutyCycle(0.0, 1);
	TIMER_A0_PWM_DutyCycle(0.0, 2);
	TIMER_A0_PWM_DutyCycle(0.0, 3);
	TIMER_A0_PWM_DutyCycle(0.0, 4);
	DC_duty = 0.0;
}

void toggleDCMotorForward(double percentDutyCycle){
	// Toggles M1B and M2B:
	TIMER_A0_PWM_DutyCycle(percentDutyCycle/100.0, 2);
	TIMER_A0_PWM_DutyCycle(percentDutyCycle/100.0, 4);
	
	// Sets M1A and M2A to 0:
	TIMER_A0_PWM_DutyCycle(0, 1);
	TIMER_A0_PWM_DutyCycle(0, 3);
	DC_duty = percentDutyCycle;
}

// slow down right motor:
void rightTurn(double error) {
	// Toggles M1B and M2B:
	double percentDutyCycle = DC_duty - (DC_duty * ((fabs(error) * 5.0) / 100.0));
	TIMER_A0_PWM_DutyCycle(percentDutyCycle/100.0, 2);
	
	// Sets M1A and M2A to 0:
	TIMER_A0_PWM_DutyCycle(0, 1);
	TIMER_A0_PWM_DutyCycle(0, 3);
}

// slow down left motor:
void leftTurn(double error) {
	// Toggles M1B and M2B:
	double percentDutyCycle = DC_duty - (DC_duty * ((fabs(error) * 5.0) / 100.0));
	TIMER_A0_PWM_DutyCycle(percentDutyCycle/100.0, 4);
	
	// Sets M1A and M2A to 0:
	TIMER_A0_PWM_DutyCycle(0, 1);
	TIMER_A0_PWM_DutyCycle(0, 3);
}

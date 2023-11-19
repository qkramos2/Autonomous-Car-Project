/*
 * Main module for ServoMotor
 * 
 * Author : Quentin Ramos II & Robbie Riviere
 * Created: 10/22/23  
*/

#include "msp.h"
#include "Common.h"
#include "TimerA.h"
#include "ServoMotor.h"

/* 
 * Servomotor Notes:
 *
 * - 1.5ms pulse = stationary
 * - 1.0ms pulse = CW Rotation
 * - 2.0ms pulse = CCW Rotation
*/

/*
 * Create 1.5ms pulse on 50Hz clock (20ms period)
 */

void servoInit(){
	uint16_t period = servoClock / servoFrequency;
	double straight = 1.5 / servoPeriod;
	TIMER_A2_PWM_Init(period, straight, 1);
}

/*
 * Change the servo pulse between 1.0 and 2.0ms
*/
void toggleServo(double pulse){
	double dutyCycle = pulse / servoPeriod;
	TIMER_A2_PWM_DutyCycle(dutyCycle,1);
}

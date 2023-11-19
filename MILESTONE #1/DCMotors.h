/* 
   --------------------------------------------------------------
 * Name:    Quentin Ramos II & Robbie Riviere    
 * File:    DCMotors.h
 * Purpose: Provide function prototypes
   --------------------------------------------------------------
*/

// Include Statements:

#include "Common.h"

// -------- Custom Defines ------- //

#define DCClock  		(48000000)
#define DCFrequency (10000)

// ----- Function Prototypes ----- //

void DC_motors_init(void);
void enable_DCMotor(void);
void disable_DCMotor(void);
void stopDCMotor(void);
void toggleDCMotorForward(double percentDutyCycle);
void leftTurn(double error);
void rightTurn(double error);

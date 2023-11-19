/* 
   --------------------------------------------------------------
 * Name:    Quentin Ramos II & Robbie Riviere    
 * File:    ServoMotor.h
 * Purpose: Provide function prototypes
   --------------------------------------------------------------
*/

// Include Statements:

#include "Common.h"

// -------- Custom Defines ------- //

#define servoPeriod 		(20) 
#define servoClock  		(3000000)
#define servoFrequency 	(50)

// ----- Function Prototypes ----- //

void servoInit(void);
void toggleServo(double pulse);

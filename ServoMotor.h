/* 
   --------------------------------------------------------------
 * Name:    Quentin Ramos II & Robbie Riviere    
 * File:    ServoMotor.h
 * Purpose: Provide function prototypes
   --------------------------------------------------------------
*/

// ----- Include Statements ----- // 

#include "msp.h"
#include "Common.h"
#include "TimerA.h"

// -------- Custom Defines ------- //

#define servoPeriod 	  (20) 
#define servoClock  	  (3000000)
#define servoFrequency 	(50)

// ----- Function Prototypes ----- //

void initServo(void);
void toggleServo(double pulse);

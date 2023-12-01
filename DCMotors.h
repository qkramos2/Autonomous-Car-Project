/* 
   --------------------------------------------------------------
 * Name:    Quentin Ramos II & Robbie Riviere    
 * File:    DCMotors.h
 * Purpose: Provide function prototypes
   --------------------------------------------------------------
*/

// ----- Include Statements ----- // 

#include <math.h>

#include "msp.h"
#include "Common.h"
#include "TimerA.h"

// -------- Custom Defines ------- //

#define DCClock  	(48000000)
#define DCFrequency (10000)
#define MIN_SPEED   (25.0)  // tweak, try 20.0
#define MAX_SPEED   (100.0)
#define STATIONARY  (0.0)
#define DIFF_COEF   (0.28)  // tweak

// ----- Function Prototypes ----- //

void initDC(void);
void enableMotors(void);
void disableMotors(void);
void stop(void);
void drive(double speed);
void differentialTurn(double error, double max_error, double setSpeed);

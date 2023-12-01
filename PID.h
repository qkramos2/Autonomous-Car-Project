/* 
   --------------------------------------------------------------
 * Name:    Quentin Ramos II & Robbie Riviere    
 * File:    PID.h
 * Purpose: Provide function prototypes
   --------------------------------------------------------------
*/

// ----- Include Statements ----- // 

#include <stdio.h>
#include <stdlib.h>

#include "Common.h"

// -------- Custom Defines ------- //

#define Kp 1.05  	// tweak
#define Ki 0.0001  	// tweak
#define Kd 0.256  	// tweak

// ----- Function Prototypes ----- //
double compute_pid(double reference, double current_position);

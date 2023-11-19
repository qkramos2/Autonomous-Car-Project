/* 
   --------------------------------------------------------------
 * Name:    Quentin Ramos II & Robbie Riviere  
 * File:    uart.h
 * Purpose: Provide function prototypes
   --------------------------------------------------------------
*/

// Include Statements:

#include "Common.h"
#include <stdint.h>

// ----- Function Prototypes ----- // 

// Timer A0 Prototypes:

int TIMER_A0_PWM_Init(uint16_t period, double percentDutyCycle, uint16_t pin);
void TIMER_A0_PWM_DutyCycle(double percentDutyCycle, uint16_t pin);

// Timer A2 Prototypes:

int TIMER_A2_PWM_Init(uint16_t period, double percentDutyCycle, uint16_t pin);
void TIMER_A2_PWM_DutyCycle(double percentDutyCycle, uint16_t pin);

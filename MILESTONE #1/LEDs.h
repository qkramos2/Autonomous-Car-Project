/* 
   --------------------------------------------------------------
 * Name:    Quentin Ramos II    
 * File:    LEDs.h
 * Purpose: Provide function prototypes
   --------------------------------------------------------------
*/

// Include Statements:

#include <stdint.h>
#include "Common.h"

// ----- Function Prototypes ----- // 

// Initalizations:
void LED1_Init(void);
void LED2_Init(void);

// LED 1 Toggle:
BOOLEAN LED1_State(void);
void LED1_On (void);
void LED1_Off (void);

// LED 2 Toggle:
BOOLEAN LED2_State(void);
void LED2_On (uint16_t);
void LED2_Off (uint16_t);

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
#include "msp.h"  

#define RED     (BIT0)
#define GREEN   (BIT1)
#define BLUE    (BIT2)
#define CYAN    (BIT1 | BIT2)
#define MAGENTA (BIT0 | BIT2)
#define YELLOW  (BIT0 | BIT1)
#define WHITE   (BIT0 | BIT1 | BIT2)

// ----- Function Prototypes ----- // 

// Initalizations:
void initLED1(void);
void initLED2(void);

// LED 1 Toggle:
BOOLEAN LED1_State(void);
void LED1_On (void);
void LED1_Off (void);

// LED 2 Toggle:
BOOLEAN LED2_State(void);
void LED2_On (uint16_t);
void LED2_Off (uint16_t);

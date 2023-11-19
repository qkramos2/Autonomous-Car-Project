/* 
   --------------------------------------------------------------
 * Name:    Quentin Ramos II & Robbie Riviere    
 * File:    ServoMotor.h
 * Purpose: Provide function prototypes
   --------------------------------------------------------------
*/

// Include Statements:

#include "Common.h"
#include <stdint.h>

// -------- Custom Defines ------- //


// ----- Function Prototypes ----- //
void INIT_Camera(void);
void data_smoother(uint16_t* data, uint16_t* smoothData);
int track_position(uint16_t* smoothData);

/* 
   --------------------------------------------------------------
 * Name:    Quentin Ramos II & Robbie Riviere    
 * File:    ServoMotor.h
 * Purpose: Provide function prototypes
   --------------------------------------------------------------
*/

// ----- Include Statements ----- // 

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "msp.h"
#include "ADC14.h"
#include "Common.h"
#include "ControlPins.h"
#include "CortexM.h"

// -------- Custom Defines ------- //

#define MIN_CARPET_COUNT  (63)
#define CAM_RESOLUTION    (128)

// ----- Function Prototypes ----- //
void initCamera(void);
void data_smoother(uint16_t* data, uint16_t* smoothData);
double track_position(uint16_t* smoothData);
int carpetDetection(uint16_t* smoothData, int referenceADC);

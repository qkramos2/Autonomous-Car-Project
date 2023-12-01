/*
Program Name: CAR.c
 Description: Main program for TI Car Race Project
		 Authors: Quentin Ramos II & Robbie Riviere
--------------------------------------------------------------
Milestone #1 (DONE):
	- Drive car around oval

Milestone #2:
	- Drive car around figure 8

Milestone #3:
	- Drive car around custom track
		- Must be race speed
*/

//////////////////////////////////////////////////////////////
// Include all necessary header files for the car:
//////////////////////////////////////////////////////////////

// Include standard C headers as well:

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// Include all header files from past labs:

#include "Camera.h"
#include "Common.h"
#include "CortexM.h"
#include "DCMotors.h"
#include "LEDs.h"
#include "OLED.h"
#include "msp.h"
#include "PID.h"
#include "ServoMotor.h"
#include "StartMenu.h"
#include "Switches.h"
#include "UART.h"



//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\///////////////////////////////
//============================================================
//                    *** IMPORTANT ***
//           WHEN DEBUGGING COMMENT OUT RACE_MODE
//        (Turns off OLED, LEDs, UART, and Switches)
					  #define RACE_MODE
//============================================================
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\///////////////////////////////



//////////////////////////////////////////////////////////////
// --- DEFINES --- 
//////////////////////////////////////////////////////////////

// Reference Track Values:
#define REFERENCE_ADC 		(16300)
#define REFERENCE_TRACK_POS (64)

// Turning Defines:
#define TURN_THRESHOLD 	 	(6.000)	// tweak, try 5.00-4.00
#define MAX_TURN_THRESH  	(19.00)	// tweak
#define SERVO_STEP_SIZE	 	(0.020)	// tweak, try 0.017-0.010

// Camera Defines:
#define CARPET_THRESHOLD 	(500)

//////////////////////////////////////////////////////////////
// --- EXTERNAL VARIABLES --- 
//////////////////////////////////////////////////////////////

// Camera Variables:
extern BOOLEAN g_sendData;
extern uint16_t line[128];

// OLED Variables:
extern unsigned char OLED_TEXT_ARR[1024];

// Start Menu Variables:
extern double setSpeed;		// Default: 0.0
extern int driveMode;		// Default: Drive Mode
extern int steerMode;		// Default: Bang-Bang

//////////////////////////////////////////////////////////////
// --- INTERNAL VARIABLES --- 
//////////////////////////////////////////////////////////////

char str[100] = "";
double error = 0.0;
double decrementer = 0;
int carpetCounter = 0;
uint16_t color = GREEN;
uint16_t smoothData[128];

//////////////////////////////////////////////////////////////
//
// miscellaneous functions:
//
//////////////////////////////////////////////////////////////

// Millisecond delay:
void delay_ms(unsigned int milliseconds){ for (unsigned int i = 0; i < milliseconds * 48000; i++); }

// Button delay:
static void btnDelay(int delay) {int tmp; for (tmp = 0; tmp <= delay; tmp++){ continue; } }

// Flash Red LED:
void redFlash(void){ LED2_On(RED); delay_ms(40); LED2_Off(RED); delay_ms(5); }

//////////////////////////////////////////////////////////////
//
// Initalize the car by enabling all motors & sensors and
//   enabling interrupts
//
//////////////////////////////////////////////////////////////

void initCar(void){

	// Disable Interrupts & Initalize:
	DisableInterrupts();
	
	// Initalize Car Tings:
	initServo(); initDC(); initCamera();

	#ifndef RACE_MODE
		// Initalize Accessories:
		uart0_init(); OLED_Init();
	#endif

	// Initialize Switches & LED to indicate speed:
	initLED2(); initSwitch1(); initSwitch2();

	// Enable Interrupts and begin:
	EnableInterrupts();
	
	#ifndef RACE_MODE
		// Call the start menu:
		startMenu();
	#else
		// Select speed:
		int counterOne = 1; int lastCounter = 1;
		while(1) {
        // Toggle through speeds if switch 1 pressed:
        if (Switch1_Pressed()) { btnDelay(1500000); (counterOne == 6) ? counterOne = 1 : counterOne++; }
		if (counterOne == lastCounter) {
            // Go thru different speeds:
            switch (counterOne) {
                case 1:
					LED2_Off(WHITE); LED2_On(RED); setSpeed = 30.0; decrementer = 4.0; break; 		// works
                
                case 2:
                    LED2_Off(RED); LED2_On(YELLOW); setSpeed = 32.0; decrementer = 4.0; break;		// works
                
                case 3:
                    LED2_Off(YELLOW); LED2_On(GREEN); setSpeed = 34.0; decrementer = 4.0; break;	// works
                
                case 4:
                    LED2_Off(GREEN); LED2_On(MAGENTA); setSpeed = 36.0; decrementer = 6.0; break;	// works

				case 5:
                    LED2_Off(MAGENTA); LED2_On(CYAN); setSpeed = 38.0; decrementer = 6.0; break;	// not turning hard enough, try -8
				
				case 6:
                    LED2_Off(CYAN); LED2_On(WHITE); setSpeed = 40.0;  decrementer = 6.0; break;		// not turning hard enough,  try -10
            }
			// Change the temp variable
            (lastCounter == 6) ? lastCounter = 1 : lastCounter++;
        }
		
		if (Switch2_Pressed()) {LED2_Off(WHITE); return;}
		}

	#endif
}

//////////////////////////////////////////////////////////////
//
// set_steering function
//
// sets the steering angle based on the error from the
//  error calculation derived from the reference point.
//
// Notes:
// 	Positive error = left || Negative error = right
//
// Servo Calculations:
// 	Servo turns 120° total | 60° each side
// 	1.0 to 2.0 = 120° | 0.5ms = 60° | 0.1ms = 12°
//
// Wheel Calculations:
// 	Wheel turns 60° total | 30° each side
// 	1.0 to 2.0 = 60° | 0.5ms = 30° | 0.1ms = 6°
//
// Threshold Value Calculations:
//	> SERVO_STEP_SIZE = 0.5ms / abs(error)
//	> MAX_TURN_THRESH = abs(max camera error)
//	> TURN_THRESHOLD  = <trial and error value> 
//
//////////////////////////////////////////////////////////////

#ifndef RACE_MODE

void steer(double error){
	// If above turn threshold, turn the car:
	if (fabs(error) > TURN_THRESHOLD) {

		// Take a hard turn:
		if (fabs(error) >= MAX_TURN_THRESH) {

			// Change to red light:
			LED2_Off(color); color = RED; LED2_On(color);

			// Turn servo and adjust motor speed:
			toggleServo((error > 0) ? 2.0 : 1.0);
			differentialTurn(MAX_TURN_THRESH, MAX_TURN_THRESH, setSpeed);
		}
		// Ease into turn:
		else {
			// Change to yellow light:
			LED2_Off(color); color = YELLOW; LED2_On(color);

			// Turn servo and adjust motor speed:
			toggleServo(1.5 + (SERVO_STEP_SIZE * error));
			differentialTurn(error, MAX_TURN_THRESH, setSpeed);
		}

	}
	// if below threshold keep going straight:
	else {
		// Change to green light:
		LED2_Off(color); color = GREEN; LED2_On(color);

		// Go straight and go at setSpeed:
		toggleServo(1.5);
		drive(setSpeed);
	}
}

#else

void steer(double error){
	// If above turn threshold, turn the car:
	if (fabs(error) >= TURN_THRESHOLD) {
		// Take a hard turn:
		if (fabs(error) >= MAX_TURN_THRESH) {
			// Turn servo and adjust motor speed:
			toggleServo((error > 0) ? 2.0 : 1.0);
			differentialTurn(MAX_TURN_THRESH, MAX_TURN_THRESH, (setSpeed-decrementer));
		}
		// Ease into turn:
		else {
			// Turn servo and adjust motor speed:
			toggleServo(1.5 + (SERVO_STEP_SIZE * error));
			differentialTurn(error, MAX_TURN_THRESH, (setSpeed-decrementer));
		}
	}
	// if below threshold keep going straight:
	else {
		// Go straight and go at setSpeed:
		toggleServo(1.5);
		drive(setSpeed);
	}
}

#endif

//////////////////////////////////////////////////////////////
//
// Main Car Loop
//
//////////////////////////////////////////////////////////////

#ifndef RACE_MODE

int main(void){
	// Initalize Car:
	initCar();

	// Infinitely loop:
	while(1) {

		if (g_sendData == TRUE){

			// Smoothen the data and display data on OLED:
			data_smoother(line,smoothData);
			OLED_DisplayCameraData(smoothData);

			// Calculate current position:
			double current_pos = track_position(smoothData);

			// Calculate Error according to steerMode (0 = bang-bang | 1 = PID):
			error = (steerMode == 1) ? compute_pid(REFERENCE_TRACK_POS, current_pos) : REFERENCE_TRACK_POS - current_pos;
			
			// 0 = drive mode (carpet detection) | 1 = debug mode:
			if (driveMode == 1) {
				// Print the error to UART:
				sprintf(str, "%f", error); uart0_put(str); uart0_put("\r");
			}
			else {
				// if carpet counter is over threshold, stop the car:
				if (carpetCounter > CARPET_THRESHOLD) {
					stop(); carpetCounter = 0;
				}
				// if carpet detected; keep turning in same direction to find track:
				else if (carpetDetection(smoothData, REFERENCE_ADC) == 1) {
					// Change light to Blue to indicate carpet:
					LED2_Off(color); color = BLUE; LED2_On(color);
					carpetCounter++; 
				}
				// steer the motor regularly and reset carpet counter:
				else { 
					steer(error); carpetCounter = 0; 
				}
			}
		}
	}
}

#else

int main(void){
	// Initalize Car:
	initCar();

	// Infinitely loop:
	while(1) {

		if (g_sendData == TRUE){

			// Smoothen the data:
			data_smoother(line,smoothData);

			// Calculate current position:
			double current_pos = track_position(smoothData);

			// Calculate Error
			error = compute_pid(REFERENCE_TRACK_POS, current_pos);
			
			// if carpet counter is over threshold, stop the car:
			if (carpetCounter > CARPET_THRESHOLD) { stop(); carpetCounter = 0; }
			
			// if carpet detected; keep turning in same direction to find track:
			else if (carpetDetection(smoothData, REFERENCE_ADC) == 1) { carpetCounter++; }
			
			// steer the motor regularly and reset carpet counter:
			else { carpetCounter = 0; steer(error); }
		}
	}
}

#endif

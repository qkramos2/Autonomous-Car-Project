/*
Program Name: CAR.c
 Description: Main program for TI Car Race Project
		 Authors: Quentin Ramos II & Robbie Riviere
--------------------------------------------------------------
Car Shield Pinout:

OLED Screen:			Camera:							DC Motors:
	- SDA = P1.6			- CLK = P5.4				- M1A  = P2.4 (TA0.1)
	- SCL = P1.7			-  SI = P5.5				- M1B  = P2.5 (TA0.2)
										-  AO = P4.7				- M2A  = P2.6 (TA0.3)
																				- M2B  = P2.7 (TA0.4)																				
Servo Motor:			Carpet Det:						- M1EN = P3.6
	- SIG = P5.2			- SIG = P5.2				- M2EN = P3.7
	- SIG = P5.6													
--------------------------------------------------------------
TODO:
	Milestone #1:
		- Drive car around circle:
			- Enable DC Motors (Drive forward)
			- Enable Camera (Border Detection)
			- Enable Servo (turn given the camera signal)
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

// Include all header files from past labs:

#include "ADC14.h"
#include "Bluetooth.h"
#include "Camera.h"
#include "Common.h"
#include "ControlPins.h"
#include "CortexM.h"
#include "DCMotors.h"
#include "LEDS.h"
#include "OLED.h"
#include "msp.h"
#include "PID.h"
#include "ServoMotor.h"
#include "Switches.h"
#include "SysTickTimer.h"
#include "Timer32.h"
#include "TimerA.h"
#include "UART.h"

//////////////////////////////////////////////////////////////
// --- VARIABLES --- 
//////////////////////////////////////////////////////////////

extern BOOLEAN g_sendData;
extern uint16_t line[128];
extern BOOLEAN printCamera;
extern unsigned char OLED_TEXT_ARR[1024];
extern double DC_duty;

uint16_t smoothData[128];
int referenceTrackPosition = 0;
int error = 0;

//////////////////////////////////////////////////////////////
//
// Initalize the car by enabling all motors & sensors and
//   enabling interrupts
//
//////////////////////////////////////////////////////////////

void initCar(void){
	
	// Disable Interrupts & Initalize
	DisableInterrupts();
	
	// Initalize Motors & Camera:
	servoInit();
	DC_motors_init();
	enable_DCMotor();
	INIT_Camera();
	
	// Initalize LEDs, OLED, Switches and UART:
	LED1_Init();
	LED2_Init();
	OLED_Init();
	OLED_display_on();
	OLED_display_clear();
	OLED_display_on();
	Switch1_Init();
	Switch2_Init();
	uart0_init(SystemCoreClock);
	uart2_init(SystemCoreClock);
	
	
	// Enable Interrupts and begin:
	EnableInterrupts();
}

//////////////////////////////////////////////////////////////
//
// set_steering fuinction
//
// sets the steering angle based on the error from the
//  error calculation derived from the reference point.
//
//////////////////////////////////////////////////////////////

void set_steering(double error){
	
	// Positive error = left
	// Negative error = right
	
	double servoIncrement = 0.05;
	// double DCIncrement = 0.05;
	
	
	// Threshold of 3 before turning
	if (fabs(error) > 3.0) {
		
		// Hard left
		if (error >= 10.0) { toggleServo(2.0); leftTurn(10.0);}
		
		// Hard right
		else if (error <= -10.0) { toggleServo(1.0); rightTurn(10.0);}
		
		// Turn approaching:
		else {
			toggleServo(1.5 + (servoIncrement * error));
			if (error > 0) { leftTurn(error); }
			else { rightTurn(error); }
		}
	}
	else {
		toggleServo(1.5);
		toggleDCMotorForward(20.0);
	}
}

//////////////////////////////////////////////////////////////
//
// Main Car Loop
//
//////////////////////////////////////////////////////////////

void msdelay(int del){
	volatile int i;
	for (i=0; i<del*50000; i++){;}
}

int main(void){
	
	// Initalize the car:
	initCar();
	uart2_put("TI Car Initalized\r\n");
	display_menu();
	
	// Infinitely loop:
	while(1){
		
		// If there was a command given via bluetooth execute it:
		if (uart2_dataAvailable() == TRUE) {
			execute_cmd(uart2_getchar());
		}
		
		if (g_sendData == TRUE){
			data_smoother(line,smoothData);
			OLED_DisplayCameraData(smoothData);
			
			// Set the reference point if not set
			if (referenceTrackPosition == 0){
				referenceTrackPosition = track_position(smoothData);
				char temp[50] = ""; 
				sprintf(temp, "%d", referenceTrackPosition);
				uart2_put("\n\r ---- Threshold Value: "); uart2_put(temp); 
				uart2_put(" ---- \n\r"); uart2_put("Type a command:\r\n");
			}
			
			// If the reference point is set calculate the current track
			//  position and determine steering direction:
			else {
				
				// Error bounds: -64 < error < 64
				error = referenceTrackPosition - track_position(smoothData);
				
				// Print the error if stated:
				if (printCamera == TRUE) {
					char result[50] = ""; 
					sprintf(result, "%d", error);
					uart2_put(result); uart2_put("\n\r");
				}
				
				// steer the gawd dayum motor:
				set_steering(error);
				toggleDCMotorForward(20.0);
			}
		}
	}
}

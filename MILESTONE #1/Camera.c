/*
 * Main module for Line Scan Camera
 * 
 * Author : Quentin Ramos II & Robbie Riviere
 * Created: 10/22/23  
*/

#include <stdio.h>
#include <stdlib.h>

#include "msp.h"
#include "uart.h"
#include "leds.h"
#include "switches.h"
#include "Timer32.h"
#include "CortexM.h"
#include "Common.h"
#include "ADC14.h"
#include "ControlPins.h"

///////////////////////////////////////////////////////
//
// NOTE: For the camera, you may want to change the default
//       clock to 48MHz
//
// To do that: Edit system_msp432p401r.c
//             Change:   #define  __SYSTEM_CLOCK    3000000
//             To:       #define  __SYSTEM_CLOCK    48000000 
// ADC will be P4.7 A6
//
// SI Pin will be P5.5 A0
//
// CLK Pin will be P5.4 A1//
//
// line stores the current array of camera data
//
///////////////////////////////////////////////////////

extern BOOLEAN g_sendData;
int numDataPoints = 128;

///////////////////////////////////////////////////////
//
// INIT_Camera function
//
///////////////////////////////////////////////////////

void INIT_Camera(void) {
	g_sendData = FALSE;
	ControlPin_SI_Init();
	ControlPin_CLK_Init();
	ADC0_InitSWTriggerCh6();
}

///////////////////////////////////////////////////////
//
// data_smoother function
//
// smoothens the raw data (used to then binarize if on
//   track during car race)
//
///////////////////////////////////////////////////////

void data_smoother(uint16_t* data, uint16_t* smoothData){
	
	// Calculate 5-pt Moving Average of raw data:
	for (int i = 2; i < numDataPoints - 2; i++) {
		smoothData[i] = (data[i - 2] + data[i - 1] + data[i] + data[i + 1] + data[i + 2]) / 5;
	}
	
	// Handle the first and last two points separately (since they don't have 5 neighbors)
	smoothData[0] = (data[0] + data[1] + data[2]) / 3;
	smoothData[1] = (data[0] + data[1] + data[2] + data[3]) / 4;
	smoothData[numDataPoints - 2] = (data[numDataPoints - 4] + data[numDataPoints - 3] + data[numDataPoints - 2] + data[numDataPoints - 1]) / 4;
	smoothData[numDataPoints - 1] = (data[numDataPoints - 3] + data[numDataPoints - 2] + data[numDataPoints - 1]) / 3;
}

///////////////////////////////////////////////////////
//
// track_position function
//
// takes a weighted sum to figure out where you are
//   relative to the center of the track
//
///////////////////////////////////////////////////////

int track_position(uint16_t* smoothData){
	int sum = 0;
	int weightedSum = 0;

	// Calculate the position on the track:
	for (int i = 0; i < numDataPoints; i++) {
		sum += smoothData[i];
		weightedSum += smoothData[i] * i;
	}
	
	return weightedSum / sum;
}

/*
 * Main module for Line Scan Camera
 * 
 * Author : Quentin Ramos II & Robbie Riviere
 * Created: 10/22/23  
*/

#include "Camera.h"

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
// CLK Pin will be P5.4 A1
//
// line stores the current array of camera data
//
///////////////////////////////////////////////////////

extern BOOLEAN g_sendData;

///////////////////////////////////////////////////////
//
// initCamera function
//
///////////////////////////////////////////////////////

void initCamera(void) {
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
	for (int i = 2; i < CAM_RESOLUTION - 2; i++) {
		smoothData[i] = (data[i - 2] + data[i - 1] + data[i] + data[i + 1] + data[i + 2]) / 5;
	}
	
	// Handle the first and last two points separately (since they don't have 5 neighbors)
	smoothData[0] = (data[0] + data[1] + data[2]) / 3;
	smoothData[1] = (data[0] + data[1] + data[2] + data[3]) / 4;
	smoothData[CAM_RESOLUTION - 2] = (data[CAM_RESOLUTION - 4] + data[CAM_RESOLUTION - 3] + data[CAM_RESOLUTION - 2] + data[CAM_RESOLUTION - 1]) / 4;
	smoothData[CAM_RESOLUTION - 1] = (data[CAM_RESOLUTION - 3] + data[CAM_RESOLUTION - 2] + data[CAM_RESOLUTION - 1]) / 3;
}

///////////////////////////////////////////////////////
//
// track_position function
//
// takes a weighted sum to figure out where you are
//   relative to the center of the track
//
///////////////////////////////////////////////////////

double track_position(uint16_t* smoothData){
	double sum = 0;
	double weightedSum = 0;

	// Calculate the position on the track:
	for (int i = 0; i < CAM_RESOLUTION; i++) {
		sum += smoothData[i];
		weightedSum += smoothData[i] * i;
	}

	return weightedSum / sum;;
}

//////////////////////////////////////////////////////////////
//
// Carpet Detection:
//
// Detects if every other point out of 128 data points across 
//	the line scan camera are lower than the carpet threshold
//
//////////////////////////////////////////////////////////////

int carpetDetection(uint16_t* smoothData, int referenceADC) {
    int carpetThreshold = referenceADC - 10000;
    int counter = 0;

    for (int i = 0; i < CAM_RESOLUTION; i+=2) {
        if (smoothData[i] < carpetThreshold) {
            counter++;
			if (counter >= MIN_CARPET_COUNT) { return 1; }
        }
    }

    return 0;
}

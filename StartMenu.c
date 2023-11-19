/*
 * Start menu module to toggle settings:
 * 
 * Author : Quentin Ramos II
 * Created: 11/05/23  
*/

#include "StartMenu.h"

//////////////////////////////////////////////////////////////
// Variables to be passed on into car:
//////////////////////////////////////////////////////////////

// Variables to be passed on into car:

int driveMode = 0;      // 0 = race, 1 = debug
int steerMode = 0;      // 0 = bang-bang, 1 = PID
double setSpeed = 0.0;  // Speed of car

// Other Misc.

int counterOne = 1;
int counterTwo = 1;
int executeOptionMenu = 0;

//////////////////////////////////////////////////////////////
// Opening Screen:
//////////////////////////////////////////////////////////////

void openingScreen(void){
    // Initial Start Menu Screen:
    clear_OLED();
    OLED_Print(1,1,"<< Start Menu >>");
    OLED_Print(2,1," -SW1- = Scroll ");
    OLED_Print(3,1," -SW2- = Select ");
    OLED_Print(4,1," SW1/2 to Start ");
}

//////////////////////////////////////////////////////////////
// Quick Start Screen:
//////////////////////////////////////////////////////////////

void quickStart(void){
    // Initial Start Menu Screen:
    clear_OLED();
    OLED_Print(1,1,"Quick Start Menu");
	OLED_Print(2,1,"<< Choose one >>");
    OLED_Print(3,1," -SW1- = Quick  ");
    OLED_Print(4,1," -SW2- = Custom ");
}

//////////////////////////////////////////////////////////////
// Option Menu:
//////////////////////////////////////////////////////////////

void optionMenu(void){
    // Create a new menu:
    clear_OLED();
    OLED_Print(1,1,"-> Choose Mode  ");
    OLED_Print(2,1,"Change Speed    ");
    OLED_Print(3,1,"Steer Mode      ");
    OLED_Print(4,1,"Start Race!     ");
    executeOptionMenu = 0;
}

//////////////////////////////////////////////////////////////
// Small delay:
//////////////////////////////////////////////////////////////

static void delay(int delay) {
	int tmp; for (tmp = 0; tmp <= delay; tmp++){ continue; }
}

//////////////////////////////////////////////////////////////
// Set Mode option:
//////////////////////////////////////////////////////////////
void setMode(void){
	// Display new prompt:
    clear_OLED();
    OLED_Print(1,1,"Select a mode:  ");
    OLED_Print(2,1,"SW1:Scrl SW2:Sel");
    OLED_Print(3,4,"Drive Mode      ");
    OLED_Print(4,4,"Debug Mode      ");
	
	// Change the counter to show the mode if changed before:
	counterTwo = driveMode + 1;
	
	// Choose the mode:
    while(executeOptionMenu == 0) {
        if (Switch1_Pressed()) { delay(DELAY_TIME); (counterTwo == 2) ? counterTwo = 1 : counterTwo++; }
        switch (counterTwo) {
            case 1:
                OLED_Print(4,1,"  "); OLED_Print(3,1,"->"); 
                if (Switch2_Pressed()) { driveMode = 0; executeOptionMenu = 1; }
                break;
            case 2:
                OLED_Print(3,1,"  "); OLED_Print(4,1,"->");
                if (Switch2_Pressed()) { driveMode = 1; executeOptionMenu = 1; }
                break;
        }
    }
    if (executeOptionMenu) { optionMenu(); }
}

//////////////////////////////////////////////////////////////
// Speed Change:
//////////////////////////////////////////////////////////////

void changeSpeed(void) {
	// Display new prompt:
    clear_OLED();
    OLED_Print(1,1,"Change the speed");
    OLED_Print(2,1,"SW1 = Change Spd");
    OLED_Print(3,1,"SW2 = Save & Set");
    OLED_Print(4,1,">>  - 00.0 -  <<");
	
	// Initalize counter & string var:
	char temp[20]; counterTwo = (((int) setSpeed) - 5) / 5;

	// Change and Set speed:
    while (executeOptionMenu == 0) {
        if (Switch1_Pressed()) {
            delay(DELAY_TIME * 25);
            counterTwo = (counterTwo == 20) ? 1 : counterTwo + 1;
        }
        switch (counterTwo) {
            case 1 ... 20:
                setSpeed = (counterTwo * 5.0) - 5;
				sprintf(temp,">>  - %02d.0 -  <<", (int) setSpeed);
                OLED_Print(4, 1, temp);
                if (Switch2_Pressed()) { executeOptionMenu = 1; }
                break;
        }
    }
	// Print option menu and exit:
    if (executeOptionMenu) { optionMenu(); }
}

//////////////////////////////////////////////////////////////
// Steering Change:
//////////////////////////////////////////////////////////////
void changeSteering(void){
	// Display new prompt:
    clear_OLED();
    OLED_Print(1,1,"Select Mode:    ");
    OLED_Print(2,1,"SW1:Scrl SW2:Sel");
    OLED_Print(3,4,"Bang-Bang       ");
    OLED_Print(4,4,"PID Control     ");
	
	// Change the counter to show the steer mode if changed before:
	counterTwo = steerMode + 1;
	
	// Change the steer mode:
    while(executeOptionMenu == 0) {
        if (Switch1_Pressed()) { delay(DELAY_TIME); (counterTwo == 2) ? counterTwo = 1 : counterTwo++; }
        switch (counterTwo) {
            case 1:
                OLED_Print(4,1,"  "); OLED_Print(3,1,"->"); 
                if (Switch2_Pressed()) { steerMode = 0; executeOptionMenu = 1; }
                break;
            case 2:
                OLED_Print(3,1,"  "); OLED_Print(4,1,"->");
                if (Switch2_Pressed()) { steerMode = 1; executeOptionMenu = 1; }
                break;
        }
    }
    if (executeOptionMenu) { optionMenu(); }
}
//////////////////////////////////////////////////////////////
// Start Menu:
//////////////////////////////////////////////////////////////

void startMenu(void) {

    // lastCounter allows to check if counter changed:
    int lastCounter = 1;

    // Display Opening Screen:
    openingScreen();

    // Wait til user presses switch 1 or 2:
    while(1) {if(Switch1_Pressed() || Switch2_Pressed()){ delay(DELAY_TIME); break; }}
	
	// Prompt for quick start:
	quickStart();
	
    // Choose between quick start & custom:
    while(1) {

		// Drive Speed = 35 // Race Mode // PID Mode:
		if(Switch1_Pressed()) { setSpeed = 35.0; driveMode = 0; steerMode = 1; return; }
		
		// Toggle Options Instead:
		else if(Switch2_Pressed()){ delay(DELAY_TIME); optionMenu(); break; }
	}

    // Loop through options:
    while(1) {
        
        // Toggle through prompt if switch 1 pressed:
        if (Switch1_Pressed()) { delay(DELAY_TIME); (counterOne == 4) ? counterOne = 1 : counterOne++; }
        
        // Change the display prompts:
        if (counterOne == lastCounter) {

            // Go trhough different seetings:
            switch (counterOne) {

                // Mode toggle:
                case 1:
					OLED_Print(2,1,"Change Speed   "); // 2
                    OLED_Print(3,1,"Steer Mode     "); // 3
					OLED_Print(4,1,"Start Race!    "); // 4
					OLED_Print(1,1,"-> Choose Mode "); // 1
                    break;
                
                // Speed Change: 
                case 2:
                    OLED_Print(1,1,"Choose Mode    "); // 4
                    OLED_Print(3,1,"Steer Mode     "); // 2
					OLED_Print(4,1,"Start Race!    "); // 3
					OLED_Print(2,1,"-> Change Speed"); // 1
                    break;
                
                // Set Steering:
                case 3:
                    OLED_Print(1,1,"Choose Mode    "); // 3
					OLED_Print(2,1,"Change Speed   "); // 4
					OLED_Print(4,1,"Start Race!    "); // 2
					OLED_Print(3,1,"-> Steer Mode  "); // 1
                    break;
                
                // Start Race:
                case 4:
                    OLED_Print(1,1,"Choose Mode    "); // 2
					OLED_Print(2,1,"Change Speed   "); // 3
                    OLED_Print(3,1,"Steer Mode     "); // 4
					OLED_Print(4,1,"-> Start Race! "); // 1
                    break;
            }

            // Change the temp variable
            (lastCounter == 4) ? lastCounter = 1 : lastCounter++;
        }

        // Click on the shown prompt if switch 2 pressed:
        if (Switch2_Pressed()) {

            // Ensure no double click:
            delay(DELAY_TIME);

            // Go into the selected mode:
            switch (counterOne) {

                // Mode toggle:
                case 1: setMode(); break;
                
                // Speed Change: 
                case 2: changeSpeed(); break;
                
                // Set Steering:
                case 3: changeSteering(); break;
                
                // Start Race:
                case 4: return;
            }

            // Reset the counter back up to 1:
            counterOne = 1;
        }
    }    
}

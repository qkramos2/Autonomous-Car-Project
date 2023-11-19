/*
 * Main module for PID Controller
 * 
 * Author : Quentin Ramos II & Robbie Riviere
 * Created: 10/22/23  
*/

#include "PID.h"

double last_error = 0;
double integral = 0;
double derivative = 0;

double compute_pid(double reference, double current_position) {

    // Calculate the error
    double error = reference - current_position;

    // Calculate the integral term
    integral += error;

    // Calculate the derivative term
    derivative = error - last_error;

    // Compute the PID control signal
    double output = (Kp * error) + (Ki * integral) + (Kd * derivative);

    // Update the last error for the next iteration
    last_error = error;

    return output;
}

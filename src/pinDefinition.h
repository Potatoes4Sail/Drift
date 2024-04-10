//
// Created by Patrick on 2024-02-21.
//

#include "helperFunctions.h"

#ifndef DRIFT_PINDEFINITION_H
#define DRIFT_PINDEFINITION_H

/*
 *  MOTOR DEFINITIONS
 */

// Pins are defined with the 'digital' numbering (0-19)
#define MOTOR_PIN_PWM          3
#define MOTOR_PIN_FORWARD      4
#define MOTOR_PIN_REVERSE      2

#define SERVO_CONTROL_PIN_1     5
#define SERVO_CONTROL_PIN_2     6
#define SERVO_CONTROL_BANK_1    OCR0A
#define SERVO_CONTROL_BANK_2    OCR0B
#define USE_SERVO_PIN_2         false   // Allows for disabling parts of the code for servo2


//#define MOTOR_PWM_REGISTER     OCR2B   // Based on MOTOR_PIN_PWM.

// TODO: Add some method of verifying that the right pins are used (ALL PART OF THE SAME BANK)

#endif //DRIFT_PINDEFINITION_H
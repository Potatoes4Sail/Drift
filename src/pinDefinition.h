//
// Created by Patrick on 2024-02-21.
//

#pragma once
#include "helperFunctions.h"

// #define INCLUDECUSTOM
#ifndef DRIFT_PINDEFINITION_H
#define DRIFT_PINDEFINITION_H

/*
 *  MOTOR DEFINITIONS
 */

// Pins are defined with the 'digital' numbering (0-19)
#define MOTOR_PIN_PWM          6
#define MOTOR_PIN_FORWARD      5
#define MOTOR_PIN_REVERSE      4
#define MOTOR_PWM_REGISTER     OCR0A   // Based on MOTOR_PIN_PWM.

// TODO: Add some method of verifying that the right pins are used (ALL PART OF THE SAME BANK)


/*
 * ULTRASONIC SENSOR PINS:
 */

// Use digital pin numbering
#define ULTRASONIC_SENSOR_TRIGGER_PIN 7
#define ULTRASONIC_SENSOR0_ECHO_PIN 8
#define ULTRASONIC_SENSOR1_ECHO_PIN 9
#define ULTRASONIC_SENSOR2_ECHO_PIN 10

#define ULTRASONIC_SENSOR0_INT PCINT0
#define ULTRASONIC_SENSOR1_INT PCINT1
#define ULTRASONIC_SENSOR2_INT PCINT2

#define ULTRASONIC_SENSORS_PIN_MASK PCMSK0

#define ULTRASONIC_SENSORS_INT_vect PCINT0_vect

#endif //DRIFT_PINDEFINITION_H
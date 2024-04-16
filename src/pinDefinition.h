//
// Created by Patrick on 2024-02-21.
//
#pragma once

#ifndef DRIFT_PINDEFINITION_H
#define DRIFT_PINDEFINITION_H

#include "helperFunctions.h"
/*
 *  MOTOR DEFINITIONS
 */

// Pins are defined with the 'digital' numbering (0-19)
#define MOTOR_PIN_PWM          3
#define MOTOR_PIN_FORWARD      4
#define MOTOR_PIN_REVERSE      2

#define MOTOR_PWM_REGISTER     OCR0A   // Based on MOTOR_PIN_PWM.

#define SERVO_CONTROL_PIN_1     5
#define SERVO_CONTROL_PIN_2     6
#define SERVO_CONTROL_BANK_1    OCR0A
#define SERVO_CONTROL_BANK_2    OCR0B
#define USE_SERVO_PIN_2         false   // Allows for disabling parts of the code for servo2



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

#define SENSOR_MASK_MACRO(P) _BV(ULTRASONIC_SENSOR##P##_INT)
#define ULTRASONIC_SENSOR_INTERRUPT_MASK (SENSOR_MASK_MACRO(0) | SENSOR_MASK_MACRO(1) | SENSOR_MASK_MACRO(2))

#define ULTRASONIC_SENSORS_PIN_MASK PCMSK0
#define ULTRASONIC_SENSORS_BANK     PCIE0

#define ULTRASONIC_SENSORS_INT_vect PCINT0_vect

// TODO: Add some method of verifying that the right pins are used (ALL PART OF THE SAME BANK)
#endif //DRIFT_PINDEFINITION_H
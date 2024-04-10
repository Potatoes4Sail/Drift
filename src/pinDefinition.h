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

// Servo Pins
#define SERVO_CONTROL_PIN_1     5
#define SERVO_CONTROL_PIN_2     6

//#define MOTOR_PWM_REGISTER     OCR2B   // Based on MOTOR_PIN_PWM.

// TODO: Add some method of verifying that the right pins are used (ALL PART OF THE SAME BANK)

// Wheel Encoder Pins
#define LEFT_WHEEL_ENCODER      14
#define RIGHT_WHEEL_ENCODER     15

#define LEFT_WHEEL_INT_MASK     PCINT8
#define RIGHT_WHEEL_INT_MASK    PCINT9

#define WHEEL_ENCODER_INTERRUPT_MASK _BV(LEFT_WHEEL_INT_MASK) | _BV(RIGHT_WHEEL_INT_MASK)

#define WHEEL_ENCODER_PIN_MASK  PCMSK1

#define WHEEL_ENCODER_INT_vect  PCINT1_vect


#endif //DRIFT_PINDEFINITION_H
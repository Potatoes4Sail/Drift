//
// Created by Patrick on 2024-02-21.
//
#pragma once

#ifndef DRIFT_PINDEFINITION_H
#define DRIFT_PINDEFINITION_H

#include "helperFunctions.h"
// Pins are defined with the 'digital' numbering (0-19)

//  ==================================
//
//         MOTOR CONTROL PINS:
//
//  ==================================
#define MOTOR_PIN_PWM          3
#define MOTOR_PIN_FORWARD      4
#define MOTOR_PIN_REVERSE      2

#define MOTOR_PWM_REGISTER     OCR2B   // Based on MOTOR_PIN_PWM.

//  ==================================
//
//         SERVO PINS:
//
//  ==================================

#define SERVO_CONTROL_PIN_1     5
#define SERVO_CONTROL_PIN_2     6
#define SERVO_CONTROL_BANK_1    OCR0A
#define SERVO_CONTROL_BANK_2    OCR0B
#define USE_SERVO_PIN_2         false   // Allows for disabling parts of the code for servo2


//  ==================================
//
//         ULTRASONIC SENSOR PINS:
//
//  ==================================

#define ULTRASONIC_SENSOR_TRIGGER_PIN 7
#define ULTRASONIC_SENSOR0_ECHO_PIN 8
#define ULTRASONIC_SENSOR1_ECHO_PIN 9
#define ULTRASONIC_SENSOR2_ECHO_PIN 10

#define ULTRASONIC_SENSOR0_INT PCINT0
#define ULTRASONIC_SENSOR1_INT PCINT1
#define ULTRASONIC_SENSOR2_INT PCINT2

#define ULTRASONIC_SENSORS_PIN_MASK PCMSK0
#define ULTRASONIC_SENSORS_BANK     PCIE0

#define ULTRASONIC_SENSORS_INT_vect PCINT0_vect

//  ===========================
//
//         ENCODER PINS:
//
//  ===========================

#define MOTOR_ENCODER_A         14
#define MOTOR_ENCODER_B         15
#define LEFT_WHEEL_ENCODER_A    16
#define LEFT_WHEEL_ENCODER_B    17
#define RIGHT_WHEEL_ENCODER_A   18
#define RIGHT_WHEEL_ENCODER_B   19

// TODO: Add some method of verifying that the right pins are used (ALL PART OF THE SAME BANK)
#endif //DRIFT_PINDEFINITION_H
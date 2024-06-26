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
#define MOTOR_PIN_PWM_FORWARD      11
#define MOTOR_PIN_PWM_REVERSE      3

#define MOTOR_PWM_FORWARD_REGISTER     OCR2A   // Based on MOTOR_PIN_PWM.
#define MOTOR_PWM_REVERSE_REGISTER     OCR2B   // Based on MOTOR_PIN_PWM.

#define MOTOR_PWM_REGISTER MOTOR_PWM_REVERSE_REGISTER

//  ==================================
//
//         SERVO PINS:
//
//  ==================================

#define SERVO_CONTROL_PIN     5
#define SERVO_CONTROL_BANK    OCR2A

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

#define BACK_ENCODER_A         14
#define BACK_ENCODER_B         15
#define BACK_ENCODER_PULSES_PER_REV    261

#define LEFT_WHEEL_ENCODER_A    16
#define LEFT_WHEEL_ENCODER_B    17
#define RIGHT_WHEEL_ENCODER_A   18
#define RIGHT_WHEEL_ENCODER_B   19
#define WHEEL_ENCODER_PULSES_PER_REV    600

#define ENCODER_PIN_MASK        0b00111111

// TODO: Add some method of verifying that the right pins are used (ALL PART OF THE SAME BANK)
#endif //DRIFT_PINDEFINITION_H
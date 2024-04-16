//
// Created by Patrick on 2024-04-09.
//

#ifndef DRIFT_SERVODRIVER_H
#define DRIFT_SERVODRIVER_H

#include <avr/io.h>
#include "helperFunctions.h"
#include "pinDefinition.h"

volatile static uint8_t PULSE_SIZE = 12;          // Angle
const uint8_t PULSE_MIN = 8;           // min width pulse (1 ms)
const uint8_t PULSE_MAX = 16;           // max width pulse (2 ms)
const double ANGLE_CONV = (PULSE_MAX - PULSE_MIN) / 90.0; //(Currently reduces to  7/45;

void servoDriverInit();

int setAngle(uint8_t angleAmount);

#endif //DRIFT_SERVODRIVER_H

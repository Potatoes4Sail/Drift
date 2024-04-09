//
// Created by Patrick on 2024-04-09.
//

#ifndef DRIFT_SERVODRIVER_H
#define DRIFT_SERVODRIVER_H

#include <avr/io.h>
#include "helperFunctions.h"
#include "pinDefinition.h"

const uint8_t PULSE_MIN = 15;           // min width pulse (1 ms)
const uint8_t PULSE_MAX = 31;           // max width pulse (2 ms)
const double ANGLE_CONV = (PULSE_MAX - PULSE_MIN) / 90.0; //(Currently reduces to  7/45;

void servoDriverInit();

int setAngleA(uint8_t angleAmount);

int setAngleB(uint8_t angleAmount);

//class servoDriver {
//public:
//    servoDriver;
//    // servoDriver(uint8_t servoPin, uint8_t deadbandVal = 15);
//
//    int setAngle(uint8_t angleAmount);
//
//private:
////    uint8_t servoPin;
//    uint8_t deadBandAmount;
//    uint8_t angle;
//
//
//};


#endif //DRIFT_SERVODRIVER_H

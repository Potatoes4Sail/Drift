//
// Created by Patrick on 2024-04-10.
//

#ifndef DRIFT_ENCODERDRIVER_H
#define DRIFT_ENCODERDRIVER_H

#include "pinDefinition.h"
#include <avr/io.h>

class encoderDriver {

public:
    encoderDriver();

    void initalizeEncoderDriver(uint8_t driverPinIN);

    int8_t getSpeed();

    volatile bool handleInterrupt();

private:
    uint8_t driverPin;
    bool isInitialized = false;
    volatile bool isTiming = false;
    volatile uint16_t startTime, endTime;
};


#endif //DRIFT_ENCODERDRIVER_H

//
// Created by Patrick on 2024-04-10.
//

#include "encoderDriver.h"


encoderDriver::encoderDriver() {
    isInitialized = false;
    startTime = 0;
    endTime = 0;
}

// TODO: Can add error handling if this is called twice in a single objs
void encoderDriver::initalizeEncoderDriver(uint8_t driverPinIN) {
    if (!isInitialized) {
        isInitialized = true;
        startTime = 0;
        endTime = 0;
        this->driverPin = driverPinIN;

        *portModeRegister(digitalPinToPort(driverPin)) &= !digitalPinToBitMask(
                driverPin);      // Sets driver pin to input
    }
}

// TODO: Have proper logic for converting from timing to rotational speed.
int8_t encoderDriver::getSpeed() {
    if (isTiming) {
        isTiming = false;
        return -1; // Is in the process of timing (in theory).
    }
    uint16_t deltaTime = endTime - startTime;
    return deltaTime;
}

volatile bool encoderDriver::handleInterrupt() {
    if (*portInputRegister(digitalPinToPort(driverPin)) &= digitalPinToBitMask(driverPin)) { // Was brought high?
        if (!isTiming) {
            isTiming = true;
            startTime = TCNT1;
            return true;
        } else {
            isTiming = false;
            endTime = TCNT1;
            return true;
        }
    }
    return false;
}
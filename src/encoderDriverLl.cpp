//
// Created by Patrick on 2024-04-17.
//

#include "encoderDriverLl.h"
#include "helperFunctions.h"

encoderDriverLL::encoderDriverLL() {
    isInitialized = false;
}

encoderDriverLL::encoderDriverLL(uint8_t encoderPinA, uint8_t encoderPinB, uint16_t pulsesPerRev) {
    // Storing relevant variables
    isInitialized = true;

    this->encoderPinA = encoderPinA;
    this->encoderPinB = encoderPinB;
    this->pulsesPerRev = pulsesPerRev;

    // Setup pins
    *portModeRegister(digitalPinToPort(this->encoderPinA)) &= ~digitalPinToBitMask(
            this->encoderPinA);      // Sets encoder pin to input
    *portModeRegister(digitalPinToPort(this->encoderPinB)) &= ~digitalPinToBitMask(
            this->encoderPinB);      // Sets encoder pin to input

    // Set all other variables to 0
    startTimeA = 0;
    endTimeA = 0;
    startTimeB = 0;
    endTimeB = 0;
    forwardDirection = false;
    speed = 0;
}

int encoderDriverLL::getSpeed() {
    calculateSpeed();
    return speed;
}

bool encoderDriverLL::isForward() {
    calculateSpeed();
    return forwardDirection;
}

void encoderDriverLL::processInterrupt(encoderPinSide pinSide, uint8_t bitState) {
    if (pinSide == A) {
        if (bitState) { // this means it went high
            timingA = true;
            startTimeA = TCNT1;
        } else {
            timingA = false;
            endTimeA = TCNT1;
        }
    } else {
        if (bitState) { // this means it went high
            timingB = true;
            startTimeB = TCNT1;
        } else {
            timingB = false;
            endTimeB = TCNT1;
        }
    }
}

/// This is the function which processes the data and will actually calculate
/// the speed. It is called by both isForward and getsSpeed
void encoderDriverLL::calculateSpeed() {
    // Validate A has finished.
    if ((startTimeA > endTimeA) && (startTimeB > endTimeB)) {
        // Both finished timing, compare A & B starts.
        if ((startTimeA < startTimeB) && (startTimeB << endTimeA)) {
            forwardDirection = true;
        } else if ((startTimeB < startTimeA) && (startTimeA << endTimeB)) {
            forwardDirection = false;
        } else {
            // Impossible logical state?
            Serial.println("Illegal logical state.\n\n");
        }
    }

    if ((startTimeA > startTimeB) && (endTimeA > endTimeB)) {

    }
}

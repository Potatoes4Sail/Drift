//
// Created by Patrick on 2024-04-17.
//

#ifndef DRIFT_ENCODERDRIVERLL_H
#define DRIFT_ENCODERDRIVERLL_H

#include <avr/io.h>

enum encoderPinSide {
    A,  // 0
    B   // 1
};

class encoderDriverLL {
public:
    encoderDriverLL();

    encoderDriverLL(uint8_t encoderPinA, uint8_t encoderPinB, uint16_t pulsesPerRev);

    int getSpeed();

    bool isForward(); /// Returns true if going forwards, false if going backwards
    void processInterrupt(encoderPinSide pinSide, uint8_t bitState);

private:
    uint8_t encoderPinA, encoderPinB;
    uint16_t pulsesPerRev;

    uint16_t startTimeA, endTimeA;
    uint16_t startTimeB, endTimeB;
    bool timingA, timingB; // Variables to track if A & B are being timed? // idk if this is the best way

    int speed;
    bool forwardDirection;
    bool isInitialized = false;

    void calculateSpeed();
};


#endif //DRIFT_ENCODERDRIVERLL_H

//
// Created by Patrick on 2024-02-20.
//

#ifndef DRIFT_L298DRIVER_H
#define DRIFT_L298DRIVER_H
#include <avr/io.h>
#include "helperFunctions.h"
#include "pinDefinition.h"

class L298Driver {
public:
    L298Driver(uint8_t pwmPinVal, uint8_t forwardPinVal, uint8_t reversePinVal, uint8_t deadbandVal = 10);

    int setSpeed(int8_t speedVar);
    int setBrake(uint8_t breakAmountVar);
private:
    /// Speed of motor, ranging from -128 to 127
    int8_t speed;

    /// Amount 0-255 to apply break
    uint8_t brakeAmount;

    /// Pin used for PWM. Here it will be constrained to two specific pins due to which timer bank is uesed
    uint8_t pwmPin;

    /// Pin assignment for GPIO of forward and reverse pins
    uint8_t forwardPin, reversePin;

    /// Minimum throttle to have an effect.
    uint8_t deadband;
};



#endif //DRIFT_L298DRIVER_H

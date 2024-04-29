//
// Created by Patrick on 2024-04-29.
//

#ifndef DRIFT_BST7960DRIVER_H
#define DRIFT_BST7960DRIVER_H

#include <avr/io.h>
#include "helperFunctions.h"
#include "pinDefinition.h"

enum motorDirection {
    NONE,
    FORWARD,
    REVERSE
};

class BST7960Driver {
public:
    BST7960Driver(uint8_t forwardPWMPin, uint8_t reversePWMPin, uint8_t deadbandVal = 15);

    int setSpeed(int8_t speedVar);
//    int setBrake(uint8_t breakAmountVar);
private:
    /// Speed of motor, ranging from -128 to 127
    int8_t speed;
    uint8_t direction;

    /// Amount 0-255 to apply break
    uint8_t brakeAmount;

    /// Pin used for PWM. Here it will be constrained to two specific pins due to which timer bank is uesed
    uint8_t forwardPWMPin, reversePWMPin;

    /// Minimum throttle to have an effect.
    uint8_t deadband;

    void setRegisterSpeed(uint8_t motor);
};


#endif //DRIFT_BST7960DRIVER_H

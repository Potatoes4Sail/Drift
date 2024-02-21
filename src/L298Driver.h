//
// Created by Patrick on 2024-02-20.
//

#ifndef DRIFT_L298DRIVER_H
#define DRIFT_L298DRIVER_H
#include <avr/io.h>
#include "helperFunctions.h"


class L298Driver {
public:
    L298Driver(uint8_t pwmPin,uint8_t forwardPin,uint8_t reversePin, uint8_t PWMRegister, uint8_t deadband = 10);

    int setSpeed(int8_t speedVar);
    int setBrake(uint8_t breakAmountVar);
private:
    int8_t speed; /// Speed of motor, ranging from -128 to 127
    uint8_t brakeAmount; /// Amount 0-255 to apply break

    uint8_t pwmPin; /// Pin used for PWM. Here it will be constrained to two specific pins due to which timer bank is uesed
    uint8_t forwardPin, reversePin; /// Pin assignment for GPIO of forward and reverse pins

    uint8_t deadband; /// Minimum throttle to have an effect.
    uint8_t PWMRegister; /// Stores which register the object should use
};



#endif //DRIFT_L298DRIVER_H

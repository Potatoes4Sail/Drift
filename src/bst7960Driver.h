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
    /// Constructor for class which drives the L298 motor drivers.
    /// \n LIMITATIONS: Both pwm signals must come from the same timer, and TIMER0 is currently chosen.
    /// Does not validate if the two motors are on the two outputs of the timers
    /// \param pwmPin - Pin used for PWMing the enable (MUST BE ONE OF TWO)
    /// \param forwardPWMPin - Any GPIO for controlling forward pin
    /// \param reversePWMPin - Any GPIO for controlling reverse pin
    /// \param deadband - Amount #/255 below which no response will occur from motor. Used as there is likely minimum pwm for anything to occur
    BST7960Driver(uint8_t forwardPWMPin, uint8_t reversePWMPin, uint8_t deadbandVal = 15);

    /// Function with sets the speed of the motor
    /// \param speedVar - int8_t value, must be between -127 and 127
    /// \return
    int setSpeed(int8_t speedVar);
//    int setBrake(uint8_t breakAmountVar);
private:
    /// Speed of motor, ranging from -128 to 127
    uint8_t speed;
    uint8_t direction;

    /// Amount 0-255 to apply break
    uint8_t brakeAmount;

    /// Pin used for PWM. Here it will be constrained to two specific pins due to which timer bank is uesed
    uint8_t forwardPWMPin, reversePWMPin;

    /// Minimum throttle to have an effect.
    uint8_t deadband;

    // PID Controller
    float integral = 0;
    float lastError = 0;
    unsigned long lastTime = 0;

    void setRegisterSpeed(uint8_t motor);
};


#endif //DRIFT_BST7960DRIVER_H

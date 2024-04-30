//
// Created by Patrick on 2024-04-29.
//

#include <HardwareSerial.h>
#include "bst7960Driver.h"

/// Constructor for class which drives the L298 motor drivers.
/// \n LIMITATIONS: Both pwm signals must come from the same timer, and TIMER0 is currently chosen.
/// Does not validate if the two motors are on the two outputs of the timers
/// \param pwmPin - Pin used for PWMing the enable (MUST BE ONE OF TWO)
/// \param forwardPWMPin - Any GPIO for controlling forward pin
/// \param reversePWMPin - Any GPIO for controlling reverse pin
/// \param deadband - Amount #/255 below which no response will occur from motor. Used as there is likely minimum pwm for anything to occur
BST7960Driver::BST7960Driver(uint8_t forwardPWMPin, uint8_t reversePWMPin, uint8_t deadbandVal) {

    // Sets variables of the class.
    this->forwardPWMPin = forwardPWMPin;
    this->reversePWMPin = reversePWMPin;
    this->direction = NONE;
    this->speed = 0;
    this->deadband = deadbandVal;

    // ========================================
    //
    //              Configuring PWM
    //
    // ========================================

    *portModeRegister(digitalPinToPort(forwardPWMPin)) |= digitalPinToBitMask(
            forwardPWMPin);      // Sets PWM pin to output
    *portModeRegister(digitalPinToPort(reversePWMPin)) |= digitalPinToBitMask(
            reversePWMPin);      // Sets PWM pin to output
}

/// Function with sets the speed of the motor
/// \param speedVar - int8_t value, must be between -127 and 127
/// \return
int BST7960Driver::setSpeed(int8_t speedVar) {
    this->direction = ((0 < speedVar) ? FORWARD : REVERSE);
    this->speed = 2 * abs(speedVar);

    if (speed < deadband) {
        this->direction = NONE;
        this->speed = 0;
    }
    setRegisterSpeed(direction);
    return 0;
}


// TODO: Allow for setting the alternative register to interrupt, and imporve resolution of servo.
void BST7960Driver::setRegisterSpeed(uint8_t motor) {
    TCCR2A &= ~0b11110000; // Turns off both of the outputs.
    // Set both comparator timers to 0. 
    MOTOR_PWM_FORWARD_REGISTER = 0;
    MOTOR_PWM_REVERSE_REGISTER = 0;

    *portOutputRegister(digitalPinToPort(forwardPWMPin)) &= ~digitalPinToBitMask(
            forwardPWMPin); // Set forward pin to low
    *portOutputRegister(digitalPinToPort(reversePWMPin)) &= ~digitalPinToBitMask(
            reversePWMPin); // Set forward pin to low

    switch (motor) {
        case 0:
            break;
        case FORWARD:
//            Serial.println("FORWARD!");
            OCR2A = this->speed;
            TCCR2A |= _BV(COM2A1);
            break;
        case REVERSE:
            OCR2B = this->speed;
            TCCR2A |= _BV(COM2B1);
            break;
    }
}
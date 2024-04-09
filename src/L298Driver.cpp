//
// Created by Patrick on 2024-02-20.
//

#include "L298Driver.h"

#define MOTOR_PWM_REGISTER     OCR2B

/// Constructor for class which drives the L298 motor drivers.
/// \n LIMITATIONS: Both pwm signals must come from the same timer, and TIMER0 is currently chosen.
/// Does not validate if the two motors are on the two outputs of the timers
/// \param pwmPin - Pin used for PWMing the enable (MUST BE ONE OF TWO)
/// \param forwardPin - Any GPIO for controlling forward pin
/// \param reversePin - Any GPIO for controlling reverse pin
/// \param deadband - Amount #/255 below which no response will occur from motor. Used as there is likely minimum pwm for anything to occur
L298Driver::L298Driver(uint8_t pwmPinVal, uint8_t forwardPinVal, uint8_t reversePinVal, uint8_t deadbandVal) {

    // Sets variables of the class.

    this->pwmPin = pwmPinVal;
    this->forwardPin = forwardPinVal;
    this->reversePin = reversePinVal;
    this->deadband = deadbandVal;

    // ========================================
    //
    //              Configuring PWM
    //
    // ========================================

    *portModeRegister(digitalPinToPort(pwmPin)) |= digitalPinToBitMask(pwmPin);      // Sets PWM pin to output

    OCR2B = (uint8_t) 0; // Set PWM to 0

    TCCR2A = 0;

    // TCCR2A |= (1 << COM2A1); // Enables PWM for timer side A
    TCCR2A |= (1 << COM2B1); // Enables PWM for timer side B

//    TCCR2A |= (1 << WGM21) | (1 << WGM20); // Enables fast PWM Mode
    TCCR2A |= (1 << WGM20); // Enables fast PWM Mode

    // TCCR2A |= (1 << WGM20); // Enables phase corrected PWM Mode.

    TCCR2B = 0;
    TCCR2B |= 0b101; // Sets Prescaler to 8 (~8 kHz), will start PWM at 0 duty cycle.

    // ========================================
    //
    //          Configuring Other IOs
    //
    // ========================================

    // Set mode of both forward & reverse pins to output
    *portModeRegister(digitalPinToPort(forwardPin)) &= ~digitalPinToBitMask(forwardPin);
    *portModeRegister(digitalPinToPort(reversePin)) &= ~digitalPinToBitMask(reversePin);

    // Set both pins to low
    *portOutputRegister(digitalPinToPort(forwardPin)) &= ~digitalPinToBitMask(forwardPin);
    *portOutputRegister(digitalPinToPort(reversePin)) &= ~digitalPinToBitMask(reversePin);
}

/// Function with sets the speed of the motor
/// \param speedVar - int8_t value, must be between -
/// \return
int L298Driver::setSpeed(int8_t speedVar) {
    this->speed = speedVar;

    if (speed > deadband) { // Set throttle to positive
        // TODO: Would ideally set both pins at the same time to avoid different states,
        //  but this will impose significant restrictions on which pins can be used, and has a minor improvement
        *portOutputRegister(digitalPinToPort(reversePin)) &= ~digitalPinToBitMask(reversePin); // Set reverse pin to low
        *portOutputRegister(digitalPinToPort(forwardPin)) |= digitalPinToBitMask(forwardPin); // Set forward pin to high

        if (MOTOR_PWM_REGISTER == 0) {
            // Motor was previously off, set to full speed to start
            MOTOR_PWM_REGISTER = (uint8_t) 255;
        } else {
            MOTOR_PWM_REGISTER = (uint8_t) 2 * speed; // Set PWM duty cycle to speed variable * 2
        }
    } else if (speed < -deadband) { // Set throttle to negative
        *portOutputRegister(digitalPinToPort(reversePin)) |= digitalPinToBitMask(reversePin); // Set reverse pin to high
        *portOutputRegister(digitalPinToPort(forwardPin)) &= ~digitalPinToBitMask(forwardPin); // Set forward pin to low

        if (MOTOR_PWM_REGISTER == 0) {
            // Motor was previously off, set to full speed to start
            MOTOR_PWM_REGISTER = (uint8_t) 255;
        } else {
            MOTOR_PWM_REGISTER = (uint8_t) 2 * -speed; // Set PWM duty cycle to speed variable * 2
        }
    } else { // Turn off throttle
        *portOutputRegister(digitalPinToPort(reversePin)) &= ~digitalPinToBitMask(reversePin); // Set reverse pin to low
        *portOutputRegister(digitalPinToPort(forwardPin)) &= ~digitalPinToBitMask(forwardPin); // Set forward pin to low

        MOTOR_PWM_REGISTER = (uint8_t) 0; // Set PWM duty cycle to speed variable * 2
    }
    return 0;
}

int L298Driver::setBrake(uint8_t brakeAmountVar) {
    this->brakeAmount = brakeAmountVar;
    if (brakeAmount > deadband) {
        // Braking is accomplished by setting forward and reverse to same state, and having pwm high
        *portOutputRegister(digitalPinToPort(reversePin)) |= digitalPinToBitMask(reversePin); // Set reverse pin to high
        *portOutputRegister(digitalPinToPort(forwardPin)) |= digitalPinToBitMask(forwardPin); // Set forward pin to high

        MOTOR_PWM_REGISTER = (uint8_t) brakeAmount;
    } else {
        // Called brake function but didn't break enough to actually do anything
        *portOutputRegister(digitalPinToPort(reversePin)) |= digitalPinToBitMask(reversePin); // Set reverse pin to high
        *portOutputRegister(digitalPinToPort(forwardPin)) |= digitalPinToBitMask(forwardPin); // Set forward pin to high

        MOTOR_PWM_REGISTER = (uint8_t) 0;
    }
    return 0;
}

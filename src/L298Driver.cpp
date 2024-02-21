//
// Created by Patrick on 2024-02-20.
//

#include "L298Driver.h"

/// Constructor for class which drives the L298 motor drivers.
/// \n LIMITATIONS: Both pwm signals must come from the same timer, and TIMER0 is currently chosen.
/// Does not validate if the two motors are on the two outputs of the timers
/// \param pwmPin - Pin used for PWMing the enable (MUST BE ONE OF TWO)
/// \param forwardPin - Any GPIO for controlling forward pin
/// \param reversePin - Any GPIO for controlling reverse pin
/// \param PWMRegister - The PWM Register used by the PWM pin. Both must be on same timer bank
/// \param deadband - Amount #/255 below which no response will occur from motor. Used as there is likely minimum pwm for anything to occur
L298Driver::L298Driver(uint8_t pwmPin, uint8_t forwardPin, uint8_t reversePin, uint8_t PWMRegister, uint8_t deadband) {

    // Sets variables of the class.

    this->pwmPin = pwmPin;
    this->forwardPin = forwardPin;
    this->reversePin = reversePin;
    this->deadband = deadband;

    if ((PWMRegister == OCR0A) || (PWMRegister == OCR0B)) {
        this->PWMRegister = PWMRegister;
    } else {
        return; // ERROR, FUNCTION WILL NOT WORK
        // TODO: Write a method of throwing that error
    }

    // ========================================
    //
    //              Configuring PWM
    //
    // ========================================

    *portModeRegister(digitalPinToPort(pwmPin)) &= ~digitalPinToBitMask(pwmPin);      // Sets PWM pin to output

    DDRD |= _BV(DDD6); // Sets PD6 to output

    PWMRegister = (uint8_t) 0; // Set PWM to 0

    TCCR0A |= (1 << COM0A1); // Enables non-inverting mode
    TCCR0A |= (1 << WGM01) | (1 << WGM00); // Enables fast PWM Mode

    TCCR0B |= 0b010; // Sets Prescaler to 8 (~8 kHz), will start PWM at 0 duty cycle.
    // set prescaler to 8 and starts PWM

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

int L298Driver::setSpeed(int8_t speedVar) {
    this->speed = speedVar;

    if (speed > deadband) { // Set throttle to positive
        // TODO: Would ideally set both pins at the same time to avoid different states,
        //  but this will impose significant restrictions on which pins can be used
        *portOutputRegister(digitalPinToPort(reversePin)) &= ~digitalPinToBitMask(reversePin); // Set reverse pin to low
        *portOutputRegister(digitalPinToPort(forwardPin)) |= digitalPinToBitMask(forwardPin); // Set forward pin to high

        PWMRegister = (uint8_t) 2 * speed; // Set PWM duty cycle to speed variable * 2
    } else if (speed < -deadband) { // Set throttle to negative
        *portOutputRegister(digitalPinToPort(reversePin)) |= digitalPinToBitMask(reversePin); // Set reverse pin to high
        *portOutputRegister(digitalPinToPort(forwardPin)) &= ~digitalPinToBitMask(forwardPin); // Set forward pin to low

        PWMRegister = (uint8_t) 2 * -speed; // Set PWM duty cycle to speed variable * 2
    } else { // Turn off throttle
        *portOutputRegister(digitalPinToPort(reversePin)) &= ~digitalPinToBitMask(reversePin); // Set reverse pin to low
        *portOutputRegister(digitalPinToPort(forwardPin)) &= ~digitalPinToBitMask(forwardPin); // Set forward pin to low

        PWMRegister = (uint8_t) 0; // Set PWM duty cycle to speed variable * 2
    }
    return 0;
}

int L298Driver::setBrake(uint8_t brakeAmountVar) {
    this->brakeAmount = brakeAmountVar;
    if (brakeAmount > deadband) {
        // Braking is accomplished by setting forward and reverse to same state, and having pwm high
        *portOutputRegister(digitalPinToPort(reversePin)) |= digitalPinToBitMask(reversePin); // Set reverse pin to high
        *portOutputRegister(digitalPinToPort(forwardPin)) |= digitalPinToBitMask(forwardPin); // Set forward pin to high

        PWMRegister = (uint8_t) brakeAmount;
    } else {
        // Called brake function but didn't break enough to actually do anything
        *portOutputRegister(digitalPinToPort(reversePin)) |= digitalPinToBitMask(reversePin); // Set reverse pin to high
        *portOutputRegister(digitalPinToPort(forwardPin)) |= digitalPinToBitMask(forwardPin); // Set forward pin to high

        PWMRegister = (uint8_t) 0;
    }
    return 0;
}

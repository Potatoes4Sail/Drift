//
// Created by Patrick on 2024-02-16.
//

#include "Ultrasonic.h"
#include "helperFunctions.h"
#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

Ultrasonic::Ultrasonic(uint8_t triggerPinIN, uint8_t echoPinIN) {
    this->triggerPin = triggerPinIN;
    this->echoPin = echoPinIN;

    *portModeRegister(digitalPinToPort(triggerPin)) |= digitalPinToBitMask(triggerPin); // Sets trigger pin to input
    *portModeRegister(digitalPinToPort(echoPin)) &= ~digitalPinToBitMask(echoPin);      // Sets echo pin to output
}


float Ultrasonic::pollSensor() {
    //TODO: Write code that will function to time how long it takes to get response from sensor
    triggerUltrasound();

    stopInterrupts();
    float duration = measurePulse();
    startInterrupts();

    return (duration/ 5.88235f);
}

void Ultrasonic::triggerUltrasound() {
    *portOutputRegister(digitalPinToPort(triggerPin)) &= ~digitalPinToBitMask(
            triggerPin);  // Turns off pin if it was on before
    _delay_us(10);
    *portOutputRegister(digitalPinToPort(triggerPin)) |= digitalPinToBitMask(triggerPin);  // Turns on pin for 10 us
    _delay_us(10);
    *portOutputRegister(digitalPinToPort(triggerPin)) &= ~digitalPinToBitMask(triggerPin);  // Turns off pin again
}

int Ultrasonic::measurePulse() {
    // cache the port and bit of the pin in order to speed up the
    // pulse width measuring loop and achieve finer resolution.  calling
    // digitalRead() instead yields much coarser resolution.
    uint8_t bit = digitalPinToBitMask(echoPin);
    uint8_t port = digitalPinToPort(echoPin);

    uint8_t stateMask = (true ? bit : 0);

    // convert the timeout from microseconds to a number of times through
    // the initial loop; it takes approximately 16 clock cycles per iteration
    unsigned long maxloops = microsecondsToClockCycles(23529.4) / 16.0;
    unsigned long width = countPulse(portInputRegister(port), bit, stateMask, maxloops);

    // prevent clockCyclesToMicroseconds to return bogus values if countPulseASM timed out
    if (width)
        return clockCyclesToMicroseconds(width * 16 + 16);
    else
        return 0;
}
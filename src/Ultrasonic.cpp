//
// Created by Patrick on 2024-02-16.
//

#include "Ultrasonic.h"

Ultrasonic::Ultrasonic(int triggerPin, int triggerPinBank, int echoPin, int echoPinBank) {
    triggerPinBank |= _BV(triggerPin); // Set trigger to output
    echoPinBank &= ~_BV(echoPin); // Set echo to input
}

int Ultrasonic::pollSensor() {
    // Write code to actuall work.
    return -1;
}

void triggerUltrasound() {
    //TODO: how do i do this right?
    PORTB &= ~_BV(Ultrasonic::triggerPin); // Turns it off

    _delay_us(10);
    PORTB |= _BV(triggerPin);
    _delay_us(10);
    PORTB &= ~_BV(triggerPin);
}
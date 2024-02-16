//
// Created by Patrick on 2024-02-16.
//

#include "Ultrasonic.h"
#include <avr/io.h>
#include <util/delay.h>

Ultrasonic::Ultrasonic(int triggerPinBankVar, int triggerPinVar, int echoPinBankVar, int echoPinVar) {
    this->triggerPin = triggerPinVar;
    this->triggerPinBank = triggerPinBankVar;
    this->echoPin = echoPinVar;
    this->echoPinBank = echoPinBankVar;

    triggerPinBank |= _BV(triggerPin); // Set trigger to output
    echoPinBank &= ~_BV(echoPin); // Set echo to input
}

int Ultrasonic::pollSensor() {
    //TODO: Write code that will function to time how long it takes to get response from sensor
    this->triggerUltrasound();

    return -1;
}

void Ultrasonic::triggerUltrasound() {
    this->triggerPinBank &= ~_BV(this->triggerPin); // Turns it off
    _delay_us(10);
    this->triggerPinBank |= _BV(this->triggerPin);
    _delay_us(10);
    this->triggerPinBank &= ~_BV(this->triggerPin);
}

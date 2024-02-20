//
// Created by Patrick on 2024-02-16.
//

#include "Ultrasonic.h"
#include "helperFunctions.h"
#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>
#include <wiring_private.h>

Ultrasonic::Ultrasonic(int triggerPinBankVar, int triggerPinVar, int echoPinBankVar, int echoPinVar) {
    this->triggerPin = triggerPinVar;
    this->triggerPinBank = triggerPinBankVar;
    this->echoPin = echoPinVar;
    this->echoPinBank = echoPinBankVar;

    triggerPinBank |= _BV(triggerPin); // Set trigger to output
    echoPinBank &= ~_BV(echoPin); // Set echo to input
}


Ultrasonic::Ultrasonic(int triggerPinIN, int echoPinIN) {
//TODO: Verify that this method works for setting variables. If so, it will be more maintainable:
//  It does use pgmspace.h to work, but it likely a worthwhile tradeoff for readability and ease of use.

//    uint8_t timer = digitalPinToTimer(pin);
//    uint8_t bit = digitalPinToBitMask(pin);
//    uint8_t port = digitalPinToPort(pin);

    this->triggerPin = digitalPinToBitMask(triggerPinIN);
    this->triggerPinBank = digitalPinToPort(triggerPinIN);

    this->echoPin = digitalPinToBitMask(echoPinIN);
    this->echoPinBank = digitalPinToPort(echoPinIN);

    triggerPinBank |= _BV(triggerPin); // Set trigger to output
    echoPinBank &= ~_BV(echoPin); // Set echo to input
}


int Ultrasonic::pollSensor() {
    //TODO: Write code that will function to time how long it takes to get response from sensor
    this->triggerUltrasound();

    stopInterrupts();
    measurePulse();
    startInterrupts();
    return -1;
}

void Ultrasonic::triggerUltrasound() {
    this->triggerPinBank &= ~_BV(this->triggerPin); // Turns it off
    _delay_us(10);
    this->triggerPinBank |= _BV(this->triggerPin);
    _delay_us(10);
    this->triggerPinBank &= ~_BV(this->triggerPin);
}

int Ultrasonic::measurePulse() {
    // pulseIn();
    // Uses countPulseASM at low level;
    this->echoPin;
    return -1;
}

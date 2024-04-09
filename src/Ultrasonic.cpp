//
// Created by Patrick on 2024-02-16.
//

#include "Ultrasonic.h"
#include "helperFunctions.h"
#include <avr/io.h>
#include <util/delay.h>

/// Ultrasonic Object - Object which is used with the HC-SR04 sensor
/// \param triggerPinIN - digital pin number of trigger pin
/// \param echoPinIN - digital pin number of echo pin
Ultrasonic::Ultrasonic(uint8_t triggerPinIN, uint8_t echoPinIN) {
    this->triggerPin = triggerPinIN;
    this->echoPin = echoPinIN;

    this->sensorDistance = -1;

    *portModeRegister(digitalPinToPort(triggerPin)) |= digitalPinToBitMask(triggerPin); // Sets trigger pin to output
    *portModeRegister(digitalPinToPort(echoPin)) &= ~digitalPinToBitMask(echoPin);      // Sets echo pin to input
}

/// pollSensor - Sends a pulse and times how long it takes for a response
///
/// \return [float] Distance in mm
float Ultrasonic::pollSensor() {
    triggerUltrasound();

    stopInterrupts();
    float duration = measurePulse(echoPin, HIGH, 23529);
    startInterrupts();
    sensorDistance = (duration/ 5.88235f);
    return sensorDistance;
}

void Ultrasonic::triggerUltrasound() {
    // Turns off pin if it was on before
    *portOutputRegister(digitalPinToPort(triggerPin)) &= ~digitalPinToBitMask(triggerPin);
    _delay_us(10);
    *portOutputRegister(digitalPinToPort(triggerPin)) |= digitalPinToBitMask(triggerPin);  // Turns on pin for 10 us
    _delay_us(10);
    *portOutputRegister(digitalPinToPort(triggerPin)) &= ~digitalPinToBitMask(triggerPin);  // Turns off pin again
}

Ultrasonic::~Ultrasonic() = default;
//
// Created by Patrick on 2024-02-16.
//

#include <avr/io.h>
#include <util/delay.h>
#include "ultrasonicDriver.h"
#include "helperFunctions.h"

ultrasonicDriver::ultrasonicDriver() {
    triggerPin = -1;
    echoPin = -1;
    sensorDistance = -1;
}


/// ultrasonicDriver Object - Object which is used with the HC-SR04 sensor
/// \param triggerPinIN - digital pin number of trigger pin
/// \param echoPinIN - digital pin number of echo pin
//void ultrasonicDriver::init(uint8_t triggerPinIN, uint8_t echoPinIN) {
ultrasonicDriver::ultrasonicDriver(uint8_t triggerPinIN, uint8_t echoPinIN) {
    initialized = true;
    this->triggerPin = triggerPinIN;
    this->echoPin = echoPinIN;

    *portModeRegister(digitalPinToPort(triggerPin)) |= digitalPinToBitMask(triggerPin); // Sets trigger pin to input
    *portModeRegister(digitalPinToPort(echoPin)) &= ~digitalPinToBitMask(echoPin);      // Sets echo pin to output
}

/// pollSensor - Sends a pulse and times how long it takes for a response
///
/// \return [float] Distance in mm
void ultrasonicDriver::pollSensor() {
    if (!initialized) return;
    triggerUltrasound();
}

void ultrasonicDriver::triggerUltrasound() {
    // Turns off pin if it was on before
    *portOutputRegister(digitalPinToPort(triggerPin)) &= ~digitalPinToBitMask(triggerPin);
    _delay_us(10);
    *portOutputRegister(digitalPinToPort(triggerPin)) |= digitalPinToBitMask(triggerPin);  // Turns on pin for 10 us
    _delay_us(10);
    *portOutputRegister(digitalPinToPort(triggerPin)) &= ~digitalPinToBitMask(triggerPin);  // Turns off pin again
}

ultrasonicDriver::~ultrasonicDriver() = default;

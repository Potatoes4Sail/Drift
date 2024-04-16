//
// Created by Patrick on 2024-02-16.
//

#include <util/delay.h>
#include "UltrasonicDriver.h"
#include "helperFunctions.h"

UltrasonicDriver::UltrasonicDriver() {
    this->initialized = false;
    this->triggerPin = -1;
    this->echoPin = -1;
}

UltrasonicDriver::UltrasonicDriver(uint8_t triggerPinIN, uint8_t echoPinIN) {
    initialized = true;
    this->triggerPin = triggerPinIN;
    this->echoPin = echoPinIN;

    *portModeRegister(digitalPinToPort(triggerPin)) |= digitalPinToBitMask(triggerPin); // Sets trigger pin to output
    *portModeRegister(digitalPinToPort(echoPin)) &= ~digitalPinToBitMask(echoPin);      // Sets echo pin to input
}

int8_t UltrasonicDriver::pollSensor() {
    if (!initialized) return -1;
    triggerUltrasound();
    return 0;
}

void UltrasonicDriver::triggerUltrasound() const {
    // Turns off pin if it was on before
    *portOutputRegister(digitalPinToPort(triggerPin)) &= ~digitalPinToBitMask(triggerPin);
    _delay_us(10);
    *portOutputRegister(digitalPinToPort(triggerPin)) |= digitalPinToBitMask(triggerPin);  // Turns on pin for 10 us
    _delay_us(10);
    *portOutputRegister(digitalPinToPort(triggerPin)) &= ~digitalPinToBitMask(triggerPin);  // Turns off pin again
}

int32_t UltrasonicDriver::readDistance() {
    Serial.print(echoDetected);
    if (echoDetected) {     // TODO: Investigate this, there seems to be a a bug if only pin8 is connected with pin10
        Serial.println("ERROR, echo not detected ;c");
        return -1;
    }
    Serial.print("distance to be read on pin");
    Serial.print(echoPin);
    Serial.print("\tCalculating distance; startTime is ");
    Serial.print(startTime);
    Serial.print("\t end time is ");
    Serial.print(endTime);
    Serial.print("\t :)\n");

    uint16_t deltaTime = endTime - startTime;
    return deltaTime;
    //    return (uint16_t)(deltaTime / (4 * 5.88235f));
}

volatile bool UltrasonicDriver::handleInterrupt() {
    // Rising Edge
    if(!echoDetected && (*portInputRegister(digitalPinToPort(echoPin)) &= digitalPinToBitMask(echoPin))) {
        TCNT1 = 0;
        startTime = TCNT1;
        echoDetected = true;
    } else if (echoDetected && !(*portInputRegister(digitalPinToPort(echoPin)) &= digitalPinToBitMask(echoPin))) {
        endTime = TCNT1;
        echoDetected = false;
        return true;
    }

    return false;
}

bool UltrasonicDriver::isInitialized() const {
    return initialized;
}

uint8_t UltrasonicDriver::getTriggerPin() const {
    return triggerPin;
}

uint8_t UltrasonicDriver::getEchoPin() const {
    return echoPin;
}

//
// Created by Patrick on 2024-02-22.
//
#include "ultrasonic.h"

Ultrasonic::Ultrasonic(uint8_t trig, uint8_t leftEcho, uint8_t frontEcho, uint8_t rightEcho) {
    // Initialize variables for object
    triggerPin = trig;
    currentPolledSensor = UltrasonicDirection::LEFT;

    leftDriver = UltrasonicDriver(triggerPin, leftEcho);
    frontDriver = UltrasonicDriver(triggerPin, frontEcho);
    rightDriver = UltrasonicDriver(triggerPin, rightEcho);

    // Enable pin mask for 3 specific pins
    ULTRASONIC_SENSORS_PIN_MASK |=
            _BV(ULTRASONIC_SENSOR0_INT) | _BV(ULTRASONIC_SENSOR1_INT) | _BV(ULTRASONIC_SENSOR2_INT);
    PCICR |= _BV(ULTRASONIC_SENSORS_BANK);

    startInterrupts();
}

void Ultrasonic::sendEcho() {
    this->leftDriver.pollSensor();
}

int32_t Ultrasonic::readLeftDistance() {
    return this->leftDriver.readDistance();
}

int32_t Ultrasonic::readFrontDistance() {
    return this->frontDriver.readDistance();
}

int32_t Ultrasonic::readRightDistance() {
    return this->rightDriver.readDistance();
}

void volatile Ultrasonic::handleInterrupt() {
    switch (this->currentPolledSensor) {
        case LEFT:
            if (leftDriver.handleInterrupt()) {
                this->currentPolledSensor = FRONT;
            }
            break;
        case FRONT:
            if (frontDriver.handleInterrupt()) {
                this->currentPolledSensor = RIGHT;
            }
            break;
        case RIGHT:
            if (rightDriver.handleInterrupt()) {
                this->currentPolledSensor = LEFT;
            }
            break;
    }
}

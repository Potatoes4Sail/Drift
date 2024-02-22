//
// Created by Patrick on 2024-02-22.
//

#include <Arduino.h>
#include "ultrasonic.h"

Ultrasonic::Ultrasonic(uint8_t trig, uint8_t echoPins[], uint8_t n) {
    triggerPin = trig;
    this->n = n;
    currentPolledSensor = 0;

    this->sensorArray = new ultrasonicDriver[n];
    this->echoPins = new uint8_t[n];
    this->distances = new uint16_t[n];

    for (uint8_t i = 0; i < n; ++i) {
        this->echoPins[i] = echoPins[i];
        this->sensorArray[i] = ultrasonicDriver(this->triggerPin, this->echoPins[i]);
        distances[i] = 0;
    }

    // Turns on pin change interrupt control register
    PCICR |= _BV(PCIE0);

    // Enable interrupts:
    // Sets pin mask for all ultrasonic sensor echo pins
    ULTRASONIC_SENSORS_PIN_MASK |= (_BV(ULTRASONIC_SENSOR0_INT) | _BV(ULTRASONIC_SENSOR1_INT) |
                                    _BV(ULTRASONIC_SENSOR2_INT));

    PCMSK0 |= 0b0111;
    // Enable global interrupts:
    sei();

    timing = false;

}

Ultrasonic::~Ultrasonic() {
    delete[] echoPins;
    delete[] sensorArray;
} // Destructor

void Ultrasonic::pollNextSensor() {
    if (timing) {
        uint16_t deltaTime = endTime - startTime; // Calculate the difference in time
        distances[currentPolledSensor] = deltaTime / 5.88235f;
    } else { // Failed to return a proper value
        Serial.println((String) "Function was still timing when this happened;\t startTime = " + startTime + "\t endTime = " + endTime + "\n");
        startTime = 0;
        endTime = 0;
        timing = false;
    }

    if (++currentPolledSensor >= n) currentPolledSensor = 0;
    sensorArray[currentPolledSensor].pollSensor();
}

uint16_t Ultrasonic::readSensor(uint8_t sensorNumber) {
    //return sensorArray;
    if (sensorNumber < n) {
        return distances[sensorNumber];
    }
    return -1;
}

//#define checkPinMask(pin) (pinMask == _BV(ULTRASONIC_SENSOR##pin_INT) && currentPolledSensor == pin)
// Unused, but can be used here to simplify adding more functions.
// checkPinMask(0) || checkPinMask(1) || checkPinMask(2) ;

void volatile Ultrasonic::interruptTrigger(uint8_t pinMask, uint32_t time)  {
    if ((pinMask == _BV(ULTRASONIC_SENSOR0_INT) && currentPolledSensor == 0) ||
        (pinMask == _BV(ULTRASONIC_SENSOR1_INT) && currentPolledSensor == 1) ||
        (pinMask == _BV(ULTRASONIC_SENSOR2_INT) && currentPolledSensor == 2)) {
        if (!timing) {
            startTime = time;
            timing = true;
        } else {
            endTime = time;
            timing = false;
        }
    }
}
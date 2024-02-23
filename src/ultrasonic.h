//
// Created by Patrick on 2024-02-22.
//

#ifndef DRIFT_ULTRASONIC_H
#define DRIFT_ULTRASONIC_H

#include <avr/io.h>
#include "pinDefinition.h"
#include "ultrasonicDriver.h"

class Ultrasonic {
public:
    Ultrasonic(uint8_t trig, uint8_t echoPins[], uint8_t n); // Constructor
    ~Ultrasonic(); // Destructor


    void pollNextSensor();
    uint16_t readSensorData(uint8_t sensorNumber) const;
    volatile void interruptTrigger(uint8_t pinMask, uint32_t time);
public: // TODO: Remake all of these private after fixing bugs
    ultrasonicDriver *sensorArray;
    uint8_t triggerPin;
    uint8_t *echoPins;
    uint16_t *distances;
    uint8_t n;
    uint8_t currentPolledSensor;
public:
    /// Status of timing, false when not timing, true when timing
    volatile bool timing;
    volatile uint32_t startTime, endTime;

};

#endif //DRIFT_ULTRASONIC_H

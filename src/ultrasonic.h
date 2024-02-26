//
// Created by Patrick on 2024-02-22.
//

#ifndef DRIFT_ULTRASONIC_H
#define DRIFT_ULTRASONIC_H
#include <avr/io.h>
#include "pinDefinition.h"
#include "UltrasonicDriver.h"

enum UltrasonicDirection {
    LEFT,   // 0
    FRONT,  // 1
    RIGHT   // 2
};

class Ultrasonic {
public:
    /// Constructor of Ultrasonic sensor.
    /// \param trig - Pin shared between all triggers of the sensors
    /// \param echoPins - Array of pins used for echo responses
    /// \param n - number of sensors used
    Ultrasonic(uint8_t trig, uint8_t leftEcho, uint8_t frontEcho, uint8_t rightEcho); // Constructor
    ~Ultrasonic() = default; // Destructor

    void sendEcho();
    int32_t readLeftDistance();
    int32_t readFrontDistance();
    int32_t readRightDistance();

    volatile void handleInterrupt();

private:
    uint8_t triggerPin;
    UltrasonicDirection currentPolledSensor;

    UltrasonicDriver leftDriver;
    UltrasonicDriver frontDriver;
    UltrasonicDriver rightDriver;
};

#endif //DRIFT_ULTRASONIC_H

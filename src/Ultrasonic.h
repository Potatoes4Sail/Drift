//
// Created by Patrick on 2024-02-16.
//


#ifndef DRIFT_ULTRASONIC_H
#define DRIFT_ULTRASONIC_H

#include <stdint-gcc.h>

class Ultrasonic {
public:
    uint8_t triggerPin, echoPin;

    Ultrasonic(uint8_t triggerPinIN, uint8_t echoPinIN);
    virtual ~Ultrasonic();
    float pollSensor();

private:
    uint16_t sensorDistance;
    void triggerUltrasound(); // Actually sends pulse to Ultrasonic sensor,
};


#endif //DRIFT_ULTRASONIC_H

//
// Created by Patrick on 2024-02-16.
//

#ifndef DRIFT_ULTRASONIC_H
#define DRIFT_ULTRASONIC_H

// #define UltrasonicSensor1_triggerPinBank DDRB

#include <stdint-gcc.h>

class Ultrasonic {
public:
    uint8_t triggerPin, echoPin;
    // int triggerPin, triggerPinBank;
    // int echoPin, echoPinBank;
    Ultrasonic(int triggerPinBank, int  triggerPinVar, int echoPinBank, int echoPin);
    Ultrasonic(uint8_t triggerPinIN, uint8_t echoPinIN);

    float pollSensor();

private:
    void triggerUltrasound(); // Actually sends pulse to Ultrasonic sensor,
    int measurePulse();
};


#endif //DRIFT_ULTRASONIC_H

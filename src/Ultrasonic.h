//
// Created by Patrick on 2024-02-16.
//

#ifndef DRIFT_ULTRASONIC_H
#define DRIFT_ULTRASONIC_H

#include <avr/io.h>

class Ultrasonic {
public:
    int triggerPin, triggerPinBank;
    int echoPin, echoPinBank;
    Ultrasonic(int  triggerPin, int triggerPinBank, int echoPin, int echoPinBank);

    int pollSensor();
};


#endif //DRIFT_ULTRASONIC_H

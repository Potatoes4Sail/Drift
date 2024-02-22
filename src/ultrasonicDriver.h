//
// Created by Patrick on 2024-02-16.
//


#ifndef DRIFT_ULTRASONICDRIVER_H
#define DRIFT_ULTRASONICDRIVER_H

// #define UltrasonicSensor1_triggerPinBank DDRB

#include <stdint-gcc.h>

class ultrasonicDriver {
public:

    ultrasonicDriver(uint8_t triggerPinIN, uint8_t echoPinIN);
    ultrasonicDriver();
//    void init(uint8_t triggerPinIN, uint8_t echoPinIN);
    virtual ~ultrasonicDriver();
    void pollSensor();

private:
    bool initialized = false;
    uint8_t triggerPin, echoPin;
    uint16_t sensorDistance;
    void triggerUltrasound(); // Actually sends pulse to ultrasonicDriver sensor,
};


#endif //DRIFT_ULTRASONICDRIVER_H

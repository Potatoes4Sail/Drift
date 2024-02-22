//
// Created by Patrick on 2024-02-16.
//


#ifndef DRIFT_ULTRASONICDRIVER_H
#define DRIFT_ULTRASONICDRIVER_H

class ultrasonicDriver {
public:
    ultrasonicDriver(uint8_t triggerPinIN, uint8_t echoPinIN);
    ultrasonicDriver();

    virtual ~ultrasonicDriver();
    void pollSensor();

private:
    bool initialized = false;
    uint8_t triggerPin, echoPin;
    void triggerUltrasound() const; // Actually sends pulse to ultrasonicDriver sensor,
};


#endif //DRIFT_ULTRASONICDRIVER_H

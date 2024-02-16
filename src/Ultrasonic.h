//
// Created by Patrick on 2024-02-16.
//

#ifndef DRIFT_ULTRASONIC_H
#define DRIFT_ULTRASONIC_H

class Ultrasonic {
public:
    int triggerPin, triggerPinBank;
    int echoPin, echoPinBank;
    Ultrasonic(int triggerPinBank, int  triggerPinVar, int echoPinBank, int echoPin);

    int pollSensor();

private:
    void triggerUltrasound(); // Actually sends pulse to Ultrasonic sensor,
};


#endif //DRIFT_ULTRASONIC_H

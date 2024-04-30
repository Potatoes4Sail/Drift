//
// Created by Patrick on 2024-04-17.
//

#ifndef DRIFT_ENCODERS_H
#define DRIFT_ENCODERS_H

#include <avr/io.h>
#include "pinDefinition.h"

enum WheelEncoder {
    BACK_ENCODER,   // 0
    LEFT_ENCODER,   // 1
    RIGHT_ENCODER   // 2
};

#define MINIMUM_US 10000
class encoders {
public:
    encoders();

    void processInterrupt(WheelEncoder whichEncoder);

    void calculateSpeeds();

    double getSpeed(WheelEncoder whichEncoder);
    void printEncoderStatus(WheelEncoder whichEncoder);

private:
    uint16_t backEncoderPulseCount;
    uint16_t leftEncoderPulseCount;
    uint16_t rightEncoderPulseCount;

    uint32_t lastTime;

    double backEncoderSpeed;
    double leftEncoderSpeed;
    double rightEncoderSpeed;
};


#endif //DRIFT_ENCODERS_H

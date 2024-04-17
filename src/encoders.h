//
// Created by Patrick on 2024-04-17.
//

#ifndef DRIFT_ENCODERS_H
#define DRIFT_ENCODERS_H

#include <avr/io.h>
#include "encoderDriverLl.h"
#include "pinDefinition.h"

enum WheelEncoder {
    BACK_ENCODER,   // 0
    LEFT_ENCODER,   // 1
    RIGHT_ENCODER   // 2
};

class encoders {
public:
    encoders();

    int16_t getSpeed(WheelEncoder whichEncoder);

    void processInterrupt(WheelEncoder whichEncoder, encoderPinSide whichPin, uint8_t bitState);

private:
    encoderDriverLL backEncoder;
    encoderDriverLL leftEncoder;
    encoderDriverLL rightEncoder;
};


#endif //DRIFT_ENCODERS_H

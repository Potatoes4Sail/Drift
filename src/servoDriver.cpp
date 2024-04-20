//
// Created by Patrick on 2024-04-09.
//

#include "servoDriver.h"
#include "pinDefinition.h"

void servoDriverInit() {
    // Sets PWM pin to output
    *portModeRegister(digitalPinToPort(SERVO_CONTROL_PIN)) |= digitalPinToBitMask(SERVO_CONTROL_PIN);

    // All time configuration is done in the customInitialization() function.
}

int setAngle(uint8_t angleAmount) {
    if ((angleAmount < 0) || (angleAmount > 90)) { // Invalid angle to attempt to set to, do nothing.
        return -1;
    }

    PULSE_SIZE = (uint8_t) (angleAmount * ANGLE_CONV + PULSE_MIN);

    return 0;
}


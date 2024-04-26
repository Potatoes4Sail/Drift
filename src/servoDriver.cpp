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

    setPulseSize((uint8_t) (angleAmount * ANGLE_CONV + PULSE_MIN));

    return 0;
}


int setAngle_us(uint16_t angleAmount) {
    uint16_t angleAmountLimited = constrain(angleAmount, 1000, 2000);
    setPulseSize(24 - angleAmountLimited / 125 - 1);
    return 0;
}

void setPulseSize(uint8_t PULSE) {
    PULSE_SIZE = PULSE;
}

uint8_t getPulseSize() {
    return PULSE_SIZE;
}




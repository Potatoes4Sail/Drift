//
// Created by Patrick on 2024-04-09.
//

#include "servoDriver.h"

void servoDriverInit() {
    // Configure timer for interrupt based servo running.

    // Sets PWM pin to output
    *portModeRegister(digitalPinToPort(SERVO_CONTROL_PIN)) |= digitalPinToBitMask(SERVO_CONTROL_PIN);

    DDRD |= _BV(PD5);

/*    // For timer0;

    TCCR0A = 0;
    TCCR0B = 0;

    TCCR0A |= _BV(COM0A1);      // Sets A output high at bottom, and clears once it reaches OCR0A's value.

    TCCR0A |= _BV(WGM01) | _BV(WGM00); // Sets Fast PWM bits.

    TCCR0B = 0b0101; // 1024 prescaler.*/

    // This gives a frequency of 60.9824 Hz.
    // At 1 ms for minimum pulse ->
    // (1 * 1000) / 60.9824 = 16

    // At 2 mm for max pulse
    // (2 * 1000) / 60.9824 = 33
}

int setAngle(uint8_t angleAmount) {
    if ((angleAmount < 0) || (angleAmount > 90)) { // Invalid angle to attempt to set to, do nothing.
        return -1;
    }

    PULSE_SIZE = (uint8_t) (angleAmount * ANGLE_CONV + PULSE_MIN);

    return 0;
}


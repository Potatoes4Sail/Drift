//
// Created by Patrick on 2024-04-09.
//

#include "servoDriver.h"

void servoDriverInit() {
    // Configure timer for interrupt based servo running.

    // Sets PWM pin to output
    *portModeRegister(digitalPinToPort(SERVO_CONTROL_PIN_1)) |= digitalPinToBitMask(SERVO_CONTROL_PIN_1);
#if USE_SERVO_PIN_2
    *portModeRegister(digitalPinToPort(SERVO_CONTROL_PIN_2)) |= digitalPinToBitMask(SERVO_CONTROL_PIN_2);
#endif
    // For timer0;

    TCCR0A = 0;
    TCCR0B = 0;

    TCCR0A |= _BV(COM0A1);      // Sets A output high at bottom, and clears once it reaches OCR0A's value.

#if USE_SERVO_PIN_2
    TCCR0A |= _BV(COM0B1);      // Sets B output high at bottom, and clears once it reaches OCR0B's value.
#endif

    TCCR0A |= _BV(WGM01) | _BV(WGM00); // Sets Fast PWM bits.

    TCCR0B = 0b0101; // 1024 prescaler.

    // This gives a frequency of 60.9824 Hz.
    // At 1 ms for minimum pulse ->
    // (1 * 1000) / 60.9824 = 16

    // At 2 mm for max pulse
    // (2 * 1000) / 60.9824 = 33
}

int setAngleA(uint8_t angleAmount) {
    if ((angleAmount < 0) || (angleAmount > 90)) { // Invalid angle to attempt to set to, do nothing.
        return -1;
    }

    SERVO_CONTROL_BANK_1 = (uint8_t) (angleAmount * ANGLE_CONV + PULSE_MIN);

    return 0;
}

#if USE_SERVO_PIN_2
int setAngleB(uint8_t angleAmount) {
    if ((angleAmount < 0) || (angleAmount > 90)) { // Invalid angle to attempt to set to, do nothing.
        return -1;
    }

    SERVO_CONTROL_BANK_2 = (uint8_t) (angleAmount * ANGLE_CONV + PULSE_MIN);

    return 0;
}
#endif

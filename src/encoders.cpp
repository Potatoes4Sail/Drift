//
// Created by Patrick on 2024-04-17.
//

#include <Arduino.h>
#include "encoders.h"

encoders::encoders() {
    // Init variables
    backEncoderPulseCount = 0;
    leftEncoderPulseCount = 0;
    rightEncoderPulseCount = 0;

    lastTime = 0;

    backEncoderSpeed = 0;
    leftEncoderSpeed = 0;
    rightEncoderSpeed = 0;

    //     Initialize the pins for the three encoder objects.

    // Sets encoder pin to input
    *portModeRegister(digitalPinToPort(BACK_ENCODER_A)) &= ~digitalPinToBitMask(BACK_ENCODER_A);
    *portModeRegister(digitalPinToPort(BACK_ENCODER_B)) &= ~digitalPinToBitMask(BACK_ENCODER_B);

    *portModeRegister(digitalPinToPort(LEFT_WHEEL_ENCODER_A)) &= ~digitalPinToBitMask(LEFT_WHEEL_ENCODER_A);
    *portModeRegister(digitalPinToPort(LEFT_WHEEL_ENCODER_B)) &= ~digitalPinToBitMask(LEFT_WHEEL_ENCODER_B);

    *portModeRegister(digitalPinToPort(RIGHT_WHEEL_ENCODER_A)) &= ~digitalPinToBitMask(RIGHT_WHEEL_ENCODER_A);
    *portModeRegister(digitalPinToPort(RIGHT_WHEEL_ENCODER_B)) &= ~digitalPinToBitMask(RIGHT_WHEEL_ENCODER_B);

    // Configure the pin interrupts:
    uint8_t oldSREG = SREG;
    cli();

    PCICR |= _BV(PCIE1); // enable interrupts on bank C.

//    PCMSK1 |= _BV(digitalPinToBitMask(BACK_ENCODER_A)) | _BV(digitalPinToBitMask(LEFT_WHEEL_ENCODER_A)) |
//              _BV(digitalPinToBitMask(RIGHT_WHEEL_ENCODER_A));

    // Set up the bitmask
    PCMSK1 |= _BV(digitalPinToBitMask(BACK_ENCODER_A)) | _BV(digitalPinToBitMask(BACK_ENCODER_B)) |
              _BV(digitalPinToBitMask(LEFT_WHEEL_ENCODER_A)) | _BV(digitalPinToBitMask(LEFT_WHEEL_ENCODER_B)) |
              _BV(digitalPinToBitMask(RIGHT_WHEEL_ENCODER_A)) | _BV(digitalPinToBitMask(RIGHT_WHEEL_ENCODER_B));

    PORTC |= _BV(digitalPinToBitMask(BACK_ENCODER_A)) | _BV(digitalPinToBitMask(BACK_ENCODER_B)) |
             _BV(digitalPinToBitMask(LEFT_WHEEL_ENCODER_A)) | _BV(digitalPinToBitMask(LEFT_WHEEL_ENCODER_B)) |
             _BV(digitalPinToBitMask(RIGHT_WHEEL_ENCODER_A)) | _BV(digitalPinToBitMask(RIGHT_WHEEL_ENCODER_B));


    sei();
    SREG = oldSREG;
}

void encoders::processInterrupt(WheelEncoder whichEncoder) {
    switch (whichEncoder) {
        case BACK_ENCODER:
            backEncoderPulseCount++;
            break;
        case LEFT_ENCODER:
            leftEncoderPulseCount++;
            break;
        case RIGHT_ENCODER:
            rightEncoderPulseCount++;
            break;
    }
}

double encoders::getSpeed(WheelEncoder whichEncoder) {
    // Returns the speed of the motor based on calculations done by calculateSpeed function.
    switch (whichEncoder) {
        case BACK_ENCODER:
            return backEncoderSpeed;
        case LEFT_ENCODER:
            return leftEncoderSpeed;
        case RIGHT_ENCODER:
            return rightEncoderSpeed;
        default:
            return -1;
    }
}

// TODO: Validate the math behind this function. It's currently wrong (+create github issue for this)
void encoders::calculateSpeeds() {
    // Calculate the speed based on timeElasped/pulseCount
    uint32_t currentTime = micros();
    uint32_t deltaTime = currentTime - lastTime;

    if (deltaTime < MINIMUM_US) return; // Don't re-calculate speed if below this time.

    // Calculate as rev/s
    backEncoderSpeed = 1e6 * (backEncoderPulseCount * BACK_ENCODER_PULSES_PER_REV) / deltaTime;
    leftEncoderSpeed = 1e6 * (leftEncoderPulseCount * WHEEL_ENCODER_PULSES_PER_REV) / deltaTime;
    rightEncoderSpeed = 1e6 * (rightEncoderPulseCount * WHEEL_ENCODER_PULSES_PER_REV) / deltaTime;

    lastTime = currentTime;
}

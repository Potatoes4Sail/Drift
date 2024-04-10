//
// Created by Patrick on 2024-04-10.
//

#include "wheelEncoder.h"

wheelEncoder::wheelEncoder() {
    // Left and right wheels are initialized
    leftWheel.initalizeEncoderDriver(LEFT_WHEEL_ENCODER);
    rightWheel.initalizeEncoderDriver(RIGHT_WHEEL_ENCODER);
    // This will set up the interrupt masks

    WHEEL_ENCODER_PIN_MASK |= WHEEL_ENCODER_INTERRUPT_MASK;
}

int32_t wheelEncoder::readLeftSpeed() {
    return leftWheel.getSpeed();
}

int32_t wheelEncoder::readRightSpeed() {
    return rightWheel.getSpeed();
}

volatile uint8_t previousBankState = 0b00000000;

volatile void wheelEncoder::handleInterrupt() {
    // Store the current state of PORTC
    uint8_t currentBankState = PORTC;

    // Check for changes and high state for LEFT_WHEEL_ENCODER
    if ((currentBankState & _BV(digitalPinToBitMask(LEFT_WHEEL_ENCODER))) &&
        !(previousBankState & _BV(digitalPinToBitMask(LEFT_WHEEL_ENCODER)))) {
        leftWheel.handleInterrupt();
    }

    // Check for changes and high state for RIGHT_WHEEL_ENCODER
    if ((currentBankState & _BV(digitalPinToBitMask(RIGHT_WHEEL_ENCODER))) &&
        !(previousBankState & _BV(digitalPinToBitMask(RIGHT_WHEEL_ENCODER)))) {
        rightWheel.handleInterrupt();
    }

    // Update the previous state
    previousBankState = PORTC;
    return;
}
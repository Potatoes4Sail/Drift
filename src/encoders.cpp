//
// Created by Patrick on 2024-04-17.
//

#include "encoders.h"

encoders::encoders() {
    // Create the three encoderDriverLL objects
    backEncoder = encoderDriverLL(MOTOR_ENCODER_A, MOTOR_ENCODER_B, MOTOR_ENCODER_PULSES_PER_REV);
    leftEncoder = encoderDriverLL(LEFT_WHEEL_ENCODER_A, LEFT_WHEEL_ENCODER_B, WHEEL_ENCODER_PULSES_PER_REV);
    rightEncoder = encoderDriverLL(RIGHT_WHEEL_ENCODER_A, RIGHT_WHEEL_ENCODER_B, WHEEL_ENCODER_PULSES_PER_REV);

    // Configure the pin interrupts:
    uint8_t oldSREG = SREG;
    cli();

    PCICR |= _BV(PCIE1); // enable interrupts on bank C.
    // Setup the bitmask
    PCMSK1 |= _BV(digitalPinToBitMask(MOTOR_ENCODER_A)) | _BV(digitalPinToBitMask(MOTOR_ENCODER_B)) |
              _BV(digitalPinToBitMask(LEFT_WHEEL_ENCODER_A)) | _BV(digitalPinToBitMask(LEFT_WHEEL_ENCODER_B)) |
              _BV(digitalPinToBitMask(RIGHT_WHEEL_ENCODER_A)) | _BV(digitalPinToBitMask(RIGHT_WHEEL_ENCODER_B));

    PORTC |= _BV(digitalPinToBitMask(MOTOR_ENCODER_A)) | _BV(digitalPinToBitMask(MOTOR_ENCODER_B)) |
             _BV(digitalPinToBitMask(LEFT_WHEEL_ENCODER_A)) | _BV(digitalPinToBitMask(LEFT_WHEEL_ENCODER_B)) |
             _BV(digitalPinToBitMask(RIGHT_WHEEL_ENCODER_A)) | _BV(digitalPinToBitMask(RIGHT_WHEEL_ENCODER_B));

    sei();
    SREG = oldSREG;
}

int16_t encoders::getSpeed(WheelEncoder whichEncoder) {
    return 0;
}

void encoders::processInterrupt(WheelEncoder whichEncoder, encoderPinSide whichPin, uint8_t bitState) {
    switch (whichEncoder) {
        case BACK_ENCODER:
            backEncoder.processInterrupt(whichPin, bitState);
            break;
        case LEFT_ENCODER:
            leftEncoder.processInterrupt(whichPin, bitState);
            break;
        case RIGHT_ENCODER:
            rightEncoder.processInterrupt(whichPin, bitState);
            break;
    }
}

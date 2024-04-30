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
/*    *portModeRegister(digitalPinToPort(BACK_ENCODER_A)) &= ~digitalPinToBitMask(BACK_ENCODER_A);
    *portModeRegister(digitalPinToPort(BACK_ENCODER_B)) &= ~digitalPinToBitMask(BACK_ENCODER_B);

    *portModeRegister(digitalPinToPort(LEFT_WHEEL_ENCODER_A)) &= ~digitalPinToBitMask(LEFT_WHEEL_ENCODER_A);
    *portModeRegister(digitalPinToPort(LEFT_WHEEL_ENCODER_B)) &= ~digitalPinToBitMask(LEFT_WHEEL_ENCODER_B);

    *portModeRegister(digitalPinToPort(RIGHT_WHEEL_ENCODER_A)) &= ~digitalPinToBitMask(RIGHT_WHEEL_ENCODER_A);
    *portModeRegister(digitalPinToPort(RIGHT_WHEEL_ENCODER_B)) &= ~digitalPinToBitMask(RIGHT_WHEEL_ENCODER_B);*/

    DDRC &= ~0b00111111; // Set to inputs
    // Configure the pin interrupts:
    uint8_t oldSREG = SREG;
    cli();

    PCICR |= _BV(PCIE1); // enable interrupts on bank C.
    MCUCR &= ~_BV(PUD); // disable pull up disable.


    PCICR |= _BV(PCIE1); // enable interrupts on bank C.

    // Set up the bitmask
    PCMSK1 |= _BV(0) | _BV(1);      // Rear encoder
//    PCMSK1 |= _BV(2) | _BV(3);    // Left encoder (broken)
    PCMSK1 |= _BV(4) | _BV(5);      // Right encoder

    // Enable pull up
//    PORTC |= _BV(2) | _BV(3); // Left encoder, which is broken.
    PORTC |= _BV(4) | _BV(5);

    MCUCR &= ~_BV(PUD); // disable pull up disable.

    sei();
//    SREG = oldSREG;
}

volatile void encoders::processInterrupt(WheelEncoder whichEncoder) {
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

    float timeAmount = 1.0e6 / deltaTime;

    // Calculate as rev/s
    backEncoderSpeed = (timeAmount * backEncoderPulseCount) / (BACK_ENCODER_PULSES_PER_REV * 1.0);
    leftEncoderSpeed = (timeAmount * leftEncoderPulseCount) / (WHEEL_ENCODER_PULSES_PER_REV * 1.0) * 3.777;
    rightEncoderSpeed = (timeAmount * rightEncoderPulseCount) / (WHEEL_ENCODER_PULSES_PER_REV * 1.0) * 3.777;

    // Zero counts.
    backEncoderPulseCount = 0;
    leftEncoderPulseCount = 0;
    rightEncoderPulseCount = 0;

    // Reset time
    lastTime = currentTime;
}

void encoders::printEncoderStatus(WheelEncoder whichEncoder) {
    Serial.print("Encoder ");
    Serial.print((WheelEncoder) whichEncoder);
    Serial.print("'s speed is ");
    Serial.print(getSpeed(whichEncoder) * 60, 6);
    Serial.print("\n");
}

//
// Created by Patrick on 2024-04-16.
//

#include "testCases.h"

/// Temporary function which runs various speed commands to see if the motor is behaving properly.
/// Motor will run in forward direction, and then coast down, speed up and then brake
void motorTestSpinUnidirectional() {
    int8_t speed = 127;
    L298Driver motor(MOTOR_PIN_PWM, MOTOR_PIN_FORWARD, MOTOR_PIN_REVERSE, 25);
    motor.setSpeed(speed);
    _delay_ms(1000);
    motor.setSpeed(0);
    _delay_ms(1000);

    motor.setSpeed(speed);
    _delay_ms(1000);
    motor.setBrake(100);
    _delay_ms(1000);
}


/// Test function to run motor at full power forwards, coast for 2 seconds, and then run in reverse, and then coast again.
void motorTestSpinBidirectional() {
    int8_t maxSpeed = 127;
    L298Driver motor(MOTOR_PIN_PWM, MOTOR_PIN_FORWARD, MOTOR_PIN_REVERSE, 25);

    // Go forwards
    motor.setSpeed(maxSpeed);
    _delay_ms(2000);
    motor.setSpeed(0);
    _delay_ms(2000);

    // Go backwards
    motor.setSpeed(-maxSpeed);
    _delay_ms(2000);
    motor.setSpeed(0);
    _delay_ms(2000);
}

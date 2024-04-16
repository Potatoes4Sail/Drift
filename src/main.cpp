#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

#include "Ultrasonic.h"
#include "helperFunctions.h"
#include "L298Driver.h"
#include "servoDriver.h"
#include "pinDefinition.h"

void motorTestSetup(){
    int8_t speed = 0;
    L298Driver motor(MOTOR_PIN_PWM, MOTOR_PIN_FORWARD, MOTOR_PIN_REVERSE, 25);
    motor.setSpeed(127);
//    _delay_ms(100);
//    motor.setSpeed(100);
    _delay_ms(1000);
    motor.setSpeed(0);
    _delay_ms(1000);

    motor.setSpeed(127);
//    _delay_ms(100);
//    motor.setSpeed(100);
    _delay_ms(1000);
    motor.setBrake(100);
    _delay_ms(1000);
}

int main() {

//    DDRD = _BV(PD5) | _BV(PD6);
//
//    TCCR0A = 0;
//    TCCR0B = 0;
//
//    TCCR0A |= _BV(COM0A1);      // Sets A output high at bottom, and clears once it reaches OCR0A's value.
//    TCCR0A |= _BV(COM0B1);      // Sets B output high at bottom, and clears once it reaches OCR0B's value.
//
//    TCCR0A |= _BV(WGM01) | _BV(WGM00); // Sets Fast PWM bits.
//
//    TCCR0B = 0b0101; // 1024 prescaler.
//
//    // This gives a frequency of 60.9824 Hz.
//    // At 0.5 ms for minimum pulse ->
//    // (0.5 * 1000) / 60.9824 = 8.2
//
//    // At 2.3 mm for max pulse
//    // (2.3 * 1000) / 60.9824 = 8.2
//    const uint8_t PULSE_MIN = 15;           // min width pulse (1 ms)
//    const uint8_t PULSE_MAX = 31;           // max width pulse (2 ms)
//
//    OCR0A = (uint8_t) PULSE_MIN;
//    OCR0B = (uint8_t) PULSE_MAX;

    servoDriverInit();

    uint8_t num = 0;
    int wait = 1000;
    while (true) {
        setAngleA(0);
        _delay_ms(wait);
#if USE_SERVO_PIN_2
        setAngleB(90);
#endif
        _delay_ms(wait);
    }

    return 1;
}
#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

#include "Ultrasonic.h"
#include "helperFunctions.h"
#include "L298Driver.h"
#include "servoDriver.h"
#include "pinDefinition.h"
#include "encoderDriver.h"
#include "wheelEncoder.h"

// GLOBAL VARIABLES:
wheelEncoder wheels;

void initalizeTimer1();

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

    unsigned long startTime1 = 0;
    unsigned long startTime2 = 0;

    init();

    Serial.begin(115200);
    Serial.println("Begin logging");

    WHEEL_ENCODER_PIN_MASK |= WHEEL_ENCODER_INTERRUPT_MASK;

    initalizeTimer1();
    sei();
    Serial.println("Timers all go!");
    while (true) {
        if ((millis() - startTime1) > 500) {

//            Serial.println(wheels.readLeftSpeed());
            startTime1 = millis();
        }
    }


    servoDriverInit();

    uint8_t num = 0;
    int wait = 1000;
    while (true) {
        setAngleA(0);
        _delay_ms(wait);
        setAngleB(90);
        _delay_ms(wait);
    }

    return 1;
}

/**
 * @brief Initializes Timer1 for time measurement.
 *
 * This function resets Timer1 settings to their original state, sets the prescaler to divide the clock by 64,
 * and resets the timer to zero.
 *
 * @note Timer1 Interrupt Mask Register (TIMSK1) is reset.
 * @note Timer1 control registers TCCR1A and TCCR1B are reset to their initial values.
 * @note The prescaler is set to divide the clock by 64, providing a maximum time measurement of 262.1 ms with a minimum time step of 4 us.
 */
void initalizeTimer1() {
    TIMSK1 = 0; // RESET Timer1 Interrupt Mask Register (pg 112)
    // RESET TIMER1 control register to original state:
    TCCR1A = 0x00;
    TCCR1B = 0b011; // Set prescaler to Clk i/o to 64 (pg 110)
    // This provides a max time measurement of 262.1 ms with an min timestep of 4 us
    // The other option would be 8 but 32.8 ms is too short
    // OR, 256, with a total time of 1.048 s, and 16 us, however this is likely more than anything we need to time? (But 1 s is nice xD)

    TCNT1 = 0; // Reset timer to 0.
}

ISR(WHEEL_ENCODER_INT_vect) { // Currently the only thing triggering this bank of interrupt pins is the wheels
    Serial.println("ISR TRIGGERED!");
    wheels.handleInterrupt();
}